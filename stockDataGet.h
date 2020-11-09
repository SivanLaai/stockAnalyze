#ifndef STOCKDATAGET_H
#define STOCKDATAGET_H

#include <QObject>

class stockDataGet : public QObject
{
    Q_OBJECT
public:
    explicit stockDataGet(QObject *parent = 0);
    //处理至showapi_res_body
    static QJsonValue stockKLine(QString str);//股票实时K线数据
    static QJsonValue stockIndexKLine(QString str);//股指实时K线数据
    static QJsonValue stockInfoReal(QString str);//股票实时数据
    static  QJsonValue stockInfoBatchReal(QString str);//股票实时行情批量
    static QJsonValue stockList(QString str);//股票列表查询
    static  QJsonValue stockSzShHistory(QString str);//沪深及港股历史行情
    static QJsonValue stockTimeLine(QString str);//股票实时分时线数据
    static QJsonValue stockIndexBatch(QString str);//股指实时行情批量
    static QJsonValue stockIndexTimeLine(QString str);//股指实时分时线
    static QJsonValue stockIndexSearch(QString str);//股指列表查询
    static QJsonValue stockFromName(QString str);//名称编码拼音查询股票信息
    static QJsonValue stockEveryTrade(QString str);//沪深股票最新50条逐笔交易
    static QJsonValue stockKIndexData(QString str);//K线指标数据

signals:

public slots:
};

#endif // STOCKDATAGET_H
