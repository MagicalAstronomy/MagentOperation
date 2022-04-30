#include "ImageProcessing.h"


ImageProcessing::ImageProcessing()
{

}

ImageProcessing::~ImageProcessing()
{
}

void ImageProcessing::locateMagnet(cv::Mat img, cv::Point2f& center, float& radius)
{
	int64 starttime = cv::getTickCount();

	

	//首先尝试直接在原图大小操作,速度较慢，需要50ms左右。
	cv::Mat grey = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
	cv::cvtColor(img, grey, cv::COLOR_RGB2GRAY);

	cv::threshold(grey, grey, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY_INV);
	/*cv::imshow("大津", grey);
	cv::waitKey(0);*/

	std::vector<std::vector<cv::Point>> contours;
	/*只取最外部轮廓，保留边界上所有连续的轮廓点*/
	cv::findContours(grey, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	/*找到最大的轮廓认为是磁珠*/
	int max_index = 0;
	int max_area = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		int temp_area = cv::contourArea(contours.at(i));
		if (temp_area > max_area)
		{
			max_area = temp_area;
			max_index = i;
		}
	}

		cv::minEnclosingCircle(contours.at(max_index), center, radius);


}


void ImageProcessing::locateMagnet(cv::Mat img, cv::RotatedRect& mag)
{




	//首先尝试直接在原图大小操作,速度较慢，需要50ms左右。
	cv::Mat grey = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
	cv::cvtColor(img, grey, cv::COLOR_RGB2GRAY);

	cv::threshold(grey, grey, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY_INV);
	/*cv::imshow("大津", grey);
	cv::waitKey(0);*/


	std::vector<std::vector<cv::Point>> contours;
	/*只取最外部轮廓，保留边界上所有连续的轮廓点*/
	cv::findContours(grey, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	/*找到最大的轮廓认为是磁珠*/
	int max_index = 0;
	int max_area = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		int temp_area = cv::contourArea(contours.at(i));
		if (temp_area > max_area)
		{
			max_area = temp_area;
			max_index = i;
		}
	}

	mag = cv::minAreaRect(contours.at(max_index));



	/*cv::Point2f* vertices = new cv::Point2f[4];
	mag.points(vertices);

	for (int i = 0; i < 4; i++)
	{
		cv::line(img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 0, 255));
	}*/



}