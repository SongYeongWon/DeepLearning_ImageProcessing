///*
//
//	DLIP MIDTERM 2022  Submission
//
//
//	NAME:Song Yeong Won
//
//*/
//#include <iostream>
//#include <opencv2/opencv.hpp>
//
//using namespace cv;
//using namespace std;
//
//// Default Morphology iteration value
//int opening_iteration = 1;
//int closing_iteration = 0;
//int erode_iteration = 20;
//int dilation_iteration = 20;
//
//Mat image;
//Point origin;
//Mat dstSegment, drawing_copy, dst, value_gray, Mask_out, New;
//
//int hmin = 1, hmax = 14, smin = 43, smax = 255, vmin = 0, vmax = 255;
//
//void Morphology_Demo(int, void*, int iteration);
//void contour_Demo(const Mat& _src);
//
//// Global variables for Morphology
//int element_shape = MORPH_RECT;		// MORPH_RECT, MORPH_ELIPSE, MORPH_CROSS
//int n = 3;
//Mat element = getStructuringElement(element_shape, Size(n, n));
//vector<Mat>  channels;
//
//
//int main()
//{
//	Mat image_disp1, hsv, hue, mask;
//	vector<vector<Point> > contours;
//	/******** Segmatation of Facial Area  ********/
//	//Segmentatation result is in B/W image, with white for facial area
//
//	image = imread("../../Image/groupimage_crop.jpg");
//	image.copyTo(image_disp1);
//
//	imshow("Source", image);
//	cvtColor(image, hsv, COLOR_BGR2HSV); //BRG -> HSV 로 변환 하는 과정
//
//
//	/// set dst as the output of InRange
//	inRange(hsv, Scalar(MIN(hmin, hmax), MIN(smin, smax), MIN(vmin, vmax)),
//		Scalar(MAX(hmin, hmax), MAX(smin, smax), MAX(vmin, vmax)), dst);
//
//	Morphology_Demo(3, 0, opening_iteration);
//
//	//Save the contour image as a new image after thresholding
//	drawing_copy.copyTo(dst);
//
//	Morphology_Demo(1, 0, erode_iteration);
//	Morphology_Demo(2, 0, dilation_iteration);
//
//	dst.copyTo(dstSegment);
//
//	namedWindow("InRange", 0);
//	imshow("InRange", dst);
//
//	image.copyTo(image_disp1);
//	cvtColor(dstSegment, New, COLOR_GRAY2BGR);
//	namedWindow("InRange", 0);
//	imshow("InRange", image - New);
//
//	Mat Mask = Mat::zeros(image.size(), CV_8UC3);
//	bitwise_and(image, New, Mask);
//	int i = 9;
//	medianBlur(Mask, Mask_out, i);
//
//	//namedWindow("InRange", 0);
//	//imshow("InRange", image - New + Mask_out);
//	Mat last = image - New + Mask_out;
//	namedWindow("InRange", 0);
//	imshow("InRange", last);
//
//	//split(hsv, channels); //split to h,s,v
//	//channels[2].copyTo(value_gray); //Copy value gray scale image to Mat value_gray
//	//cvtColor(image, value_gray, COLOR_BGR2GRAY);
//	////namedWindow("value_gray", 1);
//	////imshow("value_gray", value_gray);
//	//Mat Mask = Mat::zeros(value_gray.size(), CV_8UC1);
//	//bitwise_and(value_gray, dstSegment, Mask);
//
//	//cvtColor(Mask, New, COLOR_GRAY2BGR);
//	//New.convertTo(New, CV_8UC3);
//	////int i = 5;
//	////medianBlur(New, Mask_out, i);
//	//namedWindow("New", 1);
//	//imshow("New", New);
//
//	
//
//	while (true)
//	{
//
//		/******** Draw Rectangles around Faces  ********/
//		// Show Tight Fit Rectangle on both  source image(src) and on (dstSegment)
//			///  Find All Contour   ///
//		findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//
//		if (contours.size() > 0)
//		{
//			///  Draw the max Contour on Black-background  Image ///
//			Mat dst_out = Mat::zeros(dst.size(), CV_8UC3);
//			drawContours(dst_out, contours, -1, Scalar(0, 0, 255), 2, 8);
//			namedWindow("Contour", 0);
//			imshow("Contour", dst_out);
//
//			/// Draw the Contour Box on Original Image ///
//			Rect boxPoint1 = boundingRect(contours[0]);
//			Rect boxPoint2 = boundingRect(contours[1]);
//			Rect boxPoint3 = boundingRect(contours[2]);
//			Rect boxPoint4 = boundingRect(contours[3]);
//
//			rectangle(last, boxPoint1, Scalar(255, 0, 255), 3);
//			rectangle(last, boxPoint2, Scalar(255, 0, 255), 3);
//			rectangle(last, boxPoint3, Scalar(255, 0, 255), 3);
//			rectangle(last, boxPoint4, Scalar(255, 0, 255), 3);
//
//			rectangle(dstSegment, boxPoint1, Scalar(255, 0, 255), 3);
//			rectangle(dstSegment, boxPoint2, Scalar(255, 0, 255), 3);
//			rectangle(dstSegment, boxPoint3, Scalar(255, 0, 255), 3);
//			rectangle(dstSegment, boxPoint4, Scalar(255, 0, 255), 3);
//
//			namedWindow("Contour_Box", 0);
//			imshow("Contour_Box", last);
//
//			namedWindow("Contour_dstSegment", 0);
//			imshow("Contour_dstSegment", dstSegment);
//
//		/*	int init_x = boxPoint1.x;
//			int init_y = boxPoint1.y;
//			int width =  boxPoint1.width;
//			int height = boxPoint1.height;*/
//
//			//int Value = 0;
//			//for (int v = 100; v < init_x+ width; v++) {
//			//	for (int u = 100; u < init_y+ height; u++) {
//			//		image_disp1.at<uchar>(u, v) = 0;
//			//		//printf("dd=%d\n", init_x);
//			//		}
//			//}
//			//int pixel_temp_r, pixel_temp_g, pixel_temp_b;
//			//int cnt = 0;
//			//for (int v = init_x; v < init_x + width; v++)
//			//{
//			//	uchar* img_data = image.ptr<uchar>(v);
//			//	for (int u = init_y; u < (init_y + height) * image.channels(); u = u + 3)
//			//	{
//			//		pixel_temp_r = img_data[u];
//			//		pixel_temp_g = img_data[u + 1];
//			//		pixel_temp_b = img_data[u + 2];
//			//		img_data[u] = 0; //B
//			//		img_data[u + 1] = 0; //G
//			//		img_data[u + 2] = 0; //R
//			//	}
//			//}
//
//
//		}
//		/*namedWindow("image", 0);
//		imshow("image", image);*/
//		//namedWindow("re", 1);
//	//imshow("re", image - Mask_out);
//		char c = (char)waitKey(10);
//		if (c == 27)
//			break;
//	}
//
//	return 0;
//}
//
//
//
//void Morphology_Demo(int morphology_type, void*, int iteration)  // default form of callback function for trackbar
//{
//	/*
//	* 0: None
//	* 1: Erode
//	* 2: Dilate
//	* 3: Close
//	* 4: Open
//	*/
//
//	switch (morphology_type) {
//	case 0: dst.copyTo(dst);	break;
//	case 1: erode(dst, dst, element, Point(-1, -1), iteration); break;
//	case 2: dilate(dst, dst, element, Point(-1, -1), iteration); break;
//	case 3: morphologyEx(dst, dst, CV_MOP_OPEN, element, Point(-1, -1), iteration); break;
//	case 4: morphologyEx(dst, dst, CV_MOP_CLOSE, element, Point(-1, -1), iteration); break;
//	}
//	contour_Demo(dst);
//
//}
//
//void contour_Demo(const Mat& _src) {
//
//	vector<vector<Point>> contours;
//
//	findContours(_src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	/// Draw all contours excluding holes
//	Mat drawing(_src.size(), CV_8U, Scalar(0));
//
//	drawContours(drawing, contours, -1, Scalar(255), CV_FILLED);
//	cout << " Number of Face are =" << contours.size() << endl;
//
//	for (int i = 0; i < contours.size(); i++)
//	{
//		printf(" * Contour[%d] -  Area OpenCV: %.2f - Length: %.2f \n", i, contourArea(contours[i]), arcLength(contours[i], true));
//	}
//	//namedWindow("contour", 0);
//	//imshow("contour", drawing);
//	drawing.copyTo(drawing_copy);
//
//}