#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setFixedSize(sirkaGrid + sirkaGrid + mezeraGrid,sirkaGrid);
    w.show();
    w.setIconSize(QSize(16,16));

    return a.exec();
}
