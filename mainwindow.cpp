#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 720, 480);

    createActions();
    createMenus();
    createStatusBar();
    createBarChart();
}

MainWindow::~MainWindow()
{
    delete inputDevices;
    delete outputDevices;
}

void MainWindow::populateAudioDeviceMenu()
{
    // Create an action group for exclusive selection
    QActionGroup *deviceActionGroup = new QActionGroup(this);
    deviceActionGroup->setExclusive(true);

    // Initialize the QList objects
    inputDevices = new QList<QAudioDevice>;
    outputDevices = new QList<QAudioDevice>;

    // Retrieve and add audio input/output devices
    *inputDevices = QMediaDevices::audioInputs();
    *outputDevices = QMediaDevices::audioOutputs();

    for (const QAudioDevice &device : *inputDevices)
    {
        QAction *deviceAction = inputDeviceMenu->addAction(device.description());
        deviceAction->setCheckable(true);
        deviceAction->setActionGroup(deviceActionGroup);
        connect(deviceAction, &QAction::triggered, this, [this, device]() {
            selectDevice(device.description());
        });
    }

    for (const QAudioDevice &device : *outputDevices)
    {
        QAction *deviceAction = outputDeviceMenu->addAction(device.description());
        deviceAction->setCheckable(true);
        deviceAction->setActionGroup(deviceActionGroup);
        connect(deviceAction, &QAction::triggered, this, [this, device]() {
            selectDevice(device.description());
        });
    }
}

void MainWindow::createActions()
{
    testAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DialogInformation),
                          tr("&KKCK"), this);
    testAct->setWhatsThis("TEST WHATS THIS");
    // connect with a lambda function since testAction needs parameters
    connect(testAct, &QAction::triggered, this, [this]()
            {this->testAction("KKCK");
            });
}

void MainWindow::createMenus()
{
    menuBar = new QMenuBar(this);

    deviceMenu = new QMenu("Device", this);
    inputDeviceMenu = deviceMenu->addMenu("Input Devices");
    outputDeviceMenu = deviceMenu->addMenu("Output Devices");
    menuBar->addMenu(deviceMenu);

    populateAudioDeviceMenu();

    setMenuBar(menuBar);
}

void MainWindow::createStatusBar()
{
    statusBar = new QStatusBar(this);
    statusBar->setStyleSheet("QStatusBar { border-top: 1px solid black; }");

    statusMsg = new QLabel();

    statusBar->addWidget(statusMsg);

    setStatusBar(statusBar);
}

void MainWindow::updateStatusBar(const QString &message)
{
    statusMsg->setText(message);
}

void MainWindow::createBarChart()
{
    // DATA

    // Categories
    QStringList categories;
    categories << "uwu" << "nya" << "nichan";

    // Values
    QBarSet *set = new QBarSet("KKCK data");
    *set << 69 << 420 << 666;

    // The sets should be store in a bar series
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create chart and add the series
    barChart = new QChart();
    barChart->addSeries(series);
    barChart->setTitle("Frequencies detected");

    // Set up x axis
    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(categories);

    barChart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    // Set up y axis(values)
    QBarCategoryAxis *yAxis = new QBarCategoryAxis();
    yAxis->setTitleText("Frequencies");
    barChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    // Create chart view and set
    QChartView *chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setTheme(QChart::ChartThemeDark);

    setCentralWidget(chartView);
}

// Actions slots

void MainWindow::testAction(std::string activatedBy)
{
    QMessageBox msgBox;
    QString str = "Activated by: " + QString::fromStdString(activatedBy);
    msgBox.setText(str);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::selectDevice(const QString &deviceName)
{
    const QString str = "Selected device: " + deviceName;

    updateStatusBar(str);
}
