#pragma once

#include <QtWidgets/QWidget>
#include "ui_Login.h"
#include <QtCore/QDebug>
#include "Settings.h"
#include <qtimer.h>

class Login : public QWidget
{

    //元对象系统支持，能使用信号与槽等功能
    Q_OBJECT

public:
    Login(QWidget *parent = Q_NULLPTR);

private:
    Ui::LoginClass ui;

    //定时器声明
    QTimer* uiRefreshTimer;


signals:   
    void emit_refreshUI();  //定时器刷新UI的信号函数
    void skip();    //点击enter按钮进入主界面的信号函数

//自定义槽函数
private slots:
    void setModule();   //设置模式
    void setConfig();   //设置构型
    void ontimeout();   //定时器到指定值时触发槽函数
    void enter();       //进入应用界面

};
