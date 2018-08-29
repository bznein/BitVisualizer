#include "mainwindow.h"
#include <iostream>
#include <algorithm>
mainWindow::mainWindow()
{
     setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);


     /* Initialization of most variables, described in header file */
    _curValue=new QGraphicsSimpleTextItem();
    _series=new QLineSeries();
    _series->setPointsVisible();
    qint64 _curTime=QDateTime::currentMSecsSinceEpoch();
    _startTime=QDateTime::fromMSecsSinceEpoch(_curTime);
    _endTime=QDateTime::fromMSecsSinceEpoch(_curTime+_totalTimeShown);


    /* Creation of the callback to execute when the API request is processed */
    QObject::connect(&_networkManager, &QNetworkAccessManager::finished,
                     [&](QNetworkReply* reply){

        if(reply->error() != QNetworkReply::NoError){
            /* We just do nothing and wait for the next refresh */
        } else {
            //parse the reply JSON and display result in the UI
            QJsonObject jsonObject= QJsonDocument::fromJson(reply->readAll()).object();
            QString lastS=jsonObject["last"].toString();
            double last= lastS.toDouble();
            QString timestamp= jsonObject["timestamp"].toString();

            if (_presentSamples<_totalSamples)
                ++_presentSamples;
            else
            {
                _startTime.setMSecsSinceEpoch(_startTime.toMSecsSinceEpoch()+ _refreshWaitTime);
                _endTime.setMSecsSinceEpoch(_endTime.toMSecsSinceEpoch()+ _refreshWaitTime);
                _chart->axisX()->setRange(_startTime,_endTime);
            }

            if (isnan(_maxVal) && isnan(_minVal))
            {
                _maxVal=last+_initial_range;
                _minVal=last-_initial_range;

                _chart->axisY()->setRange(_minVal,_maxVal);
            }
            else if (last>_maxVal || last<_minVal)
            {
                _maxVal=std::max(last,_maxVal);
                _minVal=std::min(last,_minVal);

                _chart->axisY()->setRange(_minVal,_maxVal);

            }
            /* Note: this will eventually run out of memory, this is just a simple example. We should actually
             * consider, using an auxiliary series, and after a number of iteration clearing alternatively one
             * of the two and setting the other one as the one used for the drawing */
            _series->append(QDateTime::currentMSecsSinceEpoch(), last);
            _curValue->setPos(this->size().width()*0.6, this->size().height()*0.01);
            _curValue->setText("Current value: " + lastS);
        }
        reply->deleteLater();

    });

    /* Creation of the API Query */
    QUrl url("https://www.bitstamp.net/api/v2/ticker/btceur/");
    _networkRequest=QNetworkRequest(url);


    /* We bind the execution of the query to a timer */
    QObject::connect(&_timer, &QTimer::timeout, [&](){
        _networkManager.get(_networkRequest);
    });
    _timer.start(_refreshWaitTime);

    _networkManager.get(_networkRequest);

    /* Initialization of the chart-related variables*/
    _chart = new QChart();
    _chart->legend()->hide();
    _chart->addSeries(_series);

    _axisX = new QDateTimeAxis;
    _axisX->setTickCount(10);
    _axisX->setFormat("hh:mm:ss");
    _axisX->setTitleText("Time of the day");
    _chart->addAxis(_axisX, Qt::AlignBottom);
    _series->attachAxis(_axisX);

    _axisY= new QValueAxis;
    _axisY->setLabelFormat("%i");
    _axisY->setTitleText("BTC TO EUR CONVERSION");
    _chart->addAxis(_axisY, Qt::AlignLeft);
    _series->attachAxis(_axisY);
    _chart->setAnimationOptions(QChart::SeriesAnimations);

    _chart->setTitle("BTC/EUR Conversion");
    _chart->axisX()->setRange(_startTime,_endTime);

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);


    _curValue = new QGraphicsSimpleTextItem(_chart);
    _curValue->setFont(QFont("Arial",25));
    setCentralWidget(_chartView);

}

void mainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   _curValue->setPos(this->size().width()*0.6, this->size().height()*0.01);
}



mainWindow::~mainWindow()
{
    delete _series;
    delete _chart;
    delete _chartView;
    delete _curValue;
    delete _axisX;
    delete _axisY;
}
