#pragma once

#include <opencv2/opencv.hpp>

#include <vector>

#include <iostream>

/*用于图像处理的各种函数*/

class ImageProcessing
{
public:
	ImageProcessing();
	~ImageProcessing();

	void locateMagnet(cv::Mat img, cv::Point2f& center, float& radius, int magType);		//假设只有一个球形的磁性微粒，找到中点和半径

private:

};