#include"stdafx.h"
#include"dip.h"



int digits = 256;

void getHistogram(Mat img, double hist[256]) {
	uchar* data = 0;
	int t = 0;
	for (size_t i = 0; i < img.rows; i++)
	{
		data = img.ptr<uchar>(i);
		for (size_t j = 0; j < img.cols; j++)
		{
			t = data[j];
			hist[t] = hist[t] + 1;
		}
	}

}


void normalizeHist(double* hist, double *norm) {
	double MN = getArrSum(hist, digits);
	for (int i = 0; i < digits; i++)
	{
		norm[i] = hist[i] / MN;
	}
}


void getHistMat(double *hist/*intput size scale:(0-1)*/, Mat frame, int height, int width, int bins) {
	int ratio = 1 / getMaxValue(hist, digits);
	for (size_t i = 0; i < digits; i++)
	{
		line(frame, Point(i * bins, height),
			Point(i * bins, round(height * (1-ratio * hist[i]))), Scalar(0, 0, 255));

	}
}


void equalizeHistogramCurve(double *orignal_hist, double *processed_hist) {
	processed_hist[0] = round(digits * orignal_hist[0]);
	for (size_t i = 1; i < digits; i++)
	{
		processed_hist[i] = round(digits * orignal_hist[i]) + processed_hist[i - 1];
	}
}


void invert(double *curve) {
	for (size_t i = 0; i < digits; i++)
	{
		curve[i] = digits - i;
	}
}


void grayScaleTransform(Mat img, Mat result, double *curve) {
	int rows = img.rows;
	int cols = img.cols;
	for (size_t i = 0; i < rows; i++)
	{
		uchar* a = img.ptr<uchar>(i);
		uchar* b = result.ptr<uchar>(i);
		uchar t = 0;
		for (size_t j = 0; j < cols; j++)
		{
			t = (uint)curve[(uint)a[j]];
			b[j] = t;

		}
	}
}


void gammaTransform(Mat img, Mat result, double gamma, double c, double epsilon) {
	double curve[256] = { 0 };
	for (size_t i = 0; i < digits; i++)
	{
		curve[i] = c * pow(i + epsilon, gamma);
	}
	grayScaleTransform(img, result, curve);
}


void localEqualize(Mat img, Mat result, int size, double E, double k0, double k1, double k2, double k3) {
	double global_mean = getMatMean(img);
	double global_variance = getVariance(img, global_mean);

	double local_mean = 0;
	double local_variance = 0;

	Mat roi = img(Rect(0, 0, size, size));
	int width = round((size - 1) / 2);

	for (size_t i = width; i < img.cols - width -1; i++)
	{
		for (size_t j = width; j < img.rows - width -1; j++)
		{
			roi = img(Rect(i-width, j-width, size, size));
			local_mean = getMatMean(roi);
			local_variance = getVariance(roi, local_mean);
			if (k0*global_mean < local_mean && local_mean<k1*global_mean && k2*global_variance<local_variance && local_variance<k3 * global_variance)
			{
				result.ptr<uchar>(j, i)[0] = E * img.ptr<uchar>(j, i)[0];
			}
			else
			{
				result.ptr<uchar>(j, i)[0] = img.ptr<uchar>(j, i)[0];
			}
		}
	}
}