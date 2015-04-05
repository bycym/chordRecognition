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

    /// neuron init
    for(int i = 0; i < numNeuron; i++)
    {
        /// number of input
        for(int j = 0; j < numInput; j++)
            weights_[i].push_back(dist(mt));
    }

    /// outputs init
    for(int i = 0; i < numOutput; i++)
        outputs_.push_back(0.00);

    /// biases init
    for(int i = 0; i < numNeuron; i++)
        biases_.push_back(1);
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


std::vector<double> NeuronLayer::computeOutputs(std::vector<double> inp, int alg)
{
    /// compute:
    /// 1 -> b(n)
    /// x1(n) -> w1(n) ->
    /// x2(n) -> w2(n) ->
    /// ...
    /// xn(n) -> wn(n) -> sum() -> activationF()-> y(n)

    // all neuron result to zero
    for(auto out : outputs_)
        out = 0;

    // all neuron
    for(int neuron = 0; neuron < numNeuron_; neuron++)
    {
        // all input * weights
        for(int i = 0; i < inp.size(); i++)
        {
            outputs_[neuron] += inp[i] * weights_[neuron][i];
        }
    }

    // activation compute
    for(auto out: outputs_)
        out = activationFunction(out, alg);

    return outputs_;

}


void NeuronLayer::updateWeights()
{

}

void NeuronLayer::updateInputs(std::vector<double> inp)
{
    for(int i = 0; i < inp.size(); i++)
        inputs_[i] = inp[i];
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
