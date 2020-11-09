#ifndef DATACAL_H
#define DATACAL_H

#include <QObject>
#include <QJsonValue>

class dataCal : public QObject
{
    Q_OBJECT
public:
    explicit dataCal(QObject *parent = 0);
    static double minKCal(QJsonValue jsonvalue);//最大值计算
    static double maxKCal(QJsonValue jsonvalue);//最小值计算
    static double sumKCal(QJsonValue jsonvalue);//差值计算
    static double minRCal(QJsonValue jsonvalue);//最大值计算
    static double maxRCal(QJsonValue jsonvalue);//最小值计算
    static double sumRCal(QJsonValue jsonvalue);//差值计算
    static double maxVolumnKCal(QJsonValue jsonvalue);//成交量最大值计算
    static double minVolumnKCal(QJsonValue jsonvalue);//成交量最小值计算
    static double sumVolumnKCal(QJsonValue jsonvalue);//成交量差值计算
    static double maxVolumnRCal(QJsonValue jsonvalue);//成交量最大值计算
    static double minVolumnRCal(QJsonValue jsonvalue);//成交量最小值计算
    static double sumVolumnRCal(QJsonValue jsonvalue);//成交量差值计算
    static double maxKIndexCal(QJsonArray jsonarray);//MACD最大值计算
    static double minKIndexCal(QJsonArray jsonarray);//MACD最小值计算
    static double maxKRSIIndexCal(QJsonArray jsonarray);//RSI最大值计算
    static double minKRSIIndexCal(QJsonArray jsonarray);//RSI最小值计算

signals:

public slots:
};

#endif // DATACAL_H
