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
	if (x < 1e-4 && y < 1e-4)	//同时为0时 避免除零
	{
		e[0] = 0;
		e[1] = 0;
		e[2] = 0;
		e[3] = 0;
	}
	else
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
}

float Control::pid(float uk, float ek, float ek_1, float ek_2)
{
	float deltaUk = kp * (ek - ek_1) + ki * ek + kd * (ek - 2 * ek_1 + ek_2);
	return uk + deltaUk;
}

void Control::resetControl()
{
	for (int i = 0; i < 4; i++)
	{
		e[i] = 0;
		e_1[i] = 0;
		e_2[i] = 0;
		u[i] = 0;
	}
}

float* Control::pidLocationControl(float* cur, float* tar)
{
	//得到x轴和y轴上的差距
	float diff_x = tar[0] - cur[0];
	float diff_y = tar[1] - cur[1];

	//得到四个轴上的误差值
	calcError(diff_x, diff_y);

	//对四个轴分别使用pid控制
	for (int i = 0; i < 4; i++)
	{
		u[i] = pid(u[i], e[i], e_1[i], e_2[i]);
	}

	//TODO:饱和设置
	//TODO:死区设置

	//更新误差
	for (int i = 0; i < 4; i++)
	{
		e_2[i] = e_1[i];
		e_1[i] = e[i];
	}

	return u;
}

void Control::setParam(float p, float i, float d)
{
	this->kp = p;
	this->ki = i;
	this->kd = d;
}