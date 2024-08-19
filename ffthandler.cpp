#include "ffthandler.h"

FFTHandler::FFTHandler()
{

}

FFTHandler::~FFTHandler()
{}

std::vector<double> FFTHandler::normalizedFFTResult()
{
    // Convert complex numbers to magnitudes
    std::vector<double> magnitudes;
    magnitudes.reserve(fftresult.size());

    for (const auto& value : fftresult)
    {
        magnitudes.push_back(std::abs(value));
    }

    // Find the maximum magnitude
    double maxMagnitude = *std::max_element(magnitudes.begin(), magnitudes.end());

    // Normalize the magnitudes
    std::vector<double> normalizedMagnitudes;
    normalizedMagnitudes.reserve(magnitudes.size());

    if (maxMagnitude > 0) // Avoid division by zero
    {
        for (const auto& magnitude : magnitudes)
        {
            normalizedMagnitudes.push_back(magnitude / maxMagnitude);
        }
    }
    else
    {
        // If maxMagnitude is zero, just fill with zeros
        normalizedMagnitudes.assign(magnitudes.size(), 0.0);
    }

    return normalizedMagnitudes;
}

std::vector<double> FFTHandler::getFFTResult()
{
    // Convert complex numbers to magnitudes
    std::vector<double> magnitudes;
    magnitudes.reserve((fftresult.size() + 1) / 2);

    for (size_t i = 0; i < (fftresult.size() + 1) / 2; i++)
    {
        magnitudes.push_back(std::abs(fftresult[i]));
    }

    return magnitudes;
}

// Method to GET fft result
std::vector<std::complex<double>> FFTHandler::getRawFFTResult()
{
    // Normalize result
    return fftresult;
}

// FFT caller
void FFTHandler::doFFT(std::vector<std::complex<double>>& input)
{
    fftresult = input;
    FFT(fftresult);
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
