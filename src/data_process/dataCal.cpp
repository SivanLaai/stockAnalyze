#include "dataCal.h"
#include "painter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

double dataCal::minKCal(QJsonValue jsonvalue)
{
    //获取数据
    QJsonArray array = jsonvalue.toObject().value(data).toArray();
    double min = array.size() > 0 ? array[0].toObject()[lower_price].toString().toDouble() : 0.0;
    for (int i = 1; i < array.size(); i++)
    {
        QJsonValue v = array[i].toObject().value(lower_price);
        min = (min < v.toString().toDouble()) ? min : v.toString().toDouble();
    }
    return min;
}

double dataCal::maxKCal(QJsonValue jsonvalue)
{
    //获取数据
    QJsonArray array = jsonvalue.toObject().value(data).toArray();
    double max = array.size() > 0 ? array[0].toObject()[high_price].toString().toDouble() : 0.0;
    for (int i = 1; i < array.size(); i++)
    {
        QJsonValue v = array[i].toObject().value(high_price);
        max = (max > v.toString().toDouble()) ? max : v.toString().toDouble();
    }
    return max;
}
double dataCal::maxVolumnKCal(QJsonValue jsonvalue)
{
    //获取数据
    QJsonArray array = jsonvalue.toObject().value(data).toArray();
    double max = array.size() > 0 ? array[0].toObject()[volume].toString().toDouble() : 0.0;
    for (int i = 1; i < array.size(); i++)
    {
        QJsonValue v = array[i].toObject().value(volume);
        max = (max > v.toString().toDouble()) ? max : v.toString().toDouble();
    }
    return max;
}
double dataCal::minVolumnKCal(QJsonValue jsonvalue)
{
    QJsonArray array = jsonvalue.toObject().value(data).toArray();
    double min = array.size() > 0 ? array[0].toObject()[volume].toString().toDouble() : 0.0;
    for (int i = 1; i < array.size(); i++)
    {
        QJsonValue v = array[i].toObject().value(volume);
        min = (min < v.toString().toDouble()) ? min : v.toString().toDouble();
    }
    return min;
}

double dataCal::sumKCal(QJsonValue jsonvalue)
{
    //定义差值
    double _sum;
    _sum = dataCal::maxKCal(jsonvalue) - dataCal::minKCal(jsonvalue);
    return _sum;
}

double dataCal::sumVolumnKCal(QJsonValue jsonvalue)
{
    double _sum;
    _sum = dataCal::maxVolumnKCal(jsonvalue) - dataCal::minVolumnKCal(jsonvalue);
    return _sum;
}

double dataCal::minRCal(QJsonValue jsonvalue)
{
    return minKCal(jsonvalue);
}

double dataCal::minVolumnRCal(QJsonValue jsonvalue)
{
    return minVolumnKCal(jsonvalue);
}

double dataCal::maxRCal(QJsonValue jsonvalue)
{
    return maxKCal(jsonvalue);
}
double dataCal::maxVolumnRCal(QJsonValue jsonvalue)
{
    return maxVolumnKCal(jsonvalue);
}

double dataCal::sumRCal(QJsonValue jsonvalue)
{
    //定义差值
    double _sum;
    _sum = dataCal::maxRCal(jsonvalue) - dataCal::minRCal(jsonvalue);
    return _sum;
}

double dataCal::sumVolumnRCal(QJsonValue jsonvalue)
{
    double _sum;
    _sum = dataCal::maxVolumnRCal(jsonvalue) - dataCal::minVolumnRCal(jsonvalue);
    return _sum;
}

double dataCal::maxKIndexCal(QJsonArray jsonarray)
{
    double _max;
    _max = jsonarray[0].toObject().value("DEA").toString().toDouble();
    for (int i = 0; i < jsonarray.size(); i++)
    {
        if (jsonarray[i].toObject().value("DEA").toString().toDouble() > _max)
        {
            _max = jsonarray[i].toObject().value("DEA").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("DIFF").toString().toDouble() > _max)
        {
            _max = jsonarray[i].toObject().value("DIFF").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("MACD").toString().toDouble() > _max)
        {
            _max = jsonarray[i].toObject().value("MACD").toString().toDouble();
        }
    }
    return _max;
}

double dataCal::minKIndexCal(QJsonArray jsonarray)
{
    double _min;
    _min = jsonarray[0].toObject().value("DEA").toString().toDouble();
    for (int i = 0; i < jsonarray.size(); i++)
    {
        if (jsonarray[i].toObject().value("DEA").toString().toDouble() < _min)
        {
            _min = jsonarray[i].toObject().value("DEA").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("DIFF").toString().toDouble() < _min)
        {
            _min = jsonarray[i].toObject().value("DIFF").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("MACD").toString().toDouble() < _min)
        {
            _min = jsonarray[i].toObject().value("MACD").toString().toDouble();
        }
    }
    return _min;
}

double dataCal::maxKRSIIndexCal(QJsonArray jsonarray)
{
    double _max;
    _max = jsonarray[0].toObject().value("rsiData6").toString().toDouble();
    for (int i = 0; i < jsonarray.size(); i++)
    {
        if (jsonarray[i].toObject().value("rsiData6").toString().toDouble() > _max)
        {
            _max = jsonarray[i].toObject().value("rsiData6").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("rsiData12").toString().toDouble() > _max)
        {
            _max = jsonarray[i].toObject().value("rsiData12").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("rsiData24").toString().toDouble() > _max)
        {
            _max = jsonarray[i].toObject().value("rsiData24").toString().toDouble();
        }
    }
    return _max;
}

double dataCal::minKRSIIndexCal(QJsonArray jsonarray)
{
    double _min;
    _min = jsonarray[0].toObject().value("rsiData6").toString().toDouble();
    for (int i = 0; i < jsonarray.size(); i++)
    {
        if (jsonarray[i].toObject().value("rsiData6").toString().toDouble() < _min)
        {
            _min = jsonarray[i].toObject().value("rsiData6").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("rsiData12").toString().toDouble() < _min)
        {
            _min = jsonarray[i].toObject().value("rsiData12").toString().toDouble();
        }
        if (jsonarray[i].toObject().value("rsiData24").toString().toDouble() < _min)
        {
            _min = jsonarray[i].toObject().value("rsiData24").toString().toDouble();
        }
    }
    return _min;
}
