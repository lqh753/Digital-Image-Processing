#include"stdafx.h"
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<numeric>
#include"dip.h"
#include"operation.h"

using namespace std;
using namespace cv;


int main() {

	Mat orignal_image = imread("Images_1/2.jpg", 0);
	imshow("original image", orignal_image);
	Mat equalized_image(orignal_image.rows, orignal_image.cols, CV_8UC1, Scalar(0, 0, 0));
	
	////Mat equalized_image(orignal_image.size, orignal_image.type);

	int height = 256;
	int width = 512;
	double pixel_hist[256] = { 0 };
	double norm_hist[256] = { 0 };
	double equal_hist[256] = { 0 };
	double equal_hist_curve[256] = { 0 };

	Mat hist_mat(height, width, CV_8UC3, Scalar(0,0,0));
	Mat equal_hist_mat(height, width, CV_8UC3, Scalar(0, 0, 0));

	//calculate and show the histogram of original image
	getHistogram(orignal_image, pixel_hist);
	normalizeHist(pixel_hist, norm_hist);
	getHistMat(norm_hist, hist_mat, height, width, 2);
	imshow("histogram of original image", hist_mat);
	imwrite("results/histogram of original image.jpg", hist_mat);

	//generate the intensity transformation function
	equalizeHistogramCurve(norm_hist, equal_hist_curve);
	grayScaleTransform(orignal_image, equalized_image, equal_hist_curve);

	//calculate and show the histogram of enqualized image
	getHistogram(equalized_image, pixel_hist);
	normalizeHist(pixel_hist, equal_hist);
	getHistMat(equal_hist, equal_hist_mat, height, width, 2);
	imshow("histogram of enqualized image", equal_hist_mat);
	imwrite("results/enqualized image.jpg", equalized_image);
	imwrite("results/histogram of enqualized image.jpg", equal_hist_mat);

	//loacl enhance image operation
	Mat local_enhance_image(orignal_image.rows, orignal_image.cols, CV_8UC1, Scalar(0, 0, 0));
	localEqualize(orignal_image, local_enhance_image, 7, 2.0, 0.1, 0.6, 0.01, 0.2);
	imshow("local enhance image", local_enhance_image);
	imwrite("results/local enhance image.jpg", local_enhance_image);
	
	//calculate and show the histogram of enhanced image
	double enhanced_hist[256] = { 0 };
	Mat enhanced_hist_mat(height, width, CV_8UC3, Scalar(0, 0, 0));
	getHistogram(local_enhance_image, pixel_hist);
	normalizeHist(pixel_hist, enhanced_hist);
	getHistMat(enhanced_hist, enhanced_hist_mat, height, width, 2);
	imshow("histogram of enhanced image", enhanced_hist_mat);
	imwrite("results/histogram of enhanced image.jpg", enhanced_hist_mat);

	waitKey(0);
	system("pause");
	return 0;
}

