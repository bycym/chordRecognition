#ifndef NEURONLAYER_H
#define NEURONLAYER_H

/***
 * NeuronLayer class:
 *
 * inputs -> weights -> neuron -> activationFunction(.) -> outputs
 *                       |
 *                      bias
 *
 * */


#include <vector>
#include <iostream>
#include <math.h>
#include <random>>

class NeuronLayer
{
private:

    int numInput_;
    int numOutput_;
    int numNeuron_;

    std::vector<double> inputs_;
    std::vector<double> biases_;




    /*!
     * \brief weights_ is a two dimensional vector matrix:
     * row      | column (1)    | column (2)    | ...
     * ---------+---------------+---------------+----
     * neoron(1)| weights_(1)   | weights_(2)   | ...
     * neuron(2)| weights_(1)   | weights_(2)   | ...
     * ...
     * neuron(n)| weights_(1)   | weights_(2)   | ...
     */
    std::vector< std::vector<double>> weights_;
    std::vector<double> outputs_;

    // TODO: gradient function

    // TUDO: sum function
    double sum();

    // TODO: activaton function
    /*!
     * \brief activation function for neurons
     * \param x
     * \param alg What kind of algorithm should we use? default (=1): sigmoid
     * \return return with the activatin algorithm result
     */
    double activationFunction(double x, int alg = 1);

    /// activation functions ///

    double sigmoid(double x);
    double devSigmoid(double x);

    /// END activation functions ///

public:

    /*!
     * \brief NeuronLayer create hidden layer
     * \param numInput number of input
     * \param numOutput number of output
     * \param numNeuron number of neuron
     */
    NeuronLayer(int numInput, int numOutput, int numNeuron);
    ~NeuronLayer();

    /*!
     * \brief computeOutputs
     * \param inp input from other neuron layer or simple input
     * \param alg whitch activation function should use
     * \return outputs
     */
    std::vector<double> computeOutputs(std::vector<double> inp, int alg = 1);

    void updateWeights();


    void updateInputs(std::vector<double>);


    int numInput(){ return numInput_; }
    int numOutput(){ return numOutput_; }
    int numNeuron(){ return numNeuron_; }

};

#endif // NEURONLAYER_H
