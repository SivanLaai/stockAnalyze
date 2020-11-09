#ifndef QWEBDATA_H
#define QWEBDATA_H

#include <QObject>
#include <QXMLStreamReader>
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QRect>
#include "xtp_trader_api.h"
#include <string>
#include <map>
#include <iostream>
#include <QtNetwork>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#include "trade_spi.h"
#include "FileUtils.h"
#include "xtp_quote_api.h"
#include "quote_spi.h"

const QString HOST= QString("http://stock.market.alicloudapi.com/");

struct MemoryStruct {
  char *memory;
  size_t size;
};

class QWebData : public QObject
{
    Q_OBJECT
//public:
//    QString answer;
public:
    explicit QWebData(QObject *parent = 0);
    virtual QString getResponse() = 0;
    void getPostSlot(QNetworkReply *reply);
protected:
    QString getByPost(const char* WebSite,const char* Post);
private:
     QNetworkAccessManager manager;
     QString answer;
};


//------------------------QWebTData-----------------------------查询某个股票详细信息包括信息有交易量、成交额等
class QWebTData : public QWebData
{
public:
    QString code;
public:
    QWebTData(QString code);
    QString getResponse();
};
//------------------------QWebTimeLine-----------------------------查询股票的分时数据 包括交易量
class QWebTimeLine : public QWebData
{
public:
    QString code;
    QString day;
public:
    QWebTimeLine(QString code,QString day);
    QString getResponse();
};

//------------------------QWebKLine-----------------------------查询某只股票的K线数据（含有RSI 和 MACD数据）
class QWebKLine : public QWebData
{
public:
    QString code; // 股票代码
    QString begin; // 开始日期
    QString end; // 结束日期
public:
    QWebKLine(QString code,QString begin,QString end):code(code),begin(begin),end(end) {};
    QString getResponse();//获取K线相关数据
};

//------------------------QWebStockIndexData-----------------------------股指详细数据包括信息有交易量、成交额等
class QWebStockIndexData : public QWebData
{
public:
    QString code;
public:
    QWebStockIndexData(QString code);
    QString getResponse();
};
//------------------------QWebStockTimeLine-----------------------------股指实时分时数据 包括交易量
class QWebStockTimeLine : public QWebData
{
public:
    QString code;
    QString day;
public:
    QWebStockTimeLine(QString code,QString day);
    QString getResponse();
};
//------------------------QWebStockIndexKLine-----------------------------股指K线数据（含有RSI和MACD数据）
class QWebStockIndexKLine : public QWebData
{
public:
    QString code;
    QString time;
    QString beginDay;
public:
    QWebStockIndexKLine(QString code,QString time,QString beginDay);
    QString getResponse();
};
//------------------------QWebLogin-----------------------------登录验证
class QWebLogin : public QWebData
{
public:
    QString username;
    QString password;
public:
    QWebLogin(QString username,QString password);
    QString getResponse();
    bool isLogin();//返回标致，登录成功为真，失败为假，默认返回假
};
//--------------------QWebOptionalStock-----------------------处理自选股
class QWebOptionalStock : public QWebData
{
private:
    QString userName;
public:

    QWebOptionalStock();
    void setUserName(QString userName);
    QString getResponse();//根据用户名得到自选股列表信息
    QString getCodeMarket(QString code);//根据股票code得到相关信息
    bool deleteOptionalStock(QString code);//删除自选股
    bool addOptionalStock(QString code);//添加自选股
};

/*
 * author: laixinhua
 * description:
*/
class QWebRealTimeStock : public QWebData
{
private:
    QString userName;
public:
    QWebRealTimeStock() {

    }
    int test();
};



#endif // QWEBDATA_H
