#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileoperator = new FileOperator(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString imp = "sample.wav";
    Sounddata snd(imp);
}

void MainWindow::on_openButton_clicked()
{
    if(fileoperator->open())
        ui->success->setText("OK");
    else
        ui->success->setText("ERROR");
}
