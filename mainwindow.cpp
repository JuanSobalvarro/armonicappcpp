#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 1280, 480);

    // SETUP GUI
    createActions();
    createMenus();
    createStatusBar();
    createBarChart();
    createAudioChart();
    createCentralWidget();
    createLayout();

    // AUDIO COLLECTING
    setUpAudio();

}

MainWindow::~MainWindow()
{
    qInfo() << "MainWindow destructor called\n";

    delete inputDevices;
    delete outputDevices;
    delete barChartView;
    delete audioChartView;

    stopAudioRecording(); // Ensure audioInput is stopped and cleaned up
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
            selectDevice(device);
        });
    }

    for (const QAudioDevice &device : *outputDevices)
    {
        QAction *deviceAction = outputDeviceMenu->addAction(device.description());
        deviceAction->setCheckable(true);
        deviceAction->setActionGroup(deviceActionGroup);
        connect(deviceAction, &QAction::triggered, this, [this, device]() {
            selectDevice(device);
        });
    }

    qInfo() << "MAINWINDOW::POPULATEAUDIODEVICEMENU: Audio Devices recognized successfully\n";
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

    qInfo() << "MAINWINDOW::CREATEACTIONS: Actions created successfully\n";
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

    qInfo() << "MAINWINDOW::CREATEMENUS: Menus created successfully\n";
}

void MainWindow::createStatusBar()
{
    statusBar = new QStatusBar(this);
    statusBar->setStyleSheet("QStatusBar { border-top: 1px solid black; }");

    statusMsg = new QLabel();

    statusBar->addWidget(statusMsg);

    setStatusBar(statusBar);

    qInfo() << "MAINWINDOW::CREATESTATUSBAR: Status bar created\n";
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
    barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->chart()->setTheme(QChart::ChartThemeDark);

    qInfo() << "MAINWINDOW::CREATEBARCHART: Bar Chart created successfully\n";
}

void MainWindow::createAudioChart()
{
    audioChart = new QChart();

    audioChartView = new QChartView(audioChart);

    qInfo() << "MAINWINDOW::CREATEAUDIOCHART: Audio chart created successfully\n";
}

void MainWindow::createCentralWidget()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    qInfo() << "MAINWINDOW::CREATECENTRALWIDGET: Central Widget created and set\n";
}

void MainWindow::createLayout()
{
    layout = new QGridLayout();

    // Buttons Layout
    buttonsLayout = new QHBoxLayout();

    startRecordingButton = new QPushButton("Start", this);
    connect(startRecordingButton, &QPushButton::clicked, this, &MainWindow::startAudioRecording);

    buttonsLayout->addWidget(startRecordingButton);

    stopRecordingButton = new QPushButton("Stop", this);
    connect(stopRecordingButton, &QPushButton::clicked, this, &MainWindow::stopAudioRecording);

    buttonsLayout->addWidget(stopRecordingButton);


    // Charts Layout
    chartsLayout = new QHBoxLayout();

    if (!barChartView)
    {
        qCritical() << "MAINWINDOW::CREATELAYOUT: BARCHARTVIEW NOT LOADED IN MEMORY\n";
        return;
    }
    chartsLayout->addWidget(barChartView);

    if (!audioChartView)
    {
        qCritical() << "MAINWINDOW::CREATELAYOUT: AUDIOCHARTVIEW NOT LOADED IN MEMORY\n";
        return;
    }
    chartsLayout->addWidget(audioChartView);

    // Add to mainLayout
    layout->addLayout(buttonsLayout, 0, 0, Qt::AlignTop);
    layout->addLayout(chartsLayout, 1, 0, Qt::AlignBottom);

    // Set Central Widget layout
    centralWidget->setLayout(layout);

    qInfo() << "MAINWINDOW::CREATELAYOUT: Layout created\n";
}

void MainWindow::updateStatusBar(const QString &message)
{
    statusMsg->setText(message);
}

QAudioFormat MainWindow::createAudioFormat(unsigned int sampleRate, unsigned int channelCount, QAudioFormat::SampleFormat sampleFormat)
{
    QAudioFormat format;
    // Set up the format, adjust these parameters as needed
    format.setSampleRate(sampleRate);           // Sample rate (Hz)
    format.setChannelCount(channelCount);             // Mono channel
    format.setSampleFormat(sampleFormat);              // Sample size (bits)

    if (!format.isValid()) {
        qWarning() << "Invalid audio format settings.";
    }

    return format;
}

void MainWindow::setUpAudio()
{
    // Configure Audio Format
    audioFormat = createAudioFormat(44100, 1, QAudioFormat::Int16);

    // Create QAudioInput
    audioInput = new QAudioInput(audioFormat, this);

    // Create Custom


}

// slots

void MainWindow::testAction(std::string activatedBy)
{
    QMessageBox msgBox;
    QString str = "Activated by: " + QString::fromStdString(activatedBy);
    msgBox.setText(str);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::errorMsgBox(const QString msg, QMessageBox::Icon icon)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(icon);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::selectDevice(const QAudioDevice &device)
{
    // Check if device is already selected
    if (deviceSelected == device)
    {
        qInfo() << "Device already selected\n";
        return;
    }

    // Check if device support format
    if (device.isFormatSupported(createAudioFormat()))
    {
        qInfo() << "Selected device supports desired audio format.";
    }
    else
    {
        const QString msg = "Device " + device.description() + " does not support format";
        errorMsgBox(msg, QMessageBox::Warning);
    }

    // UPDATE device selected
    deviceSelected = device;

    // UPDATE UI
    const QString str = "Selected device: " + deviceSelected.description();
    updateStatusBar(str);
}

void MainWindow::startAudioRecording()
{
    if (startRecordingButton->isFlat())
    {
        return;
    }
    qInfo() << "Starting Record\n";

    startRecordingButton->setFlat(true);
    stopRecordingButton->setFlat(false);

    // ...
}

void MainWindow::stopAudioRecording()
{
    if (stopRecordingButton->isFlat())
    {
        return;
    }

    if (!startRecordingButton->isFlat())
    {
        return;
    }
    qInfo() << "Stopping Record\n";

    startRecordingButton->setFlat(false);
    stopRecordingButton->setFlat(true);

    // ...
}

void MainWindow::handleAudioInput()
{

}
