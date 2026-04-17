#include "timemanager.h"
 #include <QTime>

TimeManager::TimeManager(QObject *parent)
    : QObject{parent}
{
    m_time = QTime::currentTime().toString("hh:mm:ss.zzz");
}
// void TimeManager::updateTime(QString time){
//     m_time = time;
//     qDebug()<<"时间变化"+time;
// }