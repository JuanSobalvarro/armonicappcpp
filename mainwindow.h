#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audiobufferiodevice.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QActionGroup>
#include <QAction>
#include <QtMultimedia/QAudioDevice>
#include <QtMultimedia/QMediaDevices>
#include <QList>
#include <QLabel>
#include <QGraphicsWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QLineSeries>

#include <QtDebug>

#include <QAudioSource>
#include <QIODevice>
#include <QByteArray>
#include <QAudioFormat>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void testAction(std::string activatedBy);
    void errorMsgBox(const QString msg, QMessageBox::Icon icon);

    void selectDevice(const QAudioDevice &device);

    void startAudioRecording();
    void stopAudioRecording();
    void handleAudioInputStateChanged(QAudio::State state);

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    void createStatusBar();
    void createBarChart();
    void createAudioChart();
    void createCentralWidget();
    void createLayout();

    void setAll();

    void populateAudioDeviceMenu();

    void updateStatusBar(const QString &message);

    QAudioFormat createAudioFormat(unsigned int sampleRate=44100,
                                unsigned int channelCount=1,
                                QAudioFormat::SampleFormat sampleFormat=QAudioFormat::Int16);

    void setUpAudio();

    // Menu components
    QMenuBar *menuBar;

    QMenu *testMenu;
    QMenu *deviceMenu;
    QMenu *inputDeviceMenu;
    QMenu *outputDeviceMenu;

    // Buttons
    QPushButton *startRecordingButton;
    QPushButton *stopRecordingButton;

    // Status Bar
    QStatusBar *statusBar;
    QLabel *statusMsg;

    // Layout components
    QGridLayout *layout;
    QHBoxLayout *buttonsLayout;
    QHBoxLayout *chartsLayout;
    QWidget *centralWidget;

    // Actions
    QAction *testAct;

    // Charts
    QChart *barChart;
    QChartView *barChartView;

    QChart *audioChart;
    QChartView *audioChartView;

    // Audio
    QList<QAudioDevice> *inputDevices;
    QList<QAudioDevice> *outputDevices;
    QAudioDevice deviceSelected;

    QAudioFormat audioFormat;
    QAudioSource *audioInput;
    AudioBufferIODevice *audioIODevice;
};
#endif // MAINWINDOW_H
