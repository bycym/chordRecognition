#include "neuralnetworks.h"

NeuralNetworks::NeuralNetworks(int numInput, int numOutput, int numHiddenLayer, int numHiddenNeuron, double learningrate)
{
    this->numInput_ = numInput;
    this->numHiddenLayer_ = numHiddenLayer;
    this->numHiddenNeuron_ = numHiddenNeuron;
    this->learningRate_ = learningrate;
    this->numOutput_ = numOutput;
    this->resultTag_ = "";

    if(numHiddenLayer == 1)
    {
        /// input -> hidden neuron layer -> output
        this->neuronlayer_.push_back(new NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron, true, learningrate));
        /// neuron output layer
        this->outputNeuronLayer_ = new NeuronLayer(numHiddenNeuron, numOutput, numOutput, false, learningrate);
    }
    else if(numHiddenLayer >= 2)
    {
        /// input -> hidden neuron layer -> hidden neuron layer -> output
        this->neuronlayer_.push_back(new NeuronLayer(numInput, numHiddenNeuron, numHiddenNeuron, true, learningrate));
        for(int i = 0; i < numHiddenLayer-1; i++)
            this->neuronlayer_.push_back(new NeuronLayer(numHiddenNeuron, numHiddenNeuron, numHiddenNeuron, true, learningrate));

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
    this->numOutput_ = 11; // a am b bm c d dm e em f g


    this->numInput_ = numInput;

    /// hidden neuron layer
    this->neuronlayer_.push_back(new NeuronLayer(numInput_, numOutput_, numHiddenNeuron_, true, learningRate_));
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
    //std::cout << "delete neural networks" << endl;
    delete outputNeuronLayer_;
}


void NeuralNetworks::computeOutputs()
{

    if(numHiddenLayer_ == 1)
    {
        //NeuronLayer n = neuronlayer_[0];
        neuronlayer_[0]->updateInputs(inputs_);
        neuronlayer_[0]->computeOutputs(1);
        outputNeuronLayer_->updateInputs(neuronlayer_[0]->outputs());
        outputNeuronLayer_->computeOutputs(1);
    }
    else if(numHiddenLayer_ >= 2)
    {
        //NeuronLayer n1 = neuronlayer_[0];
        std::vector<double> out;

        neuronlayer_[0]->updateInputs(inputs_);
        neuronlayer_[0]->computeOutputs(1);


        for(int j = 0; j < neuronlayer_[0]->numNeuron(); j++)
            out.push_back(neuronlayer_[0]->outputs(j));

        for(int i = 1; i < numHiddenLayer_; i++)
        {
            //NeuronLayer n = neuronlayer_[i];
            neuronlayer_[i]->updateInputs(out);
            neuronlayer_[i]->computeOutputs(1);
            out.clear();
            for(int j = 0; j < neuronlayer_[i]->numNeuron(); j++)
                out.push_back(neuronlayer_[i]->outputs(j));
        }
        outputNeuronLayer_->updateInputs(out);
        outputNeuronLayer_->computeOutputs(1);
    }

    this->outputs_.clear();
    for(int i = 0; i < outputNeuronLayer_->numOutput(); i++)
    {
        outputs_.push_back(outputNeuronLayer_->outputs(i));
    }

    int index = 0;
    double max = 0.0;
    for(int i = 0; i < outputs_.size(); i++)
    {
        if(outputs_[i] > max)
        {
            index = i;
            max = outputs_[i];
        }
    }
    if(DEBUG) std::cout << "tags_[" << index << "]: " << tags_[index] << std::endl;
    resultTag_ = tags_[index];
}


std::vector<double> NeuralNetworks::outputs()
{

    std::vector<double> result;
    for(int i = 0; i < outputNeuronLayer_->numOutput(); i++)
        result.push_back(outputNeuronLayer_->outputs(i));
    return result;
}



void NeuralNetworks::updateErrorSignal(std::vector<double> array)
{

    std::vector<double> tmpErrorSignal = outputNeuronLayer_->updateErrorSignal(array);

    for(int i = neuronlayer_.size()-1; i >= 0; i--)
    {
        tmpErrorSignal = neuronlayer_[i]->updateErrorSignal(tmpErrorSignal);
    }
}


void NeuralNetworks::updateWeights()
{
    std::vector<double> out;


    for(auto i : inputs_)
        out.push_back(i);

    /*
    for(auto& nl : neuronlayer_)
    {
        nl->updateWeights(out);
        out = nl->outputs();
    }*/



    for(int i = 0; i < neuronlayer_.size(); i++)
    {
        neuronlayer_[i]->updateWeights(out);
        out.clear();
        for(int j = 0; j < neuronlayer_[i]->numNeuron(); j++)
            out.push_back(neuronlayer_[i]->outputs(j));
    }
    outputNeuronLayer_->updateWeights(out);
}

void NeuralNetworks::updateInputs(std::vector<double> inp)
{
    inputs_.clear();
    for(auto i : inp)
        inputs_.push_back(i);
}

void NeuralNetworks::setLearningRate(double l)
{
    outputNeuronLayer_->setLearningrate(l);
    for(auto &n : neuronlayer_)
        n->setLearningrate(l);
}


bool NeuralNetworks::setTag(std::vector<std::string> t)
{
    if(t.size() != outputs_.size())
    {
        qDebug() << "Input tag list too big!\nTag list size must equals with outputs size";
        qDebug() << "outputs_.size() = " << outputs_.size() << " | t.size() = " << t.size();
        return false;
    }
    tags_.clear();
    for(auto i : t)
        tags_.push_back(i);

    if(t.size() == tags_.size())
        return true;

}

std::string NeuralNetworks::getTag()
{
    /*
    int index = 0;
    double max = -2.0;
    for(int i = 0; i < outputs_.size(); i++)
    {
        if(outputs_[i] > max)
        {
            index = i;
            max = outputs_[i];
        }
    }*/
    return resultTag_;
}
