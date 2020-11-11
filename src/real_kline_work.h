#ifndef REALKLINEWORK_H
#define REALKLINEWORK_H

#include <QObject>
#include "qwebdata.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32
#include <iostream>
#include "trade_spi.h"
#include "FileUtils.h"
#include "xtp_quote_api.h"
#include "quote_spi.h"

class RealKLineWork : public QObject
{
    Q_OBJECT
public:
    QJsonObject realJsonObj; // 存放实时K线的
public slots:
    void doWork();

signals:
    void resultReady(QJsonObject realJsonObj);
};

#endif // REALKLINEWORK_H
