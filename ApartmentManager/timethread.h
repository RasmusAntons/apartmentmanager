#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>

class TimeThread : public QThread
{
    Q_OBJECT
public:
    explicit TimeThread(QObject *parent = 0);
    void run();


signals:
    void TimeChanged();

public slots:

};

#endif // TIMETHREAD_H
