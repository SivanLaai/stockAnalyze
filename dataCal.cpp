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
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray array = jsonvalue.toObject().value("list").toArray();
        if (array.size() > 0)
        {
            //转为数组
            //    const int arraySize = 1000;
            //    double arr[arraySize] = {};
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("max_price");
                arr[i] = v.toString().toDouble();
            }
            //最大值定义
            double _max = arr[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] > _max)
                {
                    _max = arr[i];
                }
            }
            free(arr);
            return _max;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
double dataCal::maxVolumnKCal(QJsonValue jsonvalue)
{
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray array = jsonvalue.toObject().value("list").toArray();
        if (array.size() > 0)
        {
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("trade_num");
                arr[i] = v.toString().toDouble();
            }
            //最大值定义
            double _max = arr[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] > _max)
                {
                    _max = arr[i];
                }
            }
            free(arr);
            return _max;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
double dataCal::minVolumnKCal(QJsonValue jsonvalue)
{
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray array = jsonvalue.toObject().value("list").toArray();
        if (array.size() > 0)
        {
            //转为数组
            //    const int arraySize = 1000;
            //    double arr[arraySize] = {};
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));

            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("trade_num");
                arr[i] = v.toString().toDouble();
            }
            //最小值定义
            double _min = arr[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] < _min)
                {
                    _min = arr[i];
                }
            }
            return _min;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
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
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray arrayFirst = jsonvalue.toObject().value("list").toArray();
        QJsonArray array;
        if (arrayFirst.size() > 0)
        {
            array = arrayFirst[0].toObject().value("minuteList").toArray();
            //转为数组
            //    const int arraySize = 1000;
            //    double arr[arraySize] = {};
            //    double arr2[arraySize]={};
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            double *arr2 = (double *)malloc(sizeof(double) * 10);
            arr2 = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("avgPrice");
                arr[i] = v.toString().toDouble();
            }
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("nowPrice");
                arr2[i] = v.toString().toDouble();
            }
            //最小值定义
            double _minarr = arr[0];
            double _minarr2 = arr2[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] < _minarr)
                {
                    _minarr = arr[i];
                }
            }
            for (int i = 0; i < array.size(); i++)
            {
                if (arr2[i] < _minarr2)
                {
                    _minarr2 = arr2[i];
                }
            }
            //            free(arr);
            //            free(arr2);
            if (_minarr <= _minarr2)
            {
                return _minarr;
            }
            else
            {
                return _minarr2;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

double dataCal::minVolumnRCal(QJsonValue jsonvalue)
{
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray arrayFirst = jsonvalue.toObject().value("list").toArray();
        QJsonArray array;
        if (arrayFirst.size() > 0)
        {
            array = arrayFirst[0].toObject().value("minuteList").toArray();
            //转为数组
            //    const int arraySize = 1000;
            //    double arr[arraySize] = {};
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));

            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("trade_num");
                arr[i] = v.toString().toDouble();
            }
            //最小值定义
            double _min = arr[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] < _min)
                {
                    _min = arr[i];
                }
            }
            return _min;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

double dataCal::maxRCal(QJsonValue jsonvalue)
{
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray arrayFirst = jsonvalue.toObject().value("list").toArray();
        QJsonArray array;
        if (arrayFirst.size() > 0)
        {
            array = arrayFirst[0].toObject().value("minuteList").toArray();

            //转为数组
            //    const int arraySize = 1000;
            //    double arr[arraySize] = {};
            //    double arr2[arraySize]={};
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            double *arr2 = (double *)malloc(sizeof(double) * 10);
            arr2 = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("avgPrice");
                arr[i] = v.toString().toDouble();
            }
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("nowPrice");
                arr2[i] = v.toString().toDouble();
            }
            //最小值定义
            double _maxarr = arr[0];
            double _maxarr2 = arr2[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] > _maxarr)
                {
                    _maxarr = arr[i];
                }
            }
            for (int i = 0; i < array.size(); i++)
            {
                if (arr2[i] > _maxarr2)
                {
                    _maxarr2 = arr2[i];
                }
            }
            free(arr);
            free(arr2);
            if (_maxarr <= _maxarr2)
            {
                return _maxarr2;
            }
            else
            {
                return _maxarr2;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
double dataCal::maxVolumnRCal(QJsonValue jsonvalue)
{
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonArray arrayFirst = jsonvalue.toObject().value("list").toArray();
        QJsonArray array;
        if (arrayFirst.size() > 0)
        {
            array = arrayFirst[0].toObject().value("minuteList").toArray();
            double *arr = (double *)malloc(sizeof(double) * 10);
            arr = (double *)realloc(arr, (array.size() + 1) * sizeof(double));
            for (int i = 0; i < array.size(); i++)
            {
                QJsonValue v = array[i].toObject().value("trade_num");
                arr[i] = v.toString().toDouble();
            }
            //最大值定义
            double _max = arr[0];
            for (int i = 0; i < array.size(); i++)
            {
                if (arr[i] > _max)
                {
                    _max = arr[i];
                }
            }
            free(arr);
            return _max;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
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
