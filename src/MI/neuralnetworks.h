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

    // pointerként megcsinálni
    NeuronLayer * outputNeuronLayer_;

    std::vector<NeuronLayer> neuronlayer_;
    std::vector<double> inputs_;                       // input to hidden
    std::vector<std::string> tags_;
    std::vector<double> outputs_;
    std::vector<double> error_;





public:

    /*!
     * \brief NeuralNetworks create the whole neural network
     * \param numInput number of input
     * \param numHiddenLayer number of hidden layer
     * \param numHiddenNeuron number of neuron in one hidden layer
     * \param numOutput number of output
     * \param learningrate learning rate
     */
    NeuralNetworks(int numInput, int numOutput, int numHiddenLayer, int numHiddenNeuron, double learningrate);


    /*!
     * \brief NeuralNetworks default learningrate, number of neurons in the hidden layer and number of hidden layer
     * \param numInput input
     */
    NeuralNetworks(int numInput);
    ~NeuralNetworks();



    /*!
     * \brief computeOutputs
     * compute input to output with the neural network
     */
    void computeOutputs();
    std::vector<double> outputs();



    /*!
     * \brief updateErrorSignal
     * update all neuron layer errorSignal
     * \param array previous layer's value for update
     * more comments in neuronLayer.h
     */
    void updateErrorSignal(std::vector<double> array);

    /*!
     * \brief updateWeights update all NeuronLayer weights
     */
    void updateWeights();

    /*!
     * \brief updateInputs
     * refill the inputs_
     * \param inp input from the project
     */
    void updateInputs(std::vector<double> inp);

    double learningRate(){ return learningRate_; }

    /*!
     * \brief setLearningRate update all NeuronLayer's learningrate
     * \param l input learning rate parameter
     */
    void setLearningRate(double l);

    std::string getTag();

    /*!
     * \brief setTag create the tag list
     * \param t tags list
     * \return any problem then false
     */
    bool setTag(std::vector<std::string> t);
};

#endif // NEURALNETWORKS_H
