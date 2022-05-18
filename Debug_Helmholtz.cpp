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

	helmholtzTimer = new QTimer(this);
	helmholtzTimer->setInterval(helmholtzCycle);
	/*亥姆霍兹控制定时器使能*/
	connect(helmholtzTimer, SIGNAL(timeout()), this, SLOT(helmholtzTest()));

	/*亥姆霍兹开始和关闭使能*/
	connect(ui.startHelmholtz, SIGNAL(clicked()), this, SLOT(startHelmholtz()));
	connect(ui.stopHelmholtz, SIGNAL(clicked()), this, SLOT(stopHelmholtz()));



	/*开启串口使能*/
	connect(ui.openCom, SIGNAL(clicked()), this, SLOT(changeCom()));

	/*旋转方向初始化为xy*/
	ui.modeXY->setChecked(1);


	/*电流值初始化*/
	helmholtzCurrent = new float[3]();
	rad = 0;
	helmholtzCurrent[0] = 0;
	helmholtzCurrent[1] = 0;
	helmholtzCurrent[2] = 0;

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

void Debug_Helmholtz::startHelmholtz()
{
	//initOutput();
	helmholtzCurrent[0] = 0;
	helmholtzCurrent[1] = 0;
	helmholtzCurrent[2] = 0;

	helmholtzFrequency = ui.helmholtzFrequency->text().toFloat();
	helmholtzStrength = ui.helmholtzStrength->text().toFloat();
	directionAngle = ui.customAngle->text().toFloat();
	radStep = 2 * 3.1415 * helmholtzFrequency * helmholtzCycle / 1000;

	helmholtzTimer->start();

}

void Debug_Helmholtz::stopHelmholtz()
{
	helmholtzTimer->stop();
	initOutput();

}

void Debug_Helmholtz::helmholtzTest()
{

	/*使用角度旋转的顺逆时针控制转动方向*/
	/*if (directionAngle >= 180 && directionAngle <= 360)
	{
		rad = rad + radStep;

	}
	else
	{
		rad = rad - radStep;
	}*/

	rad = rad - radStep;
	
	if (rad > 2 * 3.1415)
	{
		rad = rad - 2 * 3.1415;
	}
	else if (rad < -2 * 3.1415)
	{
		rad = rad + 2 * 3.1415;
	}
	if (ui.modeXY->isChecked())
	{
		float mag_x = helmholtzStrength * cos(rad);
		float mag_y = helmholtzStrength * sin(rad);
		helmholtzCurrent[0] = (mag_x - b_x) / a_x;
		helmholtzCurrent[1] = (mag_y - b_y) / a_y;
	}
	else if (ui.modeXZ->isChecked())
	{
		float mag_x = helmholtzStrength * cos(rad);
		float mag_z = helmholtzStrength * sin(rad);
		helmholtzCurrent[0] = (mag_x - b_x) / a_x;
		helmholtzCurrent[2] = (mag_z - b_z) / a_z;
	}
	else if (ui.modeYZ->isChecked())
	{
		float mag_y = helmholtzStrength * cos(rad);
		float mag_z = helmholtzStrength * sin(rad);
		helmholtzCurrent[1] = (mag_y - b_y) / a_y;
		helmholtzCurrent[2] = (mag_z - b_z) / a_z;
	}
	else if (ui.modeCustom->isChecked())
	{
		float mag_plane = helmholtzStrength * cos(rad);
		float mag_z = helmholtzStrength * sin(rad);
		float mag_x = mag_plane * cos(directionAngle * 3.1415 / 180);
		float mag_y = mag_plane * sin(directionAngle * 3.1415 / 180);
		helmholtzCurrent[0] = (mag_x - b_x) / a_x;
		helmholtzCurrent[1] = (mag_y - b_y) / a_y;
		helmholtzCurrent[2] = (mag_z - b_z) / a_z;
		qDebug() << "x:" << mag_x << ',' << "z:" << mag_z;

	}
	
	inputCurrent();
	

}

void Debug_Helmholtz::inputCurrent()
{
	serialportcommunication->writeCurrent(helmholtzCurrent[0], 0x05);	//x轴输出 两个驱动器电流相同
	serialportcommunication->writeCurrent(helmholtzCurrent[0], 0x09);
	serialportcommunication->writeCurrent(helmholtzCurrent[1], 0x03);	//y轴输出
	serialportcommunication->writeCurrent(helmholtzCurrent[2], 0x02);	//z轴输出

}

void Debug_Helmholtz::initOutput()
{
	rad = 0;
	helmholtzCurrent[0] = 0;
	helmholtzCurrent[1] = 0;
	helmholtzCurrent[2] = 0;
	inputCurrent();
}