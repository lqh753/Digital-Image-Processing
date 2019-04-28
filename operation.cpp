#include "operation.h"
#include"utility.h"

void padImage(Mat &img, Mat &result, int top, int bottom, int left, int right, int code){
	int width = img.cols;
	int height = img.rows;
	
	switch (code)
	{
	case 0:
	{
		Mat tclip(Mat(top, width, img.type(), Scalar(0, 0, 0)));
		vconcat(tclip, img, result);

		Mat bclip(Mat(bottom, width, img.type(), Scalar(0, 0, 0)));
		vconcat(result, bclip, result);

		Mat lclip(Mat(top + height + bottom, left, img.type(), Scalar(0, 0, 0)));
		hconcat(lclip, result, result);

		Mat rclip(Mat(top + height + bottom, right, img.type(), Scalar(0, 0, 0)));
		hconcat(result, rclip, result);
		break;
	}
	case 1:
	{
		img.copyTo(result);
		Mat tclip(Mat(1, width, img.type(), Scalar(0, 0, 0)));
		img.row(0).copyTo(tclip);
		for (size_t i = 0; i < top; i++)
		{
			vconcat(tclip, result, result);
		}

		Mat bclip(Mat(1, width, img.type(), Scalar(0, 0, 0)));
		img.row(height-1).copyTo(bclip);
		for (size_t i = 0; i < bottom; i++)
		{
			vconcat(result, bclip, result);
		}

		Mat lclip(Mat(top + height + bottom, 1, img.type(), Scalar(0, 0, 0)));
		result.col(0).copyTo(lclip);
		for (size_t i = 0; i < left; i++)
		{
			hconcat(lclip, result, result);
		}
		Mat rclip(Mat(top + height + bottom, 1, img.type(), Scalar(0, 0, 0)));
		result.col(left+width-1).copyTo(rclip);
		for (size_t i = 0; i < right; i++)
		{
			hconcat(result, rclip, result);
		}
		break;
		
	}
	case 2:
	{
		Mat tclip;
		img.rowRange(0, top).copyTo(tclip);
		flip(tclip, tclip, 0);
		vconcat(tclip, img, result);

		Mat bclip;
		img.rowRange(height-bottom-1, height-1).copyTo(bclip);
		flip(bclip, bclip, 0);
		vconcat(result, bclip, result);

		Mat lclip;
		result.colRange(0, left).copyTo(lclip);
		flip(lclip, lclip, 1);
		hconcat(lclip, result, result);

		Mat rclip;
		result.colRange(left + width - right- 1, left + width - 1).copyTo(rclip);
		flip(rclip, rclip, 1);
		hconcat(result, rclip, result);
		break;
	}
	default:
		break;
	}
}


void conv2D(Mat &img, Mat &result, Mat &kernel, int padding, int h_stride, int v_stride) {
	int width = img.cols;
	int height = img.rows;
	int k_w = kernel.cols;
	int k_h = kernel.rows;

	Mat corp(kernel);
	Mat temp;
	Mat image;
	padImage(img, image, k_h, k_h, k_w, k_w, padding);
	//copyMakeBorder(img, image, (k_h - 1) / 2, (k_h - 1) / 2, (k_w - 1) / 2, (k_w - 1) / 2)
	img.copyTo(result);
	for (size_t i = 0; i < width; i=i+h_stride)
	{
		for (size_t j = 0; j < height; j=j+v_stride)
		{
			corp = image(Rect(i, j, k_w, k_h));
			temp = corp.mul(kernel);
			result.ptr<uchar>(j, i)[0] = (uchar)getMatMean(temp);
		}
	}


}

