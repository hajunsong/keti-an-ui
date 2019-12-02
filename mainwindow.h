#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QtDebug>
#include <QApplication>
#include <QFileDialog>
#include <QTimer>
#include <QProcess>
#include <QDateTime>

#include "stepthread.h"
#include "qcustomplot.h"
#include "qclicklabel.h"
#include "qroundprogressbar.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // button event
    void btnStartPressed();
    void btnStartReleased();
    void btnStep2Clicked();

    // timer event
    void timeoutStep1Check();
    void timeoutStep2Check();

    // thread event
    void threadFinish();

private:
    Ui::MainWindow *ui;
    QString sharedDirPath, dirName, fileName, dirStep1, dirStep2;
    QFile *file;
    QTimer *timerStep1Check, *timerStep2Check;
    StepThread *stepThread;
    QCustomPlot *plot;
    int indx;
    QVector<double> x, y, y2;
    int totalProgressValue;

    void clickLabelDrawImage(QClickLabel *clickLabel, QString imagePath, double scale);
};
#endif // MAINWINDOW_H
