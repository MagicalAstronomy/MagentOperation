#pragma once

#include <QWidget>
#include "SerialPortCommunication.h"
#include "ui_Debug_Pole.h"

class Debug_Pole : public QWidget
{
	Q_OBJECT

public:
	Debug_Pole(QWidget *parent = Q_NULLPTR);
	~Debug_Pole();

private:
	Ui::Debug_Pole ui;
	SerialPortCommunication* serialportcommunication;
};
