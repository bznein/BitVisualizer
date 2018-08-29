#include "mainwindow.h"
#include <iostream>
mainWindow::mainWindow()
{
    _series=new QLineSeries();
    qint64 _curTime=QDateTime::currentMSecsSinceEpoch();
    _startTime=QDateTime::fromMSecsSinceEpoch(_curTime);
    _endTime=QDateTime::fromMSecsSinceEpoch(_curTime+_totalTimeShown);
    QObject::connect(&_networkManager, &QNetworkAccessManager::finished,
                     [&](QNetworkReply* reply){

        if(reply->error() != QNetworkReply::NoError){
            /* Check For Error */
        } else {
            //parse the reply JSON and display result in the UI
            QJsonObject jsonObject= QJsonDocument::fromJson(reply->readAll()).object();
            QString last= jsonObject["last"].toString();
            QString timestamp= jsonObject["timestamp"].toString();
            _series->append(QDateTime::currentMSecsSinceEpoch(), last.toDouble());
            _chart->axisY()->setRange(5000,7000);
        }
        reply->deleteLater();

    });

    QUrl url("https://www.bitstamp.net/api/v2/ticker/btceur/");
    _networkRequest=QNetworkRequest(url);


    QObject::connect(&_timer, &QTimer::timeout, [&](){
        _networkManager.get(_networkRequest);
    });
    _timer.start(_refreshWaitTime);



    _chart = new QChart();
    _chart->legend()->hide();
    _chart->addSeries(_series);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Date");
    _chart->addAxis(axisX, Qt::AlignBottom);
    _series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("BTC TO EUR CONVERSION");
    _chart->addAxis(axisY, Qt::AlignLeft);
    _series->attachAxis(axisY);
    _chart->setAnimationOptions(QChart::AllAnimations);

    _chart->setTitle("BTC/EUR Conversion");
    _chart->axisX()->setRange(_startTime,_endTime);
    _networkManager.get(_networkRequest);

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);



    setCentralWidget(_chartView);

}

mainWindow::~mainWindow()
{
    delete _series;
}
