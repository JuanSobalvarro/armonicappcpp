#ifndef AUDIOCHARTHANDLER_H
#define AUDIOCHARTHANDLER_H

#include <QObject>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

#include "audioprocessor.h"

class AudioChartHandler : public QObject
{
    Q_OBJECT

public:
    explicit AudioChartHandler(AudioProcessor *audioProcessor, QObject *parent = nullptr);
    ~AudioChartHandler();

    QChartView* getAudioChartView();

private:
    QChart *audioChart;
    QChartView *audioChartView;

    AudioProcessor *audioProcessor;

    void createAudioChart();
};

#endif // AUDIOCHARTHANDLER_H
