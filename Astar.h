#pragma once


#include <vector>
#include <list>
#include<opencv2/opencv.hpp>


//定义经过对角线/直线时距离 对角线距离为14，直线距离为10
#define COST_DIA 14
#define COST_PARA 10

/*可达到的坐标点 包含坐标，FGH，父坐标等属性*/
class Point	
{
private:
	int x, y;	//坐标
	int F, G, H;	//函数值
	Point* parent;
public:
	Point(int x, int y);

	void setOpen(Point* parent);	//在被放入开始列表时 与父节点关联上

	void updateG(Point* new_parent);	//当走新路径的G值更小时，更新父节点和G,F

	void calcF();	//计算G,H和F，更新值
	void calcG(Point* temp_parent);	//	如果不是计算新路径G值的时候，可以令输入参数为此实例的父节点
	void calcH_Manh(Point* endPoint);	//使用曼哈顿距离计算H值,值为（横坐标距离+纵坐标距离）*10

	int getF();
	int getG();
	int getH();
	int getX();
	int getY();
	Point* getParent();

};

/*Astar算法相关类，包括开启列表，关闭列表，循环寻路算法*/
class Astar
{
private:
	std::list<Point*> openList;	//开启列表
	std::list<Point*> closeList;	//关闭列表
	//std::vector<std::vector<int>> map;			//使用双层vector存储的地图
	cv::Mat map;
	std::list<Point*> path;

	Point* start;
	Point* end;
	Point* N;	//目前进行操作的坐标点用N代称
	Point* preAiming;	//预瞄点
	int preAimLocate;	//预瞄点在路径中位置

	bool reachAble(Point* N,int x,int y);	//判断当前操作点周围八方的点是否可以到达（不可到达情况包括墙壁和障碍物和被障碍物绊住的对角线上的点）
	Point* isInList(std::list<Point*> list, int x, int y);	//给定坐标值判断是否在列表中

	void borderExpand(int expandPixels);	//工作空间配置：对障碍物进行膨胀,使用形态学运算中的腐蚀


public:
	//Astar(std::vector<std::vector<int>> map);
	Astar(cv::Mat map ,Point* start,Point* end,int expandPixels = 0);	//最后一项为膨胀的像素点个数
	std::list<Point*> findPath();
	Point* findPreAimingPoint(int len);	//得到下一个预瞄点位置
};