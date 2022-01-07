#pragma once

#include <QWidget>
#include <qtimer.h>
#include <QtCore/QDebug>
#include "ui_Debug_Pole.h"
#include "SerialPortCommunication.h"
#include "Settings.h"


class Debug_Pole : public QWidget
{
	Q_OBJECT

public:
	Debug_Pole(QWidget *parent = Q_NULLPTR);
	~Debug_Pole();

private:
	Ui::Debug_Pole ui;
	SerialPortCommunication* serialportcommunication;

	QList<QSerialPortInfo> list;
	QList<QSerialPortInfo> listCache;	//可用串口列表缓存

	QTimer* spRefreshTimer;	//刷新串口选择的定时器

signals:
	void emit_refreshsp();  //定时器刷新串口的信号函数


private slots:
	void ontimeout();   //定时器到指定值时触发槽函数
	void changeCom();
};
