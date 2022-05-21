////#include "opencv2/video/tracking.hpp"
////#include "opencv2/imgproc/imgproc.hpp"
////#include "opencv2/highgui/highgui.hpp"
////#include <ctype.h>
//#include <iostream>
//#include <opencv2/opencv.hpp>
//
//using namespace cv;
//using namespace std;
//
//Mat image,src;
//Point origin;
//Rect selection;
//bool selectObject = false;
//bool trackObject = false;
//int hmin = 1, hmax = 179, smin = 0, smax = 0, vmin = 0, vmax = 0;
//
///// On mouse event 
//static void onMouse(int event, int x, int y, int, void*);
//
//int main()
//{
//
//	VideoCapture cap(0);
//
//	//미리 카메라 해상도 사이즈 결정해주어도 가능
//	int cam_W = 640;
//	int cam_H = 480;
//	cap.set(CAP_PROP_FRAME_WIDTH, cam_W);
//	cap.set(4, cam_H);
//
//
//	if (!cap.isOpened())	// if not success, exit the programm
//	{
//		cout << "Cannot open the video cam\n";
//		return -1;
//	}
//
//	/*  read a new frame from video  */
//	//bool bSuccess = cap.read(src);
//	//Mat dst_track = Mat::zeros(src.size(), CV_8UC3);
//	Mat dst_track = Mat::zeros(cam_H,cam_W, CV_8UC3);
//	//namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);
//
//	Mat image_disp, hsv, hue, mask, dst;
//	vector<vector<Point> > contours;
//
//	//image = imread("../../Image/color_ball.jpg");
//	//image.copyTo(image_disp);
//
//	//Mat dst_track = Mat::zeros(image.size(), CV_8UC3);
//
//	// TrackBar 설정
//	namedWindow("Source", 0);
//	setMouseCallback("Source", onMouse, 0);		//
//	createTrackbar("Hmin", "Source", &hmin, 179, 0);
//	createTrackbar("Hmax", "Source", &hmax, 179, 0);
//	createTrackbar("Smin", "Source", &smin, 255, 0);
//	createTrackbar("Smax", "Source", &smax, 255, 0);
//	createTrackbar("Vmin", "Source", &vmin, 255, 0);
//	createTrackbar("Vmax", "Source", &vmax, 255, 0);
//
//	int key = 0;
//	int kernel_size = 11;
//	int draw_type = 0;
//	while (true)
//	{
//
//		Mat  dst;
//
//		/*  read a new frame from video  */
//		bool bSuccess = cap.read(src);
//
//		if (!bSuccess)	// if not success, break loop
//		{
//			cout << "Cannot find a frame from  video stream\n";
//			break;
//		}
//
//		src.copyTo(image_disp);
//
//		/*Mat dst_track = Mat::zeros(src.size(), CV_8UC3);*/
//
//		imshow("Source", src);
//		/******** Convert BGR to HSV ********/
//		// input mat: image
//		// output mat: hsv
//		// cvtColor(               ); //// YOUR CODE GOES HERE
//		cvtColor(src, hsv, COLOR_BGR2HSV); //BRG -> HSV 로 변환 하는 과정
//
//		/******** Add Pre-Processing such as filtering etc  ********/
//		// YOUR CODE GOES HERE
//		// YOUR CODE GOES HERE
//
//
//
//		/// set dst as the output of InRange
//		inRange(hsv, Scalar(MIN(hmin, hmax), MIN(smin, smax), MIN(vmin, vmax)),
//			Scalar(MAX(hmin, hmax), MAX(smin, smax), MAX(vmin, vmax)), dst);
//		//inRange 함수를 거쳐서 출력된 dst 는 binary , data type =  8UC1 , 범위안에서 정의된 것만 출력하므로
//
//
//		/******** Add Post-Processing such as morphology etc  ********/
//		// YOUR CODE GOES HERE
//		// YOUR CODE GOES HERE
//
//
//
//		namedWindow("InRange", 0);
//		imshow("InRange", dst);
//
//		/// once mouse has selected an area bigger than 0
//		if (trackObject)
//		{
//			trackObject = false;					// Terminate the next Analysis loop
//			Mat roi_HSV(hsv, selection); 			// Set ROI by the selection box		Select 는 rectangle box 임. 거기에 해당되는 이미지만 가져온다.
//			Scalar means, stddev;
//			meanStdDev(roi_HSV, means, stddev);		//roi_HSV 의 MENA 값과 표준편차 찾아준다.
//			cout << "\n Selected ROI Means= " << means << " \n stddev= " << stddev;
//
//			// Change the value in the trackbar according to Mean and STD //
//			hmin = MAX((means[0] - stddev[0]), 0);
//			hmax = MIN((means[0] + stddev[0]), 179);
//			smin = MAX((means[1] - stddev[1]), 0);
//			smax = MIN((means[1] + stddev[1]), 255);
//			vmin = MAX((means[2] - stddev[2]), 0);
//			vmax = MIN((means[2] + stddev[2]), 255);
//
//			setTrackbarPos("Hmin", "Source", hmin);  //setTrackbarPos 해야 hmin 값 실제로 적용된다.
//			setTrackbarPos("Hmax", "Source", hmax);
//			setTrackbarPos("Smin", "Source", smin);
//			setTrackbarPos("Smax", "Source", smax);
//			setTrackbarPos("Vmin", "Source", vmin);
//			setTrackbarPos("Vmax", "Source", vmax);
//
//			/******** Repeat for S and V trackbar ********/
//			// YOUR CODE GOES HERE
//			// YOUR CODE GOES HERE
//			// YOUR CODE GOES HERE
//
//		}
//
//
//		//드래그 반전하기 위한 코드
//		if (selectObject && selection.area() > 0)  // Left Mouse is being clicked and dragged
//		{
//			// Mouse Drag을 화면에 보여주기 위함
//			Mat roi_RGB(image_disp, selection);
//			bitwise_not(roi_RGB, roi_RGB);
//			imshow("Source", image_disp);
//		}
//		src.copyTo(image_disp);
//
//		key = waitKey(30);
//			if (key == 27) // wait for 'ESC' press for 30ms. If 'ESC' is pressed, break loop
//			{
//				cout << "ESC key is pressed by user\n";
//				break;
//			}
//			//else if (key == 'c' || key == 'C')
//			//{
//			//	draw_type = 1;	// blur
//			//}
//			//else if (key == 'z' || key == 'Z')
//			//{
//			//	Mat dst_track = Mat::zeros(src.size(), CV_8UC3);
//			//}
//		
//		///  Find All Contour   ///
//		findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//		
//		if (contours.size() > 0)
//		{
//			/// Find the Contour with the largest area ///
//			double maxArea = 0;
//			int maxArea_idx = 0;
//
//			for (int i = 0; i < contours.size(); i++)
//				if (contourArea(contours[i]) > maxArea) {
//					maxArea = contourArea(contours[i]);
//					maxArea_idx = i;
//				}
//
//			///  Draw the max Contour on Black-background  Image ///
//			Mat dst_out = Mat::zeros(dst.size(), CV_8UC3);
//			drawContours(dst_out, contours, maxArea_idx, Scalar(0, 0, 255), 2, 8);
//			namedWindow("Contour", 0);
//			imshow("Contour", dst_out);
//
//
//			/// Draw the Contour Box on Original Image ///
//			Rect boxPoint = boundingRect(contours[maxArea_idx]);
//			rectangle(image_disp, boxPoint, Scalar(255, 0, 255), 3);
//			namedWindow("Contour_Box", 0);
//			imshow("Contour_Box", image_disp);
//
//			//if (draw_type == 1) {
//			/// Continue Drawing the Contour Box  ///
//			rectangle(dst_track, boxPoint, Scalar(255, 0, 255), 3);
//			namedWindow("Contour_Track", 0);
//			imshow("Contour_Track", dst_track);
//			dst_track *= 0.95;
//			//}
//		}
//		// 잔상 사라지는 범 분홍색깔 intensity 0으로 감소하도록 왜냐면 검은색 되면 되니까 배경이 검은색
//
//
//		//char c = (char)waitKey(10);
//		//if (c == 27)
//		//	break;
//		//else if (key == 'r' || key == 'R')
//		//		{
//		//	dst_track = Mat::zeros(cam_H, cam_W, CV_8UC3);
//		//		}
//		//else if (key == 'c' || key == 'C')
//		//		{
//		//			draw_type = 1;	// blur
//		//		}
//	} // end of for(;;)
//
//	return 0;
//}
//
//
//
///// On mouse event 
//static void onMouse(int event, int x, int y, int, void*)
//{
//	if (selectObject)  // for any mouse motion
//	{
//		//드래그한 좌표의 정보를 가져오는 과정
//		selection.x = MIN(x, origin.x);
//		selection.y = MIN(y, origin.y);
//		selection.width = abs(x - origin.x) + 1; 
//		selection.height = abs(y - origin.y) + 1;
//		selection &= Rect(0, 0, src.cols, src.rows);  /// Bitwise AND  check selectin is within the image coordinate 
//		// 이미지 밖으로 나가지 못하게 하는 안전 장치
//	}
//
//	switch (event)
//	{
//	case CV_EVENT_LBUTTONDOWN:   //누르는 그 순간만 활성화 된다.
//		selectObject = true;
//		origin = Point(x, y);   //origin 누르는 첫 시작 위치
//		break;
//	case CV_EVENT_LBUTTONUP:
//		selectObject = false;
//		if (selection.area())  //마우스 커서 드래그 끝나면 다시 false 되고 area 구하는 과정 실행된다.
//			trackObject = true;
//		break;
//	}
//}