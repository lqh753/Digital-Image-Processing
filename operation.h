#pragma once

#include"stdafx.h"
#include<opencv2\opencv.hpp>
#include<string>


using namespace cv;
using namespace std;


void padImage(Mat &img, Mat &result, int top, int bottom, int left, int right, int code=0);


void conv2D(Mat &img, Mat &result, Mat &kernel, int padding = 0, int h_stride=1, int v_stride=1);


