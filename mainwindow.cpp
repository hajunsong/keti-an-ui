#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnStart, SIGNAL(mousePressed()), this, SLOT(btnStartPressed()));
    connect(ui->btnStart, SIGNAL(mouseReleased()), this, SLOT(btnStartReleased()));

    clickLabelDrawImage(ui->btnStart, ":/image/start_released.png", 0.45);

    ui->pbStep1->setFormat("Motion Study");
    ui->pbStep1->setAlignment(Qt::AlignCenter);
    ui->pbStep2->setFormat("AI");
    ui->pbStep2->setAlignment(Qt::AlignCenter);
    ui->pbStep3->setFormat("Step Parsing");
    ui->pbStep3->setAlignment(Qt::AlignCenter);

    clickLabelDrawImage(ui->keti_an, ":/image/keti_an.png", 1.0);
    clickLabelDrawImage(ui->keti_logo, ":/image/keti_logo.jpg", 0.15);

    ui->pbTotal->setDecimals(0);
    ui->pbTotal->setRange(0, 100);
    ui->pbTotal->setValue(0);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    ui->pbTotal->setAutoFillBackground(true);
    ui->pbTotal->setPalette(pal);

    ui->pbStep1->setEnabled(true);
    ui->pbStep2->setEnabled(false);
    ui->pbStep3->setEnabled(false);

    clickLabelDrawImage(ui->led, ":/image/led_gray.png", 0.4);

    timerStep1Check = new QTimer(this);
    connect(timerStep1Check, SIGNAL(timeout()), this, SLOT(timeoutStep1Check()));
    timerStep2Check = new QTimer(this);
    connect(timerStep2Check, SIGNAL(timeout()), this, SLOT(timeoutStep2Check()));

//    sharedDirPath = QFileDialog::getExistingDirectory(this, "", "C:/Users/hajun/Desktop");
    sharedDirPath = "//192.168.201.205/SharedFolder";
    qDebug() << sharedDirPath;
    if (sharedDirPath.size() == 0){
        exit(0);
    }

    dirName = "Mission_3";
    dirStep1 = "Step1";
    dirStep2 = "Step2";

    // folder exit check
    QDir dir;
    bool dir_exist = dir.exists(sharedDirPath + "/" + dirName);
    qDebug() << dir_exist;
    if (!dir_exist){
        dir.mkdir(sharedDirPath + "/" + dirName);
    }
    dir.mkdir(sharedDirPath + "/" + dirName + "/" + dirStep1);
    dir.mkdir(sharedDirPath + "/" + dirName + "/" + dirStep2);

    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidthF(4);
    ui->plot->graph(0)->setPen(blueDotPen);
    ui->plot->graph(0)->setName("data1");

    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis2);
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    ui->plot->graph(1)->setName("data2");

    ui->plot->xAxis->setRange(-1, 1);
    ui->plot->yAxis->setRange(-1, 1);
    ui->plot->yAxis2->setRange(-1, 1);
    ui->plot->yAxis2->setVisible(true);
    ui->plot->replot();
    ui->plot->xAxis->setLabel("step");
    ui->plot->yAxis->setLabel("data1");
    ui->plot->yAxis2->setLabel("data2");

    ui->plot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->plot->legend->setFont(legendFont);
    ui->plot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    indx = 0;

    qsrand(static_cast<unsigned int>(QDateTime::currentMSecsSinceEpoch() / 1000));

    totalProgressValue = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnStartPressed(){
    qDebug() << "Pressed Start Button";

    clickLabelDrawImage(ui->btnStart, ":/image/start_pressed.png", 0.45);
}

void MainWindow::btnStartReleased(){
    qDebug() << "Released Start Button";

    clickLabelDrawImage(ui->btnStart, ":/image/start_released.png", 0.45);

    stepThread = new StepThread();
    connect(stepThread, SIGNAL(finish()), this, SLOT(threadFinish()));
    stepThread->start();

    timerStep1Check->start(100);
}

void MainWindow::clickLabelDrawImage(QClickLabel *clickLabel, QString imagePath, double scale) {
    QImage *image = new QImage();
    QPixmap *buffer = new QPixmap();
    if (image->load(imagePath)) {
        *buffer = QPixmap::fromImage(*image);
        *buffer = buffer->scaled(static_cast<int>(image->width()*scale), static_cast<int>(image->height()*scale));
    }
    clickLabel->setPixmap(*buffer);
    clickLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

    delete image;
    delete buffer;
}

//void MainWindow::btnStep1Clicked(){
//    stepThread = new StepThread();
//    connect(stepThread, SIGNAL(finish()), this, SLOT(threadFinish()));
//    stepThread->start();

//    timerStep1Check->start(100);
//}

void MainWindow::timeoutStep1Check(){
    QDir dir(sharedDirPath + "/" + dirName + "/" + dirStep1);
    double max_count = 50;
    if (dir.exists()){
        QStringList files = dir.entryList();
        int progressValue = static_cast<int>(((files.count()-2)/max_count)*100);
        qDebug() << progressValue;
        ui->pbStep1->setValue(progressValue);
//        ui->lbStatus->setText("Step 1 ...");
        totalProgressValue = progressValue/3;
        ui->pbTotal->setValue(totalProgressValue);

        if (progressValue == 100){
            qDebug() << "Step 1 Finished !!!";
            timerStep1Check->stop();
            ui->pbStep1->setEnabled(false);
            ui->pbStep2->setEnabled(true);
            btnStep2Clicked();
        }
    }
}

void MainWindow::btnStep2Clicked(){
    file = new QFile();
    file->setFileName(sharedDirPath + "/" + dirName + "/" + dirStep2 + "/start.txt");
    file->open(QIODevice::Append | QIODevice::Text);
    file->close();

    timerStep2Check->start(100);
}

void MainWindow::timeoutStep2Check(){
    QDir dir(sharedDirPath + "/" + dirName + "/" + dirStep2);
    double max_count = 500;
    if (dir.exists()){
        QStringList files = dir.entryList();
        int progressValue = static_cast<int>(((files.count()-3)/max_count)*100);
        qDebug() << progressValue;
        ui->pbStep2->setValue(progressValue);
//        ui->lbStatus->setText("Step 2 ...");
        totalProgressValue = 33 + progressValue/3;
        ui->pbTotal->setValue(totalProgressValue);

        x.push_back(indx++);
        y.push_back(progressValue);
        ui->plot->graph(0)->setData(x,y);

        y2.push_back(progressValue*qrand());
        ui->plot->graph(1)->setData(x,y2);

        std::vector<double> x_temp, y_temp, y2_temp;
        x_temp = x.toStdVector();
        y_temp = y.toStdVector();
        y2_temp = y2.toStdVector();
        sort(y_temp.begin(), y_temp.end());
        sort(y2_temp.begin(), y2_temp.end());

        ui->plot->xAxis->setRange(x_temp.front(), x_temp.back());
        ui->plot->yAxis->setRange(y_temp.front(), y_temp.back());
        ui->plot->yAxis2->setRange(y_temp.front(), y2_temp.back());

        ui->plot->replot();
    }

    QFile file(sharedDirPath + "/" + dirName + "/" + dirStep2 + "/finish.txt");
    if(file.exists()){
        qDebug() << "Step 2 Finished !!!";
        timerStep2Check->stop();
    }
}

void MainWindow::threadFinish(){

}

