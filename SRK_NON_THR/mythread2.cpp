
#include "mythread2.h"
#include <QtCore>

myThread2::myThread2(QObject *parent) :
    QThread(parent)
{

}

void myThread2::run()
{
    /*
     * TUTAJ CO ROBI WĄTEK -> EMITUJE DO go()
    for(int i=0; i<1000; i++)
    {
        QMutex mutex;
        mutex.lock();
        if(this->Stop) break;
        while(this->Pause);
        mutex.unlock();
        emit NumberChanged(i);
        this->msleep(100);
    }
    */
    emit go(1);

}
