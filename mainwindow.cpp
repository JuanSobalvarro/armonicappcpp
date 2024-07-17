#include "mainwindow.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(100, 100, 720, 480);

    // Initialize Audio Handler
    audioHandler = new AudioHandler();

    // Initialize UI Components
    uiComponents = new UIComponents(this);
    uiComponents->populateAudioDeviceMenu(*audioHandler->inputDevices,
                                          *audioHandler->outputDevices,
                                          [this](const QAudioDevice &device)
                                          {
                                              selectDevice(device);
                                          });
    uiComponents->connectRecordButtons(
                                        // Start Recording lambda to callback
                                        [this]()
                                        {
                                            startRecordingCallback();
                                        },
                                        // Stop Recording lambda to callback
                                        [this]()
                                        {
                                            stopRecordingCallback();
                                        });
}

MainWindow::~MainWindow()
{
    delete uiComponents;
}

void MainWindow::selectDevice(const QAudioDevice &device)
{
    if (!audioHandler->deviceSelected.isNull())
    {
        stopRecordingCallback();
    }
    // Update device in audio
    audioHandler->selectDevice(device);

    // Update device in UI
    QString str = "Selected device: " + audioHandler->deviceSelected.description();
    uiComponents->updateStatusMsg(str);
}

void MainWindow::startRecordingCallback()
{
    // Start recording audio
    if (!audioHandler->startAudioRecording())
    {
        qInfo() << "MAINWINDOW::STARTRECORDINGCALLBACK: Error starting audio recording\n";
        return;
    }

    // Update UI
    uiComponents->startButtonPressed();
}

void MainWindow::stopRecordingCallback()
{
    // Stop audio
    if (!audioHandler->stopAudioRecording())
    {
        qInfo() << "MAINWINDOW::STOPRECORDINGCALLBACK: Error stopping audio recording\n";
        return;
    }

    // Update UI
    uiComponents->stopButtonPressed();
}
