#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "neuralnetworkform.h"
#include "IOMusicHandler/sounddata.h"
#include "IOMusicHandler/fileoperator.h"
#include "IOMusicHandler/playsound.h"
#include "MI/getfeatures.h"
#include "MI/neuralnetworks.h"
#include <fftw3.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QTableWidget>
#include <QVector>
#include "MI/neuralnetworks.h"
#include <QMovie>
#include <fstream>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void info(QVector<QString> info);


private slots:
    void on_pushButton_clicked();

    void on_openButton_clicked();

    void on_playButton_clicked();

    void on_readDir_clicked();

    void on_neuralNetwork_Button_clicked();

    NeuralNetworks* createNeuralNetwork(int numInput, int numOutput, int numHiddenLayer, int numHiddenNeuron, double learningrate);


    void on_reInitNeuralNetwork_clicked();

    void on_readDir_dirDevel_clicked();

    void on_switch_Button_clicked();

    void on_simple_Button_clicked();

private:
    Ui::MainWindow *ui;

    fftw_complex *in_, *out_;
    FileOperator * fileOperator_;
    PlaySound * playSound_;
    SoundData * sndData_;
    NeuralNetworkForm * nnf_;
    NeuralNetworks * neuralnetworks_;


    GetFeatures * sndDataFeatures_;
    std::vector<GetFeatures* > * databaseFeatures_;


    // for database
    QVector<SoundData*> database_;

    void coreFunction();

    bool databaseRead_;
    bool sampleRead_;
    bool sampleDBRead_;
    bool train_;

    std::vector<std::string> tags_;


    void train();
    void devel();



    //////////////////////////////////
    /// database dev stuff
    //////////////////////////////////

    void dataBaseDev();
    QVector<GetFeatures* > * dev_databaseFeatures_;
    QVector<SoundData*> dev_database_;
    bool flip;


    //////////////////////////////////
    /// database dev stuff EEEENNDDD
    //////////////////////////////////
    ///

    int myrandom(int i){ return std::rand()%i;}


};

#endif // MAINWINDOW_H
