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
#include "ImageProcessing.h"

#include <fstream>


constexpr int COLUMN = 0;
constexpr int SPHERE = 1;

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay(QWidget *parent = Q_NULLPTR);
	~ImageDisplay();

	

private:
	Ui::ImageDisplay ui;
	Ui::ImageDisplay* uiptr;

	QTimer* RefreshTimer;	//查询是否有新帧的定时器
	QTimer* saveTxtTimer;	//将数据写入文件的定时器
	
	/*
	cv::VideoCapture capture;
	*/
	cv::Mat frame;
	cv::Mat frameShow;	//用于展示用的frame，从2248*2048到562*512
	std::string winName;	//展示用窗口的名字
	cv::VideoWriter* videowriter = nullptr;	//保存视频用的类
	

	QImage* image;

	QDateTime time;		//取当前时间戳，用来保存图片和视频
	QString str_time;	//时间戳改为qstring格式
	QString path;		//使用绝对路径表示存储位置
	int counter = 0;

	std::ofstream fout;	//txt数据文件对象

	Cam* cam;
	ImageProcessing* imageprocessing;

	int magType = COLUMN;	//初始化微粒为圆柱体

	//得到单个磁珠在原图像中的坐标位置和半径，如果是圆柱体则还有角度
	cv::Point2f magent_center = cv::Point2f(0,0);
	float magent_radius = 1;
	float angle = 360;	//由于圆柱体角度可以为-90到0，球体默认角度为360
	cv::RotatedRect columnMag;

	bool flag_drawCenteral = false;
	bool flag_drawTarget = false;
	bool flag_locatemag = false;

	cv::Point2f target_center = cv::Point2f(0, 0);
	float coor[4] = { 0,0,0,0 };	//传递当前坐标与目标坐标给控制窗口的数组

	//鼠标所在位置点坐标，初始化为0
	int mouse_x = 0, mouse_y = 0;

	static void OnMouse(int event, int x, int y, int flags, void* param);


signals:
	void emit_showFrame();	//在程序中展示帧的信号
	void emit_saveVideo();	//保存视频的信号
	void emit_feedback(float*);	//得到的磁珠中点和目标点传递给控制子窗口

//自定义槽函数
private slots:
	void queryStatus();   //定时器到指定值时触发槽函数,只有一个线程，因此尽量少做耗时的工作，否则帧数会随之降低
	void startCapture();	//开始采集图像
	void stopCapture();		//停止采集图像
	void savePic();			//保存当前图像
	void saveVideoOn();		//开始保存视频
	void saveVideoOff();	//停止保存视频
	void slot_showFrame();	//在程序中展示帧的槽函数
	void slot_saveVideo();	//保存视频的槽函数
	void setMagSphere();	//设定微粒为球体
	void setMagColumn();	//设定微粒为圆柱体
	void drawCenteralPoint(); //在图像中心点处绘制圆形目标区域	如果是圆柱体则绘制中心点
	void drawTargetPoint();	//在指定目标点处绘制圆形目标区域
	void stopdrawCenteralPoint();	//停止绘制中心点圆形
	void stopdrawTargetPoint();		//停止绘制目标点圆形
	void startLocateMag();		//开始绘制磁珠位置圆形
	void stopLocateMag();		//结束绘制磁珠位置圆形
	void saveTxtOn();		//开始将当前位置和目标位置保存到txt
	void saveTxtOff();		//停止将当前位置和目标位置保存到txt
	void slot_saveTxt();	//保存txt槽函数

	



};
