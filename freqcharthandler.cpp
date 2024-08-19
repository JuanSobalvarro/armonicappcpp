#include "freqcharthandler.h"

FreqChartHandler::FreqChartHandler(AudioProcessor *audioProcessor, QObject *parent)
    : QObject(parent), audioProcessor(audioProcessor),freqChart(nullptr), freqChartView(nullptr)
{
    createFreqChart();
}

FreqChartHandler::~FreqChartHandler()
{
    delete freqChartView;
}

void FreqChartHandler::createFreqChart()
{
    QLineSeries *series = new QLineSeries;
    audioProcessor->frequencySeries = series;

    freqChart = new QChart();
    freqChart->addSeries(audioProcessor->frequencySeries);
    freqChart->setTitle("Frequency Spectrum");

    QValueAxis *xAxis = new QValueAxis();
    xAxis->setRange(0, 1000);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("Frequency (Hz)");
    freqChart->addAxis(xAxis, Qt::AlignBottom);
    audioProcessor->frequencySeries->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(0, 100);
    yAxis->setTitleText("Magnitude");
    freqChart->addAxis(yAxis, Qt::AlignLeft);
    audioProcessor->frequencySeries->attachAxis(yAxis);

    freqChartView = new QChartView(freqChart);
    freqChartView->setRenderHint(QPainter::Antialiasing);
    freqChartView->chart()->setTheme(QChart::ChartThemeDark);
}


QChartView* FreqChartHandler::getBarChartView()
{
    return freqChartView;
}

