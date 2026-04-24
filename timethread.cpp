#include "timethread.h"
#include <QDebug>

timeThread::timeThread(TimeManager *tm, QObject *parent): QThread(parent) {
    m_isRuning = true;
    m_tm =tm;
    connect(this,&timeThread::timeChange,m_tm,&TimeManager::timeReceive);
}
void timeThread::run(){
    while (m_isRuning) {
        // qDebug()<<"发射信号 ";
        emit timeChange();
        msleep(1000);
    }
}

timeThread::~timeThread(){
    m_isRuning =false;
    wait();
}