#include "image_info_show.h"
#include "painter.h"
#include "stockDataGet.h"
#include "ui_mainwindow.h"
#include <QJsonValue>
#include <QDebug>
image_info_show::image_info_show(QWidget *parent) : QWidget(parent)
{

}


void image_info_show::paintEvent(QPaintEvent *event)
{
    QJsonValue jsonvalue = stockDataGet::stockInfoReal(string);
    painter::painteInfo(jsonvalue,this);
    this->update();

//    QTimer *updateTimer = new QTimer(this);
//    updateTimer->start(2000);
//    connect(updateTimer,SIGNAL(timeout()),this,SLOT(update()));

}

