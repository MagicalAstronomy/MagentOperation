#include "ImageDisplay.h"

ImageDisplay::ImageDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    cam = new Cam();

    QTimer* RefreshTimer = new QTimer(this);

    RefreshTimer->setTimerType(Qt::TimerType::PreciseTimer);

    //设置查询flag时间间隔1ms，这样每两帧输出的时间差最长为1ms
    RefreshTimer->setInterval(10);
    connect(RefreshTimer, SIGNAL(timeout()), this, SLOT(queryStatus()));
    RefreshTimer->start();

    //capture = cv::VideoCapture(0);
    //上下两种方式都可以打开摄像头
    
    /*capture.open(0);
    */
    //qDebug() << capture.get(3);

    
    
    
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

    
    cv::imshow("摄像头", cam->getFrame());


}
