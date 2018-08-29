#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtNetwork/QtNetwork>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QLayout>
#include <QPen>
#include <math.h>

QT_CHARTS_USE_NAMESPACE

class mainWindow : public QMainWindow
{
public:
    mainWindow();
    ~mainWindow() override;

    void resizeEvent(QResizeEvent* event) override;

private:
   QLineSeries *_series; // Series of btc/eur values

   /* Visualization members */
   QChartView *_chartView;
   QChart * _chart;
   QGraphicsSimpleTextItem *_curValue;
   QValueAxis *_axisY;
   QDateTimeAxis *_axisX;
   const int _refreshWaitTime=10000; /* 10 seconds */
   const int _totalTimeShown=150000; /* 150 seconds of history */
   const int _totalSamples=_totalTimeShown/_refreshWaitTime; /* To know when the linechart is full */

   QDateTime _startTime, _endTime; /* Range for the x-axis */
   int _presentSamples=0;

   /* Initial guesses for the max and min val, they will get updated, used for the y-axis
      Since the actual value is an always positive ratio, -1 is a valid initializer */
   double _maxVal=nan("");
   double _minVal=nan("");
   int _initial_range=50;



   /* Networking/API members */
   QNetworkAccessManager _networkManager;
   QNetworkRequest _networkRequest;
   QTimer _timer;
};

#endif // MAINWINDOW_H
