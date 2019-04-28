#pragma once

#include"stdafx.h"
#include<opencv2\opencv.hpp>
using namespace cv;

double getMaxValue(double *arr, int len);

double getArrSum(double *arr, int length);

double getMatSum(Mat img);

double getMatMean(Mat img);

double getVariance(Mat img, double mean);



