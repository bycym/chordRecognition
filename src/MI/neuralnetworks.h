#ifndef NEURALNETWORKS_H
#define NEURALNETWORKS_H

#include "../Utils/matrixhelpers.h"
#include "neuronlayer.h"
#include <vector>
#include <iostream>

class NeuralNetworks
{
private:

    int numInput_;

    /// number of hidden layer
    int numHiddenLayer_;

    /// number of neuron in on hidden layer
    int numHiddenNeuron_;

    /// learining rate
    double learningRate_;

    int numOutput_;

    std::vector<NeuronLayer> neuronlayer_;
    std::vector<double> inputs_;                       // input to hidden


public:
    /*!
     * \brief NeuralNetworks create the whole neural network
     * \param numInput number of input
     * \param numHiddenLayer number of hidden layer
     * \param numHiddenNeuron number of neuron in one hidden layer
     * \param numOutput number of output
     * \param learningrate learning rate
     */
    NeuralNetworks(int numInput, int numHiddenLayer, int numHiddenNeuron, int numOutput, double learningrate);


    /*!
     * \brief NeuralNetworks default learningrate, number of neurons in the hidden layer and number of hidden layer
     * \param numInput input
     * \param numOutput output
     */
    NeuralNetworks(int numInput, int numOutput);
    ~NeuralNetworks();

    void UpdateWeights(std::vector<double> tValues, double eta, double alpha);
    void setWeights(std::vector<double> weights);
    std::vector<double> GetWeights();
    std::vector<double> ComputeOutputs(std::vector<double> xValues);
};

#endif // NEURALNETWORKS_H
