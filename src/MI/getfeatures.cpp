#include "getfeatures.h"

bool getFeatures(SoundData*& sndData)
{
    // the dimension of the pointers in and out
    int N = 2;

    // the pointer that stores the input data.
    fftw_complex *in;
    // the pointer that stores the output data.
    fftw_complex *out;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

    fftw_plan my_plan;

    /***
     * int FFTW_FORWARD: FFTW_FORWARD is an integer constant of the package
     * that tells to the function that the FFT to perform must be the forward
     * one. To perform the bacward one, we will introduce FFTW_BACKWARD. To
     * see the differences between these two, the reader can refer to
     * section 1.
     * */

    /***
     * unsigned FFTW_ESTIMATE: FFTW_ESTIMATE is a flag that tells to the
     * function how well must be optimized, with respect to the computational
     * time, the FFT algorithm. If we are getting started to the package we
     * will use this flag. For other values of this flag the reader can
     * consult appendix A.
     * */

    my_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);


    /***
     * These routines create plans for n0 by n1 two-dimensional (2d)
     * transforms and n0 by n1 by n2 3d transforms, respectively.
     * All of these transforms operate on contiguous arrays in the
     * C-standard row-major order, so that the last dimension has
     * the fastest-varying index in the array.
     *
     * my_plan = fftw_plan_dft_2d(int n0, int n1,
     *                         fftw_complex *in, fftw_complex *out,
     *                         int sign, unsigned flags);
     * */
    my_plan = fftw_plan_dft_2d(n0, n1,
                               in, out,
                               sign, flags);

    /***
     * performs the FFT stored in my_plan.
     * */
    fftw_execute(my_plan);

    /***
     * deallocate the memory stored by the plan and the pointers.
     * Note that for the pointers we use fftw_free and not the
     * stdlib.h function free
     * */
    fftw_destroy_plan(my_plan);
    fftw_free(in);
    fftw_free(out);


    return false;
}


