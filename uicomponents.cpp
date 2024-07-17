#include "uicomponents.h"
#include "utils.h"

UIComponents::UIComponents(QMainWindow *window, QObject *parent)
    : QObject(parent)
{
    mainWindow = window;

    // Initialize charts
    chartHandler = new ChartHandler();

    createActions();
    createMenus();
    createStatusBar();
    createButtons();
    createCentralWidget();
    createLayouts();

    setAll();
}

UIComponents::~UIComponents()
{
}

void UIComponents::setAll()
{
    mainWindow->setMenuBar(menuBar);
    mainWindow->setStatusBar(statusBar);
    mainWindow->setCentralWidget(centralWidget);
    centralWidget->setLayout(layout); // Set layout to central widget
    mainWindow->setCentralWidget(centralWidget); // Set central widget to main window

    qInfo() << "UICOMPONENTS::SETALL: successfully setup\n";
}

// Connect callbacks to record buttons
void UIComponents::connectRecordButtons(
    std::function<void()> startAudioRecordingCallback,
    std::function<void()> stopAudioRecordingCallback)
{
    // Connect Start button
    connect(startRecordingButton,
            &QPushButton::clicked,
            mainWindow,
            startAudioRecordingCallback);

    connect(stopRecordingButton,
            &QPushButton::clicked,
            mainWindow,
            stopAudioRecordingCallback);

    qInfo() << "UICOMPONENTS::CONNECTRECORDBUTTONS: successfully connected buttons\n";
}

void UIComponents::createActions()
{
    testAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DialogInformation), tr("&KKCK"), this);
    testAct->setWhatsThis("TEST WHATS THIS");
    connect(testAct, &QAction::triggered, this, &UIComponents::onTestAction);

    qInfo() << "UICOMPONENTS::CREATEACTIONS: actions created successfully\n";
}

// You should ALWAYS populate the deviceMenu after creating menus
void UIComponents::createMenus()
{
    menuBar = new QMenuBar();

    deviceMenu = new QMenu("Device");
    inputDeviceMenu = deviceMenu->addMenu("Input Devices");
    outputDeviceMenu = deviceMenu->addMenu("Output Devices");
    menuBar->addMenu(deviceMenu);

    qInfo() << "UICOMPONENTS::CREATEMENUS: menus created successfully\n";
}

void UIComponents::createStatusBar()
{
    // Create status bar
    statusBar = new QStatusBar();
    statusBar->setStyleSheet("QStatusBar { border-top: 1px solid black; }");
    statusMsg = new QLabel();
    statusBar->addWidget(statusMsg);
    mainWindow->setStatusBar(statusBar);

    qInfo() << "UICOMPONENTS::CREATESTATUSBAR: statusbar created successfully\n";
}

void UIComponents::createButtons()
{
    startRecordingButton = new QPushButton("Start");
    stopRecordingButton = new QPushButton("Stop");

    qInfo() << "UICOMPONENTS::CREATEBUTTONS: buttons created successfully\n";
}

void UIComponents::createCentralWidget()
{
    centralWidget = new QWidget();

    qInfo() << "UICOMPONENTS::CREATECENTRALWIDGET: Central Widget created successfully\n";
}

void UIComponents::createLayouts()
{
    layout = new QGridLayout(); // Set the parent as centralWidget
    buttonsLayout = new QHBoxLayout(); // No need to set parent
    chartsLayout = new QHBoxLayout(); // No need to set parent

    startRecordingButton = new QPushButton("Start"); // Set parent as centralWidget

    stopRecordingButton = new QPushButton("Stop"); // Set parent as centralWidget

    buttonsLayout->addWidget(startRecordingButton);
    buttonsLayout->addWidget(stopRecordingButton);

    if (!chartHandler->audioChartView || !chartHandler->barChartView)
    {
        qCritical() << "UICOMPONENTS::CREATELAYOUT: CHARTS VIEW NOT LOADED\n";
    }

    chartsLayout->addWidget(chartHandler->barChartView);
    chartsLayout->addWidget(chartHandler->audioChartView);

    // Add layouts to main layout
    layout->addLayout(buttonsLayout, 0, 0);
    layout->addLayout(chartsLayout, 1, 0);

    qInfo() << "UICOMPONENTS::CREATELAYOUTS: Layouts created\n";
}

void UIComponents::populateAudioDeviceMenu(
    const QList<QAudioDevice> &inputDevices,
    const QList<QAudioDevice> &outputDevices,
    std::function<void(const QAudioDevice &)> selectDeviceCallback)
{
    // Create an action group for exclusive selection
    deviceActionGroup = new QActionGroup(this);
    deviceActionGroup->setExclusive(true);

    if (!inputDeviceMenu || !outputDeviceMenu)
    {
        qCritical() << "INPUT/OUTPUT DEVICES MENU NOT CREATED\n";
        return;
    }

    for (const auto &device : inputDevices)
    {
        QAction *deviceAction = new QAction(device.description(), inputDeviceMenu);
        deviceAction->setCheckable(true);
        deviceAction->setActionGroup(deviceActionGroup);
        connect(deviceAction, &QAction::triggered, [device, selectDeviceCallback]()
        {
            selectDeviceCallback(device);
        });
        inputDeviceMenu->addAction(deviceAction);
    }

    for (const auto &device : outputDevices)
    {
        QAction *deviceAction = new QAction(device.description(), outputDeviceMenu);
        deviceAction->setCheckable(true);
        deviceAction->setActionGroup(deviceActionGroup);
        connect(deviceAction, &QAction::triggered, [device, selectDeviceCallback]()
        {
            selectDeviceCallback(device);
        });
        outputDeviceMenu->addAction(deviceAction);
    }

    qInfo() << "UICOMPONENTS::POPULATEAUDIODEVICEMENU: DEVICE MENU SUCCESSFULLY POPULATED\n";
}

void UIComponents::updateStatusMsg(const QString &msg)
{
    statusMsg->setText(msg);
}

// SLOTS
void UIComponents::onTestAction()
{
    qInfo() << "Test slot\n";
}

void UIComponents::startButtonPressed()
{
    if (startRecordingButton->isFlat())
    {
        return;
    }

    startRecordingButton->setFlat(true);
    stopRecordingButton->setFlat(false);

    qInfo() << "UICOMPONENTS::STARTBUTTONPRESSED\n";
}

void UIComponents::stopButtonPressed()
{
    if (stopRecordingButton->isFlat())
    {
        return;
    }

    startRecordingButton->setFlat(false);
    stopRecordingButton->setFlat(true);

    qInfo() << "UICOMPONENTS::STOPBUTTONPRESSED\n";
}
