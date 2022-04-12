#pragma once

/*相机的各种图像处理函数*/

#include <opencv2\opencv.hpp>

#include "DahengDriver.h"
#include <qDebug>

class Cam
{
public:
	Cam();
	~Cam();
	void startVideoCapture();	//开始采集图像
	void stopVideoCapture();	//停止采集图像
	cv::Mat getFrame();		//得到一帧
	bool getStatus();	//得到目前状态 是否采集到的帧还未推送


	


private:
	DahengDriver* dahengcam;
	cv::Mat frame;

};