#include "neuralnetworks.h"

NeuralNetworks::NeuralNetworks(int numInput, int numHiddenLayer, int numHiddenNeuron, int numOutput, double learningrate)
{
    this->numInput_ = numInput;
    this->numHiddenLayer_ = numHiddenLayer;
    this->numHiddenNeuron_ = numHiddenNeuron;
    this->learningRate_ = learningrate;
    this->numOutput_ = numOutput;


    if(numHiddenLayer == 1)
    {
        /// input -> neuron layer -> output
        this->neuronlayer_.push_back(NeuronLayer(numInput, numOutput, numHiddenNeuron));
    }
    else if(numHiddenLayer == 2)
    {
        /// input -> neuron layer -> neuron layer -> output
        this->neuronlayer_.push_back(NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron));
        this->neuronlayer_.push_back(NeuronLayer(numHiddenNeuron, numOutput, numHiddenNeuron));
    }
    else if(numHiddenLayer >= 3)
    {
        /// input -> neuron layer -> neuron layer (one or more) -> neuron layer -> output
        this->neuronlayer_.push_back(NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron));

        for(int i = 0; i < numInput; i++)
        {
            this->neuronlayer_.push_back(NeuronLayer(numHiddenNeuron, numHiddenNeuron, numHiddenNeuron));
        }

        this->neuronlayer_.push_back(NeuronLayer(numHiddenNeuron, numOutput, numHiddenNeuron));
    }
    else
    {
        std::cerr << "Problem with the numHiddenLayer. It must be pozitive!" << std::endl;
    }

}


NeuralNetworks::NeuralNetworks(int numInput)
{
    this->learningRate_ = 0.001;
    this->numHiddenLayer_ = 1;
    this->numHiddenNeuron_ = 35;
    this->numOutput_ = 10; // a am bm c d dm e em f g


    this->numInput_ = numInput;


    this->neuronlayer_.push_back(NeuronLayer(numInput_, numOutput_, numHiddenNeuron_));
}



NeuralNetworks::~NeuralNetworks()
{

}


std::vector<double> NeuralNetworks::computeOutputs(const std::vector<double> xValues)
{

    std::vector<double> inout;
    if(numHiddenLayer_ == 1)
    {
        NeuronLayer n = neuronlayer_[0];
        inout = n.computeOutputs(xValues);
        return inout;
    }
    else if(numHiddenLayer_ == 2)
    {
        NeuronLayer n1 = neuronlayer_[0];
        NeuronLayer n2 = neuronlayer_[1];
        inout = n1.computeOutputs(xValues);
        inout = n2.computeOutputs(inout);
        return inout;

    }
    else if(numHiddenLayer_ >= 3)
    {
        NeuronLayer n1 = neuronlayer_[0];
        NeuronLayer nn = neuronlayer_[numHiddenLayer_];

        inout = n1.computeOutputs(xValues);
        for(int i = 1; i < numHiddenLayer_ - 1; i++)
        {
            NeuronLayer n = neuronlayer_[i];
            inout = n.computeOutputs(inout);
        }
        inout = nn.computeOutputs(inout);
        return inout;
    }


    /// TODO: output layer!!!!

    return inout;
}


std::vector<double> NeuralNetworks::outputs()
{
    std::vector<double> result;
    for(int i = 0; i < outputs_.size(); i++)
        result.push_back(outputs_[i]);
    return result;
}
