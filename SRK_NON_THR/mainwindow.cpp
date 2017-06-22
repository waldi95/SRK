#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    paused = false;
    ui->setupUi(this);
    /* ========================= DRAW_SCENE =============================== */
    scene = new QGraphicsScene(this);
    ui->sce_scene->setScene(scene);
    /* ==================================================================== */

    /* ======================== TIME THREAD =============================== */
    QTimer *timer=new QTimer(this);
       connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
       timer->start();
    /* ==================================================================== */
    /* ======================== FIRST TRAIN THREAD ======================== */
    mThread = new myThread(this);
    connect(mThread,SIGNAL(go(int)),this,SLOT(on_go(int)));
    /* ==================================================================== */
    /* ======================== NEXT TRAIN THREAD ========================= */
    mThread2 = new myThread(this);
    connect(mThread2,SIGNAL(go(int)),this,SLOT(on_go2(int)),Qt::DirectConnection);
    /* ==================================================================== */
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* ========== ROZPOCZYNA SYMULACJE / WYPISZCZA PIERWSZY POCIAG ================= */
void MainWindow::on_btn3_clicked()
{   ui->btn3->setEnabled(false);
    ui->btnAdd1Train->setEnabled(true);
    int idBlokady = 0, maxSpeed = 0, lineLen = 0,
           blockLen = 0, numOfBlocks = 0, simulationSpeed = 1,
           trainLen = 0, offset = 0, tracePx = 1000,
           blockLenPx = 0, trainLenPx = 0;
       double tmp = 0;
       idBlokady = ui->cbb_wybierzBlokade->currentIndex();
       maxSpeed = ui->spb_maksymalnaPredkosc->value(); // [km/h]
       lineLen = ui->spb_dlugoscLinii->value()*1000;   // [m]
       blockLen = ui->spb_dlugoscOdcinkow->value();    // [m]
       tmp = blockLen/(double)lineLen;
       blockLenPx = tracePx*tmp;                       // [px]
       numOfBlocks = ui->spb_iloscOdcinkow->value();   //
       trainLen = ui->spb_dlugoscSkladu->value();      // [m]
       tmp = tracePx/(double)lineLen;
       trainLenPx = trainLen*tmp;                      // [px]
       simulationSpeed = ui->spb_predkoscSymulacji->value();
       offset = tracePx/numOfBlocks; // 1000px ma linia

       /* ================= RYSUJE SEMAFORY ODPOWIEDNIE DLA BLOKADY ============ */
       switch(idBlokady)
       {
       case 1: pbl();break;
       case 2: sbl2(numOfBlocks,offset);break;
       case 3: sbl3(numOfBlocks,offset);break;
       case 4: sbl4(numOfBlocks,offset);break;
       }
       /* ====================================================================== */

       draw(offset);
      // BusyBlocks(tracePx,blockLenPx,trainLenPx,offset);
}
/* ============================================================================= */

/* ================ SYMULUJE JAZDE POCIAGU PO TORZE ============================ */
void MainWindow::BusyBlocks(int tracePx, int blockLenPx, int trainLenPx, int offset)
{
    int poczatek = 0, koniec = 0, tmp;
    int simulationSpeed = ui->spb_predkoscSymulacji->value();
    int idBlokady = ui->cbb_wybierzBlokade->currentIndex();
       QPen blackPen(Qt::black);
       QPen redPen(Qt::red);
       QPen whitePen(Qt::white);
       blackPen.setWidth(6);
       redPen.setWidth(6);
       for(int i=0; i<=tracePx+trainLenPx+9; i+=simulationSpeed)
       {
           simulationSpeed = ui->spb_predkoscSymulacji->value(); // umożliwia zmiane prędkości symulacji w trakcie jej działania
           ui->label->setNum(i);
           poczatek = i;
           koniec = i-trainLenPx;
           tmp = koniec-simulationSpeed;
           if(poczatek>1000) poczatek = 1000;
           if(koniec<0) koniec = 0;
           if(tmp<0) tmp = 0;
           if (poczatek<=1000) //994 bo grubość linii
            train = scene->addLine(koniec,250,poczatek,250,redPen);
           if (koniec <= 1000)
            train = scene->addLine(tmp,250,koniec,250,blackPen);
           /* ================= WYBIERA ODPOWIEDNI SYSTEM DO ZAŚWIECANIA SEMAFOROW ======= */
           switch(idBlokady)
           {
           case 1:pblOn(poczatek,koniec);break;
           case 2:sbl2On((i/blockLenPx)+1,((i-trainLenPx)/blockLenPx)+1, offset);break;
           case 3:sbl3On((i/blockLenPx)+1,((i-trainLenPx)/blockLenPx)+1, offset);break;
           case 4:sbl4On((i/blockLenPx)+1,((i-trainLenPx)/blockLenPx)+1, offset);break;
           }
           /* ============================================================================ */

           delay(180); // OPÓŹNIENIE W ms
       }
}
/* =============================== RYSUJE ŚCIEŻKE ================================ */
void MainWindow::draw(int offset)
{
      int idBlokady;
      idBlokady = ui->cbb_wybierzBlokade->currentIndex();
      QPen blackPen(Qt::black);
      QPen bluePen(Qt::blue);
      blackPen.setWidth(6);
      bluePen.setWidth(4);
      block = scene->addLine(7,250,994,250,blackPen);
      if (idBlokady!=1)
          for(int i=1; i<=901; i+=offset)
          {
              if(i>offset)
                sem = scene->addLine(i+1,245,i+1,244,bluePen);
          }
}
/* ============================================================================== */

/* =============================== OPÓŹNIENIE =================================== */
void MainWindow::delay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
/* ============================================================================== */

/* ============================== DRAWING PBL =================================== */
void MainWindow::pbl()
{
    QPen blackPen(Qt::black);
    QBrush blackBrush(Qt::black);
    blackPen.setWidth(2);
     semafor = scene -> addRect(0,110,20,100,blackPen);
        light = scene -> addEllipse(3,115,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(3,134,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(3,153,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(3,172,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(3,191,14,14,blackPen,blackBrush);
        sem = scene -> addLine(-10,215,30,215,blackPen);
        sem = scene -> addLine(-10,215,-10,235,blackPen);
        sem = scene -> addLine(30,215,30,235,blackPen);
        sem = scene -> addLine(-10,235,0,240,blackPen);
        sem = scene -> addLine(0,240,20,240,blackPen);
        sem = scene -> addLine(20,240,30,235,blackPen);
        light = scene -> addEllipse(-7,222,9,3,blackPen,blackBrush);
        light = scene -> addEllipse(5,222,9,3,blackPen,blackBrush);
        light = scene -> addEllipse(17,222,9,3,blackPen,blackBrush);
     semafor = scene -> addRect(978,110,20,100,blackPen);
        light = scene -> addEllipse(981,115,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(981,134,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(981,153,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(981,172,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(981,191,14,14,blackPen,blackBrush);
        sem = scene -> addLine(968,215,1008,215,blackPen);
        sem = scene -> addLine(968,215,968,235,blackPen);
        sem = scene -> addLine(1008,215,1008,235,blackPen);
        sem = scene -> addLine(968,235,978,240,blackPen);
        sem = scene -> addLine(978,240,998,240,blackPen);
        sem = scene -> addLine(998,240,1008,235,blackPen);
        light = scene -> addEllipse(971,222,9,3,blackPen,blackBrush);
        light = scene -> addEllipse(983,222,9,3,blackPen,blackBrush);
        light = scene -> addEllipse(995,222,9,3,blackPen,blackBrush);


}
/* ============================================================================== */

/* ============================== DRAWING SBL 2S ================================ */
void MainWindow::sbl2(int numOfBlocks, int offset)
{
      QPen blackPen(Qt::black);
      QBrush blackBrush(Qt::black);
      QPen greenPen(Qt::green);
      QBrush greenBrush(Qt::green);
      blackPen.setWidth(2);
      for(int i=0; i<numOfBlocks; i++)
      {
          semafor = scene -> addRect(i*offset+1,200,20,40,blackPen);
          light = scene -> addEllipse(i*offset+4,204,14,14,blackPen,blackBrush);
          light = scene -> addEllipse(i*offset+4,222,14,14,greenPen,greenBrush);
      }
}
/* ============================================================================== */

/* ============================= DRAWING SBL 3S ================================= */
void MainWindow::sbl3(int numOfBlocks, int offset)
{
    QPen blackPen(Qt::black);
    QBrush blackBrush(Qt::black);
    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);
    blackPen.setWidth(2);
    for(int i=0; i<numOfBlocks; i++)
    {
        semafor = scene -> addRect(i*offset+1,180,20,60,blackPen);
        light = scene -> addEllipse(i*offset+4,184,14,14,greenPen,greenBrush);
        light = scene -> addEllipse(i*offset+4,202,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(i*offset+4,222,14,14,blackPen,blackBrush);
    }
}
/* ============================================================================== */

/* ============================== DRAWING SBL 4S ================================ */
void MainWindow::sbl4(int numOfBlocks, int offset)
{
    QPen blackPen(Qt::black);
    QBrush blackBrush(Qt::black);
    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);
    blackPen.setWidth(2);
    for(int i=0; i<numOfBlocks; i++)
    {
        semafor = scene -> addRect(i*offset+1,180,20,60,blackPen);
        light = scene -> addEllipse(i*offset+4,184,14,14,greenPen,greenBrush);
        light = scene -> addEllipse(i*offset+4,202,14,14,blackPen,blackBrush);
        light = scene -> addEllipse(i*offset+4,222,14,14,blackPen,blackBrush);
    }

}
/* ============================================================================== */

/* ============================== SYSTEM PBL ==================================== */
void MainWindow::pblOn(int head, int tail)
{
    //pobiera informacje z radio i checkboxow a nastepnie zaswieca
    // na ich podstawie odpowiednie lampki na pierwszym i na drugim (ostatnim) semaforze
}
/* ============================================================================== */

/* ============================== SYSTEM SBL 2S ================================ */
void MainWindow::sbl2On(int head, int tail, int offset)
{
    int numOfBlocks = ui->spb_dlugoscOdcinkow->value();
    QPen blackPen(Qt::black);
    QBrush blackBrush(Qt::black);
    QPen redPen(Qt::red);
    QBrush redBrush(Qt::red);
    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);

 //   if(head<numOfBlocks-2)
    {
        light = scene -> addEllipse((head-1)*offset+4,204,14,14,redPen,redBrush);
        light = scene -> addEllipse((head-1)*offset+4,222,14,14,blackPen,blackBrush);
    }

    if(tail-2>=0)
    {
        light = scene -> addEllipse((tail-2)*offset+4,204,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-2)*offset+4,222,14,14,greenPen,greenBrush);
    }

}
/* ============================================================================== */

/* ============================== DRAWING SBL 3S ================================ */
void MainWindow::sbl3On(int head, int tail, int offset)
{
    int numOfBlocks = ui->spb_dlugoscOdcinkow->value();
    QPen blackPen(Qt::black);
    QBrush blackBrush(Qt::black);
    QPen redPen(Qt::red);
    QBrush redBrush(Qt::red);
    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);
    QPen yellowPen(Qt::yellow);
    QBrush yellowBrush(Qt::yellow);

 //   if(head<numOfBlocks-2)
    {
        light = scene -> addEllipse((head-1)*offset+4,184,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((head-1)*offset+4,202,14,14,redPen,redBrush);
           light = scene -> addEllipse((head-1)*offset+4,222,14,14,blackPen,blackBrush);
    }

    if(tail-2>=0)
    {
        light = scene -> addEllipse((tail-2)*offset+4,184,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-2)*offset+4,202,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-2)*offset+4,222,14,14,yellowPen,yellowBrush);
    }

    if(tail-3>=0)
    {
        light = scene -> addEllipse((tail-3)*offset+4,184,14,14,greenPen,greenBrush);
        light = scene -> addEllipse((tail-3)*offset+4,202,14,14,blackPen,blackBrush);
           light = scene -> addEllipse((tail-3)*offset+4,222,14,14,blackPen,blackBrush);
    }
}
/* ============================================================================== */

/* ============================== DRAWING SBL 4S ================================ */
void MainWindow::sbl4On(int head, int tail, int offset)
{
    int numOfBlocks = ui->spb_dlugoscOdcinkow->value();
    QPen blackPen(Qt::black);
    QBrush blackBrush(Qt::black);
    QPen redPen(Qt::red);
    QBrush redBrush(Qt::red);
    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);
    QPen yellowPen(Qt::yellow);
    QBrush yellowBrush(Qt::yellow);
    QPen whitePen(Qt::white);
    QBrush whiteBrush(Qt::white);

        ui->label->setNum(head);
    if(head<=10)
    {
        light = scene -> addEllipse((head-1)*offset+4,184,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((head-1)*offset+4,202,14,14,redPen,redBrush);
        light = scene -> addEllipse((head-1)*offset+4,222,14,14,blackPen,blackBrush);
    }
ui->label_2->setNum(tail);
    if(tail-2>=0)
    {

        light = scene -> addEllipse((tail-2)*offset+4,184,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-2)*offset+4,202,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-2)*offset+4,222,14,14,yellowPen,yellowBrush);
    }

    if(tail-3>=0)
    {
        greenPen.setWidth(5);
        light = scene -> addEllipse((tail-3)*offset+6,186,11,11,greenPen,whiteBrush);
        light = scene -> addEllipse((tail-3)*offset+4,202,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-3)*offset+4,222,14,14,blackPen,blackBrush);
        greenPen.setWidth(1);

    }

    if(tail-4>=0)
    {
        light = scene -> addEllipse((tail-4)*offset+4,184,14,14,greenPen,greenBrush);
        light = scene -> addEllipse((tail-4)*offset+4,202,14,14,blackPen,blackBrush);
        light = scene -> addEllipse((tail-4)*offset+4,222,14,14,blackPen,blackBrush);
    }
}
/* ============================================================================== */

/* ========================= WYPUSZCZANIE POCIAGOW ============================== */
void MainWindow::on_btnAdd1Train_clicked()
{
    ui->btnAddTrain->setEnabled(true);
    ui->btnAdd1Train->setEnabled(false);
    int tracePx, blockLenPx, trainLenPx, offset;
    double tmp = 0;
       tracePx = 1000;
       tmp = ui->spb_dlugoscOdcinkow->value()/((double)ui->spb_dlugoscLinii->value()*1000);
       blockLenPx = tracePx*tmp;
       tmp = tracePx/((double)ui->spb_dlugoscLinii->value()*1000);
       trainLenPx = ui->spb_dlugoscSkladu->value()*tmp;
       offset = tracePx/ui->spb_iloscOdcinkow->value();

       BusyBlocks(tracePx,blockLenPx,trainLenPx,offset);

}
void MainWindow::on_go(int i)
{/*
    int tracePx, blockLenPx, trainLenPx, offset;
    double tmp = 0;
       tracePx = 1000;
       tmp = ui->spb_dlugoscOdcinkow->value()/((double)ui->spb_dlugoscLinii->value()*1000);
       blockLenPx = tracePx*tmp;
       tmp = tracePx/((double)ui->spb_dlugoscLinii->value()*1000);
       trainLenPx = ui->spb_dlugoscSkladu->value()*tmp;
       offset = tracePx/ui->spb_iloscOdcinkow->value();

       BusyBlocks(tracePx,blockLenPx,trainLenPx,offset);*/
}
void MainWindow::on_btnAddTrain_clicked()
{
    ui->btnAddTrain->setEnabled(false);
    int tracePx, blockLenPx, trainLenPx, offset;
    double tmp = 0;
       tracePx = 1000;
       tmp = ui->spb_dlugoscOdcinkow->value()/((double)ui->spb_dlugoscLinii->value()*1000);
       blockLenPx = tracePx*tmp;
       tmp = tracePx/((double)ui->spb_dlugoscLinii->value()*1000);
       trainLenPx = ui->spb_dlugoscSkladu->value()*tmp;
       offset = tracePx/ui->spb_iloscOdcinkow->value();

       BusyBlocks(tracePx,blockLenPx,trainLenPx,offset);
}
void MainWindow::on_go2(int i)
{/*
    int tracePx, blockLenPx, trainLenPx, offset;
    double tmp = 0;
       tracePx = 1000;
       tmp = ui->spb_dlugoscOdcinkow->value()/((double)ui->spb_dlugoscLinii->value()*1000);
       blockLenPx = tracePx*tmp;
       tmp = tracePx/((double)ui->spb_dlugoscLinii->value()*1000);
       trainLenPx = ui->spb_dlugoscSkladu->value()*tmp;
       offset = tracePx/ui->spb_iloscOdcinkow->value();

       BusyBlocks(tracePx,blockLenPx,trainLenPx,offset);*/
}
/* ========================================================================================== */

/* ===================================== WYBOR BLOKADY ====================================== */
void MainWindow::on_cbb_wybierzBlokade_currentIndexChanged(int index)
{
    if(index == 1)
    {
        ui->spb_dlugoscLinii->setEnabled(true);
        ui->spb_dlugoscOdcinkow->setEnabled(false);
        ui->spb_iloscOdcinkow->setEnabled(false);
        ui->cbb_PBLSignals->setEnabled(true);
        // comment: dodatkowo jeśli zostanie wybrana PBL, to start enabled
        //          gdy zostanie wybrany sygnał
        if(ui->cbb_PBLSignals->currentIndex()>=1 && ui->cbb_PBLSignals->currentIndex()<=18)
        {
            ui->btn3->setEnabled(true);
        }else
        {
            ui->btn3->setEnabled(false);
        }
    }
    else if(index >1 && index <=4 )
    {
        ui->spb_dlugoscOdcinkow->setEnabled(true);
        ui->spb_iloscOdcinkow->setEnabled(true);
        ui->spb_dlugoscLinii->setEnabled(false);
        ui->cbb_PBLSignals->setEnabled(false);
        ui->btn3->setEnabled(true);
    }else
    {
        ui->spb_dlugoscOdcinkow->setEnabled(false);
        ui->spb_iloscOdcinkow->setEnabled(false);
        ui->spb_dlugoscLinii->setEnabled(false);
        ui->cbb_PBLSignals->setEnabled(false);
         ui->btn3->setEnabled(false);
    }
}



/* ======================================================================================== */

/* =================================== CZYSZCZENIE SYMULACJI ============================== */
// comment: działa zanim wypuścimy pociąg i jak pociąg już przejedzie

void MainWindow::on_btnClear_clicked()
{
    ui->btn3->setEnabled(true);
    scene->clear();
}
/* ======================================================================================== */

/* ======================================= PAUZOWANIE SYMULACJI =========================== */
// comment: w planach
void MainWindow::on_btnPause_clicked()
{
  /*  if (paused == false)
    {
        paused=true;
        ui->btnPause->setText("PAUSE");
    }else
    {
        paused=false;
        ui->btnPause->setText("PLAY");
    }
    while(this->paused);*/
}
/* ======================================================================================= */
