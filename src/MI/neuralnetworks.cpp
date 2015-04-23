#include "neuralnetworks.h"

NeuralNetworks::NeuralNetworks(int numInput, int numOutput, int numHiddenLayer, int numHiddenNeuron, double learningrate)
{
    this->numInput_ = numInput;
    this->numHiddenLayer_ = numHiddenLayer;
    this->numHiddenNeuron_ = numHiddenNeuron;
    this->learningRate_ = learningrate;
    this->numOutput_ = numOutput;


    if(numHiddenLayer == 1)
    {
        /// input -> hidden neuron layer -> output
        this->neuronlayer_.push_back(NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron, true));
        /// neuron output layer
        this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron, numOutput, numHiddenNeuron, false);
    }
    else if(numHiddenLayer >= 2)
    {
        /// input -> hidden neuron layer -> hidden neuron layer -> output
        this->neuronlayer_.push_back(NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron, true));
        for(int i = 0; i < numInput; i++)
            this->neuronlayer_.push_back(NeuronLayer(numHiddenNeuron, numHiddenNeuron, numHiddenNeuron, true));

        /// neuron output layer
        this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron, numOutput, numOutput, false);
    }
    else
    {
        std::cerr << "Problem with the numHiddenLayer. It must be pozitive!" << std::endl;
    }


    /// init output
    for(int i = 0; i < numOutput_; i++)
        this->outputs_.push_back(0.00);


    /// init error
    for(int i = 0; i < numOutput_; i++)
        this->error_.push_back(0.00);
}


NeuralNetworks::NeuralNetworks(int numInput)
{
    this->learningRate_ = 0.001;
    this->numHiddenLayer_ = 1;
    this->numHiddenNeuron_ = 35;
    this->numOutput_ = 10; // a am bm c d dm e em f g


    this->numInput_ = numInput;

    /// hidden neuron layer
    this->neuronlayer_.push_back(NeuronLayer(numInput_, numOutput_, numHiddenNeuron_, true));
    /// output neuron layer
    this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron_, numOutput_, numOutput_, false);


    /// init output
    for(int i = 0; i < numOutput_; i++)
        this->outputs_.push_back(0.00);


    /// init error
    for(int i = 0; i < numOutput_; i++)
        this->error_.push_back(0.00);

}



NeuralNetworks::~NeuralNetworks()
{
    delete outputNeuronLayer_;
}


void NeuralNetworks::computeOutputs(const std::vector<double> xValues)
{

    std::vector<double> inout;
    if(numHiddenLayer_ == 1)
    {
        NeuronLayer n = neuronlayer_[0];
        n.updateInputs(xValues);
        n.computeOutputs(1);
        outputNeuronLayer_->updateInputs(n.outputs());
        outputNeuronLayer_->computeOutputs(1);
    }
    else if(numHiddenLayer_ >= 2)
    {
        NeuronLayer n1 = neuronlayer_[0];

        n1.updateInputs(xValues);
        n1.computeOutputs(1);

        /// tmp NeuronLayer help to pass outputs
        /// now every n th neuron layer can pass their ouputs
        /// to the nex neuron layer
        NeuronLayer tmp(n1.numOutput(), n1.numOutput(), n1.numNeuron(), true);
        tmp.updateInputs(n1.outputs());
        tmp.inputToOutput();
        for(int i = 1; i < numHiddenLayer_; i++)
        {
            NeuronLayer n = neuronlayer_[i];
            n.updateInputs(tmp.outputs());
            n.computeOutputs(1);
            tmp.updateInputs(n1.outputs());
            tmp.inputToOutput();
        }
        outputNeuronLayer_->updateInputs(tmp.outputs());
        outputNeuronLayer_->computeOutputs(1);
    }





    return inout;
}


std::vector<double> NeuralNetworks::outputs()
{
    /*
    std::vector<double> result;
    for(int i = 0; i < outputs_.size(); i++)
        result.push_back(outputs_[i]);*/
    return outputNeuronLayer_->outputs();
}


