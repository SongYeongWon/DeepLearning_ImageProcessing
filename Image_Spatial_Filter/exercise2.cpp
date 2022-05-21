/*------------------------------------------------------/
* Image Proccessing with Deep Learning
* OpenCV : Filter Demo - Video
* Created: 2021-Spring
------------------------------------------------------*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	/*  open the video camera no.0  */
	VideoCapture cap(0);

	if (!cap.isOpened())	// if not success, exit the programm
	{
		cout << "Cannot open the video cam\n";
		return -1;
	}

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

	int key = 0;
	int kernel_size = 11; //u 누르면 홀수로 증가 또는 감소
	int filter_type = 0;
	while (1)
	{
		Mat src, dst;

		/*  read a new frame from video  */
		bool bSuccess = cap.read(src);

		if (!bSuccess)	// if not success, break loop
		{
			cout << "Cannot find a frame from  video stream\n";
			break;
		}


		key = waitKey(30);
		if (key == 27) // wait for 'ESC' press for 30ms. If 'ESC' is pressed, break loop
		{
			cout << "ESC key is pressed by user\n";
			break;
		}
		if (key == 'b' || key == 'B')
		{
			filter_type = 1;	// blur
			printf("filter_type = %d\n", filter_type);
		}
		/* Gaussian Filter */
		else if (key == 'g' || key == 'G')
		{
			filter_type = 2;	// blur
			printf("filter_type = %d\n", filter_type);
		}

		///* Median Filter */
		else if (key == 'm' || key == 'M')
		{
			filter_type = 3;	// blur
			printf("filter_type = %d\n", filter_type);
		}

		/* Laplacian Filter */
		else if (key == 'l' || key == 'L')
		{
			filter_type = 4;	// blur
			printf("filter_type = %d\n", filter_type);
		}

		/* 2D Convolution of a filter kernel */
		else if (key == 'q' || key == 'Q')
		{
			filter_type = 5;	// blur
			printf("filter_type = %d\n", filter_type);
		}

		else if (key == 'o' || key == 'O')
		{
			filter_type = 6;	// blur
			printf("filter_type = %d\n", filter_type);
		}
		///* Keyboard input for other filters*/

		if (filter_type == 1) {
			if (key == 'u' || key == 'U') {
				kernel_size++;
			}
			else if ((key == 'd' || key == 'D') && (kernel_size>1)){
				kernel_size--;
			}
			blur(src, dst, cv::Size(kernel_size, kernel_size), cv::Point(-1, -1));
			//printf(" kernel_size = %d", kernel_size);
		}
		else if (filter_type == 2)
			GaussianBlur(src, dst, cv::Size(9, 9), 0, 0);
		else if (filter_type == 3)
			medianBlur(src, dst, 3);
		else if (filter_type == 4) {
			int kernel_size = 3;
			int scale = 1;
			int delta = 0;
			int ddepth = CV_16S;
			cv::Laplacian(src, dst, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT);
			src.convertTo(src, CV_16S);
			cv::Mat result_laplcaian = src - dst;
			result_laplcaian.convertTo(result_laplcaian, CV_8U);
			dst = result_laplcaian;
		}

		else if (filter_type == 5) {

			src.convertTo(src, CV_8UC1);
			Mat kernel;
			int delta = 0;
			int ddepth = -1;
			//int kernel_size = 5;
			Point anchor = Point(-1, -1);
			kernel = Mat::ones(5, 5, CV_32F) / (float)(5 * 5);
			filter2D(src, dst, ddepth, kernel, anchor);

		}
		else if (filter_type == 6) {

			src.copyTo(dst);
		}


		else
			src.copyTo(dst);

		imshow("MyVideo", dst);
	}
	return 0;
}