#include "qwebdata.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <QGridLayout>
#include <QDebug>


QWebData::QWebData(QObject *parent) : QObject(parent)
{

}

void QWebData::getPostSlot(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    answer = reply->readAll();
}

QString QWebData::getByPost(const char* WebSite,const char* Post)
{
    qDebug() << QString(WebSite) + "?" + QString(Post);
    QEventLoop loop; // request的处理是异步处理，所以需要进行同步处理，并做超时处理
    QNetworkRequest request;
    request.setUrl(QUrl(QString(WebSite) + "?" + QString(Post)));
    request.setRawHeader(QByteArray("Authorization"), QByteArray("APPCODE a686fc9dc45b405fba6d89e331d70a6f"));
    manager.get(request);
    QObject::connect(&manager, &QNetworkAccessManager::finished,
                     this, &QWebData::getPostSlot);
    QObject::connect(&manager, &QNetworkAccessManager::finished,
                     &loop, &QEventLoop::quit);
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);
    loop.exec();
    qDebug() << answer;
    return answer;
}
//------------------------QWebTData-----------------------------查询某个股票详细信息包括信息有交易量、成交额等
QWebTData::QWebTData(QString code)
{
    this->code = code;
}

QString QWebTData::getResponse()
{
    QString WebSite = QString("%1stockAction!queryTdata.action").arg(HOST);
    QString Post = QString("code=%1").arg(code);
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());
}
//------------------------QWebTimeLine-----------------------------查询股票的分时数据 包括交易量
QWebTimeLine::QWebTimeLine(QString code,QString day)
{
    this->code = code;
    this->day = day;
}

QString QWebTimeLine::getResponse()
{
    QString WebSite = QString("%1stockAction!queryKlinetest.action").arg(HOST);
    QString Post = QString("code=%1&day=%2").arg(code).arg(day);
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());
}
//------------------------QWebKLine-----------------------------查询某只股票的K线数据（含有RSI 和 MACD数据）

QString QWebKLine::getResponse()
{
    QString WebSite = QString("%1sz-sh-stock-history").arg(HOST);
    QString Post = QString("begin=%1&code=%2&end=%3").arg(begin).arg(code).arg(end);
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());//未格式化的Json字符
}


//------------------------QWebStockIndexData-----------------------------股指详细数据包括信息有交易量、成交额等
QWebStockIndexData::QWebStockIndexData(QString code)
{
    this->code = code;
}

QString QWebStockIndexData::getResponse()
{
    QString WebSite = QString("%1stockAction!queryStockIndexdata.action").arg(HOST);
    QString Post = QString("code=%1").arg(code);
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());
}
//------------------------QWebStockTimeLine-----------------------------股指实时分时数据 包括交易量
QWebStockTimeLine::QWebStockTimeLine(QString code,QString day)
{
    this->code = code;
    this->day = day;
}

QString QWebStockTimeLine::getResponse()
{
    QString WebSite = QString("%1stockAction!queryStockTline.action").arg(HOST);
    QString Post = QString("code=%1&day=%2").arg(code).arg(day);
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());
}
//------------------------QWebStockIndexKLine-----------------------------股指K线数据（含有RSI和MACD数据）
QWebStockIndexKLine::QWebStockIndexKLine(QString code,QString time,QString beginDay)
{
    this->code = code;
    this->time = time;
    this->beginDay = beginDay;
}

QString QWebStockIndexKLine::getResponse()
{
    QString WebSite = QString("%1stockAction!queryStockIndexKline.action").arg(HOST);
    QString Post = QString("code=%1&time=%2&beginDay=19920101").arg(code).arg(time);
    QString responseStr = getByPost(WebSite.toStdString().data(),Post.toStdString().data());//未格式化的Json字符

    QJsonParseError jsonError;//
    QJsonDocument json = QJsonDocument::fromJson(responseStr.toUtf8(), &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
    if(json.isObject())
    {
        QJsonObject obj = json.object();
        QJsonObject newObj;//用来存放新的Json字符串

        //获取从Beginday已经计算的天数
        Post = QString("code=%1&time=%2&beginDay=%3").arg(code).arg(time).arg(beginDay);
        responseStr = getByPost(WebSite.toStdString().data(),Post.toStdString().data());
        QJsonDocument jsonCount = QJsonDocument::fromJson(responseStr.toUtf8(), &jsonError);
        QJsonObject objCount = jsonCount.object();
        int count = objCount["count"].toString().toInt();

        //处理Json字符
        QJsonObject dataObj;
        QJsonArray dataArr;
        int size = obj["dataList"].toArray().size();
        for(int i=size-count,pos=0; i<size; i++,pos++)
        {
            QString rsiData6 = obj["rsiData6"].toArray().at(size-1-pos).toString();
            QString rsiData12 = obj["rsiData12"].toArray().at(size-1-pos).toString();
            QString rsiData24 = obj["rsiData24"].toArray().at(size-1-pos).toString();
            QString DIFF = obj["DIFF"].toArray().at(size-1-pos).toString();
            QString DEA = obj["DEA"].toArray().at(size-1-pos).toString();
            QString MACD = obj["MACD"].toArray().at(size-1-pos).toString();

            //向Json对象添加内容
            dataObj = obj["dataList"].toArray().at(i).toObject();
            dataObj.insert("rsiData6",rsiData6);
            dataObj.insert("rsiData12",rsiData12);
            dataObj.insert("rsiData24",rsiData24);
            dataObj.insert("DIFF",DIFF);
            dataObj.insert("DEA",DEA);
            dataObj.insert("MACD",MACD);
            //向Rsi的Json数组添加Json对象
            dataArr.insert(pos,dataObj);

        }

        //添加对象
        newObj.insert("dataList",dataArr);
        newObj.insert("count",obj["count"].toString());
        newObj.insert("name",obj["name"].toString());
        newObj.insert("market",obj["market"].toString());
        newObj.insert("code",obj["code"].toString());


        return QString(QJsonDocument(newObj).toJson());
    }
    }
    return "failed to obtain data! Try Aagin";

}
//------------------------QWebLogin-----------------------------登录验证
QWebLogin::QWebLogin(QString username,QString password)
{
    this->username = username;
    this->password = password;
}

QString QWebLogin::getResponse()
{
    QString WebSite = QString("%1user/userAction_login").arg(HOST);
    QString Post = QString("user.username=%1&user.password=%2").arg(username).arg(password);
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());
}
bool QWebLogin::isLogin()
{
    QXmlStreamReader xml(getResponse());
    while (!xml.atEnd())
    {
          QString dot = xml.name().toString();
          if(QString::compare(dot,QStringLiteral("title"))==0)
              if(QString::compare(xml.readElementText(),QStringLiteral("H-ui.admin v3.0"))==0)
                  return true;
          xml.readNext();
    }
    return true;
}
//--------------------QWebOptionalStock-----------------------处理自选股
QWebOptionalStock::QWebOptionalStock()
{
    getCodeMarket("600004");
}
void QWebOptionalStock::setUserName(QString userName)
{
    this->userName = userName;
}
QString QWebOptionalStock::getCodeMarket(QString code)//根据code得到股票信息
{
    //拿到股票之后查询每只股票的相关K线信息
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd");
    QWebData *pWeb = new QWebKLine(code, QString("1001-01-01"), timeStr);

    pWeb = new QWebTData(code);
    QString stockStr = pWeb->getResponse();
    //要返回的数据
    QString codeMarket;
    //Json解析接口数据
    QJsonParseError jsonError;//
    QJsonDocument json = QJsonDocument::fromJson(stockStr.toUtf8(), &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
        if(json.isObject())
        {
            QJsonObject obj = json.object();
            codeMarket = obj["stockMarket"].toObject()["market"].toString();
        }
    }
    return codeMarket;
}

QString QWebOptionalStock::getResponse()//根据用户名得到自选股列表信息
{
    QString WebSite = QString("%1userStockAction!queryStockList.action").arg(HOST);
    QString Post = QString("user=%1&type=%2").arg(userName).arg("qt");
    return getByPost(WebSite.toStdString().data(),Post.toStdString().data());
}

bool QWebOptionalStock::deleteOptionalStock(QString code)//删除自选股
{
    QString codeMarket = getCodeMarket(code);
    QString WebSite = QString("%1userStockAction!updateStock.action").arg(HOST);
    QString Post = QString("flag=del&user=%1&code=%2%3").arg(userName).arg(codeMarket).arg(code);
    //
    bool flag = (getByPost(WebSite.toStdString().data(),Post.toStdString().data())==QString("已删除"));

    return flag;
}

bool QWebOptionalStock::addOptionalStock(QString code)//添加自选股
{
    QString codeMarket = getCodeMarket(code);
    QString WebSite = QString("%1userStockAction!updateStock.action").arg(HOST);
    QString Post = QString("flag=add&user=%1&code=%2%3").arg(userName).arg(codeMarket).arg(code);
    bool flag = (getByPost(WebSite.toStdString().data(),Post.toStdString().data())==QString("success"));

    return flag;
}


XTP::API::TraderApi *pUserApi;
bool is_connected_ = false;
std::string trade_server_ip;
int trade_server_port;
uint64_t session_id_ = 0;
std::map<uint64_t, uint64_t> map_session;
uint64_t *session_arrary = NULL;
FileUtils *fileUtils = new FileUtils();
XTPOrderInsertInfo *orderList = NULL;
std::string quote_server_ip;
int quote_server_port;
std::string quote_username;
std::string quote_password;
XTP_PROTOCOL_TYPE quote_protocol = XTP_PROTOCOL_UDP;
int QWebRealTimeStock::test()
{
    FileUtils *fileUtils = new FileUtils();
    if (!fileUtils->init())
    {
        std::cout << "The config.json file parse error." << std::endl;
#ifdef _WIN32
        system("pause");
#endif

        return 0;
    }

    //读取交易配置
    trade_server_ip = fileUtils->stdStringForKey("trade_ip");
    trade_server_port = fileUtils->intForKey("trade_port");
    int out_count = fileUtils->intForKey("out_count");
    bool auto_save = fileUtils->boolForKey("auto_save");
    int client_id = fileUtils->intForKey("client_id");
    int account_count = fileUtils->countForKey("account");
    int resume_type = fileUtils->intForKey("resume_type");
    std::string account_key = fileUtils->stdStringForKey("account_key");
#ifdef _WIN32
    std::string filepath = fileUtils->stdStringForKey("path");
#else
    std::string filepath = fileUtils->stdStringForKey("path_linux");
#endif // _WIN32

    //读取行情配置
    quote_server_ip = fileUtils->stdStringForKey("quote_ip");
    quote_server_port = fileUtils->intForKey("quote_port");
    quote_username = fileUtils->stdStringForKey("quote_user");
    quote_password = fileUtils->stdStringForKey("quote_password");
    quote_protocol = (XTP_PROTOCOL_TYPE)(fileUtils->intForKey("quote_protocol"));
    int32_t quote_buffer_size = fileUtils->intForKey("quote_buffer_size");

    //读取心跳超时配置
    int32_t heat_beat_interval = fileUtils->intForKey("hb_interval");

    //初始化行情api
    XTP::API::QuoteApi *pQuoteApi = XTP::API::QuoteApi::CreateQuoteApi(client_id, filepath.c_str(), XTP_LOG_LEVEL_DEBUG); //log日志级别可以调整
    MyQuoteSpi *pQuoteSpi = new MyQuoteSpi();
    pQuoteApi->RegisterSpi(pQuoteSpi);

    //设定行情服务器超时时间，单位为秒
    pQuoteApi->SetHeartBeatInterval(heat_beat_interval); //此为1.1.16新增接口
    //设定行情本地缓存大小，单位为MB
    pQuoteApi->SetUDPBufferSize(quote_buffer_size); //此为1.1.16新增接口

    int loginResult_quote = -1;
    //登录行情服务器,自1.1.16开始，行情服务器支持UDP连接，推荐使用UDP
    loginResult_quote = pQuoteApi->Login(quote_server_ip.c_str(), quote_server_port, quote_username.c_str(), quote_password.c_str(), quote_protocol);
    if (loginResult_quote == 0)
    {
        //登录行情服务器成功后，订阅行情
        int instrument_count = fileUtils->countForKey("quote_ticker.instrument");
        int quote_exchange = fileUtils->intForKey("quote_ticker.exchange");

        //从配置文件中读取需要订阅的股票
        char **allInstruments = new char *[instrument_count];
        for (int i = 0; i < instrument_count; i++)
        {
            allInstruments[i] = new char[7];
            std::string instrument = fileUtils->stdStringForKey("quote_ticker.instrument[%d]", i);
            strcpy(allInstruments[i], instrument.c_str());
        }

        //开始订阅,注意公网测试环境仅支持TCP方式，如果使用UDP方式会没有行情数据，实盘大多数使用UDP连接
        pQuoteApi->SubscribeMarketData(allInstruments, instrument_count, (XTP_EXCHANGE_TYPE)quote_exchange);
//        pQuoteApi->QueryAllTickers((XTP_EXCHANGE_TYPE)quote_exchange);
//        pQuoteApi->QueryAllTickersFullInfo((XTP_EXCHANGE_TYPE)quote_exchange);
        pQuoteApi->SubscribeAllOrderBook((XTP_EXCHANGE_TYPE)quote_exchange);

        //释放
        for (int i = 0; i < instrument_count; i++)
        {
            delete[] allInstruments[i];
            allInstruments[i] = NULL;
        }

        delete[] allInstruments;
        allInstruments = NULL;
    }
    else
    {
        //登录失败，获取失败原因
        XTPRI *error_info = pQuoteApi->GetApiLastError();
        std::cout << "Login to server error, " << error_info->error_id << " : " << error_info->error_msg << std::endl;
    }

    if (account_count > 0)
    {
        //针对多用户的情况
        orderList = new XTPOrderInsertInfo[account_count];
        memset(orderList, 0, sizeof(XTPOrderInsertInfo) * account_count);
    }

    //初始化交易类Api
    pUserApi = XTP::API::TraderApi::CreateTraderApi(client_id, filepath.c_str(), XTP_LOG_LEVEL_DEBUG); // 创建UserApi，log日志级别可以调整
    pUserApi->SubscribePublicTopic((XTP_TE_RESUME_TYPE)resume_type);
    pUserApi->SetSoftwareVersion("1.1.0");				//设定此软件的开发版本号，用户自定义
    pUserApi->SetSoftwareKey(account_key.c_str());		//设定用户的开发代码，在XTP申请开户时，由xtp人员提供
    pUserApi->SetHeartBeatInterval(heat_beat_interval); //设定交易服务器超时时间，单位为秒，此为1.1.16新增接口
    MyTraderSpi *pUserSpi = new MyTraderSpi();
    pUserApi->RegisterSpi(pUserSpi); // 注册事件类
    pUserSpi->setUserAPI(pUserApi);
    pUserSpi->set_save_to_file(auto_save);
    if (out_count > 0)
    {
        pUserSpi->OutCount(out_count);
    }
    else
    {
        out_count = 1;
    }

    if (account_count > 0)
    {
        //多用户时，用session数组来管理用户session_id
        session_arrary = new uint64_t[account_count];

        for (int i = 0; i < account_count; i++)
        {
            //从配置文件中读取第i个用户登录信息
            std::string account_name = fileUtils->stdStringForKey("account[%d].user", i);
            std::string account_pw = fileUtils->stdStringForKey("account[%d].password", i);

            uint64_t temp_session_ = 0;
            std::cout << account_name << " login begin." << std::endl;
            temp_session_ = pUserApi->Login(trade_server_ip.c_str(), trade_server_port, account_name.c_str(), account_pw.c_str(), XTP_PROTOCOL_TCP); //登录交易服务器

            if (session_id_ == 0)
            {
                session_id_ = temp_session_;
            }

            if (temp_session_ > 0)
            {
                //登录成功后，建立session_id与用户之间的映射关系
                map_session.insert(std::make_pair(temp_session_, i));
            }
            else
            {
                //登录失败，获取登录失败原因
                XTPRI *error_info = pUserApi->GetApiLastError();
                std::cout << account_name << " login to server error, " << error_info->error_id << " : " << error_info->error_msg << std::endl;
            }

            session_arrary[i] = temp_session_;
        }
    }

    delete fileUtils;
    fileUtils = NULL;

    if (orderList)
    {
        delete[] orderList;
        orderList = NULL;
    }

    if (session_arrary)
    {
        delete[] session_arrary;
        session_arrary = NULL;
    }

    return 0;
}


