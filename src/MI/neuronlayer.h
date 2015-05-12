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
#include <chrono>
#define DEBUG 0

class NeuronLayer
{
private:

    /*!
     * \brief hidden_ hidden or output layer
     * true = hidden layer
     * false = output layer
     */
    bool hidden_;
    int numInput_;
    int numOutput_;
    int numNeuron_;
    double learningRate_;

    std::vector<double> inputs_;
    std::vector<double> biases_;
    std::vector<double> errorSignal_;

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
    // az összes elemen fusson végig ne paraméterrel
    void activationFunction(int alg = 1);

    /// activation functions ///

    void sigmoid();
    double devSigmoid(double x, int i);
    void softmax();

    /// END activation functions ///

public:


    //NeuronLayer() { assert(!"NOT use the Default NeuronLayer!"); }
    NeuronLayer(const NeuronLayer& n);

    /*!
     * \brief NeuronLayer create hidden layer
     * \param numInput number of input
     * \param numOutput number of output
     * \param numNeuron number of neuron
     * \param hidden is it hidden layer or output layer?
     * \param learningrate learningra of the neuralnetworks
     */
    NeuronLayer(int numInput, int numOutput, int numNeuron, bool hidden, double learningrate);
    ~NeuronLayer();

    /*!
     * \brief computeOutputs
     * \param inp input from other neuron layer or simple input
     * \param alg whitch activation function should use
     * alg 1 = sigmoid (hidden layer)
     * alg 2 = softmax (output layer)
     * \return outputs
     */
    void computeOutputs(int alg = 1);



    /*!
     * \brief updateInputs update the neuron layer input
     */
    void updateInputs(const std::vector<double>);
    void inputToOutput();
    int numInput() const { return numInput_; }
    int numOutput() const { return numOutput_; }
    int numNeuron() const { return numNeuron_; }

    void setLearningrate(double l){ learningRate_ = l; }
    double learningrate(){ return learningRate_; }


    /*!
     * \brief outputs computed value of the layer
     * \return outputs of the layer
     */
    std::vector<double> outputs() const
    {
        std::vector<double> tmp;
        for(int i = 0; i < outputs_.size(); i++)
            tmp.push_back(outputs_[i]);
        return tmp;
    }

    double outputs(int x) const
    {
        if(!( x >= 0 && x < outputs_.size()))
        {
            std::cerr << "Bad outputs index" << endl;
            qDebug() << "Bad outputs index" << endl;
        }
        return outputs_[x];
    }


    /*!
     * \brief weights
     * \param x numNeuron_
     * \param y numInput_
     * \return weights value
     */
    double weights(int x, int y) const
    {
        if(!( x >= 0 && x < numNeuron_ && y >=0 && y < numInput_))
        {
            std::cerr << "Bad weights index" << endl;
            qDebug() << "Bad weights index" << endl;
        }
        return weights_[x][y];
    }

    double biases(int x) const
    {
        if(!( x >= 0 && x < biases_.size()))
        {
            std::cerr << "Bad biases index" << endl;
            qDebug() << "Bad biases index" << endl;
        }
        return biases_[x];
    }

    double errorSignal(int x) const
    {
        if(!( x >= 0 && x < errorSignal_.size()))
        {
            std::cerr << "Bad out error index" << endl;
            qDebug() << "Bad out error index" << endl;
        }
        return errorSignal_[x];
    }



    /***
     * Learning algoritms/functions/value updating
     * */
    /// <start> ///

    /*!
     * \brief updateErrorSignal
     * it compute the errorsignal for the big neuron network
     * +---------------+-----------------+--------------------------+-------------+
     * | hiddLay. 1    | hiddLay. ...    | hiddLay. n               | outputLayer |
     * | outp * arr(n) | outp * arr(n-1) | outp * arr(outputLayer)  |             |
     * +---------------+-----------------+--------------------------+-------------+
     * 1) output layer:
     *      arr array is for the target array where the goal index 1, other's zero
     * 2) hidden neuron layer:
     *      arr array is for the previous neuron layer assistant array
     * \param array previous neuron layer computed value for the errorSignal update
     * \return compute for the next neuron layer
     */
    std::vector<double> updateErrorSignal(const std::vector<double> arr);



    /*!
     * \brief updateWeights calculate the new weight
     * this->learningRate * errorSignal_[allNeuron] * prevInput[all]
     *
     * \param prevInput previous neuron layer's output

     */
    void updateWeights(const std::vector<double> prevInput);


    /// <end> ///

};

#endif // NEURONLAYER_H
