#include "Debug_Pole.h"
#pragma execution_character_set("utf-8")

Debug_Pole::Debug_Pole(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	serialportcommunication = new SerialPortCommunication(this);

	spRefreshTimer = new QTimer(this);

	spRefreshTimer->setInterval(1000);

	connect(spRefreshTimer, SIGNAL(timeout()), this, SLOT(ontimeout()));

	spRefreshTimer->start();


	connect(ui.openCom, SIGNAL(clicked()), this, SLOT(changeCom()));

	
	//list = serialportcommunication->showSerialPorts();

}

Debug_Pole::~Debug_Pole()
{
}


void Debug_Pole::ontimeout()
{
	if(!serialportcommunication->modbusState())	//只有串口未打开时才进行刷新
	{ 
		listCache = serialportcommunication->showSerialPorts();
		if (list.size() != listCache.size())	//直接比较两个qlist不成功，似乎因为qlist中存储的qserialportinfo变量没有重载过=
		{
			list.clear();
			list += listCache;
			qDebug("更新了串口");
			ui.portList->clear();
			foreach (const QSerialPortInfo &info, list)	//虽然我不知道为什么加上const才不会报错，但就这样吧
			{
				qDebug() << info.portName();
				
				ui.portList->addItem(info.portName());
			}

			
		}
	}
}


void Debug_Pole::changeCom()
{
	serialportcommunication->changeModbus(ui.portList->currentText());
	if (serialportcommunication->modbusState())
	{

		ui.portList->setDisabled(true);
		ui.openCom->setText(tr("关闭串口"));
	}

	else
	{

		ui.portList->setEnabled(true);
		ui.openCom->setText(tr("打开串口"));
	}


}