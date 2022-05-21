/*------------------------------------------------------/
* Image Proccessing with Deep Learning
* OpenCV : Filter Demo - Video
* Created: 2021-Spring
------------------------------------------------------*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat image;
Point origin;
Rect selection;
bool selectObject = false;
bool trackObject = false;
int hmin = 1, hmax = 179, smin = 30, smax = 255, vmin = 0, vmax = 255;

/// On mouse event 
static void onMouse(int event, int x, int y, int, void*);


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
	int kernel_size = 11;
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


		//	key = waitKey(30);
		//	if (key == 27) // wait for 'ESC' press for 30ms. If 'ESC' is pressed, break loop
		//	{
		//		cout << "ESC key is pressed by user\n";
		//		break;
		//	}
		//	else if (key == 'b' || key == 'B')
		//	{
		//		filter_type = 1;	// blur
		//	}

		//	/* Keyboard input for other filters*/
		//	// ADD YOUR CODE HERE
		//	// ADD YOUR CODE HERE
		//	// ADD YOUR CODE HERE

		//	if (filter_type == 1)
		//		blur(src, dst, cv::Size(kernel_size, kernel_size), cv::Point(-1, -1));
		//	/* Apply filter accroding to filter type */
		//	// ADD YOUR CODE HERE
		//	// ADD YOUR CODE HERE
		//	else
		//		src.copyTo(dst);

		//	imshow("MyVideo", dst);
		//}

		Mat image_disp, hsv, hue, mask, dst;
		vector<vector<Point> > contours;

	/*	image = imread("../../Image/color_ball.jpg");*/
		dst.copyTo(image_disp);

		Mat dst_track = Mat::zeros(dst.size(), CV_8UC3);

		// TrackBar ����
		namedWindow("Source", 0);
		setMouseCallback("Source", onMouse, 0);		//
		createTrackbar("Hmin", "Source", &hmin, 179, 0);
		createTrackbar("Hmax", "Source", &hmax, 179, 0);
		createTrackbar("Smin", "Source", &smin, 255, 0);
		createTrackbar("Smax", "Source", &smax, 255, 0);
		createTrackbar("Vmin", "Source", &vmin, 255, 0);
		createTrackbar("Vmax", "Source", &vmax, 255, 0);

		// end of for(;;)
		imshow("Source", dst);
		/******** Convert BGR to HSV ********/
		// input mat: image
		// output mat: hsv
		// cvtColor(               ); //// YOUR CODE GOES HERE
		cvtColor(dst, hsv, COLOR_BGR2HSV); //BRG -> HSV �� ��ȯ �ϴ� ����

		/******** Add Pre-Processing such as filtering etc  ********/
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE



		/// set dst as the output of InRange
		inRange(hsv, Scalar(MIN(hmin, hmax), MIN(smin, smax), MIN(vmin, vmax)),
			Scalar(MAX(hmin, hmax), MAX(smin, smax), MAX(vmin, vmax)), dst);
		//inRange �Լ��� ���ļ� ��µ� dst �� binary , data type =  8UC1 , �����ȿ��� ���ǵ� �͸� ����ϹǷ�


		/******** Add Post-Processing such as morphology etc  ********/
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE



		namedWindow("InRange", 0);
		imshow("InRange", dst);

		/// once mouse has selected an area bigger than 0
		if (trackObject)
		{
			trackObject = false;					// Terminate the next Analysis loop
			Mat roi_HSV(hsv, selection); 			// Set ROI by the selection box		Select �� rectangle box ��. �ű⿡ �ش�Ǵ� �̹����� �����´�.
			Scalar means, stddev;
			meanStdDev(roi_HSV, means, stddev);		//roi_HSV �� MENA ���� ǥ������ ã���ش�.
			cout << "\n Selected ROI Means= " << means << " \n stddev= " << stddev;

			// Change the value in the trackbar according to Mean and STD //
			hmin = MAX((means[0] - stddev[0]), 0);
			hmax = MIN((means[0] + stddev[0]), 179);
			smin = MAX((means[1] - stddev[1]), 0);
			smax = MIN((means[1] + stddev[1]), 255);
			vmin = MAX((means[2] - stddev[2]), 0);
			vmax = MIN((means[2] + stddev[2]), 255);

			setTrackbarPos("Hmin", "Source", hmin);  //setTrackbarPos �ؾ� hmin �� ������ ����ȴ�.
			setTrackbarPos("Hmax", "Source", hmax);
			setTrackbarPos("Smin", "Source", smin);
			setTrackbarPos("Smax", "Source", smax);
			setTrackbarPos("Vmin", "Source", vmin);
			setTrackbarPos("Vmax", "Source", vmax);

			/******** Repeat for S and V trackbar ********/
			// YOUR CODE GOES HERE
			// YOUR CODE GOES HERE
			// YOUR CODE GOES HERE

		}


		//�巡�� �����ϱ� ���� �ڵ�
		if (selectObject && selection.area() > 0)  // Left Mouse is being clicked and dragged
		{
			// Mouse Drag�� ȭ�鿡 �����ֱ� ����
			Mat roi_RGB(image_disp, selection);
			bitwise_not(roi_RGB, roi_RGB);
			imshow("Source", image_disp);
		}
		image.copyTo(image_disp);



		///  Find All Contour   ///
		findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		if (contours.size() > 0)
		{
			/// Find the Contour with the largest area ///
			double maxArea = 0;
			int maxArea_idx = 0;

			for (int i = 0; i < contours.size(); i++)
				if (contourArea(contours[i]) > maxArea) {
					maxArea = contourArea(contours[i]);
					maxArea_idx = i;
				}

			///  Draw the max Contour on Black-background  Image ///
			Mat dst_out = Mat::zeros(dst.size(), CV_8UC3);
			drawContours(dst_out, contours, maxArea_idx, Scalar(0, 0, 255), 2, 8);
			namedWindow("Contour", 0);
			imshow("Contour", dst_out);


			/// Draw the Contour Box on Original Image ///
			Rect boxPoint = boundingRect(contours[maxArea_idx]);
			rectangle(image_disp, boxPoint, Scalar(255, 0, 255), 3);
			namedWindow("Contour_Box", 0);
			imshow("Contour_Box", image_disp);


			/// Continue Drawing the Contour Box  ///
			rectangle(dst_track, boxPoint, Scalar(255, 0, 255), 3);
			namedWindow("Contour_Track", 0);
			imshow("Contour_Track", dst_track);
		}

		char c = (char)waitKey(10);
		if (c == 27)
			break;


	}
	return 0;
}


/// On mouse event 
static void onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)  // for any mouse motion
	{
		//�巡���� ��ǥ�� ������ �������� ����
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = abs(x - origin.x) + 1;
		selection.height = abs(y - origin.y) + 1;
		selection &= Rect(0, 0, image.cols, image.rows);  /// Bitwise AND  check selectin is within the image coordinate 
		// �̹��� ������ ������ ���ϰ� �ϴ� ���� ��ġ
	}

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:   //������ �� ������ Ȱ��ȭ �ȴ�.
		selectObject = true;
		origin = Point(x, y);   //origin ������ ù ���� ��ġ
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.area())  //���콺 Ŀ�� �巡�� ������ �ٽ� false �ǰ� area ���ϴ� ���� ����ȴ�.
			trackObject = true;
		break;
	}
}