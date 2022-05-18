#pragma once
/*控制算法相关程序
以下程序均按图像处理程序传递的格式处理，x为图像横轴，右侧为正方向；y为图像纵轴，下为正方向；原点为左上角。*/

#include <cMath>


constexpr float PI = 3.1415926;
/*定义平面四极构型中四个极对采集到的图像分别的作用效果（x,y轴的斜率）,接线从左下到右下分别是02，03，05，09*/
constexpr float k_lowerleft = -1.4056;
constexpr float k_upperleft = 0.7477;	//横轴为负，应在计算角度时将横纵都乘-1
constexpr float k_upperright = -1.0163;	//横轴为负
constexpr float k_lowerright = 0.9188;


class Control
{
public:
	Control();
	~Control();

private:


	float standardRad[4];	//平面四极中分别的弧度值
	float e[4] = { 0,0,0,0 };	//当前误差，四个值分别代表四个极方向上的误差
	float e_1[4] = { 0,0,0,0 };	//上一步误差
	float e_2[4] = { 0,0,0,0 };	//再上一步误差

	float calcRadByCoor(float x, float y);	//根据坐标值计算出从原点到该坐标点的向量与x轴正方向夹角的弧度，值从0到2Π

	void calcError(float x, float y);	//根据当前位置与目标误差计算出四个方向上分别的误差值，覆写e[4]数组

};

