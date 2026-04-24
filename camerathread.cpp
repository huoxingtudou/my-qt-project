#include "camerathread.h"
#include "cameraimageprovider.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>


CameraThread::CameraThread(CameraImageProvider *provider ,TimeManager *tm,QObject *parent )
    :QThread(parent),m_provider(provider),m_running(true),m_tm(tm)
{
    // connect(this,&CameraThread::timeChange,m_tm,&TimeManager::timeReceive);

}

void CameraThread::stop()
{
    m_running = false; //设置停止标志
    this->wait();//等待线程结束
}

void CameraThread::run()
{

    // TimeManager *tm = new TimeManager(this);
    // emit timeChange();
    // cv::VideoCapture cap(0);
    cv::VideoCapture cap(0, cv::CAP_AVFOUNDATION);
    // cv::VideoCapture cap(1, cv::CAP_AVFOUNDATION);
    if(!cap.isOpened()){
        emit statusChanged("错误，无法打开摄像头");
        return;
    }
    emit statusChanged("摄像头已成功打开，开始采集");

    cv::Mat frame;
    // QElapsedTimer timer;

    // QTimer *timer = new QTimer(this);

    while(m_running){
        // if (m_tm) {
        //      m_tm->setTime(QTime::currentTime().toString("hh:mm:ss"));
        // }
        // emit timeChange();
         // qDebug()<<"线程时间变化："+QTime::currentTime().toString("hh:mm:ss");
        // qDebug()<<QThread::currentThread();
        // timer.start();
        cap >> frame; //从摄像头读取一帧
        if(frame.empty()){
            continue;//如果帧为空，则跳过
        }
        // qDebug() << "【OpenCV】读取一帧，尺寸：" << frame.cols << "x" << frame.rows << "，通道数：" << frame.channels();
        cv::Mat rgbFrame;
        //openCV 默认使用BGR格式，需转化为QImage支持的RCB格式
        cv::cvtColor(frame,rgbFrame,cv::COLOR_BGR2RGB);
        // QImage qImg(frame.data,frame.cols,frame.step,QImage::Format_RGB888);

        QImage qImg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        if (qImg.isNull()) {
            // qDebug() << "错误：转换后的 QImage 为空！";
            continue;
        }
        // 检查 m_provider 是否有效
        if (!m_provider) {
            // qDebug() << "严重错误：m_provider 是空指针！";
            emit statusChanged("内部错误：Provider 不存在");
            break;
        }
        //将转换好的QImage 传递给ImageProvider,它会安全地更新内部图像

        QImage imgCopy = qImg.copy();
        if (imgCopy.isNull()) {
            // qDebug() << "错误：QImage copy 失败！";
            continue;
        }
        // emit newImageReady(imgCopy);
        // qDebug() << "【OpenCV】QImage 拷贝成功，准备发射信号";
        m_provider->updateImage(imgCopy);

        msleep(100);


    }
    cap.release();//释放摄像头资源
    qDebug() << "【OpenCV】线程退出";
}

CameraThread::~CameraThread()
{
    m_running = false;
    wait();  // 这一行是强制习惯，不写就埋雷
}
