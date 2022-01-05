#include "Debug_Pole.h"

Debug_Pole::Debug_Pole(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	serialportcommunication = new SerialPortCommunication(this);
	
	QList<QSerialPortInfo> list = serialportcommunication->showSerialPorts();

}

Debug_Pole::~Debug_Pole()
{
}

