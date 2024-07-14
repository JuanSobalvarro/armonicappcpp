#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>


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

    void selectDevice(const QString &deviceName);

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    void createStatusBar();
    void updateStatusBar(const QString &message);

    void populateAudioDeviceMenu();

    void createBarChart();

    // Bar Chart
    QChart *barChart;

    // Audio devices
    QList<QAudioDevice> *inputDevices;
    QList<QAudioDevice> *outputDevices;

    QStatusBar *statusBar;
    QLabel *statusMsg;

    // Menus
    QMenuBar *menuBar;

    QMenu *testMenu;
    QMenu *deviceMenu;
    QMenu *inputDeviceMenu;
    QMenu *outputDeviceMenu;

    // Actions
    QAction *testAct;
};
#endif // MAINWINDOW_H
