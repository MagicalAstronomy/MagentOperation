#include "Login.h"
#pragma execution_character_set("utf-8")

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	uiRefreshTimer = new QTimer(this);

	//设置参数值刷新时间0.5s
	uiRefreshTimer->setInterval(500);

	
	connect(uiRefreshTimer, SIGNAL(timeout()), this, SLOT(ontimeout()));

	//开启定时器
	uiRefreshTimer->start();

    //connect(ui.pushButton_enter, SIGNAL(clicked()), this, SLOT(setModule()));
    //connect(ui.pushButton_enter, SIGNAL(clicked()), this, SLOT(setConfig()));

	//将参数设置与界面刷新函数连接
	connect(this, SIGNAL(emit_refreshUI()), this, SLOT(setModule()));
	connect(this, SIGNAL(emit_refreshUI()), this, SLOT(setConfig()));
	
	//将按钮点击与进入新界面函数连接
	connect(ui.pushButton_enter, SIGNAL(clicked()), this, SLOT(enter()));

	this -> show();

}

void Login::setModule()
{
	if (ui.radioButton_debug->isChecked())
	{
		g_module = DEBUG;
		//qDebug("选择调试模式");
	}
	else if (ui.radioButton_release->isChecked())
	{
		g_module = RELEASE;
		//qDebug("选择应用模式");
	}

	else
	{
		//qDebug("尚未选定模式！");
	}

	//测试使用sender()获取信号发射者
	//QObject* obj = sender();
	//QPushButton* pushbutton = qobject_cast<QPushButton*>(obj);
	//pushbutton->setProperty("text", "响应");
}

void Login::setConfig()
{
	if (ui.radioButton_fourpole->isChecked())
	{
		g_config = FOURPOLE;
		//qDebug("选择平面构型");
	}
	else if (ui.radioButton_helmholtz->isChecked())
	{
		g_config = HELMHOLTZ;
		//qDebug("选择亥姆霍兹构型");
	}
	else if (ui.radioButton_octomag->isChecked())
	{
		g_config = OCTOMAG;
		//qDebug("选择OCTOMAG构型");
	}

	else
	{
		//qDebug("尚未选定构型！");
	}
}

void Login::ontimeout()
{
	emit emit_refreshUI();
}

void Login::enter()
{
	if ((g_config == -1) || (g_module == -1))
	{
		qDebug("配置选择未完成");
	}
	else
	{
		emit skip();
		qDebug("关闭登入界面");
		this->close();
	}
}



