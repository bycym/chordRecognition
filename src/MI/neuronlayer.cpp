#include "neuronlayer.h"

NeuronLayer::NeuronLayer(int numInput, int numOutput, int numNeuron)
{
    // random value between -1 and 1
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-1, 1);


    this->numInput_ = numInput;
    this->numOutput_ = numOutput;
    this->numNeuron_ = numNeuron;

    /// neuron
    for(int i = 0; i < numNeuron; i++)
    {
        /// number of input
        for(int j = 0; j < numInput; j++)
            weights_[i].push_back(dist(mt));
    }
}

NeuronLayer::~NeuronLayer()
{

}


double NeuronLayer::activationFunction(double x, int alg)
{
    switch(alg)
    {
        case 1:
            return sigmoid(x);
        default:
            return sigmoid(x);
    }
}

double NeuronLayer::sigmoid(double x)
{
    return 1 / ( 1 + exp( (-1) * x ) );
}

double NeuronLayer::devSigmoid(double x)
{
    double result = sigmoid(x);
    return result * ( 1 - result );
}
