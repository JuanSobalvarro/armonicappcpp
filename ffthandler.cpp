#include "ffthandler.h"

FFTHandler::FFTHandler()
{

}

FFTHandler::~FFTHandler()
{}


// Method to GET fft result
std::vector<std::complex<double>> FFTHandler::getFFTResult() const
{
    return fftresult;
}

// FFT caller
void FFTHandler::doFFT(std::vector<std::complex<double>>& input)
{
    FFT(input);
}

// Recursive FFT function (Using Cooley-Turkey FFT Algorithm)
void FFTHandler::FFT(std::vector<std::complex<double>>& data)
{
    const size_t N = data.size();

    if (N <= 1) return;

    // Divide
    std::vector<std::complex<double>> even(N / 2);
    std::vector<std::complex<double>> odd(N / 2);
    for (size_t i = 0; i < N / 2; i++)
    {
        even[i] = data[i * 2];
        odd[i] = data[i * 2 + 1];
    }

    // Uwunya
    FFT(even);
    FFT(odd);

    // Combine
    for (size_t i = 0; i < N / 2; i++)
    {
        std::complex<double> uwu = std::polar(1.0, -2.0 * PI * i / N) * odd[i];
        data[i] = even[i] + uwu;
        data[i + N / 2] = even[i] - uwu;
    }
}
