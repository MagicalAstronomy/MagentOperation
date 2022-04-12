#include "Login.h"
#include "MainInterface.h"
#include <QtWidgets/QApplication>

#include <opencv2/opencv.hpp>
#include <QTime>
#include <QDebug>


int main(int argc, char *argv[])
{


    /*测试摄像头帧率
    cv::Mat frame;
    cv::VideoCapture capture(0);
    capture.set(cv::CAP_PROP_FPS, 25);

    double fps = 0;
    QTime lasttime = QTime::currentTime();
    int framecount = 0;

    while (1)
    {
        capture >> frame;

        cv::imshow("摄像头",frame);

        framecount++;

        QTime curtime = QTime::currentTime();

        if (lasttime.msecsTo(curtime) > 10000)
        {
            fps = framecount /10;
            framecount = 0;
            lasttime = curtime;
            qDebug() << "fps:" << fps;

        }

        cv::waitKey(10);
    }
       
       */


    QApplication a(argc, argv);
    Login login;

    //登录界面的show()在构造函数中实现了
    //login.show();

    MainInterface maininterface;

    //将主界面的界面展示槽函数与登录界面成功登录的信号连接，在这个响应的槽函数中选择加入界面的子窗口并显示
    maininterface.connectWithLogin(&login);
    //debugpole.show();

    //我认为不应该在login里决定如何跳转，应该在main里实现，但没有什么好方法 倒不算实现了 在main函数里进行了连接
    //if ((login.isHidden()) && ((g_config == -1) || (g_module == -1)))
    //    {
    //        DebugPole debugpole;
    //        debugpole.show();
    //       
    //    }
    return a.exec();

}
