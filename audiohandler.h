#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include "audiobufferiodevice.h"
#include "audioprocessor.h"

#include <QObject>
#include <QAudioSource>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QMediaDevices>

class AudioHandler : public QObject
{
    Q_OBJECT

public:
    AudioHandler(QObject *parent = nullptr);
    ~AudioHandler();

    QAudioDevice deviceSelected;

    AudioBufferIODevice *audioIODevice;
    AudioProcessor *audioProcessor;

    QList<QAudioDevice> *inputDevices;
    QList<QAudioDevice> *outputDevices;

    void setUpAudio(const QAudioDevice &device);
    bool startAudioRecording();
    bool stopAudioRecording();
    QAudioFormat createAudioFormat(unsigned int sampleRate = 44100,
                                   unsigned int channelCount = 1,
                                   QAudioFormat::SampleFormat sampleFormat = QAudioFormat::Int16);
    void selectDevice(const QAudioDevice &device);

signals:
    void audioInputStateChanged(QAudio::State state);

private:
    QAudioFormat audioFormat;
    QAudioSource *audioInput;


private slots:
    void handleAudioInputStateChanged(QAudio::State state);
};

#endif // AUDIOHANDLER_H

