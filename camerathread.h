#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H


#include <QThread>
#include <QImage>
#include "opencv2/opencv.hpp"
#include <QTimer>
#include "timemanager.h"

class CameraImageProvider; //前向声明

class CameraThread :public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(CameraImageProvider *provider,TimeManager *tm,QObject *parent =nullptr);

    void stop();//用于安全的停止线程
signals:
    //用于调试，通知采集开始或报错
    void statusChanged(const QString &status);
    void newImageReady(const QImage &image);

protected:
    void run() override; //线程的主函数，在这里执行图像的采集循环

private:
    CameraImageProvider *m_provider;
    TimeManager *m_tm;


    bool m_running;//控制线程运行的标志
};

#endif // CAMERATHREAD_H
