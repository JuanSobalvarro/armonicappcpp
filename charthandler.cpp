#include "charthandler.h"

ChartHandler::ChartHandler(AudioProcessor *audioProcessor, QObject *parent)
    : QObject(parent)
{
    freqChartHandler = new FreqChartHandler(audioProcessor, this);
    audioChartHandler = new AudioChartHandler(audioProcessor, this);
}

ChartHandler::~ChartHandler()
{
    delete freqChartHandler;
    delete audioChartHandler;
}

QChartView *ChartHandler::getAudioChartView()
{
    return audioChartHandler->getAudioChartView();
}

QChartView *ChartHandler::getBarChartView()
{
    return freqChartHandler->getBarChartView();
}

void ChartHandler::updateBarChart(const FFTHandler::fftvector &fftdata)
{
    freqChartHandler->fftProcessAudioData(fftdata);
}
