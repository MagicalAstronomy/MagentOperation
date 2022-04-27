#include "ImageDisplay.h"
#pragma execution_character_set("utf-8")

ImageDisplay::ImageDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    uiptr = &ui;

    cam = new Cam();
    imageprocessing = new ImageProcessing();

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

    //保存图片按钮使能
    connect(ui.savePic, SIGNAL(clicked()), this, SLOT(savePic()));

    //开始保存视频按钮使能
    connect(ui.saveVideoOn, SIGNAL(clicked()), this, SLOT(saveVideoOn()));

    //停止保存视频按钮使能
    connect(ui.saveVideoOff, SIGNAL(clicked()), this, SLOT(saveVideoOff()));

    //显示和保存视频的信号与槽函数连接
    connect(this, SIGNAL(emit_showFrame()), this, SLOT(slot_showFrame()));

    connect(this, SIGNAL(emit_saveVideo()), this, SLOT(slot_saveVideo()));

    //开始定位磁珠按钮使能 其实不点击按钮一样会自动寻找磁珠位置，只是不会显示
    connect(ui.startLocateMag, SIGNAL(clicked()), this, SLOT(startLocateMag()));
    connect(ui.stopLocateMag, SIGNAL(clicked()), this, SLOT(stopLocateMag()));

    //开始和结束绘制中心点使能
    connect(ui.drawCenteralPoint, SIGNAL(clicked()), this, SLOT(drawCenteralPoint()));
    connect(ui.stopdrawCenteralPoint, SIGNAL(clicked()), this, SLOT(stopdrawCenteralPoint()));

    //开始和结束绘制目标点使能
    connect(ui.drawTargetPoint, SIGNAL(clicked()), this, SLOT(drawTargetPoint()));
    connect(ui.stopdrawTargetPoint, SIGNAL(clicked()), this, SLOT(stopdrawTargetPoint()));

    //capture = cv::VideoCapture(0);
    //上下两种方式都可以打开摄像头
    
    /*capture.open(0);
    */
    //qDebug() << capture.get(3);

    //把opencv的窗口控件绑定到qt的qlabel上
    frame = cv::Mat::zeros(cv::Size(2448,2048), CV_8UC3);
    frameShow = cv::Mat::zeros(cv::Size(612,512), CV_8UC3);
    //qDebug() << frame.rows;
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
        //frame = cam->getFrame();
        ////cv::resize(frame, frameShow, cv::Size(562, 512));
        ////cv::imshow(winName, frameShow);
        //cv::resize(frame, frameShow, cv::Size(0,0),0.25,0.25);

        //cv::imshow(winName, frameShow);

        //if (videowriter != nullptr && videowriter->isOpened())
        //{
        //    videowriter->write(frame);
        //}

        frame = cam->getFrame();

        //使用最近邻插值加速放缩，会消耗11ms左右
        cv::resize(frame, frameShow, cv::Size(0, 0), 0.25, 0.25,cv::INTER_NEAREST);

        //获得磁珠的中心和半径(缩小尺寸下)
        //imageprocessing->locateMagnet(frameShow, magent_center, magent_radius);
        
        coor[0] = magent_center.x * 4;
        coor[1] = magent_center.y * 4;
        coor[2] = target_center.x * 4;
        coor[3] = target_center.y * 4;
        emit emit_feedback(coor);
        emit emit_showFrame();
        emit emit_saveVideo();
        
    }

}

void ImageDisplay::slot_showFrame()
{
    
    if (flag_drawCenteral)
    {
        cv::circle(frameShow, target_center, magent_radius, CV_RGB(0, 0, 255), 1);
    }
    if (flag_drawTarget)
    {
        cv::circle(frameShow, target_center, magent_radius, CV_RGB(0, 0, 255), 1);
    }
    if (flag_locatemag)
    {
        imageprocessing->locateMagnet(frameShow, magent_center, magent_radius);

        cv::circle(frameShow, magent_center, magent_radius, CV_RGB(255, 0, 0), 1);

        
        QString magentCoor = "(" + QString::number(magent_center.y,'f', 0) + "," + QString::number(magent_center.x, 'f', 0) + ")";

        ui.magentCoor->setText(magentCoor);
    }

    

    cv::imshow(winName, frameShow);

    

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
    if (cv::imwrite(path.toStdString(), frameShow))
    {
        qDebug() << "保存图片成功！";
    }
    
}

void ImageDisplay::saveVideoOn()
{
    time = QDateTime::currentDateTime();
    str_time = time.toString("yyyy-MM-dd-hh-mm-ss");
    path = "D:/Experiment/" + str_time + ".avi";

    //一开始把videowriter作为ViderWriter类的对象，但感觉不如指针好用
    //videowriter.open(path.toStdString(), videowriter.fourcc('M', 'J', 'P', 'G'), 60, frame.size(), true);
    videowriter = new cv::VideoWriter(path.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, frameShow.size(), true);
    if (videowriter->isOpened())
    {
        qDebug() << "已开启视频录制！";
    }
}

void ImageDisplay::saveVideoOff()
{
    
    if (videowriter == nullptr)
    {
        qDebug() << "尚未开启视频录制！";
    }
    else
    {
        videowriter->release();
        videowriter = nullptr;
        qDebug() << "成功结束录制！";
    }


}void ImageDisplay::slot_saveVideo()
{

    if (videowriter != nullptr && videowriter->isOpened())
    {
        videowriter->write(frameShow);
        //qDebug() << ++counter;
    }

}

void ImageDisplay::drawCenteralPoint()
{
    target_center = cv::Point(frameShow.cols / 2, frameShow.rows / 2);
    flag_drawCenteral = true;
}

void ImageDisplay::stopdrawCenteralPoint()
{
    flag_drawCenteral = false;
}

void ImageDisplay::drawTargetPoint()
{
    if (ui.target_x->text().isEmpty() || ui.target_y->text().isEmpty())
    {
        qDebug() << "请输入原图像大小下目标点坐标！";
    }
    else
    {
        target_center = cv::Point2f(ui.target_y->text().toFloat() / 4, ui.target_x->text().toFloat() / 4);
        flag_drawTarget = true;
    }
    
}

void ImageDisplay::stopdrawTargetPoint()
{
    flag_drawTarget = false;
}

void ImageDisplay::startLocateMag()
{
    flag_locatemag = true;
}

void ImageDisplay::stopLocateMag()
{
    flag_locatemag = false;
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

