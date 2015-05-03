#include "neuronlayer.h"

NeuronLayer::NeuronLayer(int numInput, int numOutput, int numNeuron, bool hidden, double learningrate)
{
    // random value between -1 and 1
    /// TODO: random ellenőrizni, két indiítással
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-1, 1);

    this->hidden_ = hidden;
    this->numInput_ = numInput;
    this->numOutput_ = numOutput;
    this->numNeuron_ = numNeuron;
    this->learningRate_ = learningrate;


    for(int i = 0; i < numNeuron; i++)
    {
        std::vector<double> row;
        /// number of input
        for(int j = 0; j < numInput; j++){
            double tmp = dist(mt);
            row.push_back(tmp);
            //std::cout<< "w: " << tmp << ", ";
        }
        //std::cout<< std::endl;
        weights_.push_back(row);
    }


    /// outputs init
    for(int i = 0; i < numOutput; i++)
        outputs_.push_back(0.00);


    /// biases init
    for(int i = 0; i < numNeuron; i++)
        biases_.push_back(0.00);

    /// out error init
    for(int i = 0; i < numNeuron; i++)
        errorSignal_.push_back(0.00);
}

NeuronLayer::NeuronLayer(const NeuronLayer& n)
{
    numInput_ = n.numInput();
    numOutput_ = n.numOutput();
    numNeuron_ = n.numNeuron();

    /// neuron init
    for(int i = 0; i < numNeuron_; i++)
    {
        std::vector<double> row;
        /// number of input
        for(int j = 0; j < numInput_; j++)
            row.push_back(n.weights_[i][j]);
        weights_.push_back(row);
    }

    /// biases copy
    for(int i = 0; i < n.numNeuron(); i++)
        biases_.push_back(n.biases(i));

    /// output copy
    for(auto o : n.outputs_)
        outputs_.push_back(o);

    /// out error copy
    for(auto o : n.errorSignal_)
        errorSignal_.push_back(o);
}


NeuronLayer::~NeuronLayer()
{
    /*
    std::cout << "delete ";
    if(hidden_)
        std::cout << "hidden ";
    else
        std::cout << "output ";
    std::cout << "neuron layer" << endl;
    */
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
    /*for(auto out : outputs_)
        out = 0;
    */
    outputs_.clear();
    // all neuron
    for(int neuron = 0; neuron < numNeuron_; neuron++)
    {
        // all input * weights
        double out = 0;
        for(int i = 0; i < inputs_.size(); i++)
        {
            out += inputs_[i] * weights_[neuron][i];
        }
        outputs_.push_back(out);
    }

    // activation compute; default is sigmoid
    activationFunction(alg);

    /// if its output neuron layer then compute softmax
    if(!hidden_)
        softmax();
}


void NeuronLayer::updateInputs(const std::vector<double> inp)
{
    // TODO!!! check it!
    inputs_.clear();
    numInput_ = inp.size();
    for(int i = 0; i < inp.size(); i++)
        inputs_.push_back(inp[i]);
}

void NeuronLayer::sigmoid()
{
    for(auto x : outputs_)
        x = (1 / ( 1 + exp( (-1) * x ) ));
}

double NeuronLayer::devSigmoid(double x)
{
    return (1 / ( 1 + exp( (-1) * x ) )) * (1- (1 / ( 1 + exp( (-1) * x ) )));
}

void NeuronLayer::softmax()
{
    /// result(z)j = e^(zj) / sum(k=1 to K)(e^(zk)

    double sum = 0.0;
    /// for(all element) sum+=(e^inp[i])
    for(int i = 0; i < outputs_.size(); i++)
        sum += exp(outputs_[i]);

    for(auto x : outputs_)
    {
        /// e^(zj) / sum
        x = (exp(x)/sum);
    }
}

void NeuronLayer::inputToOutput()
{
    for(int i = 0; i < inputs_.size(); i++)
        outputs_[i] = inputs_[i];
}


std::vector<double> NeuronLayer::updateErrorSignal(std::vector<double> array)
{

    std::vector<double> result;

    /// if its output layer
    if(!hidden_)
    {
        /// error signal = target[pattern][i] - output[pattern][i]
        for(int i = 0; i < errorSignal_.size(); i++)
        {

            errorSignal_[i] = array[i] - outputs_[i];
        }

        /// for previous neuron layer errorSignal update
        /// iterate for all neuron of hidden neuron layer
        for(int i = 0; i < numInput_; i++)
        {
            /// compute errorSignal for every input neuron of
            /// last hidden neuron layer
            /// sooo :
            /// +------------+--------------+------------+-------------+
            /// | hiddLay. 1 | hiddLay. ... | hiddLay. n | outputLayer |
            /// +------------+--------------+------------+-------------+
            /// iterate on all output layer's neuron
            double tmp = 0;
            for(int j = 0; j < numNeuron_; j++)
            {
                tmp += weights_[j][i] * errorSignal_[j];
            }
            result.push_back(tmp);
        }


    }else
    {
        /// if its an hidden layer
        /// for(x=0; x<hidden_array_size; x++) {
        ///     for(y=0; y<output_array_size; y++) {
        ///         temp += (errorsignal_output[y] * weight_h_o[x][y]); // this is the array input
        ///     }
        ///     errorsignal_hidden[x] = hidden[x] * (1-hidden[x]) * temp;
        ///     temp = 0.0;
        /// }
        for(int i = 0; i < errorSignal_.size(); i++)
        {
            errorSignal_[i] =  devSigmoid(outputs_[i]) * array[i];
        }

        /// for all prevoius hidden neuron layer's neuron
        for(int i = 0; i < numInput_; i++)
        {
            double tmp = 0.0;
            /// += weights of previous inputs * global output signal error
            for(int j = 0; j < numNeuron_; j++)
            {
                tmp += weights_[j][i] * errorSignal_[j];
            }
            result.push_back(tmp);
        }
        /*
        // ADJUST BIASES OF HIDDEN UNITS
            for(x=hidden_array_size; x<bias_array_size; x++) {
                bias[x] += (learning_rate * errorsignal_output[x] / length);
            }
*/
        return result;
    }

}



void NeuronLayer::updateWeights(const std::vector<double> prevInput)
{
    for(int i = 0; i < numNeuron_; i++)
    {
        for(int j = 0; j < numInput_; j++)
        {
            std::cout << "previnputs: " << prevInput[j] << " , errorSignal: " << errorSignal_[i] << endl;
            std::cout << "weights: " << weights_[i][j] << endl;
            weights_[i][j] += learningRate_ * prevInput[j] * errorSignal_[i];
        }
    }
}
