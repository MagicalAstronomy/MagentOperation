#pragma once

#include<opencv2/opencv.hpp>

#include <GalaxyIncludes.h>

#include <iostream>

class DahengDriver
{
public:
	DahengDriver();
	~DahengDriver();
	void getPicture(cv::Mat& img);
	void startGrab();
	void stopGrab();

	bool frameFlag;
	


private:
	CGXDevicePointer ObjDevicePtr;	//设备指针
	CGXStreamPointer ObjStreamPtr;	//流指针
	CGXFeatureControlPointer ObjFeatureControlPtr;	//设备属性控制器
	CGXFeatureControlPointer ObjStreamFeatureControlPtr;

	ICaptureEventHandler* pCaptureEventHandler = nullptr;
	void* flagPtr;


};
