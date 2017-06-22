#ifndef MYTHREAD2_H
#define MYTHREAD2_H

#include <QThread>
class myThread2 : public QThread
{
    Q_OBJECT
public:
    explicit myThread2(QObject *parent = 0);
    void run();
signals:
    void go(int);
public slots:

};

#endif // MYTHREAD2_H
