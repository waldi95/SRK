#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QTimer>
#include <QtGui>
#include <QTime>
#include <QDateTime>
#include "mythread.h"
#include <QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    myThread *mThread, *mThread2;
    bool paused;
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsLineItem *block, *sem, *train, *part;
    QGraphicsRectItem *semafor;
    QGraphicsEllipseItem *light;

public slots:
    void on_go(int);
    void on_go2(int);

private slots:
    void on_btn3_clicked();
    void BusyBlocks(int,int, int, int);
    void draw(int);
    void delay(int);
    void pbl();
    void sbl2(int,int);
    void sbl3(int,int);
    void sbl4(int,int);
    void pblOn(int,int);
    void sbl2On(int,int,int);
    void sbl3On(int,int,int);
    void sbl4On(int,int,int);
    void on_btnAdd1Train_clicked();
    void on_btnAddTrain_clicked();
    void on_cbb_wybierzBlokade_currentIndexChanged(int index);
    void on_btnClear_clicked();
    void on_btnPause_clicked();
};

#endif // MAINWINDOW_H
