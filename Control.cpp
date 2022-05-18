#include "Control.h"


Control::Control()
{
}

Control::~Control()
{
}

float Control::calcRadByCoor(float x, float y)
{
	double l = std::sqrt(x * x + y * y);
	float temp = std::acos(x / l);
	if (y < 0)
	{
		return (2 * PI - temp);
	}
	return temp;
}