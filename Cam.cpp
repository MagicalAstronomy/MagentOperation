#include "Cam.h"

Cam::Cam()
{
	dahengcam = new DahengDriver();
}

Cam::~Cam()
{
	delete dahengcam;
}

void Cam::startVideoCapture()
{
	dahengcam->startGrab();
}


void Cam::stopVideoCapture()
{
	dahengcam->stopGrab();
}

cv::Mat Cam::getFrame()
{
	dahengcam->getPicture(frame);
	return frame;
}

bool Cam::getStatus()
{
	return dahengcam->frameFlag;
}