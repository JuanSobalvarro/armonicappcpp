#ifndef AUDIOBUFFERIODEVICE_H
#define AUDIOBUFFERIODEVICE_H

#include <QIODevice>
#include <QAudioInput>
#include <QDebug>
#include <QXYSeries>

class AudioBufferIODevice : public QIODevice
{
    Q_OBJECT
public:
    explicit AudioBufferIODevice(QObject *parent = nullptr);


    bool open(OpenMode mode) override;

    QList<QPointF> buffer;

    static const int sampleCount = 512;

signals:
    void audioDataReady();

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    QAudioInput *audioInput;
};

#endif // AUDIOBUFFERIODEVICE_H
