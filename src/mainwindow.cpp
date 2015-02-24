#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileOperator_ = new FileOperator(this);
    sndData_ = new SoundData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString imp = "sample.wav";


    if(fileOperator_->performLoadOperation(imp, sndData_)) {
        ui->successLabel->setText("OK");
        ui->successLabel->setStyleSheet("QLabel { color: green }");
    }
    else {
        ui->successLabel->setText("ERROR");
        ui->successLabel->setStyleSheet("QLabel { color: red }");
    }

    getFeatures(sndData_);
}

void MainWindow::on_openButton_clicked()
{
    if(fileOperator_->open(sndData_))
        ui->successLabel->setText("OK");
    else
        ui->successLabel->setText("ERROR");
}
