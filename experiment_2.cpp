#include "stdafx.h"

//#include "dip.h"
//#include<opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//
//using namespace cv;
//using namespace std;
//
//void dftShift(Mat complexI) {
//	complexI = complexI(Rect(0, 0, complexI.cols & -2, complexI.rows & -2));
//	int cx = complexI.cols / 2;
//	int cy = complexI.rows / 2;
//
//	Mat q1(complexI, Rect2d(0, 0, cx, cy));
//	Mat q2(complexI, Rect2d(cx, 0, cx, cy));
//	Mat q3(complexI, Rect2d(0, cy, cx, cy));
//	Mat q4(complexI, Rect2d(cx, cy, cx, cy));
//
//	Mat temp;
//	q1.copyTo(temp);
//	q4.copyTo(q1);
//	temp.copyTo(q4);
//
//	q2.copyTo(temp);
//	q3.copyTo(q2);
//	temp.copyTo(q3);
//}
//
//Mat generateLatticeMat(int width, int height) {
//	Mat image(height, width, CV_8UC1, Scalar::all(0));
//	int m = 50;
//	int n = 50;
//	bool m_b = false, n_b = false;
//	Mat black(m, n, CV_8UC1, Scalar::all(0));
//	Mat white(m, n, CV_8UC1, Scalar::all(255));
//
//
//	for (size_t i = 0; i < height; i = i+m)
//	{
//		for (size_t j = 0; j < width; j = j+n)
//		{
//			Mat temp(image,Rect(j, i, m, n));
//			if (n_b)
//			{
//				black.copyTo(temp);
//			}
//			else {
//				white.copyTo(temp);
//			}
//			n_b = !n_b;
//		}
//		if (m_b)
//		{
//			n_b = false;
//		}
//		else
//		{
//			n_b = true;
//		}
//		m_b = !m_b;
//	}
//	return image;
//}
//
//int mainn() {
//	string image_name = "2_2.tif";
//	Mat I = imread("images/"+image_name, IMREAD_GRAYSCALE);
//	Mat image = generateLatticeMat(350, 350);
//	imshow("lattice", image);
//	imwrite("results/lattice.jpg", image);
//	waitKey(1);
//	//Mat I = image;
//	if (I.empty())return -1;
//	imshow("original image", I);
//	waitKey(1);
//	
//	int m = getOptimalDFTSize(I.rows);
//	int n = getOptimalDFTSize(I.cols);
//	Mat padded;
//	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
//
//	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
//	Mat complexI;
//	merge(planes, 2, complexI);
//
//	dft(complexI, complexI);
//
//	dftShift(complexI);
//
//	split(complexI, planes);
//	Mat magI;
//	magnitude(planes[0],planes[1], magI);
//	magI += Scalar::all(1);
//	log(magI, magI);
//
//	normalize(magI, magI, 0, 1, NORM_MINMAX);
//	magI.convertTo(magI, CV_8UC1, 255);
//	imwrite("results/magnitude of " + image_name, magI);
//	vector<Rect> rois;
//	selectROIs("select rois", magI, rois);
//	for (size_t i = 0; i < rois.size(); i++)
//	{
//		complexI(rois[i]) = Scalar::all(0);
//		magI(rois[i]) = Scalar::all(0);
//	}
//	
//	dftShift(complexI);
//	imshow("magnitude of "+image_name, magI);
//	imwrite("results/filtered magnitude of " + image_name, magI);
//
//	Mat temp;
//	idft(complexI, temp, DFT_REAL_OUTPUT);
//	Mat idft_I = temp(Rect(0, 0, I.cols, I.rows)).clone();
//
//	normalize(idft_I, idft_I,0, 1, NORM_MINMAX);
//
//	idft_I.convertTo(idft_I, CV_8UC1, 255);
//	imshow("image after idft",idft_I);
//	imwrite("results/idft "+image_name, idft_I);
//	waitKey(0);
//	
//
//	system("pause");
//	return 0;
//}
//



#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>

using namespace std;
using namespace cv;
void getStatData(Mat src, int* hist, float* ave, float* var, bool isPrint) {
	float average = 0, variance = 0;
	int channels = src.channels();
	int nRows = src.rows;
	int nCols = src.cols * channels;
	if (src.isContinuous()) {//是否在内存中连续存储，若连续则读一大长条内存就能读入整张图片
		nCols *= nRows;
		nRows = 1;
	}
	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i) {
		p = src.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j) {
			hist[p[j]]++;
		}
	}
	int max = *max_element(hist, hist + 256);
	Mat histgraph = Mat::zeros(300, 512, CV_8U);
	for (i = 0; i < 256; ++i) {
		if (isPrint) {
			for (j = histgraph.rows - ((double)hist[i] / max * (histgraph.rows - 1)); j < histgraph.rows; ++j) {
				histgraph.at<uchar>(j, 2 * i) = 255;
				histgraph.at<uchar>(j, 2 * i + 1) = 255;
			}
		}
		average += i * hist[i];
	}

	average /= src.rows * src.cols;
	for (i = 0; i < 256; ++i) {
		variance += hist[i] * (i - average) * (i - average);
	}
	variance /= src.rows * src.cols;
	*ave = average; *var = variance;
	if (isPrint) {
		imshow("Original Image", src);
		imshow("Histgraph", histgraph);
		waitKey(1);
		cout << "图片的全局平均值为" << average << "，方差为" << variance << endl;
	}
}

Mat getlut(int* hist) {
	//积分直方图
	int addArray[256];
	addArray[0] = hist[0];
	for (int i = 1; i < 256; i++) {
		addArray[i] = addArray[i - 1] + hist[i];
	}
	//转换到调色盘
	Mat lut(1, 256, CV_8UC1);
	uchar* p = lut.ptr();
	for (int i = 0; i < 256; i++) {
		p[i] = addArray[i] / addArray[255] * 255;
	}
	return lut;
}

Mat equalization(Mat src) {
	float average, variance; int hist[256], addArray[256];
	getStatData(src, hist, &average, &variance, 0);
	Mat newLookUpTable = getlut(hist);
	Mat res(src.rows, src.cols, src.type());
	LUT(src, newLookUpTable, res);
	return res;
}

int main()
{
	cout << "////////////////////////计算全局直方图及统计值//////////////////////////" << endl;
	Mat image = imread("images/6.tif");
	cvtColor(image, image, CV_BGR2GRAY);
	float average, variance;
	double t = (double)getTickCount();
	int hist[256] = { 0 };
	getStatData(image, hist, &average, &variance, 1);
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "此段程序用时为" << t << "s" << endl;
	
	Mat result1 = equalization(image);
	imshow("Global equalization", result1);
	waitKey(1);
	system("pause");
	return 0;
}


