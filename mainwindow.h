#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "view.h"
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CloseApplication();
    void LogWriter();
    QString enemyInfo = "localComputer";

private slots:
    void on_pushButtonNovaHra_clicked();
    void on_pushButtonStart_clicked();
    void on_comboBoxProtivnik_currentTextChanged(const QString &arg1);
    void on_pushButtonOtocitLod_clicked();
    void on_pushButtonNacistHru_clicked();
    void on_pushButtonUlozitHru_clicked();
    void on_pushButtonUkoncit_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    View *view;
};

#endif // MAINWINDOW_H
