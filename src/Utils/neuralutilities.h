#ifndef NEURALUTILITIES_H
#define NEURALUTILITIES_H


#include "../../genchords/src/FFT.h"
#include "../MI/neuralnetworks.h"

/*!
 * \brief transforms a fragment of the input sound to a discrete Fourier transform (DFT) spectrum
 * \param fragment of the input sound
 *
 * void PowerSpectrum(int NumSamples, float *In, float *Out);
 * void RealFFT(int NumSamples,
             float *RealIn, float *RealOut, float *ImagOut);
 * ??????????
 * */
float X(float l);




/*!
 * \brief Kronecker's delta
 * \param float i
 * \param float j
 * \return int 1 or 0
 * delta    { 1, ha i==j
 *          { 0, ha i!=j
 * */
int delta(float i, float j);


/*!
 * \brief PCP*(p) = Szigma pow(||X(l)||,2) delta(M(l),p)
 * \param int p a vector defined for p = 0, 1, ..., 11
 * */
//float PCP(int p);
// PCP.cpp??


/*!
 * \brief M(l) for PCP*(p) function
 * \param float l
 * Neural Networks for Musical Chords Recognition
 * M(l) = { -1                                      l = 0
 *        { round(12*log2((fs*1/N)/fref))mod12      l = 1, ..., (N/2)-1
 *
 * fref     - is the reference frequency falling into PCP(0)*
 * N        - the number of bins in the DFT of the input signal
 * fs       - is the sampling frequency
 * example: standard scale starting with a C, the reference frequency is 130.80 Hz
 * */
float M(float l);


/*!
 * \brief idealWindowSize from genchords
 * \param samplerate
 * \return
 * if (windowSize==0) {
 *      windowSize = idealWindowSize(sd.samplerate());
 * }
 */
//int idealWindowSize(int samplerate);


//void teachNeuralNetwork(NeuralNetworks& nn);



#endif // NEURALUTILITIES_H
