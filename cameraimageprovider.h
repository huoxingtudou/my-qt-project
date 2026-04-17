#ifndef CAMERAIMAGEPROVIDER_H
#define CAMERAIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class CameraImageProvider : public QQuickImageProvider
{
public:
    //构造函数，使用Image 标志注册
    CameraImageProvider() ;
    //重写requestImage方法，QML引擎通过这个方法请求最新的图片
    QImage requestImage(const QString &id,QSize *size,const QSize &requestedSize) override;

    //提供一个公开的方法，用于从外部(采集线程)更新图像
    void updateImage(const QImage &image);

private:
    QImage m_currentImage;  // 存储当前最新的图像
    QMutex m_mutex;  // 互斥锁，保证多线程读写安全
signals:

};

#endif // CAMERAIMAGEPROVIDER_H
