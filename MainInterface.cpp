#include "MainInterface.h"
#pragma execution_character_set("utf-8")

MainInterface::MainInterface(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	hlayout = new QHBoxLayout();

	imagedisplay = new ImageDisplay();


}

MainInterface::~MainInterface()
{
}


void MainInterface::showUI()
{
	//由于两个子窗口分别是768*768和256*768，选择图像子窗口比例为3，控制子窗口比例为1
	hlayout->addWidget(imagedisplay ,3);

	//由于构造函数执行时尚未选择模式，因此通过选择的模式决定画面的代码不能放在构造函数中，可以放在显示界面时
	this->chooseUI();

	this->setLayout(hlayout);


	this->show();
}

void MainInterface::connectWithLogin(Login* ptr)
{
	connect(ptr, SIGNAL(skip()), this, SLOT(showUI()));
}

//void MainInterface::initUI()
//{
//
//
//	this->setLayout(hlayout);
//}

void MainInterface::chooseUI()
{
	switch (g_module)
	{
	case DEBUG:
	{
		switch (g_config)
		{
		case FOURPOLE:
		{
			qDebug("显示四极调试界面");
			debug_pole = new Debug_Pole();
			hlayout->addWidget(debug_pole,1);
			break;
		}
		case HELMHOLTZ:
		{
			qDebug("显示亥姆霍兹调试界面");
			//TODO:这种界面编写和调用
			break;
		}
		case OCTOMAG:
		{
			qDebug("显示octomag调试界面");
			//TODO:这种界面编写和调用
			break;
		}

		default:
			qDebug("2021.12.26当前逻辑下不可能出现");
			break;
		}
		break;
		
	}

	case RELEASE:
	{

		switch (g_config)
		{
		case FOURPOLE:
		{
			qDebug("显示四极应用界面");
			//TODO:这种界面编写和调用
			break;
		}
		case HELMHOLTZ:
		{
			qDebug("显示亥姆霍兹应用界面");
			//TODO:这种界面编写和调用
			break;
		}
		case OCTOMAG:
		{
			qDebug("显示octomag应用界面");
			//TODO:这种界面编写和调用
			break;
		}

		default:
			qDebug("2021.12.26当前逻辑下不可能出现");
			break;
		}
		break;

	}

	default:
		qDebug("2021.12.26当前逻辑下不可能出现");
		break;
	}

}