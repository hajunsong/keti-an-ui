#include "stepthread.h"
#include <QDir>
#include <QtDebug>

StepThread::StepThread()
{
    m_start = false;
}

void StepThread::run(){
    QProcess *process = new QProcess(this);
    QString file("C:/Users/hajun/Desktop/Step1.exe");
    process->execute(file);

//    qDebug() << "Start Thread";

//    QString file_name;
//    QString dir_name = "//192.168.201.205/SharedFolder";
//    QFile *file = new QFile();
//    for(uint i = 0;i < 50; i++){
//        file_name = dir_name + "/" + "Mission_3/Step1/test_" + QString::number(i) + ".txt";
//        qDebug() << file_name;
//        file->setFileName(file_name);
//        file->open(QIODevice::Append | QIODevice::Text);
//        file->close();
//    }

    emit finish();
}
