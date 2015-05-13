#include "neuronlayer.h"

NeuronLayer::NeuronLayer(int numInput, int numOutput, int numNeuron, bool hidden, double learningrate)
{
    // random value between -1 and 1
    /// TODO: random ellenőrizni, két indiítással


    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::uniform_real_distribution<double> dist (-1.00,1.00);


    //std::random_device rd;
    //std::mt19937 mt(rd());
    //std::uniform_real_distribution<double> dist(-1.000, 1.000);

    this->hidden_ = hidden;
    this->numInput_ = numInput;
    this->numOutput_ = numOutput;
    this->numNeuron_ = numNeuron;
    this->learningRate_ = learningrate;

    if(hidden_)
    {
        for(int i = 0; i < numNeuron; i++)
        {
            if(DEBUG) std::cout << "i: " << i << " ";
            std::vector<double> row;
            /// number of input
            for(int j = 0; j < numInput; j++){
                double tmp = dist(generator);
                row.push_back(tmp);
                if(DEBUG) std::cout<< "w: " << tmp << ", ";
            }
            if(DEBUG) std::cout << std::endl;
            weights_.push_back(row);
        }
        if(DEBUG) std::cout <<"--------------------------"<<std::endl;
    }
    else
    {
        double sum = 0.0;
        double tmp = 0.0;

        for(int i = 0; i < numNeuron; i++)
        {
            if(DEBUG) std::cout << "i: " << i << " ";
            std::vector<double> row;
            /// number of input
            for(int j = 0; j < numInput; j++){
                double tmp = dist(generator);
                sum+=tmp;
                row.push_back(tmp);
            }
            for(int j = 0; j < numInput; j++){
                row[j] -= sum/numInput;
            }
            if(DEBUG) std::cout << std::endl;
            weights_.push_back(row);
        }

        if(DEBUG) std::cout <<"--------------------------"<<std::endl;
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

    if(DEBUG)
    {
        std::cout <<"--------------------------"<<std::endl;
        if(hidden_ == true)
            std::cout << "hidden" << std::endl;
        else
            std::cout << "output" << std::endl;
        std::cout << std::endl << "inputs: " << std::endl;

        for(auto i : inputs_)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
        std::cout <<"--------------------------"<<std::endl;
    }
    outputs_.clear();
    // all neuron
    for(int neuron = 0; neuron < numNeuron_; neuron++)
    {
        // all input * weights
        double out = 0.0;
        for(int i = 0; i < inputs_.size(); i++)
        {
            out += inputs_[i] * weights_[neuron][i];
        }
        outputs_.push_back(out);
    }






    /// if its output neuron layer then compute softmax
    if(!hidden_)
    {
        softmax();

        if(DEBUG)
        {
            std::cout <<"--------------------------"<<std::endl;
            if(hidden_ == true)
                std::cout << "hidden" << std::endl;
            else
                std::cout << "output" << std::endl;
            std::cout << std::endl << "compute outputs: " << std::endl;
            for(int i = 0; i < outputs_.size(); i++)
            {
                std::cout << outputs_[i] << ", ";
            }
            std::cout << std::endl;
            std::cout <<"--------------------------"<<std::endl;

        }
    }
    else
    {
        // activation compute; default is sigmoid
        activationFunction(alg);
        if(DEBUG)
        {
            std::cout <<"--------------------------"<<std::endl;
            if(hidden_ == true)
                std::cout << "hidden" << std::endl;
            else
                std::cout << "output" << std::endl;
            std::cout << std::endl << "compute outputs: " << std::endl;
            for(int i = 0; i < outputs_.size(); i++)
            {
                std::cout << outputs_[i] << ", ";
            }
            std::cout << std::endl;
            std::cout <<"--------------------------"<<std::endl;

        }
    }

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
    for(int i = 0; i < outputs_.size(); i++)
    {
        //double tmp = outputs_[i];
        double tmp = outputs_[i] + biases_[i];
        outputs_[i] = (1 / ( 1 + exp( (-1) *  tmp  ) ));
    }
}

double NeuronLayer::devSigmoid(double x, int i)
{
    //double tmp = x;
    double tmp = x + biases_[i];
    return (1 / ( 1 + exp( (-1) * tmp ) )) * (1 - (1 / ( 1 + exp( (-1) * tmp ) )));
}

void NeuronLayer::softmax()
{

    /// result(z)j = e^(zj) / sum(k=1 to K)(e^(zk)
    // ellenőrizni
    double sum = 0.0;
    /// for(all element) sum+=(e^inp[i])
    for(int i = 0; i < outputs_.size(); i++)
        sum += exp(outputs_[i]);

    for(int i = 0; i < outputs_.size(); i++)
    {
        double tmp = outputs_[i];
        outputs_[i] = exp(tmp) / sum;
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
        if(DEBUG) std::cout <<"--------------------------"<<std::endl;
        if(DEBUG) std::cout << "errorsignal output update:" << std::endl;
        /// error signal = target[pattern][i] - output[pattern][i]
        for(int i = 0; i < errorSignal_.size(); i++)
        {
            errorSignal_[i] = array[i] - outputs_[i];
            //errorSignal_[i] = array[i] - outputs_[i];
            if(DEBUG) std::cout << "errS: " << errorSignal_[i]<< ", ";
        }
        if(DEBUG) std::cout << std::endl;

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


        if(DEBUG) std::cout <<"--------------------------"<<std::endl;

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
        if(DEBUG) std::cout << "errorsignal hidden update:" << std::endl;
        for(int i = 0; i < errorSignal_.size(); i++)
        {
            errorSignal_[i] =  devSigmoid(outputs_[i],i) * array[i];
            if(DEBUG) std::cout << "errS: " << errorSignal_[i] << ", devSigmoid(outputs_[i]): "<<  devSigmoid(outputs_[i],i) <<" array: " << array[i]<<std::endl;
        }
        if(DEBUG) std::cout <<"--------------------------"<<std::endl;

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
        if(DEBUG) std::cout <<"--------------------------"<<std::endl;

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
    if(DEBUG) std::cout << "updateWeights:" << std::endl;

    for(int i = 0; i < numNeuron_; i++)
    {
        if(DEBUG) std::cout << i << ". neuron" << std::endl;
        for(int j = 0; j < numInput_; j++)
        {
            if(DEBUG) std::cout << "before update weights: " << weights_[i][j] << std::endl;

            weights_[i][j] += learningRate_ * prevInput[j] * errorSignal_[i];

            //if(DEBUG) std::cout << "neuron w: " << weights_[i][j] << " <- weights_[i][j] + (learningRate_ * prevInput[j] * errorSignal_[i])"<<std::endl;
            if(DEBUG) std::cout << "previnputs: " << prevInput[j] << " , errorSignal: " << errorSignal_[i] << std::endl;
            if(DEBUG) std::cout << "weights: " << weights_[i][j] << std::endl;

        }
    }

    for(int i = 0; i < biases_.size(); i++)
    {
        biases_[i] += (learningRate_ * errorSignal_[i] );
        if(DEBUG) std::cout << "biases: " << biases_[i] << std::endl;
    }
    if(DEBUG) std::cout <<"--------------------------"<<std::endl;

}
