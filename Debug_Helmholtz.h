#pragma once

#include <QWidget>
#include "ui_Debug_Helmholtz.h"
#include <qtimer.h>
#include <QtCore/QDebug>
#include "SerialPortCommunication.h"
#include "Settings.h"

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

	float value;


private slots:
	void ontimeout();   //定时器到指定值时触发槽函数刷新串口
	void changeCom();
	//void startHelmholtz();	//开始亥姆霍兹旋转磁场输出
	//void stopHelmholtz();
	//void helmholtzOutput();	//根据当前频率开始改变磁场方向并输出

};
