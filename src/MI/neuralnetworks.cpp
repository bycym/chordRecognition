#include "neuralnetworks.h"

NeuralNetworks::NeuralNetworks(int numInput, int numHiddenLayer, int numHiddenNeuron, int numOutput, double learningrate)
{
    this->learningRate_ = 10;

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

NeuralNetworks::~NeuralNetworks()
{

}


std::vector<double> NeuralNetworks::ComputeOutputs(std::vector<double> xValues)
{
    if(numHiddenLayer_ == 1)
    {

    }
    else if(numHiddenLayer_ == 2)
    {

    }
    else if(numHiddenLayer_ >= 3)
    {

    }
}
