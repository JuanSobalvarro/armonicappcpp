#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioDevice>

#include "audiohandler.h"
#include "charthandler.h"
#include "uicomponents.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    AudioHandler *audioHandler;
    ChartHandler *chartHandler;
    UIComponents *uiComponents;

    void selectDevice(const QAudioDevice &device);
    void startRecordingCallback();
    void stopRecordingCallback();
};

#endif // MAINWINDOW_H
