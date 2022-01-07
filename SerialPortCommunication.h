#pragma once

/*包含串口打开与modbusRTU通信的内容*/

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QtCore/QDebug>
#include <QModbusRtuSerialMaster>	//modbus主类
#include <QModbusDataUnit>		//存储接收和发送数据的类
#include <QModbusReply>		//主机访问从机后得到的回复








class SerialPortCommunication : public QObject
{
	Q_OBJECT

public:
	SerialPortCommunication(QObject *parent);
	~SerialPortCommunication();

	QList<QSerialPortInfo> showSerialPorts();	//显示所有可用的串口，以qlist形式传递数据

	int modbusState();	//查询串口状态,打开为1
	int changeModbus(QString portname);	//串口关闭则打开；串口打开则关闭
	int readControlMode();	//读取驱动器是pwm控制（稳压）还是转矩控制(稳流)还是闭环控制等，0为PWM控制，1为转矩控制
	void writeCurrent(int channel);	//对某一路驱动器直接输入控制电流


private:
	QSerialPort* qserialport;
	QSerialPortInfo* qserialportinfo;

	QModbusClient* modbusObj;




private slots:
//	void openSerialPort();
//	void closeSerialPort();
	void readReady();
	
};

