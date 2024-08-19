#include "audioBufferIOdevice.h"

AudioBufferIODevice::AudioBufferIODevice(QObject *parent)
    : QIODevice(parent), audioInput(nullptr)
{
}

qint64 AudioBufferIODevice::readData(char *data, qint64 maxlen)
{
    // Not used for audio input
    Q_UNUSED(data)
    Q_UNUSED(maxlen)
    return -1;
}

qint64 AudioBufferIODevice::writeData(const char *data, qint64 len)
{
    // Write in buffer
    static const int resolution = 4;

    if (buffer.isEmpty())
    {
        buffer.reserve(sampleCount);
        for (int i = 0; i < sampleCount; i++)
            buffer.append(QPointF(i, 0));

    }
    int start = 0;

    const int availableSamples = int(len) / resolution;

    if (availableSamples < sampleCount)
    {
        start = sampleCount - availableSamples;
        for (int s = 0; s < start; s++)
            buffer[s].setY(buffer.at(s + availableSamples).y());
    }

    for (int s = start; s < sampleCount; s++, data += resolution)
        buffer[s].setY(qreal(uchar(*data) - 128) / qreal(128));

    emit audioDataReady();  // Emit signal with new audio data

    qInfo() << "AUDIOBUFFERIODEVICE::WRITEDATA: DATA READY\n";
    return (sampleCount - start) * resolution;
}


bool AudioBufferIODevice::open(OpenMode mode)
{
    if (!QIODevice::open(mode)) {
        qWarning() << "Failed to open AudioBufferIODevice";
        return false;
    }
    buffer.clear(); // Clear buffer when opening
    return true;
}
