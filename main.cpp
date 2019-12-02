#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("KETI Assembly Network Control UI");
    w.setStyleSheet("QMainWindow {background: 'white';}");
    w.show();
    return a.exec();
}
