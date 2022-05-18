#include "Control.h"


Control::Control()
{

	standardRad[0] = calcRadByCoor(1, k_lowerleft);
	standardRad[1] = calcRadByCoor(-1,-1 *  k_upperleft);
	standardRad[2] = calcRadByCoor(-1,-1 *  k_upperright);
	standardRad[3] = calcRadByCoor(1, k_lowerright);


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

void Control::calcError(float x, float y)
{
	/*根据当前状态，顺时针安装的四个方向逆时针旋转，右下角在第一象限，左下角在第四象限。因此判断当前方向在那两个方向中间时按此顺序。
	相机角度变化时需重新标定四个方向并改变判断条件*/
	float targetRad = calcRadByCoor(x, y);
	double l = std::sqrt(x * x + y * y);
	/*已知各标准方向角度和当前目标角度，可得到向量合成三角形的三个角，通过正弦定理即可求出两个方向误差值*/


	if (targetRad <= standardRad[0] && targetRad > standardRad[1])
	{
		e[2] = 0;
		e[3] = 0;
		float radToLL = targetRad - standardRad[1];		//左下角线圈产生磁场方向向量的对角 其他命名同此规律
		float radToUL = standardRad[0] - targetRad;		
		float radToTarget = PI - radToLL - radToUL;

		e[0] = sin(radToLL) * l / sin(radToTarget);
		e[1] = sin(radToUL) * l / sin(radToTarget);

	}
	else if (targetRad <= standardRad[1] && targetRad > standardRad[2])
	{
		e[0] = 0;
		e[3] = 0;
		float radToUL = targetRad - standardRad[2];
		float radToUR = standardRad[1] - targetRad;
		float radToTarget = PI - radToUL - radToUR;
		e[1] = sin(radToUL) * l / sin(radToTarget);
		e[2] = sin(radToUR) * l / sin(radToTarget);
	}
	else if (targetRad <= standardRad[2] && targetRad > standardRad[3])
	{
		e[0] = 0;
		e[1] = 0;
		float radToUR = targetRad - standardRad[3];
		float radToLR = standardRad[2] - targetRad;
		float radToTarget = PI - radToLR - radToUR;
		e[2] = sin(radToUR) * l / sin(radToTarget);
		e[3] = sin(radToLR) * l / sin(radToTarget);
	}
	else    //0和2Π分界处记得调整角度值
	{
		e[1] = 0;
		e[2] = 0;
		if (targetRad > standardRad[0])
		{
			float radToLR = targetRad - standardRad[0];
			float radToLL = standardRad[3] + 2 * PI - targetRad;
			float radToTarget = PI - radToLR - radToLL;
			e[0] = sin(radToLL) * l / sin(radToTarget);
			e[3] = sin(radToLR) * l / sin(radToTarget);
		}
		else
		{
			float radToLR = targetRad + 2 * PI - standardRad[0];
			float radToLL = standardRad[3] - targetRad;
			float radToTarget = PI - radToLR - radToLL;
			e[0] = sin(radToLL) * l / sin(radToTarget);
			e[3] = sin(radToLR) * l / sin(radToTarget);
		}
		

	}
}