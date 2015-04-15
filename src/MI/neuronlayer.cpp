#include "neuronlayer.h"

NeuronLayer::NeuronLayer(int numInput, int numOutput, int numNeuron, bool hidden)
{
    this->hidden_ = hidden;


    // random value between -1 and 1
    /// TODO: random ellenőrizni, két indiítással
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-1, 1);

    this->numInput_ = numInput;
    this->numOutput_ = numOutput;
    this->numNeuron_ = numNeuron;


    // TODO!
    /// neuron init
    for(int i = 0; i < numNeuron; i++)
    {
        /// number of input
        for(int j = 0; j < numInput; j++)
            weights_[i].push_back(dist(mt));
    }

    /*
    /// outputs init
    for(int i = 0; i < numOutput; i++)
        outputs_.push_back(0.00);
    */

    /// biases init
    for(int i = 0; i < numNeuron; i++)
        biases_.push_back(0);

    /// out error init
    for(int i = 0; i < numNeuron; i++)
        outerror_.push_back(0);
}

NeuronLayer::NeuronLayer(const NeuronLayer& n)
{
    numInput_ = n.numInput();
    numOutput_ = n.numOutput();
    numNeuron_ = n.numNeuron();

    /// neuron init
    for(int i = 0; i < numNeuron_; i++)
    {
        /// number of input
        for(int j = 0; j < numInput_; j++)
            weights_[i][j] = n.weights(i, j);
    }

    /// biases copy
    for(int i = 0; i < n.numNeuron(); i++)
        biases_.push_back(n.biases(i));

    /// output copy
    for(auto o : n.outputs_)
        outputs_.push_back(o);

    /// out error copy
    for(auto o : n.outerror_)
        outerror_.push_back(o);
}


NeuronLayer::~NeuronLayer()
{

}


void NeuronLayer::activationFunction(int alg)
{
    /*
    alg = (hidden_==true?1:2);
      */
    switch(alg)
    {
        case 1:
            return sigmoid();
        default:
            return sigmoid();
    }
}


void NeuronLayer::computeOutputs(int alg)
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
        for(int i = 0; i < inputs_.size(); i++)
        {
            outputs_[neuron] += inputs_[i] * weights_[neuron][i];
        }
    }

    // activation compute; default is sigmoid
    activationFunction(alg);

    /// if its output neuron layer then compute softmax
    if(!hidden_)
        softmax();
}


void NeuronLayer::updateWeights()
{

}

void NeuronLayer::updateInputs(const std::vector<double> inp)
{
    for(int i = 0; i < inp.size(); i++)
        inputs_[i] = inp[i];
}

void NeuronLayer::updateOuterror(const std::vector<double> inp)
{
    for(int i = 0; i < inp.size(); i++)
        outerror_[i] = inp[i];
}


void NeuronLayer::sigmoid()
{
    for(auto x : inputs_)
        outputs_.push_back(1 / ( 1 + exp( (-1) * x ) ));
}

void NeuronLayer::devSigmoid()
{
    // TODO!!!!!
    // double result = sigmoid();
    // TODO: return result * ( 1 - result );
}

void NeuronLayer::softmax()
{
    /// result(z)j = e^(zj) / sum(k=1 to K)(e^(zk)

    double sum = 0.0;
    /// for(all element) sum+=(e^inp[i])
    for(int i = 0; i < inputs_.size(); i++)
        sum += exp(inputs_[i]);

    for(auto x : inputs_)
    {
        /// e^(zj) / sum
        outputs_.push_back(exp(x)/sum);
    }
}

void NeuronLayer::inputToOutput()
{
    for(int i = 0; i < inputs_.size(); i++)
        outputs_[i] = inputs_[i];
}
