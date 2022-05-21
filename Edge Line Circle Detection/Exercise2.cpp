//#include <opencv2/opencv.hpp>
//using namespace cv;
//using namespace std;
//
//int main(int argc, char** argv)
//{
//	Mat src, gray;
//
//	String filename = "../../Image/TrafficSign1.png";
//
//	/* Read the image */
//	src = imread(filename, 1);
//
//	if (!src.data)
//	{
//		printf(" Error opening image\n");
//		return -1;
//	}
//
//	cvtColor(src, gray, COLOR_BGR2GRAY);
//
//	/* smooth it, otherwise a lot of false circles may be detected */
//	GaussianBlur(gray, gray, Size(9, 9), 2, 2);
//
//	vector<Vec3f> circles;
//	//n x 3 의 float type 2D-Array
//
//	//printf("length = %d\n", gray.rows);
//	HoughCircles(gray, circles, 3, 2, 200, 180, 100);
//	//minDist 원의 중심으로 부터 다른 원을 인식하는 거리. minDist 짧을 수록 많은 원을 인식하게 된다.
//	//min, max Radius는 미리 알고 있는 object의 정보를 알고 있으면 편하다.
//
//	for (size_t i = 0; i < circles.size(); i++)
//	{
//		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//		int radius = cvRound(circles[i][2]);
//
//		/* draw the circle center */
//		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//
//		/* draw the circle outline */
//		//circle(src, center, radius, Scalar(255, 0,0), 3, 8, 0);
//	}
//
//	namedWindow("circles", 1);
//	imshow("circles", src);
//
//	/* Wait and Exit */
//	waitKey();
//	return 0;
//}