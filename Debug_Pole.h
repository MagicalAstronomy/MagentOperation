#pragma once

#include <QWidget>
#include <qtimer.h>
#include <QtCore/QDebug>
#include "ui_Debug_Pole.h"
#include "SerialPortCommunication.h"
#include "Settings.h"

#include <opencv2/opencv.hpp>
#include <cmath>
#include "Control.h"



class Debug_Pole : public QWidget
{
	Q_OBJECT

public:
	Debug_Pole(QWidget *parent = Q_NULLPTR);
	~Debug_Pole();

private:
	Ui::Debug_Pole ui;
	SerialPortCommunication* serialportcommunication;
	Control* control;

	QList<QSerialPortInfo> list;
	QList<QSerialPortInfo> listCache;	//可用串口列表缓存

	QTimer* spRefreshTimer;	//刷新串口选择的定时器
	QTimer* spinMagTimer;	//控制旋转磁场输出的定时器，定时器触发后改变方向

	QTimer* positionControlTimer;	//闭环位置控制的驱动器，驱动器与上位机通信速率足够后考虑取消掉

	float* currentValue;	//串口输入的电流值，4个通道分别以南-北-西-东储存在数组中

	float spinFrequency;	//输入旋转磁场频率值
	float spinCurrent;	//输入旋转磁场电流值
	int spinControlCycle = 100;	//旋转磁场控制周期（定时器周期/ms）
	float rad;	//计算出每次需要转的弧度
	const double PI = 3.1415926;

	float currentMag[2] = {0,0};	//当前磁珠位置，x和y存在数组中
	float targetMag[2] = {0,0};	//目标磁珠位置,x和y存在数组中

	bool flag_positionControl = false;	//是否进行位置闭环控制的标志位

signals:
	void emit_refreshsp();  //定时器刷新串口的信号函数
	void emit_writeCurrent();



	void emit_writeCurrent02();
	void emit_writeCurrent03();
	void emit_writeCurrent05();
	void emit_writeCurrent09();


private slots:
	void ontimeout();   //定时器到指定值时触发槽函数刷新串口
	void changeCom();
	void inputCurrent();
	void slot_feedback(float*);	//得到图像窗口传递的磁珠中心点和目标点
	void slot_writeCurrent();
	void slot_writeCurrentNew();	//大四极设备使用这个输出
	void startSpin();	//开始调试旋转磁场
	void stopSpin();
	void spinMagOutput();	//根据当前频率开始改变磁场方向并输出
	void startPositionControl();	//开始位置闭环控制
	void stopPositionControl();		//结束位置闭环控制

	void positionControlOutput();	//使用定时器进行解算并输出给驱动器

	void slot_writeCurrent02();
	void slot_writeCurrent03();
	void slot_writeCurrent05();
	void slot_writeCurrent09();
};
