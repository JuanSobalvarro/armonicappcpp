#include "audioprocessor.h"

AudioProcessor::AudioProcessor(AudioBufferIODevice *audioIOdevice, QObject *parent)
    : QObject{parent}, bufferIODevice(audioIOdevice), fftHandler(new FFTHandler())
{
    audioSeries = nullptr;
    frequencySeries = nullptr;

    connect(audioIOdevice, &AudioBufferIODevice::audioDataReady,
            this, &AudioProcessor::processAudioData);
}

AudioProcessor::~AudioProcessor()
{

}

void AudioProcessor::processAudioData(const QByteArray &data)
{

    // Convert QByteArray to a format suitable for FFT
    std::vector<std::complex<double>> complexData = Utils::qByteArray2fftvector(data);

    // Perform FFT
    fftHandler->doFFT(complexData);

    // Update series
    updateAudioSeries(data);
    updateFrequencySeries(fftHandler->getFFTResult());

    qInfo() << "AUDIOPROCESSOR::Audio Processed\n";
}

void AudioProcessor::updateAudioSeries(const QByteArray &data)
{
    // Update series
    audioSeries->replace(bufferIODevice->buffer);
}

void AudioProcessor::updateFrequencySeries(FFTHandler::fftvector result)
{
    if (!frequencySeries)
    {
        qWarning() << "AUDIOPROCESSOR::Frequency series is not initialized!";
        return;
    }

    QVector<QPointF> points;
    const int n = static_cast<int>(result.size());

    qInfo() << "Result size of " << result.size() << "\n";


    // Only use the first half of the FFT result as it is symmetrical
    for (int i = 0; i < n / 2; ++i)
    {
        double frequency = i;  // This is the index, actual frequency may require scaling
        double magnitude = std::abs(result[i]);  // Magnitude of the complex number

        points.append(QPointF(frequency, magnitude));
    }

    // Update the series with the new data
    frequencySeries->replace(points);

    qInfo() << "AUDIOPROCESSOR::Frequency Series Updated\n";
}
