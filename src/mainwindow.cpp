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
    /*
    QString imp = "sample.wav";
    Sounddata snd(imp);
    */

    short x[8];
    cout << "sizeof(short x[8]): " << sizeof(x) << endl;
    cout << "sizeof(int): " << sizeof(int) << endl;
    cout << "sizeof(char): " << sizeof(char) << endl;
    cout << "sizeof(long): " << sizeof(long) << endl;
    cout << "sizeof(long long): " << sizeof(long long) << endl;
    cout << "sizeof(float): " << sizeof(float) << endl;
    cout << "sizeof(double): " << sizeof(double) << endl;
    cout << "sizeof(long double): " << sizeof(long double) << endl;
    cout << "sizeof(short): " << sizeof(short) << endl;
}

void MainWindow::on_openButton_clicked()
{
    if(fileoperator->open())
        ui->successLabel->setText("OK");
    else
        ui->successLabel->setText("ERROR");
}
