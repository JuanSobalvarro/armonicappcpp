#include "audiohandler.h"
#include "utils.h"

AudioHandler::AudioHandler(QObject *parent)
    : QObject(parent), audioInput(nullptr), audioIODevice(new AudioBufferIODevice(this))
{    
    // Initialize and set audio devices
    inputDevices = new QList<QAudioDevice>;
    outputDevices = new QList<QAudioDevice>;
    *inputDevices = QMediaDevices::audioInputs();
    *outputDevices = QMediaDevices::audioOutputs();

    // Initialize Audio Format
    audioFormat = createAudioFormat(44100, 1, QAudioFormat::Int16);
}

AudioHandler::~AudioHandler()
{
    if (audioInput)
    {
        audioInput->stop();
        delete audioInput;
    }
    delete audioIODevice;
    delete inputDevices;
    delete outputDevices;
}

// Function to setup audio for a selected device
void AudioHandler::setUpAudio(const QAudioDevice &device)
{
    if (device.isNull())
    {
        qWarning() << "AUDIOHANDLER::SETUPAUDIO: null device wtf r u doing\n";
        return;
    }

    qInfo() << "AUDIOHANDLER::SETUPAUDIO: Setting up audio for device:" << device.description() << "\n";

    if (audioInput) {
        qInfo() << "Stopping current audio input";
        audioInput->stop();
        delete audioInput;
        audioInput = nullptr;
    }

    deviceSelected = device;

    if (!deviceSelected.isFormatSupported(audioFormat))
    {
        qWarning() << "Format not supported by selected device.";
        Utils::showErrorMsgBox("Device " + deviceSelected.description() + " does not support format", QMessageBox::Warning);
        return;
    }

    audioInput = new QAudioSource(deviceSelected, audioFormat);
    connect(audioInput, &QAudioSource::stateChanged, this, &AudioHandler::handleAudioInputStateChanged);
    audioIODevice->open(QIODevice::WriteOnly);

    qInfo() << "AUDIOHANDLER::SETUPAUDIO: Setup completed\n";
}

QAudioFormat AudioHandler::createAudioFormat(unsigned int sampleRate, unsigned int channelCount, QAudioFormat::SampleFormat sampleFormat)
{
    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(channelCount);
    format.setSampleFormat(sampleFormat);

    if (!format.isValid()) {
        qWarning() << "Invalid audio format settings.";
    }

    return format;
}

bool AudioHandler::startAudioRecording()
{
    if (!audioInput || deviceSelected.isNull())
    {
        Utils::showErrorMsgBox("You should select a device before start recording", QMessageBox::Warning);
        return false;
    }
    audioInput->start(audioIODevice);

    return true;
}

bool AudioHandler::stopAudioRecording()
{
    if (!audioInput)
    {
        qWarning() << "There is no input to stop\n";
        return false;
    }

    audioInput->stop();
    return true;
}

// Handle if the audio input change state
void AudioHandler::handleAudioInputStateChanged(QAudio::State state)
{
    emit audioInputStateChanged(state);

    if (state == QAudio::IdleState)
    {
        if (audioInput)
        {
            audioInput->stop();
        }
        audioIODevice->close();
    }
    else if (state == QAudio::StoppedState)
    {
        if (audioInput && audioInput->error() != QAudio::NoError)
        {
            qWarning() << "Audio input error occurred";
        }
    }
}

// Select the device which will be used
void AudioHandler::selectDevice(const QAudioDevice &device)
{
    if (deviceSelected == device)
    {
        qInfo() << "AUDIOHANDLER::SELECTDEVICE: Device already selected\n";
        return;
    }

    if (!device.isFormatSupported(audioFormat))
    {
        qWarning() << "AUDIOHANDLER::SELECTDEVICE: Selected device DOES NOT supports desired audio format\n";
        const QString msg = "Device " + device.description() + " does not support format";
        Utils::showErrorMsgBox(msg, QMessageBox::Warning);
        return;
    }

    // Update selected device
    deviceSelected = device;

    setUpAudio(device);
    qInfo() << "AUDIOHANDLER::SELECTDEVICE: New device selected successfully\n";
}
