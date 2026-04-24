#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>
#include "timemanager.h"

class timeThread :public QThread
{
    Q_OBJECT
public:
    timeThread(TimeManager *tm,QObject *parent =nullptr);
    ~timeThread();
protected:
    void run() override;
public:
    bool m_isRuning;
    TimeManager *m_tm;

signals:
    void timeChange();
};

#endif // TIMETHREAD_H
