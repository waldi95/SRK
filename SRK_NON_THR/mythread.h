#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = 0);
    void run();
signals:
    void go(int);

};

#endif // MYTHREAD_H
