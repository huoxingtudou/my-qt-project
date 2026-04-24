#include "cameraimageprovider.h"
#include <QDebug>

CameraImageProvider::CameraImageProvider()
    :QQuickImageProvider(QQuickImageProvider::Image)
    {


}

QImage CameraImageProvider::requestImage(const QString &id,QSize *size,const QSize &requestedSize){

    // qDebug() << "【Provider】requestImage 被调用";
    Q_UNUSED(id)
    if (m_currentImage.isNull()) {
        // qDebug() << "【Provider】requestImage 但图像为空，返回占位图";
        QImage placeholder(640, 480, QImage::Format_RGB888);
        placeholder.fill(Qt::gray);
        if (size) *size = placeholder.size();
        return placeholder;
    }
    //加锁安全的读取图像
    QMutexLocker locker(&m_mutex);
    if(size){
        *size = m_currentImage.size();
    }
    // qDebug() << "【Provider】requestImage 返回图像，尺寸：" << m_currentImage.size();
    if(requestedSize.isValid()){
        return m_currentImage.scaled(requestedSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }

    return m_currentImage;

}

void CameraImageProvider::updateImage(const QImage &image){
    // qDebug() << "【Provider】updateImage 被调用，图像尺寸：" << image.size();
    if (image.isNull()) {
        // qDebug() << "【Provider】updateImage 收到空图像";
        return;
    }
    // qDebug() << "【Provider】updateImage 收到图像，尺寸：" << image.size();

    QMutexLocker locker(&m_mutex); //加锁，安全的更新图像
    m_currentImage = image.copy(); //拷贝一份，防止原数据被提前释放
}

