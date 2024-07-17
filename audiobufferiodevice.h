#ifndef AUDIOBUFFERIODEVICE_H
#define AUDIOBUFFERIODEVICE_H

#include <QIODevice>
#include <QAudioInput>
#include <QDebug>
#include <QByteArray>

class AudioBufferIODevice : public QIODevice
{
public:
    explicit AudioBufferIODevice(QObject *parent = nullptr);

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    bool open(OpenMode mode) override;

    QByteArray buffer;

private:
    QAudioInput *audioInput;
};

#endif // AUDIOBUFFERIODEVICE_H
