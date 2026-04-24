#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QThread>


class TimeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_time READ getTime WRITE setTime NOTIFY timeChanged FINAL)
public:
    explicit TimeManager(QObject *parent = nullptr);

    QString getTime(){
        return m_time;
    }
    void setTime(const QString time){
        if(m_time==time)return;
        m_time = time;
        // qDebug()<<"时间管理类的时间"+time;
        // qDebug()<<QThread::currentThread();
        emit timeChanged();
    }
    // void updateTime(QString time);

signals:
    void timeChanged();
public slots:
    void timeReceive();
private:
    QString m_time;

};

#endif // TIMEMANAGER_H
