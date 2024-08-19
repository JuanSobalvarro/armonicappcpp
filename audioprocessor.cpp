#include "audioprocessor.h"

AudioProcessor::AudioProcessor(AudioBufferIODevice *audioIOdevice, QObject *parent)
    : QObject{parent}, bufferIODevice(audioIOdevice), fftHandler(new FFTHandler())
{
    audioSeries = nullptr;
    frequencySeries = nullptr;

    sampleCount = bufferIODevice->sampleCount;

    connect(audioIOdevice, &AudioBufferIODevice::audioDataReady,
            this, &AudioProcessor::processAudioData);
}

AudioProcessor::~AudioProcessor()
{

}

void AudioProcessor::processAudioData()
{
    if (bufferIODevice->buffer.isEmpty()) {
        qWarning() << "Buffer is empty!";
        return;
    }
    // Convert QByteArray to a format suitable for FFT
    std::vector<std::complex<double>> complexData;
    complexData.reserve(bufferIODevice->buffer.size());

    for (const auto& point : bufferIODevice->buffer)
    {
        complexData.emplace_back(point.y(), 0.0);  // Use y-value as the real part, imaginary part is 0
    }

    // Perform FFT
    fftHandler->doFFT(complexData);

    // Update series
    updateAudioSeries();
    updateFrequencySeries(fftHandler->getFFTResult());

    // qInfo() << "Test result: " << fftHandler->normalizedFFTResult();

    qInfo() << "AUDIOPROCESSOR::Audio Processed\n";
}

void AudioProcessor::updateAudioSeries()
{
    // Update series
    audioSeries->replace(bufferIODevice->buffer);
}

void AudioProcessor::updateFrequencySeries(const std::vector<double>& normalizedFFTData)
{
    if (!frequencySeries)
    {
        qWarning() << "AUDIOPROCESSOR::Frequency series is not initialized!";
        return;
    }

    if (normalizedFFTData.empty())
    {
        qWarning() << "AUDIOPROCESSOR::FFT data is empty!";
        return;
    }

    qInfo() << "Result size of " << normalizedFFTData.size() << "\n";

    // Create a QVector of QPointF to batch the update
    QVector<QPointF> points;
    points.reserve(normalizedFFTData.size());

    int count = 0;
    for (const auto& magnitude : normalizedFFTData)
    {
        points.append(QPointF(count++, magnitude));
    }

    // Replace the entire series data at once
    frequencySeries->replace(points);

    qInfo() << "AUDIOPROCESSOR::Frequency Series Updated\n";
}
