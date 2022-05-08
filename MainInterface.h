#pragma once

#include <QWidget>
#include <QLayout>
#include <QtCore/QDebug>
#include "ui_MainInterface.h"
#include "Login.h"
#include "ImageDisplay.h"
#include "Debug_Pole.h"
#include "Debug_Helmholtz.h"

class MainInterface : public QWidget
{
	Q_OBJECT

public:
	MainInterface(QWidget *parent = Q_NULLPTR);
	~MainInterface();

	void connectWithLogin(Login* ptr);		//使用这个函数在登录界面和主界面间建立联系


private:
	Ui::MainInterface ui;

	QHBoxLayout* hlayout;

	ImageDisplay* imagedisplay;

	//TODO:其他种类界面补完
	Debug_Pole* debug_pole;
	Debug_Helmholtz* debug_helmholtz;


	//void initUI();

	
	void chooseUI();	//根据全局变量的值创建并选择需要添加进主界面的子窗口


private slots:
	void showUI();	//在水平布局中添加需要的两个控件，在主界面添加布局并显示
};
