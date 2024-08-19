#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>
#include <QDebug>
#include <QLineSeries>
#include <vector>
#include <complex>

#include "ffthandler.h"
#include "audiobufferiodevice.h"
#include "utils.h"

class AudioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioProcessor(AudioBufferIODevice *audioIOdevice, QObject *parent = nullptr);
    ~AudioProcessor();

    QLineSeries *audioSeries;
    QLineSeries *frequencySeries;

    int sampleCount = 2000;

public slots:
    void processAudioData();

signals:
    void fftDataReady(const std::vector<std::complex<double>> &fftResult);

private:
    void updateAudioSeries();
    void updateFrequencySeries(const std::vector<double>& normalizedFFTData);

    AudioBufferIODevice *bufferIODevice;
    FFTHandler *fftHandler;
};

#endif // AUDIOPROCESSOR_H
