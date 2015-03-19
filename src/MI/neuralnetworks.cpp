#include "neuralnetworks.h"

NeuralNetworks::NeuralNetworks(int numInput, int numHidden, int numOutput)
{
    this->numInput_ = numInput;
    this->numHidden_ = numHidden;
    this->numOutput_ = numOutput;

    this->inputs_ = new double[numInput];
    this->ihWeights_ = MatrixHelpers::MakeMatrix(numInput,numHidden);
    this->ihSums_ = new double[numHidden];
    this->ihBiases_ = new double[numHidden];
    this->ihOutputs_ = new double[numHidden];
    this->hoWeights_ = MatrixHelpers::MakeMatrix(numHidden, numOutput);
    this->hoSums_ = new double[numOutput];
    this->hoBiases_  = new double[numOutput];
    this->outputs_ = new double[numOutput];

    this->oGrads_ = new double[numOutput];
    this->hGrads_ = new double[numOutput];

    this->ihPrevWeightsDelta_ = MatrixHelpers::MakeMatrix(numInput, numHidden);
    this->ihPrevBiasesDelta_ = new double[numHidden];
    this->hoPrevWeightsDelta_ = MatrixHelpers::MakeMatrix(numHidden, numOutput);
    this->hoPrevBiasesDelta_ = new double[numOutput];
}

NeuralNetworks::~NeuralNetworks()
{

}

