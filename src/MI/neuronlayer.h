#ifndef NEURONLAYER_H
#define NEURONLAYER_H


#import <vector>
#import <iostream>

class NeuronLayer
{
private:

    int numInput_;
    int numOutput_;
    // TODO: activaton function

    vector<double> biases_;
    vector<double> inputs_;

    /*!
     * \brief weights_ is a two dimensional vector matrix:
     * row      | column (1)    | column (2)    | ...
     * ---------+---------------+---------------+----
     * neoron(1)| weights_(1)   | weights_(2)   | ...
     * neuron(2)| weights_(1)   | weights_(2)   | ...
     * ...
     * neuron(n)| weights_(1)   | weights_(2)   | ...
     */
    vector< vector<double>> weights_;
    vector<double> outputs_;

    // TODO: gradient function

    // TUDO: sum function
    double sum();


    /*!
     * \brief activation function for the neuron
     * \param What kind of algorithm should we use?
     * \return activation function
     */
    double activationFunction(int alg = 1);



public:
    NeuronLayer();
    ~NeuronLayer();
};

#endif // NEURONLAYER_H
