#ifndef NEURALNETWORKS_H
#define NEURALNETWORKS_H

#include "../Utils/matrixhelpers.h"

class NeuralNetworks
{
private:

    int numInput_;
    int numHidden_;
    int numOutput_;

    double * inputs_;                       // input to hidden
    double ** ihWeights_;
    double * ihSums_;
    double * ihBiases_;
    double * ihOutputs_;

    double ** hoWeights_;                   // hidden-to-output
    double * hoSums_;
    double * hoBiases_;
    double * outputs_;

    double * oGrads_;                       // output gradients for back-propagation
    double * hGrads_;                       // hidden gradients for back-propagation

    double ** ihPrevWeightsDelta_;          // for momentum with back-propagation
    double * ihPrevBiasesDelta_;

    double ** hoPrevWeightsDelta_;
    double * hoPrevBiasesDelta_;


    static double StepFunction(double x);
    static double SidmoidFunction(double x);
    static double HyperTanFunction(double x);


public:
    NeuralNetworks(int numINput, int numHidden, int numOutput);
    ~NeuralNetworks();

    void UpdateWeights(double * tValues, double eta, double alpha);
    void setWeights(double * weights);
    double* GetWeights();
    double* ComputeOutputs(double * xValues);
};

#endif // NEURALNETWORKS_H
