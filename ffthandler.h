#ifndef FFTHANDLER_H
#define FFTHANDLER_H

#include <vector>
#include <complex>

#define PI 3.14159265358979323846

class FFTHandler
{
public:
    FFTHandler();
    ~FFTHandler();

    typedef std::vector<std::complex<double>> fftvector;

    // call FFT
    void doFFT(fftvector& input);

    // Method to get the result of the FFT
    fftvector getFFTResult() const;

private:
    // FFT recursion
    void FFT(fftvector& data);

    fftvector fftresult;

};

#endif // FFTHANDLER_H
