#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "genchords/src/Sounddata.h"
#include "IOMusicHandler/fileoperator.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_openButton_clicked();

private:
    Ui::MainWindow *ui;
    FileOperator * fileoperator;
};

#endif // MAINWINDOW_H
