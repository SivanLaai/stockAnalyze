#ifndef WEBGETS_H
#define WEBGETS_H

#include <QObject>
//#include "curl.h"
//#include "easy.h"
#include <QDebug>

class WebGets : public QObject
{
    Q_OBJECT
public:
    struct MemoryStruct {
      char *memory;
      size_t size;
    };

signals:

public slots:

public:
    QString Host;
    QString Appcode;
    struct MemoryStruct data;


public:
    WebGets();
    void setHost(QString urlhost);
    void setAppcode(QString appcode);
    virtual QString getResponse()=0;
    QString getByHostHeader(QString Host,QString Header);
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
};


//------------------------------WebStockList----------------------------------------股票列表查询
class WebStockList:public WebGets
{
public:
    QString market;
    QString page;
public:
    WebStockList(QString market,QString page);
    QString getResponse();
};
//------------------------------WebStockHistory----------------------------------------沪深及港股历史行情
class WebStockHistory:public WebGets
{
public:
    QString begin;
    QString code;
    QString end;
public:
    WebStockHistory(QString begin,QString code,QString end);
    QString getResponse();
};

//------------------------------WebBatchRealStockinfo----------------------------------------股票行情_批量

class WebBatchRealStockinfo:public WebGets
{
public:
	QString needIndex;
	QString stocks;
public:
	WebBatchRealStockinfo(QString needIndex,QString stocks);
        QString getResponse();
};

//------------------------------WebRealStockinfo----------------------------------------股票行情
class WebRealStockinfo:public WebGets
{
public:
	QString code;
	QString needIndex;
	QString need_k_pic;
public:
	WebRealStockinfo(QString code,QString needIndex,QString need_k_pic);
        QString getResponse();
};
//------------------------------WebTimeLine----------------------------------------股票实时分时线数据
class WebTimeLine:public WebGets
{
public:
    QString code;
    QString day;
public:
    WebTimeLine(QString code,QString day);
    QString getResponse();
};
//------------------------------WebRealtimeK----------------------------------------股票K线数据
class WebRealtimeK:public WebGets
{
public:
    QString beginDay;
    QString code;
    QString time;
    QString type;
public:
    WebRealtimeK(QString beginDay,QString code,QString time="day",QString type="bfq");
    QString getResponse();
};
//------------------------------WebStockIndex----------------------------------------大盘股指行情_批量
class WebStockIndex:public WebGets
{
public:
    QString stocks;
public:
    WebStockIndex(QString stocks);
    QString getResponse();
};
//------------------------------WebIndexTimeline----------------------------------------大盘股指分时线
class WebIndexTimeline:public WebGets
{
public:
    QString code;
    QString day;
public:
    WebIndexTimeline(QString code,QString day="1");
    QString getResponse();
};
//------------------------------WebIndexKline----------------------------------------大盘股指K线数据
class WebIndexKline:public WebGets
{
public:
    QString beginDay;
    QString code;
    QString time;
public:
    WebIndexKline(QString beginDay,QString code,QString time="day");
    QString getResponse();
};
//------------------------------WebStockIndexSearch----------------------------------------大盘股指列表查询
class WebStockIndexSearch:public WebGets
{
public:
    QString market;
    QString page;
public:
    WebStockIndexSearch(QString market,QString page);
    QString getResponse();
};
//------------------------------WebStockInfoByName----------------------------------------名称编码拼音查询股票信息
class WebStockInfoByName:public WebGets
{
public:
    QString code;
    QString name;
    QString pinyin;
public:
    WebStockInfoByName(QString code,QString name,QString pinyin);
    QString getResponse();
};
//------------------------------WebStockEverytrade----------------------------------------沪深股票最新50条逐笔交易
class WebStockEverytrade:public WebGets
{
public:
    QString code;
public:
    WebStockEverytrade(QString code);
    QString getResponse();
};
//------------------------------WebStockInBlock----------------------------------------查询板块中的股票列表
class WebStockInBlock:public WebGets
{
public:
    QString page;
    QString typeId;
public:
    WebStockInBlock(QString page,QString typeId);
    QString getResponse();
};
//------------------------------WebStockBlockList----------------------------------------股票板块列表
class WebStockBlockList:public WebGets
{
public:
    WebStockBlockList();
    QString getResponse();
};
//------------------------------WebStopStartDivide----------------------------------------当日除权停复牌上市股票
class WebStopStartDivide:public WebGets
{
public:
    QString date;
public:
    WebStopStartDivide(QString date);
    QString getResponse();
};
//------------------------------WebIndexDayHis----------------------------------------大盘历史查询
class WebIndexDayHis:public WebGets
{
public:
    QString code;
    QString month;
public:
    WebIndexDayHis(QString code,QString month);
    QString getResponse();
};
#endif // WEBGETS_H
