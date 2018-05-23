#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "view.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new View(&scene);
    scene.setSceneRect(0,0,View::sirkaGrid + View::sirkaGrid + View::mezeraGrid,View::sirkaGrid);
    this->setCentralWidget(view);
    ui->comboBoxProtivnik->addItem(QIcon(),QString("LocalComputer"),QVariant());
    ui->comboBoxProtivnik->addItem(QIcon(),QString("LAN"),QVariant());
    ui->comboBoxProtivnik->addItem(QIcon(),QString("Server (online)"),QVariant());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CloseApplication()
{
    QApplication::quit();
}

void MainWindow::LogWriter()
{
//    qDebug() << View::latestConsoleText;
}

void MainWindow::on_pushButtonNovaHra_clicked()
{
    if(view->beforeStart){
        view->StartNewGame();
    }
    else{
        qDebug() << "Are you sure to start new game without saving?";
        //dialog s tlacitky OK a CANCEL
        view->StartNewGame();
    }

}

void MainWindow::on_pushButtonStart_clicked()
{
    if(!view->beforeStart && view->building && view->pocetShip1x1 == 0 && view->pocetShip2x1 == 0 && view->pocetShip3x1 == 0 && view->pocetShip4x1 == 0){
        view->StartGame(QString(enemyInfo));//docasne dokud neni na vyber vice druhu protihracu
    }
}

void MainWindow::on_comboBoxProtivnik_currentTextChanged(const QString &arg1)
{
    if(ui->comboBoxProtivnik->currentText() == "LocalComputer"){
        enemyInfo = QString("localComputer");
        qDebug() << "Selected LocalComputer as enemy";
    }
    else{
        qDebug() << "This option can not be selected now";
    }
}

void MainWindow::on_pushButtonOtocitLod_clicked()
{
    if(!view->beforeStart && view->building){
        if(view->rotation == 1){
            view->rotation = 2;
            qDebug() << "Rotation changed to vertical";
        }
        else{
            view->rotation = 1;
            qDebug() << "Rotation changed to horizontal";
        }
        view->UpdateHoverShipPublicSlots();
    }
}

void MainWindow::on_pushButtonNacistHru_clicked()
{
    qDebug() << "Opening file read dialog";
}

void MainWindow::on_pushButtonUlozitHru_clicked()
{
    qDebug() << "Opening file save dialog";
}

void MainWindow::on_pushButtonUkoncit_clicked()
{
//    CloseApplication("testovaci text");
    this->CloseApplication();
}
