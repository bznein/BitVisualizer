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

QT_CHARTS_USE_NAMESPACE

class mainWindow : public QMainWindow
{
public:
    mainWindow();
    ~mainWindow();

private:
   QLineSeries *_series; // Series of btc/eur values

   /* Visualization members */
   QChartView *_chartView;
   QChart * _chart;
   const int _refreshWaitTime=1000; /* 10 seconds */
   const int _totalTimeShown=15000; /* 150 seconds of history */
   const int _totalSamples=_totalTimeShown/_refreshWaitTime;

   QDateTime _startTime, _endTime;
   int _presentSamples=0;

   /* Initial guesses for the max and min val, they will get updated */
   double _maxVal=7000;
   double _minVal=5000;



   /* Networking/API members */
   QNetworkAccessManager _networkManager;
   QNetworkRequest _networkRequest;
   QTimer _timer;
};

#endif // MAINWINDOW_H
