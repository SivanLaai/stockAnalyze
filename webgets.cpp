#include "webgets.h"

QString HOST=QString("http://stock.market.alicloudapi.com");
QString APPCODE=QString("a686fc9dc45b405fba6d89e331d70a6f");

size_t WebGets::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
      /* 内存分配失败*/
      printf("没有足够的内存(realloc returned NULL)\n");
      return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

WebGets::WebGets()
{
    this->Appcode = APPCODE;
    this->Host = HOST;
}
QString WebGets::getByHostHeader(QString Host,QString Header)
{
//    CURLcode res;
//    data.memory = (char*)malloc(1);
//    data.size = 0;

//    curl = curl_easy_init();
//    if(curl) {
//        struct curl_slist *head = NULL;
//        head = curl_slist_append(head, Header.toStdString().data());
//        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);

//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&data);
//        curl_easy_setopt(curl, CURLOPT_URL, Host.toStdString().data());
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

//        res = curl_easy_perform(curl);
//        if(res != CURLE_OK)
//            qDebug()<<"curl_easy_perform() 失败: "
//                   <<QString(curl_easy_strerror(res));

//        curl_easy_cleanup(curl);
//        curl_slist_free_all(head);
//        return data.memory;
//      }
        return "获取信息失败";

}

//------------------------------WebStockList----------------------------------------股票列表查询

WebStockList::WebStockList(QString market,QString page)
{
    this->market = market;
    this->page = page;
}

QString WebStockList::getResponse()
{
    QString Http = QString("%1/stocklist?market=%2&page=%3").arg(Host).arg(market).arg(page);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebStockHistory----------------------------------------沪深及港股历史行情

WebStockHistory::WebStockHistory(QString begin,QString code,QString end)
{
    this->begin = begin;
    this->code = code;
    this->end = end;
}

QString WebStockHistory::getResponse()
{
    QString Http = QString("%1/sz-sh-stock-history?begin=%2&code=%3&end=%4").arg(Host).arg(begin).arg(code).arg(end);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}


//------------------------------WebBatchRealStockinfo----------------------------------------股票行情_批量
WebBatchRealStockinfo::WebBatchRealStockinfo(QString needIndex,QString stocks)
{
	this->needIndex = needIndex;
	this->stocks = stocks;
}
QString WebBatchRealStockinfo::getResponse()
{
	QString Http = QString("%1/batch-real-stockinfo?needIndex=%2&stocks=%3").arg(Host).arg(needIndex).arg(stocks);
	QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebRealStockinfo----------------------------------------股票行情
WebRealStockinfo::WebRealStockinfo(QString code,QString needIndex,QString need_k_pic)
{
	this->code = code;
	this->needIndex = needIndex;
	this->need_k_pic = need_k_pic;
}
QString WebRealStockinfo::getResponse()
{
	QString Http = QString("%1/real-stockinfo?code=%2&needIndex=%3&need_k_pic=%4").arg(Host).arg(code).arg(needIndex).arg(need_k_pic);
	QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebTimeLine----------------------------------------股票实时分时线数据
WebTimeLine::WebTimeLine(QString code,QString day)
{
    this->code = code;
    this->day = day;
}

QString WebTimeLine::getResponse()
{
    QString Http = QString("%1/timeline?code=%2&day=%3").arg(Host).arg(code).arg(day);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}

//------------------------------WebRealtimeK----------------------------------------股票K线数据
WebRealtimeK::WebRealtimeK(QString beginDay,QString code,QString time,QString type)
{
    this->beginDay = beginDay;
    this->code = code;
    this->time = time;
    this->type = type;
}

QString WebRealtimeK::getResponse()
{
    QString Http = QString("%1/realtime-k?beginDay=%2&code=%3&time=%4&type=%5").arg(Host).arg(beginDay).arg(code).arg(time).arg(type);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}

//------------------------------WebStockIndex----------------------------------------大盘股指行情_批量
WebStockIndex::WebStockIndex(QString stocks)
{
    this->stocks = stocks;
}

QString WebStockIndex::getResponse()
{
    QString Http = QString("%1/stockIndex?stocks=%2").arg(Host).arg(stocks);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}

//------------------------------WebIndexTimeline----------------------------------------大盘股指分时线
WebIndexTimeline::WebIndexTimeline(QString code,QString day)
{
    this->code = code;
    this->day = day;
}

QString WebIndexTimeline::getResponse()
{
    QString Http = QString("%1/index-timeline?code=%2&day=%3").arg(Host).arg(code).arg(day);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebIndexKline----------------------------------------大盘股指K线数据
WebIndexKline::WebIndexKline(QString beginDay,QString code,QString time)
{
    this->beginDay = beginDay;
    this->code = code;
    this->time = time;
}

QString WebIndexKline::getResponse()
{
    QString Http = QString("%1/index-kline?beginDay=%2&code=%3&time=%4").arg(Host).arg(beginDay).arg(code).arg(time);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}

//------------------------------WebStockIndexSearch----------------------------------------大盘股指列表查询
WebStockIndexSearch::WebStockIndexSearch(QString market,QString page)
{
    this->market = market;
    this->page = page;
}

QString WebStockIndexSearch::getResponse()
{
    QString Http = QString("%1/stockindexsearch?market=%2&page=%3").arg(Host).arg(market).arg(page);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}

//------------------------------WebStockInfoByName----------------------------------------名称编码拼音查询股票信息
WebStockInfoByName::WebStockInfoByName(QString code,QString name,QString pinyin)
{
    this->code = code;
    this->name = name;
    this->pinyin = pinyin;

}

QString WebStockInfoByName::getResponse()
{

    QString Http = QString("%1/name-to-stockinfo?code=%2&name=%3&pinyin=%4").arg(Host).arg(code).arg(name).arg(pinyin);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}

//------------------------------WebStockEverytrade----------------------------------------沪深股票最新50条逐笔交易
WebStockEverytrade::WebStockEverytrade(QString code)
{
    this->code = code;

}

QString WebStockEverytrade::getResponse()
{
    QString Http = QString("%1/everytrade?code=%2").arg(Host).arg(code);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebStockInBlock----------------------------------------查询板块中的股票列表
WebStockInBlock::WebStockInBlock(QString page,QString typeId)
{
    this->page = page;
    this->typeId = typeId;
}
QString WebStockInBlock::getResponse()
{
    QString Http = QString("%1/stock-in-block?page=%2&typeId=%3").arg(Host).arg(page).arg(typeId);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebStockBlockList----------------------------------------股票板块列表
WebStockBlockList::WebStockBlockList()
{

}

QString WebStockBlockList::getResponse()
{
    QString Http = QString("%1/stock-block-list").arg(Host);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebStopStartDivide----------------------------------------当日除权停复牌上市股票
WebStopStartDivide::WebStopStartDivide(QString date)
{
    this->date = date;
}

QString WebStopStartDivide::getResponse()
{
    QString Http = QString("%1/stop-start-divide?date=%2").arg(Host).arg(date);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
//------------------------------WebIndexDayHis----------------------------------------大盘历史查询
WebIndexDayHis::WebIndexDayHis(QString code,QString month)
{
    this->code = code;
    this->month = month;
}

QString WebIndexDayHis::getResponse()
{
    QString Http = QString("%1/indexDayHis?code=%2&month=%3").arg(Host).arg(code).arg(month);
    QString Header = QString("Authorization:APPCODE %1").arg(Appcode);
    return getByHostHeader(Http,Header);
}
