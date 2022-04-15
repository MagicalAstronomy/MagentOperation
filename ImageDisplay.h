#pragma once

#include <QWidget>
#include <qtimer.h>
#include <QtCore/QDebug>
#include <iostream>
#include <QtWinExtras/QtWinExtras>
#include "ui_ImageDisplay.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include "Cam.h"

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(QWidget *parent = Q_NULLPTR);
	~ImageDisplay();

	

private:
	Ui::ImageDisplay ui;
	Ui::ImageDisplay* uiptr;
	
	/*
	cv::VideoCapture capture;
	*/
	cv::Mat frame;
	cv::Mat frameShow;	//用于展示用的frame，从2248*2048到562*512
	std::string winName;	//展示用窗口的名字

	QImage* image;

	QDateTime time;		//取当前时间戳，用来保存图片和视频
	QString str_time;	//时间戳改为qstring格式
	QString path;		//使用绝对路径表示存储位置
	//int counter = 0;

	Cam* cam;

	//鼠标所在位置点坐标，初始化为0
	int mouse_x = 0, mouse_y = 0;

	static void OnMouse(int event, int x, int y, int flags, void* param);


//自定义槽函数
private slots:
	void queryStatus();   //定时器到指定值时触发槽函数
	void startCapture();
	void stopCapture();
	void savePic();


};
