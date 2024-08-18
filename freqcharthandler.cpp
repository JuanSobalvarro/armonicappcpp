#include "freqcharthandler.h"

FreqChartHandler::FreqChartHandler(AudioProcessor *audioProcessor, QObject *parent)
    : QObject(parent), audioProcessor(audioProcessor),freqChart(nullptr), freqChartView(nullptr)
{
    createFreqChart();

    // Connect the signal from AudioProcessor to handle FFT data
    connect(audioProcessor, &AudioProcessor::fftDataReady,
            this, &FreqChartHandler::fftProcessAudioData);
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
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("Frequency (Hz)");
    freqChart->addAxis(xAxis, Qt::AlignBottom);
    audioProcessor->frequencySeries->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis();
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

void FreqChartHandler::updateChart(const FFTHandler::fftvector &fftData)
{
    audioProcessor->frequencySeries->clear();

    for (size_t i = 0; i < fftData.size(); ++i)
    {
        double freq = minFreq + (maxFreq - minFreq) * i / fftData.size();
        double magnitude = std::abs(fftData[i]);
        audioProcessor->frequencySeries->append(freq, magnitude);
    }
}

void FreqChartHandler::fftProcessAudioData(const FFTHandler::fftvector &fftdata)
{
    updateChart(fftdata);
}
