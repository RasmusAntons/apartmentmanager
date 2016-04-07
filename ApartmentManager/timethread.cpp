#include "timethread.h"
#include <QtCore>

TimeThread::TimeThread(QObject *parent) :
    QThread(parent)
{
}

void TimeThread::run()
{

    while (!this->isInterruptionRequested() ) {
        this->msleep(500);
        emit TimeChanged();
    }
}
