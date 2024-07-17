#include "charthandler.h"

ChartHandler::ChartHandler(QObject *parent)
    : QObject(parent), barChart(nullptr), audioChart(nullptr), barChartView(nullptr), audioChartView(nullptr)
{
    createBarChart();
    createAudioChart();
}

ChartHandler::~ChartHandler()
{
    delete barChartView;
    delete audioChartView;
}

void ChartHandler::createBarChart()
{
    // DATA
    QStringList categories;
    categories << "uwu" << "nya" << "nichan";

    QBarSet *set = new QBarSet("KKCK data");
    *set << 69 << 420 << 666;

    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Bar chart
    barChart = new QChart();
    barChart->addSeries(series);
    barChart->setTitle("Frequencies detected");

    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(categories);
    barChart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setTitleText("Frequencies");
    barChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    // bar chart view
    barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->chart()->setTheme(QChart::ChartThemeDark);
}

void ChartHandler::createAudioChart()
{
    // audio chart
    audioChart = new QChart();

    // audio chart view
    audioChartView = new QChartView(audioChart);
}
