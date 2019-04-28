#include"stdafx.h"
#include"utility.h"



double getMaxValue(double *arr, int len) {
	double result = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (arr[i]>result)
		{
			result = arr[i];
		}
	}
	return result;
}

double getArrSum(double *arr, int length) {
	double result = 0;
	for (size_t i = 0; i < length; i++)
	{
		result = result + arr[i];
	}
	return result;
}



double getMatSum(Mat img) {
	uchar* data = 0;
	double sum = 0;
	for (size_t i = 0; i < img.rows; i++)
	{
		data = img.ptr<uchar>(i);
		for (size_t j = 0; j < img.cols; j++)
		{
			sum += data[j];
		}
	}
	return sum;
}


double getMatMean(Mat img) {
	uchar* data = 0;
	double mean = 0;
	for (size_t i = 0; i < img.rows; i++)
	{
		data = img.ptr<uchar>(i);
		for (size_t j = 0; j < img.cols; j++)
		{
			mean += data[j];
		}
	}
	mean = mean / img.rows / img.cols;
	return mean;
}



double getVariance(Mat img, double mean) {
	uchar* data = 0;
	double variance = 0;
	for (size_t i = 0; i < img.rows; i++)
	{
		data = img.ptr<uchar>(i);
		for (size_t j = 0; j < img.cols; j++)
		{
			variance += pow(data[j] - mean, 2);
		}
	}
	variance = sqrt(variance / (img.rows * img.cols));
	return variance;
}
