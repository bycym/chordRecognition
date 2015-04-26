#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "IOMusicHandler/sounddata.h"
#include "IOMusicHandler/fileoperator.h"
#include "IOMusicHandler/playsound.h"
#include "MI/getfeatures.h"
#include <fftw3.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QTableWidget>
#include <QVector>
#include "MI/neuralnetworks.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void info();


private slots:
    void on_pushButton_clicked();

    void on_openButton_clicked();

    void on_playButton_clicked();

    void on_readDir_clicked();

private:
    Ui::MainWindow *ui;
    FileOperator * fileoperator;


    fftw_complex *in_, *out_;
    FileOperator * fileOperator_;
    PlaySound * playSound_;

    SoundData * sndData_;

    // for database
    QVector<SoundData*> database_;

    void coreFunction();



};

#endif // MAINWINDOW_H
