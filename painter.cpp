#include "painter.h"
#include "dataCal.h"
#include "stockDataGet.h"
#include "QWidget"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QValidator>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPaintEvent>
#include <QRect>
#include <QByteArray>
#include <QFont>
#include <QScrollBar>
#include <QPixmap>
painter::painter(QObject *parent) : QObject(parent)
{
}

void painter::painteKLine(QJsonValue jsonvalue, QWidget *widget)
{
    //获取窗口长宽
    int width = widget->width();
    int height = widget->height();

    //绘制K线图数据
    double _min = dataCal::minKCal(jsonvalue);
    double _max = dataCal::maxKCal(jsonvalue);
    double _sum = dataCal::sumKCal(jsonvalue);

    //成交量数据
    double _volumnMin = dataCal::minVolumnKCal(jsonvalue);
    double _volumnMax = dataCal::maxVolumnKCal(jsonvalue);
    double _volumnSum = dataCal::sumVolumnKCal(jsonvalue);

    //框架画笔
    QPainter paiStd(widget);
    QPen penStd;
    penStd.setWidth(2);
    penStd.setBrush(QBrush(Qt::darkGray, Qt::Dense6Pattern));
    paiStd.setPen(penStd);

    //画y轴标准型(7条标准线),右边30区域标注刻度
    for (int i = 1; i <= 7; i++)
    {
        int yHeight = height * i / 10;
        paiStd.drawLine(0, yHeight, width, yHeight);
    }

    //    //画x轴(仅绘制右区域刻度区间，x值根据鼠标位置实时显示
    paiStd.drawLine(width - 30, 0, width - 30, height * 7 / 10);

    //    //右侧边界线
    paiStd.drawLine(width, 0, width - 1, height);

    //绘制K线图
    //获取数据
    QJsonArray array = jsonvalue.toObject().value(data).toArray();

    if (_sum == 0)
    {
        _sum = 1;
    }
    if (_volumnSum == 0)
    {
        _volumnSum = 1;
    }
    if (array.size() < 40)
    {
        width = (width - 30) / 40 * array.size();
    }
    if (array.size() > 200)
    {
        width = (width - 30) / 200 * array.size();
    }
    for (int i = array.size() - 1; i >= 0; i--)
    {
        //计算K线坐标
        int y_max_loc = height * 7 / 10 - (array[array.size() - 1 - i].toObject().value(high_price).toString().toDouble() - _min) / ((10 * _sum) / (6 * height));
        int y_min_loc = height * 7 / 10 - (array[array.size() - 1 - i].toObject().value(lower_price).toString().toDouble() - _min) / ((10 * _sum) / (6 * height));
        int y_close_loc = height * 7 / 10 - (array[array.size() - 1 - i].toObject().value(close_price).toString().toDouble() - _min) / ((10 * _sum) / (6 * height));
        int y_open_loc = height * 7 / 10 - (array[array.size() - 1 - i].toObject().value(open_price).toString().toDouble() - _min) / ((10 * _sum) / (6 * height));
        //计算成交量柱形高度
        int y_volumn_max_loc = height * 17 / 20 - 10 - (array[array.size() - 1 - i].toObject().value(volume).toString().toDouble() - _volumnMin) / (_volumnSum / (height * 3 / 20 - 20));

        //数据点距离
        int x_width = (width - 30) / array.size();
        int x_d = x_width / 2 - 1;
        int x_loc = x_width / 2 - 1 + (i * (width - 30) / array.size());
        if (y_close_loc < y_open_loc)
        {
            QPainter k_painter(widget);
            QPen k_pen;
            k_pen.setWidth(1);
            k_pen.setBrush(Qt::red);
            k_painter.setPen(k_pen);
            k_painter.drawLine(x_loc, y_max_loc, x_loc, y_close_loc);
            k_painter.drawLine(x_loc, y_open_loc, x_loc, y_min_loc);
            QRect k(x_loc - x_d, y_close_loc, x_d * 2, y_open_loc - y_close_loc);
            k_painter.drawRect(k);

            k_painter.fillRect(k, k_pen.brush());

            //成交量
            QRect v(x_loc - x_d, y_volumn_max_loc, x_d * 2, 17 * height / 20 - y_volumn_max_loc);
            k_painter.drawRect(v);

            k_painter.fillRect(v, k_pen.brush());
        }
        else
        {
            QPainter k_painter(widget);
            QPen k_pen;
            k_pen.setWidth(1);
            k_pen.setBrush(Qt::green);
            k_painter.setPen(k_pen);
            k_painter.drawLine(x_loc, y_max_loc, x_loc, y_open_loc);
            k_painter.drawLine(x_loc, y_close_loc, x_loc, y_min_loc);
            QRect k(x_loc - x_d, y_open_loc, x_d * 2, y_close_loc - y_open_loc);
            k_painter.drawRect(k);
            k_painter.fillRect(k, k_pen.brush());
            //成交量
            QRect v(x_loc - x_d, y_volumn_max_loc, x_d * 2, 17 * height / 20 - y_volumn_max_loc);
            k_painter.drawRect(v);

            k_painter.fillRect(v, k_pen.brush());
        }
    }

    //绘制股票名
    width = widget->width();
    QString name = jsonvalue.toObject().value("stock_name").toString().toUtf8();
    QString code = jsonvalue.toObject().value("stock_code").toString().toUtf8();
    QPainter n_painter(widget);
    QPen n_pen;
    n_pen.setWidth(2);
    n_pen.setBrush(Qt::white);
    n_painter.setPen(n_pen);
    if (!name.isEmpty())
        n_painter.drawText(width - 102, 10, "股 票 名:" + name);
    if (!code.isEmpty())
        n_painter.drawText(width - 102, 25, "股票代码:" + code);
    if (!name.isEmpty())
        n_painter.drawText(0, height / 10 * 7 + 20, "成交量：");
    //绘制刻度
    //刻度画笔
    QPainter yTextPainter(widget);
    QPen yTextPen;
    yTextPen.setWidth(2);
    yTextPen.setBrush(Qt::white);
    yTextPainter.setPen(yTextPen);
    if (!name.isEmpty())
    {
        for (int i = 1; i <= 7; i++)
        {
            int yHeight = height * i / 10;
            yTextPainter.drawText(width - 30, yHeight, QString::number(_max - ((i - 1) * _sum / 6)));
        }
    }
    yTextPen.setWidth(1);
    yTextPainter.setPen(yTextPen);
    yTextPainter.drawLine(0, height * 17 / 20, width, height * 17 / 20);
}

void painter::painteRealLine(QJsonValue jsonvalue, QWidget *widget)
{
    //获取窗口长宽
    int width = widget->width();
    int height = widget->height();

    //获取数据
    QJsonArray arrayFirst = jsonvalue.toObject().value("dataList").toArray();
    QJsonArray array;
    double _yestclose;
    if (arrayFirst.size() > 0)
    {
        _yestclose = arrayFirst[0].toObject().value("yestclose").toString().toDouble();
    }
    //绘制分时线图数据
    double _min = dataCal::minRCal(jsonvalue);
    double _max = dataCal::maxRCal(jsonvalue);
    double _volumnMin = dataCal::minVolumnRCal(jsonvalue);
    double _volumnSum = dataCal::sumVolumnRCal(jsonvalue);


    if (_max - _yestclose >= _yestclose - _min)
    {
        _min = 2 * _yestclose - _max;
    }
    else
    {
        _max = 2 * _yestclose - _min;
    }
    double _sum = _max - _min;
    //框架画笔
    QPainter paiStd(widget);
    QPen penStd;
    penStd.setWidth(1);
    penStd.setBrush(QBrush(Qt::darkGray));
    paiStd.setPen(penStd);

    //画y轴标准型(11条标准线),右边30区域标注刻度
    for (int i = 1; i <= 11; i++)
    {
        int yHeight = height * i / 15;
        paiStd.drawLine(30, yHeight, width - 30, yHeight);
    }
    //画x轴
    for (int i = 0; i <= 10; i++)
    {
        int xWidth = (width - 60) * i / 10 + 30;
        paiStd.drawLine(xWidth, height * 1 / 15, xWidth, height * 11 / 15);
    }
    paiStd.drawLine(width - 1, 0, width - 1, height);
    if (arrayFirst.size() > 0)
    {
        array = arrayFirst[0].toObject().value("minuteList").toArray();
        if (_sum == 0)
        {
            _sum = 1;
        }
        if (_volumnSum == 0)
        {
            _volumnSum = 1;
        }
        for (int i = 0; i < array.size() - 1; i++)
        {
            int y_nowPrice_loc = height * 11 / 15 - (array[i].toObject().value("nowPrice").toString().toDouble() - _min) / ((15 * _sum) / (10 * height));
            int y_nowPrice_next_loc = height * 11 / 15 - (array[i + 1].toObject().value("nowPrice").toString().toDouble() - _min) / ((15 * _sum) / (10 * height));
            int x_nowPrice_loc = (width - 60) * i / 242 + 30;
            int x_nowPrice_next_loc = (width - 60) * (i + 1) / 242 + 30;

            //计算成交量柱形高度
            int y_volumn_loc = height * 17 / 20 - 1 - (array[i].toObject().value("volume").toString().toDouble() - _volumnMin) / (_volumnSum / (height * 3 / 20 - 50));

            QPainter r_painter(widget);
            QPen r_pen;
            r_pen.setWidth(1);
            r_pen.setBrush(Qt::white);
            r_painter.setPen(r_pen);
            r_painter.drawLine(x_nowPrice_loc, y_nowPrice_loc, x_nowPrice_next_loc, y_nowPrice_next_loc);

            int y_avgPrice_loc = height * 11 / 15 - (array[i].toObject().value("avgPrice").toString().toDouble() - _min) / ((15 * _sum) / (10 * height));
            int y_avgPrice_next_loc = height * 11 / 15 - (array[i + 1].toObject().value("avgPrice").toString().toDouble() - _min) / ((15 * _sum) / (10 * height));
            int x_avgPrice_loc = (width - 60) * i / 242 + 30;
            int x_avgPrice_next_loc = (width - 60) * (i + 1) / 242 + 30;
            QPainter a_painter(widget);
            QPen a_pen;
            a_pen.setWidth(1);
            a_pen.setBrush(Qt::yellow);
            a_painter.setPen(a_pen);
            a_painter.drawLine(x_avgPrice_loc, y_avgPrice_loc, x_avgPrice_next_loc, y_avgPrice_next_loc);

            //成交量柱形图
            QPainter v_painter(widget);
            QPen v_pen;
            v_pen.setWidth(1);
            if (y_nowPrice_next_loc > y_nowPrice_loc)
            {
                v_pen.setBrush(Qt::red);
            }
            else
            {
                v_pen.setBrush(Qt::green);
            }
            v_painter.setPen(v_pen);
            QRect v(x_nowPrice_loc - (x_nowPrice_next_loc - x_nowPrice_loc) / 2, y_volumn_loc, (x_nowPrice_next_loc - x_nowPrice_loc) / 2, 17 * height / 20 - y_volumn_loc);
            v_painter.drawRect(v);

            v_painter.fillRect(v, v_pen.brush());
        }
    }
    //绘制股票名
    QString name = jsonvalue.toObject().value("stock_name").toString().toUtf8();
    QString code = jsonvalue.toObject().value("stock_code").toString().toUtf8();
    QPainter n_painter(widget);
    QPen n_pen;
    n_pen.setWidth(2);
    n_pen.setBrush(Qt::white);
    n_painter.setPen(n_pen);
    if (!name.isEmpty())
        n_painter.drawText(width - 120, 10, "股 票 名:" + name);
    if (!code.isEmpty())
        n_painter.drawText(width - 120, 25, "股票代码:" + code);
    //绘制刻度
    //刻度画笔
    QPainter yTextPainter(widget);
    QPen yTextPen;
    yTextPen.setWidth(1);
    yTextPen.setBrush(Qt::white);
    yTextPainter.setPen(yTextPen);
    if (!name.isEmpty())
    {
        QPainter perPainter(widget);
        QPen perPen;
        perPen.setWidth(1);
        for (int i = 1; i <= 11; i++)
        {
            int yHeight = height * i / 15;
            if (i > 6)
            {
                perPen.setBrush(Qt::green);
                perPainter.setPen(perPen);
                yTextPainter.drawText(width - 30, yHeight, QString::number(-((_max - ((i - 1) * _sum / 10)) - _yestclose) / _yestclose * 100, 'f', 2) + "%");
                yTextPainter.drawText(0, yHeight, QString::number(_max - ((i - 1) * _sum / 10)));
            }
            if (i < 6)
            {
                perPen.setBrush(Qt::red);
                perPainter.setPen(perPen);
                yTextPainter.drawText(width - 30, yHeight, QString::number(((_max - ((i - 1) * _sum / 10)) - _yestclose) / _yestclose * 100, 'f', 2) + "%");
                yTextPainter.drawText(0, yHeight, QString::number(_max - ((i - 1) * _sum / 10)));
            }
            if (i == 6)
            {

                perPen.setBrush(Qt::darkGray);
                perPainter.setPen(perPen);
                yTextPainter.drawText(width - 30, yHeight, QString::number(((_max - ((i - 1) * _sum / 10)) - _yestclose) / _yestclose * 100, 'f', 2) + "%");
                yTextPainter.drawText(0, yHeight, QString::number(_max - ((i - 1) * _sum / 10)));
            }
        }
    }
}

void painter::painteInfo(QJsonValue jsonvalue, QWidget *widget)
{
    //获取窗口长宽
    int width = widget->width();
    int height = widget->height();
    if (!jsonvalue.isNull())
    {
        //获取数据
        QJsonValue arrayStockMarket = jsonvalue.toObject().value("stockMarket");

        QPainter infoPainter(widget);
        QPen infoPenWhite;
        QPen infoPenBlue;
        QPen infoPenDarkGray;
        QPen infoPenGreen;
        QPen infoPenRed;
        infoPenWhite.setBrush(Qt::white);
        infoPenBlue.setBrush(QColor(39, 149, 213));
        infoPenDarkGray.setBrush(Qt::darkGray);
        infoPenGreen.setBrush(Qt::green);
        infoPenRed.setBrush(Qt::red);
        //代码信息
        QString code = arrayStockMarket.toObject().value("stock_code").toString().toUtf8();
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(10);
        infoPainter.setPen(infoPenWhite);
        infoPainter.setFont(font);
        infoPainter.drawText(QRect(0, 0, 50, 30), Qt::AlignCenter, code);

        //股票名
        QString name = arrayStockMarket.toObject().value("stock_name").toString().toUtf8();
        font.setPointSize(15);
        infoPainter.setPen(infoPenBlue);
        infoPainter.setFont(font);
        infoPainter.drawText(QRect(50, 0, 100, 30), Qt::AlignCenter, name);

        //当前价与涨跌
        font.setPointSize(12);
        infoPainter.setFont(font);
        QString nowPrice = arrayStockMarket.toObject().value("nowPrice").toString().toUtf8();
        QString diff_money = arrayStockMarket.toObject().value("diff_money").toString().toUtf8();
        QString diff_rate = arrayStockMarket.toObject().value("diff_rate").toString().toUtf8();
        if (!name.isEmpty())
        {
            if (diff_money[0] == '-')
            {
                infoPainter.setPen(infoPenGreen);
                infoPainter.drawText(QRect(0, 30, 50, 30), Qt::AlignCenter, nowPrice);
                font.setPointSize(9);
                infoPainter.setFont(font);
                infoPainter.drawText(QRect(50, 30, 50, 30), Qt::AlignCenter, "↓-" + diff_money);
                infoPainter.drawText(QRect(100, 30, 50, 30), Qt::AlignCenter, "↓-" + diff_rate + "%");
            }
            else
            {
                infoPainter.setPen(infoPenRed);
                infoPainter.drawText(QRect(0, 30, 50, 30), Qt::AlignCenter, nowPrice);
                font.setPointSize(9);
                infoPainter.setFont(font);
                infoPainter.drawText(QRect(50, 30, 50, 30), Qt::AlignCenter, "↑+" + diff_money);
                infoPainter.drawText(QRect(100, 30, 50, 30), Qt::AlignCenter, "↑+" + diff_rate + "%");
            }
        }
        //分隔线
        if (!name.isEmpty())
        {
            infoPainter.setPen(infoPenWhite);
            infoPainter.drawLine(0, 60, 150, 60);
        }
        //列宽(列数26))
        int columnHeight = (height - 60) / 26;

        //买入卖出数据表格
        QString closePrice = arrayStockMarket.toObject().value("nowPrice").toString().toUtf8();
        QString sell5_n = arrayStockMarket.toObject().value("sell5_n").toString().toUtf8();
        QString sell5_m = arrayStockMarket.toObject().value("sell5_m").toString().toUtf8();
        QString sell4_n = arrayStockMarket.toObject().value("sell4_n").toString().toUtf8();
        QString sell4_m = arrayStockMarket.toObject().value("sell4_m").toString().toUtf8();
        QString sell3_n = arrayStockMarket.toObject().value("sell3_n").toString().toUtf8();
        QString sell3_m = arrayStockMarket.toObject().value("sell3_m").toString().toUtf8();
        QString sell2_n = arrayStockMarket.toObject().value("sell2_n").toString().toUtf8();
        QString sell2_m = arrayStockMarket.toObject().value("sell2_m").toString().toUtf8();
        QString sell1_n = arrayStockMarket.toObject().value("sell1_n").toString().toUtf8();
        QString sell1_m = arrayStockMarket.toObject().value("sell1_m").toString().toUtf8();
        QString buy1_n = arrayStockMarket.toObject().value("buy1_n").toString().toUtf8();
        QString buy1_m = arrayStockMarket.toObject().value("buy1_m").toString().toUtf8();
        QString buy2_n = arrayStockMarket.toObject().value("buy2_n").toString().toUtf8();
        QString buy2_m = arrayStockMarket.toObject().value("buy2_m").toString().toUtf8();
        QString buy3_n = arrayStockMarket.toObject().value("buy3_n").toString().toUtf8();
        QString buy3_m = arrayStockMarket.toObject().value("buy3_m").toString().toUtf8();
        QString buy4_n = arrayStockMarket.toObject().value("buy4_n").toString().toUtf8();
        QString buy4_m = arrayStockMarket.toObject().value("buy4_m").toString().toUtf8();
        QString buy5_n = arrayStockMarket.toObject().value("buy5_n").toString().toUtf8();
        QString buy5_m = arrayStockMarket.toObject().value("buy5_m").toString().toUtf8();
        infoPainter.setPen(infoPenDarkGray);
        font.setPointSize(9);
        infoPainter.setFont(font);
        if (!name.isEmpty())
        {
            infoPainter.drawText(QRect(0, 60 + 0 * columnHeight, 30, columnHeight), Qt::AlignCenter, "卖五");
            infoPainter.drawText(QRect(0, 60 + 1 * columnHeight, 30, columnHeight), Qt::AlignCenter, "卖四");
            infoPainter.drawText(QRect(0, 60 + 2 * columnHeight, 30, columnHeight), Qt::AlignCenter, "卖三");
            infoPainter.drawText(QRect(0, 60 + 3 * columnHeight, 30, columnHeight), Qt::AlignCenter, "卖二");
            infoPainter.drawText(QRect(0, 60 + 4 * columnHeight, 30, columnHeight), Qt::AlignCenter, "卖一");
            infoPainter.drawText(QRect(0, 60 + 5 * columnHeight, 30, columnHeight), Qt::AlignCenter, "买一");
            infoPainter.drawText(QRect(0, 60 + 6 * columnHeight, 30, columnHeight), Qt::AlignCenter, "买二");
            infoPainter.drawText(QRect(0, 60 + 7 * columnHeight, 30, columnHeight), Qt::AlignCenter, "买三");
            infoPainter.drawText(QRect(0, 60 + 8 * columnHeight, 30, columnHeight), Qt::AlignCenter, "买四");
            infoPainter.drawText(QRect(0, 60 + 9 * columnHeight, 30, columnHeight), Qt::AlignCenter, "买五");

            infoPainter.setPen(infoPenWhite);
            infoPainter.drawLine(0, 60 + 5 * columnHeight, 150, 60 + 5 * columnHeight);
            infoPainter.drawLine(0, 60 + 10 * columnHeight, 150, 60 + 10 * columnHeight);
        }
        if (sell5_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (sell5_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 0 * columnHeight, 50, columnHeight), Qt::AlignCenter, sell5_m);
        if (sell4_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (sell4_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 1 * columnHeight, 50, columnHeight), Qt::AlignCenter, sell4_m);
        if (sell3_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (sell3_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 2 * columnHeight, 50, columnHeight), Qt::AlignCenter, sell3_m);
        if (sell2_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (sell2_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 3 * columnHeight, 50, columnHeight), Qt::AlignCenter, sell2_m);
        if (sell1_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (sell1_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 4 * columnHeight, 50, columnHeight), Qt::AlignCenter, sell1_m);
        if (buy1_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (buy1_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 5 * columnHeight, 50, columnHeight), Qt::AlignCenter, buy1_m);
        if (buy2_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (buy2_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 6 * columnHeight, 50, columnHeight), Qt::AlignCenter, buy2_m);
        if (buy3_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (buy3_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 7 * columnHeight, 50, columnHeight), Qt::AlignCenter, buy3_m);
        if (buy4_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (buy4_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 8 * columnHeight, 50, columnHeight), Qt::AlignCenter, buy4_m);
        if (buy5_m.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (buy5_m.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(30, 60 + 9 * columnHeight, 50, columnHeight), Qt::AlignCenter, buy5_m);

        infoPainter.setPen(infoPenWhite);
        infoPainter.drawText(QRect(40, 60 + 0 * columnHeight, 150, columnHeight), Qt::AlignCenter, sell5_n);
        infoPainter.drawText(QRect(40, 60 + 1 * columnHeight, 150, columnHeight), Qt::AlignCenter, sell4_n);
        infoPainter.drawText(QRect(40, 60 + 2 * columnHeight, 150, columnHeight), Qt::AlignCenter, sell3_n);
        infoPainter.drawText(QRect(40, 60 + 3 * columnHeight, 150, columnHeight), Qt::AlignCenter, sell2_n);
        infoPainter.drawText(QRect(40, 60 + 4 * columnHeight, 150, columnHeight), Qt::AlignCenter, sell1_n);
        infoPainter.drawText(QRect(40, 60 + 5 * columnHeight, 150, columnHeight), Qt::AlignCenter, buy1_n);
        infoPainter.drawText(QRect(40, 60 + 6 * columnHeight, 150, columnHeight), Qt::AlignCenter, buy2_n);
        infoPainter.drawText(QRect(40, 60 + 7 * columnHeight, 150, columnHeight), Qt::AlignCenter, buy3_n);
        infoPainter.drawText(QRect(40, 60 + 8 * columnHeight, 150, columnHeight), Qt::AlignCenter, buy4_n);
        infoPainter.drawText(QRect(40, 60 + 9 * columnHeight, 150, columnHeight), Qt::AlignCenter, buy5_n);
        if (!name.isEmpty())
        {
            infoPainter.drawText(QRect(0, 60 + 10 * columnHeight, 70, columnHeight), Qt::AlignLeft, "今日最高价");
            infoPainter.drawText(QRect(0, 60 + 11 * columnHeight, 70, columnHeight), Qt::AlignLeft, "今日最低价");
            infoPainter.drawText(QRect(0, 60 + 12 * columnHeight, 70, columnHeight), Qt::AlignLeft, "今日开盘价");
            infoPainter.drawText(QRect(0, 60 + 13 * columnHeight, 70, columnHeight), Qt::AlignLeft, "涨停价");
            infoPainter.drawText(QRect(0, 60 + 14 * columnHeight, 70, columnHeight), Qt::AlignLeft, "跌停价");
            infoPainter.drawText(QRect(0, 60 + 15 * columnHeight, 70, columnHeight), Qt::AlignLeft, "涨跌幅度");
            infoPainter.drawText(QRect(0, 60 + 16 * columnHeight, 70, columnHeight), Qt::AlignLeft, "涨跌金额");
            infoPainter.drawText(QRect(0, 60 + 17 * columnHeight, 70, columnHeight), Qt::AlignLeft, "成交金额");
            infoPainter.drawText(QRect(0, 60 + 18 * columnHeight, 70, columnHeight), Qt::AlignLeft, "成交量(股)");
            infoPainter.drawText(QRect(0, 60 + 19 * columnHeight, 70, columnHeight), Qt::AlignLeft, "流通市值");
            infoPainter.drawText(QRect(0, 60 + 20 * columnHeight, 70, columnHeight), Qt::AlignLeft, "总市值");
            infoPainter.drawText(QRect(0, 60 + 21 * columnHeight, 70, columnHeight), Qt::AlignLeft, "换手率");
            infoPainter.drawText(QRect(0, 60 + 22 * columnHeight, 70, columnHeight), Qt::AlignLeft, "总股本(万股)");
            infoPainter.drawText(QRect(0, 60 + 23 * columnHeight, 70, columnHeight), Qt::AlignLeft, "振幅");
            infoPainter.drawText(QRect(0, 60 + 24 * columnHeight, 70, columnHeight), Qt::AlignLeft, "市盈率(PE)");
            infoPainter.drawText(QRect(0, 60 + 25 * columnHeight, 70, columnHeight), Qt::AlignLeft, "市净率(PB)");
        }
        QString todayMax = arrayStockMarket.toObject().value("todayMax").toString().toUtf8();
        QString todayMin = arrayStockMarket.toObject().value("todayMin").toString().toUtf8();
        QString openPrice = arrayStockMarket.toObject().value("openPrice").toString().toUtf8();
        QString highLimit = arrayStockMarket.toObject().value("highLimit").toString().toUtf8();
        QString downLimit = arrayStockMarket.toObject().value("downLimit").toString().toUtf8();
        QString tradeAmount = arrayStockMarket.toObject().value("tradeAmount").toString().toUtf8();
        QString tradeNum = arrayStockMarket.toObject().value("tradeNum").toString().toUtf8();
        QString circulation_value = arrayStockMarket.toObject().value("circulation_value").toString().toUtf8();
        QString all_value = arrayStockMarket.toObject().value("all_value").toString().toUtf8();
        QString turnover = arrayStockMarket.toObject().value("turnover").toString().toUtf8();
        QString totalcapital = arrayStockMarket.toObject().value("totalcapital").toString().toUtf8();
        QString swing = arrayStockMarket.toObject().value("swing").toString().toUtf8();
        QString pe = arrayStockMarket.toObject().value("pe").toString().toUtf8();
        QString pb = arrayStockMarket.toObject().value("pb").toString().toUtf8();
        if (todayMax.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (todayMax.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(70, 60 + 10 * columnHeight, 150, columnHeight), Qt::AlignLeft, todayMax);
        if (todayMin.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (todayMin.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(70, 60 + 11 * columnHeight, 150, columnHeight), Qt::AlignLeft, todayMin);
        if (openPrice.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (openPrice.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(70, 60 + 12 * columnHeight, 150, columnHeight), Qt::AlignLeft, openPrice);
        if (highLimit.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (highLimit.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(70, 60 + 13 * columnHeight, 150, columnHeight), Qt::AlignLeft, highLimit);
        if (downLimit.toDouble() > closePrice.toDouble())
        {
            infoPainter.setPen(infoPenRed);
        }
        else if (downLimit.toDouble() == closePrice.toDouble())
        {
            infoPainter.setPen(infoPenWhite);
        }
        else
        {
            infoPainter.setPen(infoPenGreen);
        }
        infoPainter.drawText(QRect(70, 60 + 14 * columnHeight, 150, columnHeight), Qt::AlignLeft, downLimit);
        if ('=' == diff_money[0])
        {
            infoPainter.setPen(infoPenGreen);
        }
        else
        {
            infoPainter.setPen(infoPenRed);
        }
        infoPainter.drawText(QRect(70, 60 + 15 * columnHeight, 150, columnHeight), Qt::AlignLeft, diff_rate);
        if ('-' == diff_rate[0])
        {
            infoPainter.setPen(infoPenGreen);
        }
        else
        {
            infoPainter.setPen(infoPenRed);
        }
        infoPainter.drawText(QRect(70, 60 + 16 * columnHeight, 150, columnHeight), Qt::AlignLeft, diff_money);

        infoPainter.setPen(infoPenWhite);
        font.setPointSize(9);
        infoPainter.setFont(font);
        infoPainter.drawText(QRect(70, 60 + 17 * columnHeight, 150, columnHeight), Qt::AlignLeft, tradeAmount);
        infoPainter.drawText(QRect(70, 60 + 18 * columnHeight, 150, columnHeight), Qt::AlignLeft, tradeNum);
        infoPainter.drawText(QRect(70, 60 + 19 * columnHeight, 150, columnHeight), Qt::AlignLeft, circulation_value);
        infoPainter.drawText(QRect(70, 60 + 20 * columnHeight, 150, columnHeight), Qt::AlignLeft, all_value);
        infoPainter.drawText(QRect(70, 60 + 21 * columnHeight, 150, columnHeight), Qt::AlignLeft, turnover);
        infoPainter.drawText(QRect(70, 60 + 22 * columnHeight, 150, columnHeight), Qt::AlignLeft, totalcapital);
        infoPainter.drawText(QRect(70, 60 + 23 * columnHeight, 150, columnHeight), Qt::AlignLeft, swing);
        infoPainter.drawText(QRect(70, 60 + 24 * columnHeight, 150, columnHeight), Qt::AlignLeft, pe);
        infoPainter.drawText(QRect(70, 60 + 25 * columnHeight, 150, columnHeight), Qt::AlignLeft, pb);
    }
}

void painter::painteKMACDIndexLine(QJsonValue jsonvalue, QWidget *widget)
{
    //获取窗口长宽
    int width = widget->width();
    int height = widget->height();

    //
    double _max;
    double _min;
    double _sum;
    int zeroLIneLoc;

    QJsonArray jsonarray = jsonvalue.toArray();

    if (jsonarray.size() < 40)
    {
        width = (width - 30) / 40 * jsonarray.size();
    }

    if (!jsonarray.isEmpty())
    {
        _max = dataCal::maxKIndexCal(jsonarray);
        _min = dataCal::minKIndexCal(jsonarray);
        _sum = _max - _min;

        int indexheight;
        if (jsonarray.size() >= 150)
        {
            indexheight = height * 3 / 20 - 40;
            height -= 20;
        }
        else
        {
            indexheight = height * 3 / 20 - 20;
        }
        zeroLIneLoc = height - (indexheight * (-_min) / (_max - _min));

        //X坐标标准宽
        int x_width = (width - 30) / jsonarray.size();

        //MACD
        for (int i = jsonarray.size() - 1; i >= 0; i--)
        {
            //X坐标
            int x_loc = x_width / 2 - 1 + (i * (width - 30) / jsonarray.size());
            //Y坐标
            int y_MACD_loc = height - (jsonarray[jsonarray.size() - 1 - i].toObject().value("MACD").toString().toDouble() - _min) / _sum * indexheight;

            QPainter macd_painter(widget);
            QPen macd_pen;
            macd_pen.setWidth(1);
            if (jsonarray[jsonarray.size() - 1 - i].toObject().value("MACD").toString().toDouble() > 0)
            {
                macd_pen.setBrush(Qt::red);
                macd_painter.setPen(macd_pen);
                macd_painter.drawLine(x_loc, y_MACD_loc, x_loc, zeroLIneLoc);
            }
            if (jsonarray[jsonarray.size() - 1 - i].toObject().value("MACD").toString().toDouble() < 0)
            {
                macd_pen.setBrush(Qt::green);
                macd_painter.setPen(macd_pen);
                macd_painter.drawLine(x_loc, y_MACD_loc, x_loc, zeroLIneLoc);
            }
            //            qDebug()<<x_loc<<y_MACD_loc<<x_loc<<zeroLIneLoc;
        }
        //DEA
        for (int i = jsonarray.size() - 1; i > 0; i--)
        {
            //X坐标
            int x_loc = x_width / 2 - 1 + (i * (width - 30) / jsonarray.size());
            int x_next_loc = x_width / 2 - 1 + ((i - 1) * (width - 30) / jsonarray.size());
            //Y坐标
            int y_DEA_loc = height - (jsonarray[jsonarray.size() - 1 - i].toObject().value("DEA").toString().toDouble() - _min) / _sum * indexheight;
            int y_DEA_next_loc = height - (jsonarray[jsonarray.size() - i].toObject().value("DEA").toString().toDouble() - _min) / _sum * indexheight;
            QPainter macd_painter(widget);
            QPen macd_pen;
            macd_pen.setWidth(1);
            macd_pen.setBrush(Qt::yellow);
            macd_painter.setPen(macd_pen);
            macd_painter.drawLine(x_loc, y_DEA_loc, x_next_loc, y_DEA_next_loc);
            //            qDebug()<<(jsonarray[jsonarray.size()-1-i].toObject().value("DEA").toString().toDouble()-_min)/_sum*indexheight;
        }
        //DIFF
        for (int i = jsonarray.size() - 1; i > 0; i--)
        {
            //X坐标
            int x_loc = x_width / 2 - 1 + (i * (width - 30) / jsonarray.size());
            int x_next_loc = x_width / 2 - 1 + ((i - 1) * (width - 30) / jsonarray.size());
            //Y坐标
            int y_DEA_loc = height - (jsonarray[jsonarray.size() - 1 - i].toObject().value("DIFF").toString().toDouble() - _min) / _sum * indexheight;
            int y_DEA_next_loc = height - (jsonarray[jsonarray.size() - i].toObject().value("DIFF").toString().toDouble() - _min) / _sum * indexheight;
            QPainter macd_painter(widget);
            QPen macd_pen;
            macd_pen.setWidth(1);
            macd_pen.setBrush(Qt::white);
            macd_painter.setPen(macd_pen);
            macd_painter.drawLine(x_loc, y_DEA_loc, x_next_loc, y_DEA_next_loc);
        }
        //刻度线

        QPainter macd_painter(widget);
        QPen macd_pen;
        macd_pen.setWidth(2);
        macd_pen.setBrush(QBrush(Qt::darkGray, Qt::Dense6Pattern));
        macd_painter.setPen(macd_pen);
        for (int i = 0; i < 5; i++)
        {
            macd_painter.drawLine(0, (height - (i * indexheight / 4)), widget->width() - 30, (height - (i * indexheight / 4)));
        }
        //0线
        //        macd_pen.setBrush(Qt::blue);
        //        macd_painter.setPen(macd_pen);
        //        macd_painter.drawLine(0,zeroLIneLoc,width-30,zeroLIneLoc);
    }
}

void painter::painteKRSIIndexLine(QJsonValue jsonvalue, QWidget *widget)
{
    //获取窗口长宽
    int width = widget->width();
    int height = widget->height();

    double _max;
    double _min;
    double _sum;
    QJsonArray jsonarray = jsonvalue.toArray();

    if (jsonarray.size() < 40)
    {
        width = (width - 30) / 40 * jsonarray.size();
    }

    if (!jsonarray.isEmpty())
    {
        _max = dataCal::maxKRSIIndexCal(jsonarray);
        _min = dataCal::minKRSIIndexCal(jsonarray);
        _sum = _max - _min;

        int indexheight;
        if (jsonarray.size() >= 150)
        {
            indexheight = height * 3 / 20 - 40;
            height -= 20;
        }
        else
        {
            indexheight = height * 3 / 20 - 20;
        }

        //X坐标标准宽
        int x_width = (width - 30) / jsonarray.size();

        //RSI6
        for (int i = jsonarray.size() - 1; i > 0; i--)
        {
            //X坐标
            int x_loc = x_width / 2 - 1 + (i * (width - 30) / jsonarray.size());
            int x_next_loc = x_width / 2 - 1 + ((i - 1) * (width - 30) / jsonarray.size());
            //Y坐标
            int y_DEA_loc = height - (jsonarray[jsonarray.size() - 1 - i].toObject().value("rsiData6").toString().toDouble() - _min) / _sum * indexheight;
            int y_DEA_next_loc = height - (jsonarray[jsonarray.size() - i].toObject().value("rsiData6").toString().toDouble() - _min) / _sum * indexheight;
            QPainter macd_painter(widget);
            QPen macd_pen;
            macd_pen.setWidth(1);
            macd_pen.setBrush(Qt::yellow);
            macd_painter.setPen(macd_pen);
            macd_painter.drawLine(x_loc, y_DEA_loc, x_next_loc, y_DEA_next_loc);
            //            qDebug()<<(jsonarray[jsonarray.size()-1-i].toObject().value("DEA").toString().toDouble()-_min)/_sum*indexheight;
        }
        //RSI12
        for (int i = jsonarray.size() - 1; i > 0; i--)
        {
            //X坐标
            int x_loc = x_width / 2 - 1 + (i * (width - 30) / jsonarray.size());
            int x_next_loc = x_width / 2 - 1 + ((i - 1) * (width - 30) / jsonarray.size());
            //Y坐标
            int y_DEA_loc = height - (jsonarray[jsonarray.size() - 1 - i].toObject().value("rsiData12").toString().toDouble() - _min) / _sum * indexheight;
            int y_DEA_next_loc = height - (jsonarray[jsonarray.size() - i].toObject().value("rsiData12").toString().toDouble() - _min) / _sum * indexheight;
            QPainter macd_painter(widget);
            QPen macd_pen;
            macd_pen.setWidth(1);
            macd_pen.setBrush(Qt::green);
            macd_painter.setPen(macd_pen);
            macd_painter.drawLine(x_loc, y_DEA_loc, x_next_loc, y_DEA_next_loc);
            //            qDebug()<<(jsonarray[jsonarray.size()-1-i].toObject().value("DEA").toString().toDouble()-_min)/_sum*indexheight;
        }
        //RSI6
        for (int i = jsonarray.size() - 1; i > 0; i--)
        {
            //X坐标
            int x_loc = x_width / 2 - 1 + (i * (width - 30) / jsonarray.size());
            int x_next_loc = x_width / 2 - 1 + ((i - 1) * (width - 30) / jsonarray.size());
            //Y坐标
            int y_DEA_loc = height - (jsonarray[jsonarray.size() - 1 - i].toObject().value("rsiData24").toString().toDouble() - _min) / _sum * indexheight;
            int y_DEA_next_loc = height - (jsonarray[jsonarray.size() - i].toObject().value("rsiData24").toString().toDouble() - _min) / _sum * indexheight;
            QPainter macd_painter(widget);
            QPen macd_pen;
            macd_pen.setWidth(1);
            macd_pen.setBrush(Qt::blue);
            macd_painter.setPen(macd_pen);
            macd_painter.drawLine(x_loc, y_DEA_loc, x_next_loc, y_DEA_next_loc);
            //            qDebug()<<(jsonarray[jsonarray.size()-1-i].toObject().value("DEA").toString().toDouble()-_min)/_sum*indexheight;
        }
        //刻度线

        QPainter macd_painter(widget);
        QPen macd_pen;
        macd_pen.setWidth(2);
        macd_pen.setBrush(QBrush(Qt::darkGray, Qt::Dense6Pattern));
        macd_painter.setPen(macd_pen);
        for (int i = 0; i < 5; i++)
        {
            macd_painter.drawLine(0, (height - (i * indexheight / 4)), widget->width() - 30, (height - (i * indexheight / 4)));
        }
    }
}
