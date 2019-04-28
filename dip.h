#pragma once
#include"stdafx.h"
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<numeric>
#include<algorithm>
#include<math.h>
#include<fstream>
#include"utility.h"

using namespace cv;
using namespace std;


void getHistogram(Mat img, double hist[256]);

void normalizeHist(double* hist, double *norm);

void getHistMat(double *hist, Mat frame, int height, int width, int bins);

void equalizeHistogramCurve(double *orignal_hist, double *processed_hist);

void grayScaleTransform(Mat img, Mat result, double *curve);

void invert(double *curve);

void gammaTransform(Mat img, Mat result, double gamma, double c = 1.0, double epsilon = 0.0);

void localEqualize(Mat img, Mat result, int size = 3, double E=4.0, double k0 = 0.0, double k1=0.4, double k2=0.0, double k3 = 0.4);

