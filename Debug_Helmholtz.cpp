#include "Debug_Helmholtz.h"
#pragma execution_character_set("utf-8")

Debug_Helmholtz::Debug_Helmholtz(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	serialportcommunication = new SerialPortCommunication(this);

	spRefreshTimer = new QTimer(this);

	spRefreshTimer->setInterval(1000);

	/*刷新串口信息定时器使能*/
	connect(spRefreshTimer, SIGNAL(timeout()), this, SLOT(ontimeout()));

	spRefreshTimer->start();




	/*开启串口使能*/
	connect(ui.openCom, SIGNAL(clicked()), this, SLOT(changeCom()));

}

Debug_Helmholtz::~Debug_Helmholtz()
{
}


void Debug_Helmholtz::ontimeout()
{
	if (!serialportcommunication->modbusState())	//只有串口未打开时才进行刷新
	{
		listCache = serialportcommunication->showSerialPorts();
		if (list.size() != listCache.size())	//直接比较两个qlist不成功，似乎因为qlist中存储的qserialportinfo变量没有重载过=0
		{
			list.clear();
			list += listCache;
			qDebug("更新了串口");
			ui.portList->clear();
			foreach(const QSerialPortInfo & info, list)	//虽然我不知道为什么加上const才不会报错，但就这样吧
			{
				qDebug() << info.portName();

				ui.portList->addItem(info.portName());
			}


		}
	}
}

void Debug_Helmholtz::changeCom()
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