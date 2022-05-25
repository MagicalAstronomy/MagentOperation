#include "Debug_Pole.h"
#pragma execution_character_set("utf-8")

Debug_Pole::Debug_Pole(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	serialportcommunication = new SerialPortCommunication(this);
	control = new Control();

	spRefreshTimer = new QTimer(this);

	spRefreshTimer->setInterval(1000);

	/*刷新串口信息定时器使能*/
	connect(spRefreshTimer, SIGNAL(timeout()), this, SLOT(ontimeout()));

	spRefreshTimer->start();

	/*旋转磁场定时器使能*/
	spinMagTimer = new QTimer(this);
	spinMagTimer->setInterval(spinControlCycle);
	connect(spinMagTimer, SIGNAL(timeout()), this, SLOT(spinMagOutput()));

	positionControlTimer = new QTimer(this);
	positionControlTimer->setInterval(50);
	connect(positionControlTimer, SIGNAL(timeout()), this, SLOT(positionControlOutput()));
	positionControlTimer->start();
	

	
	/*开启串口使能*/
	connect(ui.openCom, SIGNAL(clicked()), this, SLOT(changeCom()));

	/*输入电流调试使能*/
	connect(ui.inputCurrent, SIGNAL(clicked()), this, SLOT(inputCurrent()));

	/*旋转磁场调试开始和关闭使能*/
	connect(ui.startSpin, SIGNAL(clicked()), this, SLOT(startSpin()));
	connect(ui.stopSpin, SIGNAL(clicked()), this, SLOT(stopSpin()));

	/*输出电流使能*/
	connect(this, SIGNAL(emit_writeCurrent()), this, SLOT(slot_writeCurrentNew()));

	/*开始和结束位置闭环控制使能*/
	connect(ui.startPositionControl, SIGNAL(clicked()), this, SLOT(startPositionControl()));
	connect(ui.stopPositionControl, SIGNAL(clicked()), this, SLOT(stopPositionControl()));



	connect(this, SIGNAL(emit_writeCurrent02()), this, SLOT(slot_writeCurrent02()));
	connect(this, SIGNAL(emit_writeCurrent03()), this, SLOT(slot_writeCurrent03()));
	connect(this, SIGNAL(emit_writeCurrent05()), this, SLOT(slot_writeCurrent05()));
	connect(this, SIGNAL(emit_writeCurrent09()), this, SLOT(slot_writeCurrent09()));



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
		if (list.size() != listCache.size())	//直接比较两个qlist不成功，似乎因为qlist中存储的qserialportinfo变量没有重载过=0
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
	//qDebug() << currentMag[0] << "," << currentMag[1];
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
	//serialportcommunication->readControlMode();

	
	//测试点击按钮给固定的电流值
	//已成功
	//serialportcommunication->writeCurrent(2.0, 0x02);
	
	//currentValue[0] = ui.current_east->text().toFloat();
	//currentValue[1] = -1 * ui.current_north->text().toFloat();	//如此输入，则南北两侧通入同号的电流在中间磁场值为0
	//currentValue[2] = ui.current_south->text().toFloat();
	//currentValue[3] = -1 * ui.current_west->text().toFloat();	//如此输入，则东西两侧通入同号的电流在中间磁场值为0

	//暂时不考虑两侧同时同磁场的情况
	currentValue[0] = ui.current_east->text().toFloat();
	currentValue[1] = ui.current_north->text().toFloat();
	currentValue[2] = ui.current_south->text().toFloat();
	currentValue[3] = ui.current_west->text().toFloat();

	//qDebug() << currentValue[0];
	serialportcommunication->writeCurrent(currentValue[0], 0x02);	//修改后为南
	serialportcommunication->writeCurrent(currentValue[1], 0x09);	//修改后为北
	serialportcommunication->writeCurrent(currentValue[2], 0x05);	//修改后为西
	serialportcommunication->writeCurrent(currentValue[3], 0x03);	//修改后为东
}

void Debug_Pole::startSpin()
{
	/*初始化磁场*/
	currentValue[0] = 0;
	currentValue[1] = 0;
	currentValue[2] = 0;
	currentValue[3] = 0;
	
	spinCurrent = ui.spinCurrent->text().toFloat();
	spinFrequency = ui.spinFrequency->text().toFloat();
	rad = 2 * PI * spinFrequency * spinControlCycle / 1000;	//得到每次操作需要转的弧度
	//qDebug() << rad;
	


	if (spinCurrent > 0.0099 && spinFrequency <= 10)	//数据合法则可以开始
	{
		spinMagTimer->start();
	}
	else
	{
		qDebug() << "输入不在允许范围内";
	}
	


	
}

void Debug_Pole::stopSpin()
{


	spinMagTimer->stop();

	/*停止移动*/
	currentValue[0] = 0;
	currentValue[1] = 0;
	currentValue[2] = 0;
	currentValue[3] = 0;
	qDebug() << "停止旋转";
	emit emit_writeCurrent();
}

void Debug_Pole::slot_feedback(float* coor)
{

	/*int64 endtime = cv::getTickCount();

	float time = (endtime - t) / cv::getTickFrequency();
	
	qDebug() << QString::number(time, 'f', 6);*/


	/*传回的数据为四个float变量，分别是当前磁珠的x和y，目标点的x和y。其中x是横轴，y是纵轴*/
	currentMag[0] = coor[0];
	currentMag[1] = coor[1];
	targetMag[0] = coor[2];
	targetMag[1] = coor[3];
	//qDebug() << QString::number(currentMag[0], 'f', 2) << ',' << QString::number(currentMag[1], 'f', 2) << ',' << QString::number(targetMag[0], 'f', 2) << ',' << QString::number(targetMag[1], 'f', 2);



	/*先使用定时器，等和驱动器通信效率能达到帧速了再在这里直接控制*/
	/*if (flag_positionControl)
	{
		float* temp;
		temp = control->pidLocationControl(currentMag, targetMag);
		for (int i = 0; i < 4; i++)
		{
			currentValue[i] = temp[i];
		}
		qDebug() << "当前磁珠坐标为：" << currentMag[0] << "," << currentMag[1] << "当前目标坐标为：" << targetMag[0] << "," << targetMag[1];
		qDebug() << "当前四个轴控制量为：" << currentValue[0] << "," << currentValue[1] << "," << currentValue[2] << "," << currentValue[3];
		emit emit_writeCurrent();
	}*/
}

void Debug_Pole::positionControlOutput()
{
	if (flag_positionControl)
	{
		float* temp;
		temp = control->pidLocationControl(currentMag, targetMag);
		for (int i = 0; i < 4; i++)
		{
			currentValue[i] = temp[i];
			
		}
		qDebug() << "当前磁珠坐标为：" << currentMag[0] << "," << currentMag[1] << "当前目标坐标为：" << targetMag[0] << "," << targetMag[1];
		qDebug() << "当前四个轴控制量为：" << currentValue[0] << "," << currentValue[1] << "," << currentValue[2] << "," << currentValue[3];
		emit emit_writeCurrent();
	}
}

void Debug_Pole::slot_writeCurrent()
{
	/*目前按照南北西东的顺序发送数组中的四个值
	保证每个方向用磁强计的突出那面测都显示为N，将北和东两路的磁场反向，让旋转磁场能顺利给出*/
	serialportcommunication->writeCurrent(currentValue[0], 0x02);	//修改后为南
	serialportcommunication->writeCurrent(-1 * currentValue[1], 0x09);	//修改后为北
	serialportcommunication->writeCurrent(currentValue[2], 0x05);	//修改后为西
	serialportcommunication->writeCurrent( -1 * currentValue[3], 0x03);	//修改后为东
	

}

void Debug_Pole::slot_writeCurrentNew()
{
	/*硬件接线上保证所有线圈在输入正向电流时,磁强计突起那边都显示为N，数组按顺序输出*/


	/*serialportcommunication->writeCurrent(currentValue[0], 0x02);
	serialportcommunication->writeCurrent(currentValue[1], 0x03);
	serialportcommunication->writeCurrent(currentValue[2], 0x05);
	serialportcommunication->writeCurrent(currentValue[3], 0x09);*/

	//尝试分开多个线程给驱动器传电流来加速
	
	emit emit_writeCurrent02();
	emit emit_writeCurrent03();
	emit emit_writeCurrent05();
	emit emit_writeCurrent09();


}

void Debug_Pole::slot_writeCurrent02()
{
	serialportcommunication->writeCurrent(currentValue[0], 0x02);
}
void Debug_Pole::slot_writeCurrent03()
{
	serialportcommunication->writeCurrent(currentValue[1], 0x03);
}
void Debug_Pole::slot_writeCurrent05()
{
	serialportcommunication->writeCurrent(currentValue[2], 0x05);
}
void Debug_Pole::slot_writeCurrent09()
{
	serialportcommunication->writeCurrent(currentValue[3], 0x09);
}

void Debug_Pole::spinMagOutput()
{
	
	float preRad;
	float x, y, newx, newy;

	/*全为0时 从0x02开始*/
	if ((abs(currentValue[0]) < 1e-4) && (abs(currentValue[1]) < 1e-4) && (abs(currentValue[2]) < 1e-4) && (abs(currentValue[3]) < 1e-4))
	{
		currentValue[0] = spinCurrent;
	}
	/*否则使用arctan判断当前方向，并增加指定角度后重新设定电流值*/
	else
	{

		/*虽然左右/东西两边不完全相等，但旋转磁场开始时已经初始化过，能最大程度避免相对两边都有电流情况，直接相减就能得到方向*/
		y = currentValue[2] - currentValue[0];	//纵坐标
		x = currentValue[3] - currentValue[1];	//横坐标

		//qDebug() << "x:" << abs(x) << "y:" << y;
		/*虽然浮点数有精度限制应该不会，但还是判断一下旋转到纵轴上的情况，避免arctan除零*/
		if (abs(x) < 1e-4)
		{
			if (y > 0)
			{
				preRad = PI / 2;
			}
			else
			{
				preRad = -PI / 2;
			}
		}
		else if(x > 0)	//14象限arctan直接原值不动即可
		{
			preRad = atan(y / x);
			
		}
		else    //23象限arctan值需要加180°
		{
			preRad = atan(y / x) + PI;
			//qDebug() << "preRad:" << preRad << "Rad:" << rad;
		}

		newx = cos(preRad + rad) * spinCurrent;
		newy = sin(preRad + rad) * spinCurrent;
		if (newx > 0)
		{
			currentValue[3] = newx;
			currentValue[1] = 0;
		}
		else
		{
			currentValue[1] = abs(newx);
			currentValue[3] = 0;
		}
		if (newy > 0)
		{
			currentValue[2] = newy;
			currentValue[0] = 0;
		}
		else
		{
			currentValue[0] = abs(newy);
			currentValue[2] = 0;
		}
		qDebug() << "当前北侧电流：" << currentValue[2];
		qDebug() << "当前南侧电流：" << currentValue[0];
		qDebug() << "当前西侧电流：" << currentValue[1];
		qDebug() << "当前东侧电流：" << currentValue[3];
		qDebug() << "*******";

	}
	/*发送输出电流的信号*/
	emit emit_writeCurrent();

}


void Debug_Pole::startPositionControl()
{
	control->setParam(ui.setKP->text().toFloat(), ui.setKI->text().toFloat(), ui.setKD->text().toFloat());
	
	flag_positionControl = true;
}

void Debug_Pole::stopPositionControl()
{
	flag_positionControl = false;
	control->resetControl();

	/*输出电流清零*/
	for (int i = 0; i < 4; i++)
	{
		currentValue[i] = 0;
	}
	emit emit_writeCurrent();
}