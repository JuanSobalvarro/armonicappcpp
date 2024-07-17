#ifndef UICOMPONENTS_H
#define UICOMPONENTS_H

#include <QObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QStatusBar>
#include <QLabel>
#include <QList>
#include <QAudioDevice>
#include <QActionGroup>

#include "charthandler.h"


class UIComponents : public QObject
{
    Q_OBJECT

public:
    UIComponents(QMainWindow *mainWindow, QObject *parent = nullptr);
    ~UIComponents();

    ChartHandler *chartHandler;

    void setAll();
    void updateStatusMsg(const QString &msg);
    void populateAudioDeviceMenu(const QList<QAudioDevice> &inputDevices,
                                 const QList<QAudioDevice> &outputDevices,
                                 std::function<void(const QAudioDevice &)> selectDeviceCallback);
    void connectRecordButtons(std::function<void()> startAudioRecordingCallback,
                              std::function<void()> stopAudioRecordingCallback);

    void startButtonPressed();
    void stopButtonPressed();

private:
    QMainWindow *mainWindow;

    QWidget *centralWidget;

    // layouts
    QGridLayout *layout;
    QHBoxLayout *buttonsLayout;
    QHBoxLayout *chartsLayout;

    QAction *testAct;

    // status bar
    QStatusBar *statusBar;
    QLabel *statusMsg;

    // menu bar
    QMenuBar *menuBar;
    QMenu *deviceMenu;
    QMenu *inputDeviceMenu;
    QMenu *outputDeviceMenu;

    QActionGroup *deviceActionGroup;

    // Buttons
    QPushButton *startRecordingButton;
    QPushButton *stopRecordingButton;

    void createActions();
    void createMenus();
    void createStatusBar();
    void createButtons();
    void createCentralWidget();
    void createCharts();
    void createLayouts();



signals:
    void testActionTriggered(const QString &msg);

private slots:
    void onTestAction();
};

#endif // UICOMPONENTS_H

