#include <iostream>
#include <opencv.hpp>
#include "cameraParam.h"

using namespace cv;

void main()
{
	cameraParam param("../../calibration resources/sourceCode/calibTest.xml");
	Mat src = imread("../../calibration resources/sourceCode/calibTest.jpg");
	Mat dst = param.undistort(src);
	
	imshow("src", src);
	imshow("dst", dst);
	 
	waitKey(0);
}
