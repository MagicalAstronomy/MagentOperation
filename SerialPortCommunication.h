#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QtCore/QDebug>

class SerialPortCommunication : public QObject
{
	Q_OBJECT

public:
	SerialPortCommunication(QObject *parent);
	~SerialPortCommunication();

	QList<QSerialPortInfo> showSerialPorts();	//显示所有可用的串口，以qlist形式传递数据

private:
	QSerialPort* qserialport;
	QSerialPortInfo* qserialportinfo;



private slots:
	void openSerialPort();
	void closeSerialPort();
};

