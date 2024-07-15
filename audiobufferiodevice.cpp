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
    // Process audio data here
    buffer.append(data, len);
    // For now, we'll just log the data length
    qInfo() << "Received audio data: " << len << " bytes";
    return len;
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
