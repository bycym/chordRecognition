#include "neuralutilities.h"

/*
void teachNeuralNetwork(NeuralNetworks &nn, std::vector<SoundData>)
{

}*/





int delta(float i, float j)
{
    if( i==j) return 1;
    else return 0;
}

/*
int idealWindowSize(int samplerate) {
    switch(samplerate) {
        case 22050:
            return 1024; break;
        case 44100:
            return 2048; break;
        default:
            return 1024;
    }
}
*/
