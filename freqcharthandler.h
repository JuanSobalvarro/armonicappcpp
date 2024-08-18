#ifndef FREQCHARTHANDLER_H
#define FREQCHARTHANDLER_H

#include <QObject>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include "audioprocessor.h"

class FreqChartHandler : public QObject
{
    Q_OBJECT

public:
    explicit FreqChartHandler(AudioProcessor *audioProcessor, QObject *parent = nullptr);
    ~FreqChartHandler();

    QChartView* getBarChartView();

    int minFreq = 0;
    int maxFreq = 1000;

public slots:
    void fftProcessAudioData(const FFTHandler::fftvector &fftData);

private:
    void createFreqChart();
    void updateChart(const FFTHandler::fftvector &fftData);

    AudioProcessor *audioProcessor;
    QChart *freqChart;
    QChartView *freqChartView;
};

#endif // FREQCHARTHANDLER_H
