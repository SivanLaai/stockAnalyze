#include "stockDataGet.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
stockDataGet::stockDataGet(QObject *parent) : QObject(parent)
{

}


//股票实时K线数据
QJsonValue stockDataGet::stockKLine(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;

}
//股指实时K线数据
QJsonValue stockDataGet::stockIndexKLine(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股票实时数据
QJsonValue stockDataGet::stockInfoReal(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股票实时行情批量
QJsonValue stockDataGet::stockInfoBatchReal(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股票列表查询
QJsonValue stockDataGet::stockList(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股票实时分时线数据
QJsonValue stockDataGet::stockTimeLine(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股指实时行情批量
QJsonValue stockDataGet::stockIndexBatch(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股指实时分时线
QJsonValue stockDataGet::stockIndexTimeLine(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//股指列表查询
QJsonValue stockDataGet::stockIndexSearch(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//名称编码拼音查询股票信息
QJsonValue stockDataGet::stockFromName(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//沪深股票最新50条逐笔交易
QJsonValue stockDataGet::stockEveryTrade(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("showapi_res_body"));
    return jsonvalue;
}
//K线指标数据
QJsonValue stockDataGet::stockKIndexData(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue jsonvalue = obj.value(("dataList"));
    return jsonvalue;
}
