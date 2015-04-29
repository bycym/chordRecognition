#ifndef NEURALNETWORKFORM_H
#define NEURALNETWORKFORM_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>

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

signals:
    void setParameters(const int numInput, const int numOutput, const int numHiddenLayer, const int numHiddenNeuron, const double learningrate);

private slots:
    void on_buttonBox_accepted();
};

#endif // NEURALNETWORKFORM_H
