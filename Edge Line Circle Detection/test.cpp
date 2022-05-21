#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const char* window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));

	//Trackbar을 움직일 때 lowThreshold를 바꾸는 것이다.
	//Threshold 1 은 2 보다 작다. 1은 먼저 약한 edge를 뽑아내고, 2를 통해서 강한 edge를 출력 겿가지제거
	//따라서 적절히 Threshold 1,2 값을 조절해야한다.
	//line detect 할때는 굵은 선(강한 것만 남으니까 그냥 ratio 1 로 두고 Threshold 1,2 같게 한다,)
	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

//opencv 세타는 0~pi
//


/** @function main */
int main(int argc, char** argv)
{
	/// Load an image
	//const char* filename = "../images/pillsetc.png";
	const char* filename = "../../Image/coins.png";
	//const char* filename = "../images/TrafficSign1.png";

	/// Read the image
	src = imread(filename, 1);

	if (!src.data)
	{
		return -1;
	}

	/// Create a matrix of the same type and size as src (for dst)
	dst.create(src.size(), src.type());

	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Create a window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// Create a Trackbar for user to enter threshold
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	/// Show the image
	CannyThreshold(0, 0);

	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}