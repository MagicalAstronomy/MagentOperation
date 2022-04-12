#pragma once

#include <QWidget>
#include <qtimer.h>
#include <QtCore/QDebug>
#include <iostream>
#include "ui_ImageDisplay.h"

#include<opencv2/opencv.hpp>

#include "Cam.h"

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(QWidget *parent = Q_NULLPTR);
	~ImageDisplay();

	

private:
	Ui::ImageDisplay ui;
	/*
	cv::VideoCapture capture;
	*/
	cv::Mat frame;

	QImage* image;

	//int counter = 0;

	Cam* cam;


//自定义槽函数
private slots:
	void queryStatus();   //定时器到指定值时触发槽函数

};
