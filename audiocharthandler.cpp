#include "audiocharthandler.h"

AudioChartHandler::AudioChartHandler(AudioProcessor *audioProcessor, QObject *parent)
    : QObject(parent), audioChart(nullptr), audioChartView(nullptr), audioProcessor(audioProcessor)
{
    createAudioChart();
}

AudioChartHandler::~AudioChartHandler()
{
    delete audioChartView;
}

void AudioChartHandler::createAudioChart()
{
    QLineSeries *series = new QLineSeries;
    audioProcessor->audioSeries = series;

    audioChart = new QChart();
    audioChartView = new QChartView(audioChart);

    audioChart->addSeries(audioProcessor->audioSeries);

    QValueAxis *audioXAxis = new QValueAxis;
    audioXAxis->setRange(0, audioProcessor->sampleCount);
    audioXAxis->setLabelFormat("%g");
    audioXAxis->setTitleText("Samples");

    QValueAxis *audioYAxis = new QValueAxis;
    audioYAxis->setRange(-1.5, 1.5);
    audioYAxis->setTitleText("Audio Level");

    audioChart->addAxis(audioXAxis, Qt::AlignBottom);
    audioProcessor->audioSeries->attachAxis(audioXAxis);

    audioChart->addAxis(audioYAxis, Qt::AlignLeft);
    audioProcessor->audioSeries->attachAxis(audioYAxis);

    audioChart->legend()->hide();
    audioChart->setTitle("Data from input");
}

QChartView* AudioChartHandler::getAudioChartView()
{
    return audioChartView;
}
