/*
 * Handles all related to the charts present in the program
 */
#ifndef CHARTHANDLER_H
#define CHARTHANDLER_H

#include <QObject>

#include "freqcharthandler.h"
#include "audiocharthandler.h"

#include "ffthandler.h"

class ChartHandler : public QObject
{
    Q_OBJECT

public:
    ChartHandler(AudioProcessor *audioProcessor, QObject *parent = nullptr);
    ~ChartHandler();

    QChartView *getAudioChartView();
    QChartView *getBarChartView();

private:
    FreqChartHandler *freqChartHandler;
    AudioChartHandler *audioChartHandler;
};

#endif // CHARTHANDLER_H
