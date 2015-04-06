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
#include <random>
#include <QDebug>

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
     * row      | input (1)     | input (2)     | ...
     * ---------+---------------+---------------+----
     * neoron(1)| weights_(1)   | weights_(2)   | ...
     * neuron(2)| weights_(1)   | weights_(2)   | ...
     * ...
     * neuron(n)| weights_(1)   | weights_(2)   | ...
     */
    std::vector< std::vector<double>> weights_;
    std::vector<double> outputs_;

    // TODO: gradient function

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


    NeuronLayer() {}
    NeuronLayer(const NeuronLayer& n);

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
    std::vector<double> computeOutputs(const std::vector<double> inp, int alg = 1);

    void updateWeights();


    void updateInputs(const std::vector<double>);


    int numInput() const { return numInput_; }
    int numOutput() const { return numOutput_; }
    int numNeuron() const { return numNeuron_; }

    std::vector<double> outputs()
    {
        std::vector<double> tmp;
        for(int i = 0; i < outputs_.size(); i++)
            tmp.push_back(outputs_[i]);
        return tmp;
    }

    /*!
     * \brief weights
     * \param x numNeuron_
     * \param y numInput_
     * \return weights value
     */
    double weights(int x, int y) const
    {
        if(!( x >= 0 && x <= numNeuron_ && y >=0 && y <= numInput_))
        {
            std::cerr << "Bad weights index" << endl;
            qDebug() << "Bad weights index" << endl;
        }
        return weights_[x][y];
    }

    double biases(int x) const
    {
        if(!( x >= 0 && x <= biases_.size()))
        {
            std::cerr << "Bad biases index" << endl;
            qDebug() << "Bad biases index" << endl;
        }
        return biases_[x];
    }

};

#endif // NEURONLAYER_H
