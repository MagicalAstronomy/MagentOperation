#pragma once

#include <QWidget>
#include "ui_Debug_Helmholtz.h"
#include <qtimer.h>
#include <QtCore/QDebug>
#include "SerialPortCommunication.h"
#include "Settings.h"
#include <cmath>

class Debug_Helmholtz : public QWidget
{
	Q_OBJECT

public:
	Debug_Helmholtz(QWidget *parent = Q_NULLPTR);
	~Debug_Helmholtz();

private:
	Ui::Debug_Helmholtz ui;
	SerialPortCommunication* serialportcommunication;

	QList<QSerialPortInfo> list;
	QList<QSerialPortInfo> listCache;	//可用串口列表缓存

	QTimer* spRefreshTimer;	//刷新串口选择的定时器
	QTimer* helmholtzTimer;	//亥姆霍兹线圈控制信号发送定时器

	int helmholtzCycle = 100;	//亥姆霍兹线圈控制周期100ms
	float helmholtzFrequency = 0.1;	//亥姆霍兹线圈转动频率1Hz
	float helmholtzStrength = 5;	//亥姆霍兹线圈磁场模5mt
	float rad = 0;	//初始转动弧度，开始和结束磁场输出时置零
	float radStep = 0;	//每次需要转动的弧度
	float directionAngle = 0;	//自定义方向时的方向角

	float* helmholtzCurrent;	//电流矩阵，对应x,y，z顺序

	/*每个方向的电流与磁场对应关系，m = a * i + b   ,最大的两个线圈是x方向*/
	float a_x = 1.6470;
	float b_x = 0.1910;
	float a_y = 2.1060;
	float b_y = 0.1980;
	float a_z = 1.8490;
	float b_z = 0.2230;

	


private slots:
	void ontimeout();   //定时器到指定值时触发槽函数刷新串口
	void changeCom();
	void startHelmholtz();	//开始亥姆霍兹旋转磁场输出
	void stopHelmholtz();
	void inputCurrent();	//使用helmholtzCurrent数组输出指定电流
	void initOutput();	//初始化角度和电流
	//void helmholtzOutput();	//根据当前频率开始改变磁场方向并输出

	void helmholtzTest();	//测试旋转磁场,太小磁场没法输出，可以做一个死区判断

};
