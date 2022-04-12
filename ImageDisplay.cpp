#include "ImageDisplay.h"

ImageDisplay::ImageDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    QTimer* uiRefreshTimer = new QTimer(this);

    //设置参数值刷新时间10ms
    uiRefreshTimer->setInterval(100);
    connect(uiRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    uiRefreshTimer->start();

    //capture = cv::VideoCapture(0);
    //上下两种方式都可以打开摄像头
    
    /*capture.open(0);
    */
    //qDebug() << capture.get(3);

    
    
    
}

ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::refresh()
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
}
