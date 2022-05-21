/*------------------------------------------------------/
* Image Proccessing with Deep Learning
* OpenCV : Threshold using Trackbar Demo
* Created: 2021-Spring
------------------------------------------------------*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Global variables for Threshold
int threshold_value = 0;
int threshold_type = 0;
int morphology_type = 0;

int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

// Global variables for Morphology
int element_shape = MORPH_RECT;		// MORPH_RECT, MORPH_ELIPSE, MORPH_CROSS
int n = 3;
Mat element = getStructuringElement(element_shape, Size(n, n));

Mat src, src_gray, dst, dst_morph, dst_Gaussian;


// Trackbar strings
String window_name = "Threshold & Morphology Demo";
String trackbar_type = "Thresh Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Invertd";
String trackbar_value = "Thresh Value";
String trackbar_morph = "Morph Type 0: None \n 1: erode \n 2: dilate \n 3: close \n 4: open";

// Function headers
void Threshold_Demo(int, void*);
void Morphology_Demo(int, void*);
void contour_Demo(const Mat& _src);

int main()
{
	// Load an image
	src = imread("../../Image/Lab_GrayScale_TestImage.jpg", IMREAD_COLOR);

	// Convert the image to Gray
	cvtColor(src, src_gray, CV_BGR2GRAY);
	cv::GaussianBlur(src_gray, dst_Gaussian, cv::Size(9, 9), 10, 0);
	namedWindow("Gaussian", WINDOW_NORMAL);
	imshow("Gaussian", dst_Gaussian);
	resizeWindow("Gaussian", Size(600, 600));


	// Create a window to display the results
	namedWindow(window_name, WINDOW_NORMAL);
	resizeWindow(window_name, Size(800, 500));

	// Create trackbar to choose type of threshold
	createTrackbar(trackbar_type, window_name, &threshold_type, max_type, Threshold_Demo);
	createTrackbar(trackbar_value, window_name, &threshold_value, max_value, Threshold_Demo);
	createTrackbar(trackbar_morph, window_name, &morphology_type, max_type, Morphology_Demo); //& �ּҸ� �޾ƿͼ� ����ϱ⿡ while ���� ��� ����ؼ� ��밡�ɤ�

	// Call the function to initialize
	Threshold_Demo(0, 0);
	Morphology_Demo(0, 0);
	//adaptiveThreshold(src, dst, 155, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 11);
	
	//plotHist(src, "src", src.cols, src.rows);
	// Wait until user finishes program
	while (true) {
		int c = waitKey(20);
		if (c == 27)
			break;
	}
}


void Threshold_Demo(int, void*)	// default form of callback function for trackbar
{
	/*
	* 0: Binary
	* 1: Threshold Truncated
	* 2: Threshold to Zero
	* 3: Threshold to Zero Inverted
	*/

	threshold(dst_Gaussian, dst, threshold_value, max_BINARY_value, threshold_type);
	imshow(window_name, dst);
}

void Morphology_Demo(int, void*)  // default form of callback function for trackbar
{
	/*
	* 0: None
	* 1: Erode
	* 2: Dilate
	* 3: Close
	* 4: Open
	*/
	switch (morphology_type) {
	case 0: dst.copyTo(dst_morph);	break;
	case 1: erode(dst, dst_morph, element); break;
	case 2: dilate(dst, dst_morph, element); break;
	case 3: morphologyEx(dst, dst_morph, CV_MOP_OPEN, element); break;
	case 4: morphologyEx(dst, dst_morph, CV_MOP_CLOSE, element); break;
	}
	imshow(window_name, dst_morph);
	contour_Demo(dst_morph); //dst_morph �� ���� ���� binary image 
}



// ��� ������ ���е� binary image�� input Mat ���� �־���� �����Ѵ�.
// �ܺο��� src�� �ǵ帮�� ���ϰ� const
// & �ּ� ���� �����ϱ� ������ �� ������ ���۰����ϴ�.

void contour_Demo(const Mat& _src) {

	// dst: binary image
	// pointer ���� array
	vector<vector<Point>> contours;

	/// Find contours

	// _src binary ������ ������ contours ���ؼ� ��� ã��, CV_RETR_EXTERNAL-> �ֿܰ� ������ ã�´�. ��, �� ���ο� �ִ� ���� ƴ���� �ȱ׷�����.
	// ���� ���� �����ϰ� �����ϰ� �ʹٸ� -> findcontour doocumentation ���� ������ ��
	findContours(_src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw all contours excluding holes
	//������
	Mat drawing(_src.size(), CV_8U, Scalar(255));

	// ��� ��濡 contours �׷���
	drawContours(drawing, contours, -1, Scalar(0));
	//drawContours(drawing, contours, -1, Scalar(0), CV_FILLED);
	cout << " Number of coins are =" << contours.size() << endl;

	//for (int i = 0; i < contours.size(); i++)
	//{
	//	printf(" * Contour[%d] -  Area OpenCV: %.2f - Length: %.2f \n", i, contourArea(contours[i]), arcLength(contours[i], true));
	//}

	for (int i = 0; i < contours.size(); i++)
	{
		cout << i << " = " << endl;
		cout << contours[i] << endl;
		//printf(" * Contour[%c] -  Area OpenCV: %.2f - Length: %.2f \n", contours[i], contours[],contourArea(contours[i]), arcLength(contours[i], true));
	}

	//contourArea -> ���� 
	//arcLength -> ��輱�� ����

	//���� �� �� ������ ��輱�� Ȱ���ؼ� 3���� �ٸ� type���� �з��ϴ� �˰��� �����ϸ� �� ��.
	//������ ��� Ȱ�� �� �� -> ����� �������� bolt �� �پ��ִ� bolt ��� ó���� ��
	//contour �� ���� point�� �մ� �ϳ��� object -> ��, �ϳ��� object�� ���� ��ǥ�� ��翡 ���� ������ ������ �� �� �ִ�.
	namedWindow("contour", 0);
	imshow("contour", drawing);

}

void plotHist(Mat src, string plotname, int width, int height) {
	/// Compute the histograms 
	Mat hist;
	/// Establish the number of bins (for uchar Mat type)
	int histSize = 256;
	/// Set the ranges (for uchar Mat type)
	float range[] = { 0, 256 };

	const float* histRange = { range };
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	double min_val, max_val;
	cv::minMaxLoc(hist, &min_val, &max_val);
	Mat hist_normed = hist * height / max_val;
	float bin_w = (float)width / histSize;
	Mat histImage(height, width, CV_8UC1, Scalar(0));
	for (int i = 0; i < histSize - 1; i++) {
		line(histImage,
			Point((int)(bin_w * i), height - cvRound(hist_normed.at<float>(i, 0))),
			Point((int)(bin_w * (i + 1)), height - cvRound(hist_normed.at<float>(i + 1, 0))),
			Scalar(255), 2, 8, 0);
	}

	imshow(plotname, histImage);
}