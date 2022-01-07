#include "SerialPortCommunication.h"
#pragma execution_character_set("utf-8")

SerialPortCommunication::SerialPortCommunication(QObject *parent)
	: QObject(parent)
{
	modbusObj = new QModbusRtuSerialMaster(this);

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

		qDebug() << "搜索到串口数：" << list.size();

	return list;

}

int SerialPortCommunication::modbusState()	//打开返回1，没打开返回0
{
	if (modbusObj->state() != QModbusDevice::ConnectedState)	//当前串口未打开
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


int SerialPortCommunication::changeModbus(QString portname)	//打开串口。至少需要给出串口名，成功返回1，失败返回0
{

	if (!modbusObj)return 0;

	if (modbusObj->state() != QModbusDevice::ConnectedState)	//当前串口未打开
	{
		//设置串口相关参数
		modbusObj->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portname);	//设置串口名称
		modbusObj->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::EvenParity);		//设置偶校验
		modbusObj->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);		//设置波特率
		modbusObj->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);		//设置一位停止位
		modbusObj->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);		//设置八位数据位

		modbusObj->setTimeout(1000);	//设置从机回复的超时时间
		modbusObj->setNumberOfRetries(2);	//设置重复发送次数

		if (!modbusObj->connectDevice())
		{
			qDebug("串口打开失败");
			return 0;
		}
		else
		{
			qDebug("串口已打开");
			return 1;
		}

	}
	else
	{
		//断开连接函数返回void，似乎是一定能断开成功的
		modbusObj->disconnectDevice();
		qDebug("串口已关闭");
		return 1;
		
	}


}

int SerialPortCommunication::readControlMode()
{
	if (!modbusObj)return -1;
	if (!this->modbusState())
	{
		qDebug("串口尚未打开！");
		return -1;
	}
	QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 0x0080, 0x0001);	//设置读模式，寄存器地址，以及读取的数据长度

	if (auto* reply = modbusObj->sendReadRequest(readUnit, 0x02))
	{
		if(!reply->isFinished())
		{
			connect(reply, &QModbusReply::finished, this, &SerialPortCommunication::readReady);
		}
		else
		{
			delete reply;
		}

	}
	else
	{
		qDebug("发送读数据请求失败！");
	}

}

void SerialPortCommunication::readReady()
{
	qDebug("开始读取数据");

	auto reply = qobject_cast<QModbusReply*>(sender());

	if (!reply)return;

	if (reply->error() == QModbusDevice::NoError)
	{
		const QModbusDataUnit unit = reply->result();
		QString total = QString("总数据量为： %1").arg(unit.valueCount());

		for (uint i = 0; i < unit.valueCount(); i++)
		{
			const QString entry = unit.value(i);
			total += entry;
		}

		qDebug() << total;
	}

	else if (reply->error() == QModbusDevice::ProtocolError)	//有协议错误
	{
		qDebug() << reply->errorString();
	}
	else
	{
		qDebug() << reply->errorString();
	}

	reply->deleteLater();
}




