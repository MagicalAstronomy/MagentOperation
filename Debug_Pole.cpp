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
	connect(ui.inputCurrent, SIGNAL(clicked()), this, SLOT(inputCurrent()));

	currentValue = new float[4]();	//数组值初始化为0


	
	//list = serialportcommunication->showSerialPorts();

}

Debug_Pole::~Debug_Pole()
{
	delete currentValue;
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

void Debug_Pole::inputCurrent()	//产生指定电流 北->0x02 ;南->0x09 ; 东->0x05 ;西->0x03 ;以东，北正电流方向为基准，西，南螺线管与其在中间抵消的电流方向为同向
{
	//测试读取数据是否成功时借用此按钮实现
	//已成功
	serialportcommunication->readControlMode();
	//测试点击按钮给固定的电流值
	//已成功
	//serialportcommunication->writeCurrent(2.0, 0x02);
	
	currentValue[0] = ui.current_North->text().toFloat();
	currentValue[1] = ui.current_South->text().toFloat();
	currentValue[2] = ui.current_East->text().toFloat();
	currentValue[3] = -1 * ui.current_West->text().toFloat();	//当西侧电流取反时，东西两侧通入相同正负的电流在中间磁场值为0
	qDebug() << currentValue[1];
	serialportcommunication->writeCurrent(currentValue[0], 0x02);
	serialportcommunication->writeCurrent(currentValue[1], 0x09);
	serialportcommunication->writeCurrent(currentValue[2], 0x05);
	serialportcommunication->writeCurrent(currentValue[3], 0x03);
}