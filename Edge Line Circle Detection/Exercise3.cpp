//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//using namespace cv;
//using namespace std;
//
//int main(int argc, char** argv)
//{
//	// Declare the output variables
//	Mat dst, cdst, cdstP, detected_edges;
//
//	// Loads an image
//	const char* filename = "../../Image/track_gray.jpg";
//	Mat src = imread(filename, IMREAD_GRAYSCALE);
//
//	// Check if image is loaded fine
//	if (src.empty()) {
//		printf(" Error opening image\n");
//		return -1;
//	}
//
//	blur(src, detected_edges, Size(3, 3));
//
//	// Edge detection
//	Canny(detected_edges, dst, 100, 210, 3);
//
//	// Copy edge results to the images that will display the results in BGR
//	cvtColor(dst, cdst, COLOR_GRAY2BGR);
//	cdstP = cdst.clone();
//
//	// (Option 1) Standard Hough Line Transform
//	vector<Vec2f> lines; //float type의 열이 2개 백터단위로 쭉 있는거 --> 한마디로 n X 2 2D-Array
//	HoughLines(dst, lines, 1, CV_PI / 180, 170, 0, 0); //선 막 그어져 있는거
//	//HoughLines 의 1은 선과 어떤 보고자하는 픽셀 사이 거리가 1 pixel이면 같은 line이라고 하자. 
//	//CV_PI / 180 => 1도씩 확인하라.
//
//	// Draw the detected lines
//	for (size_t i = 0; i < lines.size(); i++)
//	{
//		float rho = lines[i][0], theta = lines[i][1];
//		Point pt1, pt2;
//		double a = cos(theta), b = sin(theta);
//		double x0 = a * rho, y0 = b * rho;
//		pt1.x = cvRound(x0 + 1000 * (-b));
//		pt1.y = cvRound(y0 + 1000 * (a));
//		pt2.x = cvRound(x0 - 1000 * (-b));
//		pt2.y = cvRound(y0 - 1000 * (a));
//		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA); //이미지에 선을 그리는 함수 drawcontour 와 비슷 , 3-> 두께 
//	}
//
//	// (Option 2) Probabilistic Line Transform
//	vector<Vec4i> linesP;
//	HoughLinesP(dst, linesP, 1, CV_PI / 180, 200, 100, 30); //정돈 된 것
//	//dst -> edge image 50 => 길이에 대한 threshold , 10 => line gap 10 즉, gap 이 10 이내에 있으면 같은 line 으로 간주 -> 조건에
//	//해당되는 라인만 출력
//
//	// Draw the lines
//	for (size_t i = 0; i < linesP.size(); i++)
//	{
//		Vec4i l = linesP[i];
//		line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
//	}
//
//	// Show results
//	imshow("Source", src);
//	imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
//	imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
//
//	// Wait and Exit
//	waitKey();
//	return 0;
//}