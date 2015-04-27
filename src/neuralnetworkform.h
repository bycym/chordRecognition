#ifndef NEURALNETWORKFORM_H
#define NEURALNETWORKFORM_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class NeuralNetworkForm;
}

class NeuralNetworkForm : public QDialog
{
    Q_OBJECT

public:
    explicit NeuralNetworkForm(QWidget *parent = 0);
    ~NeuralNetworkForm();


    int numInput();
    int numOutput();
    int numHiddenLayer();
    int numHiddenNeuron();
    double learningrate();


private:
    Ui::NeuralNetworkForm *ui_nnform;
};

#endif // NEURALNETWORKFORM_H
