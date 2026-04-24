#include "timemanager.h"
 #include <QTime>

TimeManager::TimeManager(QObject *parent)
    : QObject{parent}
{
    m_time = QTime::currentTime().toString("hh:mm:ss");
}

void TimeManager::timeReceive()
{
    // qDebug()<<"接收信号"<<QTime::currentTime().toString("hh:mm:ss");
    setTime(QTime::currentTime().toString("hh:mm:ss"));
}
// void TimeManager::updateTime(QString time){
//     m_time = time;
//     qDebug()<<"时间变化"+time;
// }