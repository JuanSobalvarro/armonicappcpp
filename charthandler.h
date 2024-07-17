#ifndef CHARTHANDLER_H
#define CHARTHANDLER_H

#include <QObject>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QLineSeries>

class ChartHandler : public QObject
{
    Q_OBJECT

public:
    ChartHandler(QObject *parent = nullptr);
    ~ChartHandler();

    QChart *barChart;
    QChart *audioChart;
    QChartView *barChartView;
    QChartView *audioChartView;

private:    
    void createBarChart();
    void createAudioChart();
};

#endif // CHARTHANDLER_H

