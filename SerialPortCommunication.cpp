#include "SerialPortCommunication.h"
#pragma execution_character_set("utf-8")

SerialPortCommunication::SerialPortCommunication(QObject *parent)
	: QObject(parent)
{

}

SerialPortCommunication::~SerialPortCommunication()
{
}

QList<QSerialPortInfo> SerialPortCommunication::showSerialPorts()
{
	QList<QSerialPortInfo> list;
	foreach (const QSerialPortInfo & info,QSerialPortInfo::availablePorts())
	{

		list.append(info);
		/*for (int i = 0; i < serialportcommunication->showSerialPorts().size(); i++)
{
	qDebug() << serialportcommunication->showSerialPorts().at(i);
}*/
	}

		qDebug() << "ËÑË÷µ½´®¿ÚÊý£º" << list.size();

	return list;

}

void SerialPortCommunication::openSerialPort()
{

}

void SerialPortCommunication::closeSerialPort()
{

}

