#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QJsonArray>
const static char *lower_price = "min_price";   // 最低价
const static char *high_price = "max_price";    // 最高价
const static char *open_price = "open_price";   // 开盘价
const static char *close_price = "close_price"; // 收盘价
const static char *volume = "trade_num";        // 成交量
const static char *data = "list";               // 接口数据格式
const static char *stock_code = "code";         // 股票代码
const static char *stock_name = "name";         // 股票名称
const static char *yest_close = "yest_close";   // 昨天收盘价
const static char *nowPrice = "nowPrice";       // 昨天收盘价
const static char *avgPrice = "avgPrice";       // 昨天收盘价

class painter : public QObject
{
    Q_OBJECT
public:
    explicit painter(QObject *parent = 0);
    static void painteKLine(QJsonValue jsonvalue, QWidget *widget);    //绘制K线图(JSON数据,窗口)
    static void painteRealLine(QJsonValue jsonvalue, QWidget *widget); //绘制分时图
    static void painteInfo(QJsonValue jsonvalue, QWidget *widget);     //绘制股票信息
    static void painteKMACDIndexLine(QJsonValue jsonvalue, QWidget *widget);
    static void painteKRSIIndexLine(QJsonValue jsonvalue, QWidget *widget);

signals:

public slots:
};

#endif // PAINTER_H
