#include "ImageDisplay.h"

ImageDisplay::ImageDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    uiptr = &ui;

    cam = new Cam();

    QTimer* RefreshTimer = new QTimer(this);

    RefreshTimer->setTimerType(Qt::TimerType::PreciseTimer);

    //设置查询flag时间间隔1ms，这样每两帧输出的时间差最长为1ms
    RefreshTimer->setInterval(10);
    connect(RefreshTimer, SIGNAL(timeout()), this, SLOT(queryStatus()));
    RefreshTimer->start();

    //开始观察按钮使能
    connect(ui.startCapture, SIGNAL(clicked()), this, SLOT(startCapture()));

    //停止观察按钮使能
    connect(ui.stopCapture, SIGNAL(clicked()), this, SLOT(stopCapture()));

    //开始保存图片按钮使能
    connect(ui.savePic, SIGNAL(clicked()), this, SLOT(savePic()));

    //capture = cv::VideoCapture(0);
    //上下两种方式都可以打开摄像头
    
    /*capture.open(0);
    */
    //qDebug() << capture.get(3);

    //把opencv的窗口控件绑定到qt的qlabel上
    frame = cv::Mat::zeros(cv::Size(2448,2048), CV_8UC3);
    frameShow = cv::Mat::zeros(cv::Size(612,512), CV_8UC3);
    qDebug() << frame.rows;
    winName = "frame";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    HWND hwnd = (HWND)cvGetWindowHandle(winName.c_str());
    HWND hparent = GetParent(hwnd);
    SetParent(hwnd, (HWND)ui.image->winId());
    ShowWindow(hparent, SW_HIDE);
    cv::setMouseCallback(winName, OnMouse, (void* )uiptr);
    //cv::imshow(winName, frameShow);
}

ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::queryStatus()
{

    //capture >> frame;
    //上下两种方式都可以读取帧到cv::Mat矩阵
    /*
    capture.read(frame);
    const uchar* pSrc = (const uchar*)frame.data;
    QImage qImage(pSrc, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    qImage = qImage.rgbSwapped();
    ui.image->setPixmap(QPixmap::fromImage(qImage));
    */

    /*imshow("摄像头", frame);
    cv::waitKey(10);  //这个数多少在opencv里的刷新率就是多少ms
    */
    //测试1ms的定时器精度能否达到要求
    //qDebug() << "刷新次数：" << ++counter;

    
    //cv::imshow("摄像头", cam->getFrame());


    if (cam->getStatus())
    {
        frame = cam->getFrame();
        //cv::resize(frame, frameShow, cv::Size(562, 512));
        //cv::imshow(winName, frameShow);
        cv::resize(frame, frameShow, cv::Size(0,0),0.25,0.25);

        cv::imshow(winName, frameShow);
    }

}

void ImageDisplay::startCapture()
{
    cam->startVideoCapture();

    
}

void ImageDisplay::stopCapture()
{

    cam->stopVideoCapture();
}

void ImageDisplay::savePic()
{
    time = QDateTime::currentDateTime();
    //保存的文件名不能有冒号什么的
    str_time = time.toString("yyyy-MM-dd-hh-mm-ss");
    //路径中不能出现中文
    //QString path = "d:\\磁控\\实验记录\\" + str_time + ".jpg";

    path = "D:/Experiment/" + str_time + ".jpg";
    //QString path = "D:/1.jpg";
    cv::imwrite(path.toStdString(), frame);
    
}

void ImageDisplay::OnMouse(int event, int x, int y, int flags, void* param)
{
    if (event == cv::EVENT_MOUSEMOVE)
    {
        QString str_x = QString::number(x * 4);
        QString str_y = QString::number(y * 4);
        QString coorOutput = "(" + str_y + "," + str_x + ")";

        (*(Ui::ImageDisplay * )param).Coordinate->setText(coorOutput);
    }
}
