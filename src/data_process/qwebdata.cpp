#include "qwebdata.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <QGridLayout>
#include <QDebug>
#include "real_kline_work.h"


QWebData::QWebData(QObject *parent) : QObject(parent)
{
}

void QWebData::getPostSlot(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << reply->errorString();
        return;
    }

    answer = reply->readAll();
}

QString QWebData::getByPost(const char *WebSite, const char *Post)
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
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data());
}
//------------------------QWebTimeLine-----------------------------查询股票的分时数据 包括交易量
QWebTimeLine::QWebTimeLine(QString code, QString day)
{
    this->code = code;
    this->day = day;
}

QString QWebTimeLine::getResponse()
{
    QString WebSite = QString("%1stockAction!queryKlinetest.action").arg(HOST);
    QString Post = QString("code=%1&day=%2").arg(code).arg(day);
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data());
}
//------------------------QWebKLine-----------------------------查询某只股票的K线数据（含有RSI 和 MACD数据）

QString QWebKLine::getResponse()
{
    QString WebSite = QString("%1sz-sh-stock-history").arg(HOST);
    QString Post = QString("begin=%1&code=%2&end=%3").arg(begin).arg(code).arg(end);
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data()); //未格式化的Json字符
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
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data());
}
//------------------------QWebStockTimeLine-----------------------------股指实时分时数据 包括交易量
QWebStockTimeLine::QWebStockTimeLine(QString code, QString day)
{
    this->code = code;
    this->day = day;
}

QString QWebStockTimeLine::getResponse()
{
    QString WebSite = QString("%1stockAction!queryStockTline.action").arg(HOST);
    QString Post = QString("code=%1&day=%2").arg(code).arg(day);
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data());
}
//------------------------QWebStockIndexKLine-----------------------------股指K线数据（含有RSI和MACD数据）
QWebStockIndexKLine::QWebStockIndexKLine(QString code, QString time, QString beginDay)
{
    this->code = code;
    this->time = time;
    this->beginDay = beginDay;
}

QString QWebStockIndexKLine::getResponse()
{
    QString WebSite = QString("%1stockAction!queryStockIndexKline.action").arg(HOST);
    QString Post = QString("code=%1&time=%2&beginDay=19920101").arg(code).arg(time);
    QString responseStr = getByPost(WebSite.toStdString().data(), Post.toStdString().data()); //未格式化的Json字符

    QJsonParseError jsonError; //
    QJsonDocument json = QJsonDocument::fromJson(responseStr.toUtf8(), &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject obj = json.object();
            QJsonObject newObj; //用来存放新的Json字符串

            //获取从Beginday已经计算的天数
            Post = QString("code=%1&time=%2&beginDay=%3").arg(code).arg(time).arg(beginDay);
            responseStr = getByPost(WebSite.toStdString().data(), Post.toStdString().data());
            QJsonDocument jsonCount = QJsonDocument::fromJson(responseStr.toUtf8(), &jsonError);
            QJsonObject objCount = jsonCount.object();
            int count = objCount["count"].toString().toInt();

            //处理Json字符
            QJsonObject dataObj;
            QJsonArray dataArr;
            int size = obj["dataList"].toArray().size();
            for (int i = size - count, pos = 0; i < size; i++, pos++)
            {
                QString rsiData6 = obj["rsiData6"].toArray().at(size - 1 - pos).toString();
                QString rsiData12 = obj["rsiData12"].toArray().at(size - 1 - pos).toString();
                QString rsiData24 = obj["rsiData24"].toArray().at(size - 1 - pos).toString();
                QString DIFF = obj["DIFF"].toArray().at(size - 1 - pos).toString();
                QString DEA = obj["DEA"].toArray().at(size - 1 - pos).toString();
                QString MACD = obj["MACD"].toArray().at(size - 1 - pos).toString();

                //向Json对象添加内容
                dataObj = obj["dataList"].toArray().at(i).toObject();
                dataObj.insert("rsiData6", rsiData6);
                dataObj.insert("rsiData12", rsiData12);
                dataObj.insert("rsiData24", rsiData24);
                dataObj.insert("DIFF", DIFF);
                dataObj.insert("DEA", DEA);
                dataObj.insert("MACD", MACD);
                //向Rsi的Json数组添加Json对象
                dataArr.insert(pos, dataObj);
            }

            //添加对象
            newObj.insert("dataList", dataArr);
            newObj.insert("count", obj["count"].toString());
            newObj.insert("name", obj["name"].toString());
            newObj.insert("market", obj["market"].toString());
            newObj.insert("code", obj["code"].toString());

            return QString(QJsonDocument(newObj).toJson());
        }
    }
    return "failed to obtain data! Try Aagin";
}
//------------------------QWebLogin-----------------------------登录验证
QWebLogin::QWebLogin(QString username, QString password)
{
    this->username = username;
    this->password = password;
}

QString QWebLogin::getResponse()
{
    QString WebSite = QString("%1user/userAction_login").arg(HOST);
    QString Post = QString("user.username=%1&user.password=%2").arg(username).arg(password);
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data());
}
bool QWebLogin::isLogin()
{
    QXmlStreamReader xml(getResponse());
    while (!xml.atEnd())
    {
        QString dot = xml.name().toString();
        if (QString::compare(dot, QStringLiteral("title")) == 0)
            if (QString::compare(xml.readElementText(), QStringLiteral("H-ui.admin v3.0")) == 0)
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
QString QWebOptionalStock::getCodeMarket(QString code) //根据code得到股票信息
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
    QJsonParseError jsonError; //
    QJsonDocument json = QJsonDocument::fromJson(stockStr.toUtf8(), &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject obj = json.object();
            codeMarket = obj["stockMarket"].toObject()["market"].toString();
        }
    }
    return codeMarket;
}

QString QWebOptionalStock::getResponse() //根据用户名得到自选股列表信息
{
    QString WebSite = QString("%1userStockAction!queryStockList.action").arg(HOST);
    QString Post = QString("user=%1&type=%2").arg(userName).arg("qt");
    return getByPost(WebSite.toStdString().data(), Post.toStdString().data());
}

bool QWebOptionalStock::deleteOptionalStock(QString code) //删除自选股
{
    QString codeMarket = getCodeMarket(code);
    QString WebSite = QString("%1userStockAction!updateStock.action").arg(HOST);
    QString Post = QString("flag=del&user=%1&code=%2%3").arg(userName).arg(codeMarket).arg(code);
    //
    bool flag = (getByPost(WebSite.toStdString().data(), Post.toStdString().data()) == QString("已删除"));

    return flag;
}

bool QWebOptionalStock::addOptionalStock(QString code) //添加自选股
{
    QString codeMarket = getCodeMarket(code);
    QString WebSite = QString("%1userStockAction!updateStock.action").arg(HOST);
    QString Post = QString("flag=add&user=%1&code=%2%3").arg(userName).arg(codeMarket).arg(code);
    bool flag = (getByPost(WebSite.toStdString().data(), Post.toStdString().data()) == QString("success"));

    return flag;
}

void QWebRealTimeStock::handleResults(QJsonObject realJsonObj)
{
    this->realJsonObj = realJsonObj;
    imageLineShow->string = getResponse();
}

void QWebRealTimeStock::makeThread(const QString &code)
{
    // 创建线程处理类
    RealKLineWork *worker = new RealKLineWork;
    worker->moveToThread(&workerThread);
    // 线程结束的时候，worker对像自动删除
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    // 触发operate，线程开始处理工作
    connect(this, &QWebRealTimeStock::operate, worker, &RealKLineWork::doWork);
    // 处理完结果后，开始处理结果
    connect(worker, &RealKLineWork::resultReady, this, &QWebRealTimeStock::handleResults);
    // 发出线程operate信号
    emit operate();
    // 线程运行
    workerThread.start();
}

QString QWebRealTimeStock::getResponse()
{
    QString str = QString(QJsonDocument(realJsonObj).toJson());
//    qDebug() << str;

    return str;
}

void QWebRealTimeStock::setString(image_line_show* imageLineShow)
{
    this->imageLineShow = imageLineShow;
}
QJsonObject QWebRealTimeStock::getRealTimePrices()
{
    return realJsonObj;
}
