/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtNetwork/QtNetwork>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <iostream>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLineSeries *series = new QLineSeries();


    /* Get data from API */
    QChartView *chartView;
    QChart * chart;
    QNetworkAccessManager networkManager;

    QObject::connect(&networkManager, &QNetworkAccessManager::finished,
                     [&](QNetworkReply* reply){
        //this lambda is called when the reply is received
        //it can be a slot in your GUI window class
        //check for errors
        if(reply->error() != QNetworkReply::NoError){
            /* Check For Error */
        } else {
            //parse the reply JSON and display result in the UI
            QJsonObject jsonObject= QJsonDocument::fromJson(reply->readAll()).object();
            QString last= jsonObject["last"].toString();
            QString timestamp= jsonObject["timestamp"].toString();
            series->append(QDateTime::currentMSecsSinceEpoch(), last.toDouble());

            chart->axisY()->setRange(5000,7000);
        }
        reply->deleteLater();

    });

    QUrl url("https://www.bitstamp.net/api/v2/ticker/btceur/");
    QNetworkRequest networkRequest(url);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&](){
        networkManager.get(networkRequest);
    });
    timer.start(10000);




//![3]
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("BTC TO EUR CONVERSION");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->setAnimationOptions(QChart::AllAnimations);

    chart->setTitle("BTC/EUR Conversion");
    chart->axisX()->setRange(QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()),QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()+150000) );
    networkManager.get(networkRequest);
//![3]

//![4]
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
//![4]


//![5]
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
//![5]

    return a.exec();
}
