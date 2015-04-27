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
        this->neuronlayer_.push_back(NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron, true, learningrate));
        /// neuron output layer
        this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron, numOutput, numHiddenNeuron, false, learningrate);
    }
    else if(numHiddenLayer >= 2)
    {
        /// input -> hidden neuron layer -> hidden neuron layer -> output
        this->neuronlayer_.push_back(NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron, true, learningrate));
        for(int i = 0; i < numInput; i++)
            this->neuronlayer_.push_back(NeuronLayer(numHiddenNeuron, numHiddenNeuron, numHiddenNeuron, true, learningrate));

        /// neuron output layer
        this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron, numOutput, numOutput, false, learningrate);
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
    this->neuronlayer_.push_back(NeuronLayer(numInput_, numOutput_, numHiddenNeuron_, true, learningRate_));
    /// output neuron layer
    this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron_, numOutput_, numOutput_, false, learningRate_);


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
        NeuronLayer tmp(n1.numOutput(), n1.numOutput(), n1.numNeuron(), true, learningRate_);
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
}


std::vector<double> NeuralNetworks::outputs()
{
    /*
    std::vector<double> result;
    for(int i = 0; i < outputs_.size(); i++)
        result.push_back(outputs_[i]);*/
    return outputNeuronLayer_->outputs();
}


void NeuralNetworks::calculateError()
{
    for (int i = 0; i < outputs_.size(); ++i) {
        error_[i] = 0 - outputs_[i];
    }
}


void NeuralNetworks::updateErrorSignal(std::vector<double> array)
{

    std::vector<double> tmpErrorSignal = outputNeuronLayer_->updateErrorSignal(array);


    for(int i = neuronlayer_.size(); i > 0; i--)
    {
        tmpErrorSignal = neuronlayer_[i].updateErrorSignal(tmpErrorSignal);
    }
}


void NeuralNetworks::updateWeights()
{
    std::vector<double> out = inputs_;

    for(NeuronLayer nl : neuronlayer_)
    {
        nl.updateWeights(out);
        out = nl.outputs();
    }
    outputNeuronLayer_->updateWeights(out);
}

void NeuralNetworks::updateInputs(std::vector<double> inp)
{
    inputs_.clear();
    for(auto i : inp)
        inputs_.push_back(i);
}
