#include "mainwindow.h"
#include "image_line_show.h"
#include "ui_image_line_show.h"
#include "ui_mainwindow.h"
#include "dataCal.h"
#include "painter.h"
// #include "webgets.h"
#include "qwebdata.h"
#include "stockDataGet.h"
#include <QAbstractButton>
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
#include <QTimer>
#include <QVBoxLayout>
#include <QScrollArea>
image_line_show::image_line_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::image_line_show)
{
    ui->setupUi(this);
    //鼠标移动窗口
    mouseMoveKInfo = new QWidget(this);
    mouseMoveRInfo = new QWidget(this);
    mouseMoveKInfo->setMouseTracking(true);
    mouseMoveRInfo->setMouseTracking(true);
    mouseMoveKInfo->setBaseSize(150,120);
    mouseMoveRInfo->setBaseSize(150,80);
    mouseMoveKInfo->setStyleSheet("background-color:rgb(7,7,7)");
    mouseMoveRInfo->setStyleSheet("background-color:rgb(7,7,7)");
    QVBoxLayout *KInfoLayout = new QVBoxLayout(mouseMoveKInfo);
    KInfoLayout->setSpacing(0);
    mouseMoveKTime = new QLabel(mouseMoveKInfo);
    mouseMoveKClose = new QLabel(mouseMoveKInfo);
    mouseMoveKMax = new QLabel(mouseMoveKInfo);
    mouseMoveKMin = new QLabel(mouseMoveKInfo);
    mouseMoveKOpen = new QLabel(mouseMoveKInfo);
    mouseMoveKVolumn = new QLabel(mouseMoveKInfo);
    mouseMoveKTime->setBaseSize(150,20);
    mouseMoveKClose->setBaseSize(150,20);
    mouseMoveKMax->setBaseSize(150,20);
    mouseMoveKMin->setBaseSize(150,20);
    mouseMoveKOpen->setBaseSize(150,20);
    mouseMoveKVolumn->setBaseSize(150,20);
    mouseMoveKTime->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveKClose->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveKMax->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveKMin->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveKOpen->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveKVolumn->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    KInfoLayout->addWidget(mouseMoveKTime);
    KInfoLayout->addWidget(mouseMoveKMax);
    KInfoLayout->addWidget(mouseMoveKMin);
    KInfoLayout->addWidget(mouseMoveKOpen);
    KInfoLayout->addWidget(mouseMoveKClose);
    KInfoLayout->addWidget(mouseMoveKVolumn);

    QVBoxLayout *RInfoLayout = new QVBoxLayout(mouseMoveRInfo);
    RInfoLayout->setSpacing(0);
    mouseMoveRAvgPrice = new QLabel(mouseMoveRInfo);
    mouseMoveRNowPrice = new QLabel(mouseMoveRInfo);
    mouseMoveRTime = new QLabel(mouseMoveRInfo);
    mouseMoveRVolume = new QLabel(mouseMoveRInfo);
    mouseMoveRAvgPrice->setBaseSize(150,20);
    mouseMoveRNowPrice->setBaseSize(150,20);
    mouseMoveRTime->setBaseSize(150,20);
    mouseMoveRVolume->setBaseSize(150,20);
    mouseMoveRAvgPrice->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveRNowPrice->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveRTime->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    mouseMoveRVolume->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    RInfoLayout->addWidget(mouseMoveRTime);
    RInfoLayout->addWidget(mouseMoveRNowPrice);
    RInfoLayout->addWidget(mouseMoveRAvgPrice);
    RInfoLayout->addWidget(mouseMoveRVolume);
    mouseMoveKInfo->hide();
    mouseMoveRInfo->hide();

    //指标窗口在下部4/5处
    int heigh = this->height();
    int width = this->width();


    int index_heigh = heigh/5*4;
    int index_width = width;

    //图线框按钮
    image_line_show_button();


    connect(this,SIGNAL(scrollBarValueChange()),this,SLOT(scrollBar()));
}


image_line_show::~image_line_show()
{
    delete ui;
}
QString image_line_show::krButtonString()
{
    return ui->k_real_buttonGroup->checkedButton()->text();
}

QString image_line_show::timeButtonString()
{
   return ui->time_buttonGroup->checkedButton()->text();
}
QString image_line_show::fqButtonString()
{
    return ui->bqfq_buttonGroup->checkedButton()->text();
}
QString image_line_show::indexButtonString()
{
    return ui->index_buttonGroup->checkedButton()->text();
}

QString image_line_show::timeString()
{
    if(this->timeButtonString()=="5分钟")
        return "5";
    if(this->timeButtonString()=="30分钟")
        return "30";
    if(this->timeButtonString()=="60分钟")
        return "60";
    if(this->timeButtonString()=="日线")
        return "day";
    if(this->timeButtonString()=="周线")
        return "week";
    if(this->timeButtonString()=="月线")
        return "month";
}
QString image_line_show::fqString()
{
    if(this->fqButtonString()=="不复权")
        return "bfq";
    if(this->fqButtonString()=="前复权")
        return "qfq";
}
QString image_line_show::dateString()
{
    QString dateCal;
    if(ui->image_line_show_beganday_dateEdit->text().length()==8)
        dateCal = ui->image_line_show_beganday_dateEdit->text().mid(0,4)+"0"+ui->image_line_show_beganday_dateEdit->text().mid(5,1)+"0"+ui->image_line_show_beganday_dateEdit->text().mid(7,1);
    if(ui->image_line_show_beganday_dateEdit->text().length()==9)
        dateCal = ui->image_line_show_beganday_dateEdit->text().mid(0,4)+"0"+ui->image_line_show_beganday_dateEdit->text().mid(5,1)+ui->image_line_show_beganday_dateEdit->text().mid(7,2);
    if(ui->image_line_show_beganday_dateEdit->text().length()==10)
        dateCal = ui->image_line_show_beganday_dateEdit->text().mid(0,4)+ui->image_line_show_beganday_dateEdit->text().mid(5,2)+ui->image_line_show_beganday_dateEdit->text().mid(8,2);
    return dateCal;
}

void image_line_show::paintEvent(QPaintEvent *event)
{
//    if (string == "") {
//        return;
//    }
    if(krButtonString() =="分时图")
    {
        QJsonValue jsonvalue = stockDataGet::stockTimeLine(string);
        painter::painteRealLine(jsonvalue,this);

//        QTimer *updateTimer = new QTimer(this);
//        updateTimer->start(60000);
//        connect(updateTimer,SIGNAL(timeout()),this,SLOT(update()));
    }
    if(krButtonString() =="K线图")
    {
        // qDebug() << string;
        QJsonValue jsonvalue = stockDataGet::stockKLine(string);
        QJsonValue indexjsonvalue = stockDataGet::stockKIndexData(MAstring);
        QJsonArray array =jsonvalue.toObject().value("list").toArray();
        QJsonArray jsonarray = indexjsonvalue.toArray();
        painter::painteKLine(jsonvalue,this);
        if(indexButtonString()=="MACD")
        {
            painter::painteKMACDIndexLine(indexjsonvalue,this);
            if(jsonarray.size()>0)
            {
                double _max =dataCal::maxKIndexCal(jsonarray);
                double _min = dataCal::minKIndexCal(jsonarray);
                double value[5];
                QString values[5];
                for(int i=0;i<5;i++)
                {
                    value[i]=_max-i*(_max-_min)/4;
                    if(value[i]>0)
                    {
                        values[i] = " "+QString::number(value[i]);
                    }else if(value[i]<0){
                        values[i] = QString::number(value[i]);
                    }else {
                        values[i] = " 0.00";
                    }
                }
                ui->imageindexshow_value1_label->setText(values[0]);
                ui->imageindexshow_value2_label->setText(values[1]);
                ui->imageindexshow_value3_label->setText(values[2]);
                ui->imageindexshow_value4_label->setText(values[3]);
                ui->imageindexshow_value5_label->setText(values[4]);
            }
        }else if(indexButtonString()=="RSI")
        {
            painter::painteKRSIIndexLine(indexjsonvalue,this);
            if(jsonarray.size()>0)
            {
                double _max =dataCal::maxKRSIIndexCal(jsonarray);
                double _min = dataCal::minKRSIIndexCal(jsonarray);
                double value[5];
                QString values[5];
                for(int i=0;i<5;i++)
                {
                    value[i]=_max-i*(_max-_min)/4;

                     values[i]=  QString::number(value[i]);
                }
                ui->imageindexshow_value1_label->setText(values[0]);
                ui->imageindexshow_value2_label->setText(values[1]);
                ui->imageindexshow_value3_label->setText(values[2]);
                ui->imageindexshow_value4_label->setText(values[3]);
                ui->imageindexshow_value5_label->setText(values[4]);
            }
        }
        //绘制右侧刻度
        if(array.size()>0)
        {
        double max=dataCal::maxKCal(jsonvalue);
        double sum=dataCal::sumKCal(jsonvalue);

        ui->imagelineshow_value1_label->setText(QString::number(max));
        ui->imagelineshow_value2_label->setText(QString::number(max-(sum/6)));
        ui->imagelineshow_value3_label->setText(QString::number(max-(2*sum/6)));
        ui->imagelineshow_value4_label->setText(QString::number(max-(3*sum/6)));
        ui->imagelineshow_value5_label->setText(QString::number(max-(4*sum/6)));
        ui->imagelineshow_value6_label->setText(QString::number(max-(5*sum/6)));
        ui->imagelineshow_value7_label->setText(QString::number(max-(6*sum/6)));
        }
        if(array.size()>=150)
        {
            this->resize(scrollArea_width/150*array.size(),scrollArea_heigth);
            this->setMinimumSize(scrollArea_width/150*array.size(),scrollArea_heigth);
//            QScrollArea *sa = new QScrollArea(this);
//            sa->setWidget(this);
//            sa->setGeometry(0,0,this->width(),this->height());
//           kScrollArea.setGeometry(0,0,this->width(),this->height());
//        }
        /*else{*/
//                        kScrollArea.setParent(this);
//                        kScrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//                        kScrollWidget.setParent(&kScrollArea);
//                        kScrollArea.setWidget(&kScrollWidget);
//                        kScrollArea.setGeometry(0,0,this->width(),this->height());
//                        kScrollWidget.setGeometry(0,0,this->width()/200*array.size(),this->height());
//                        kScrollArea.show();
//                        kScrollWidget.show();
//                        painter::painteKLine(jsonvalue,&kScrollWidget);
//                        painter::painteKIndexLine(jsonvalue,&kScrollWidget,indexButtonString());
        }else{
            this->resize(scrollArea_width-5,scrollArea_heigth-5);
            this->setMinimumSize(scrollArea_width-5,scrollArea_heigth-5);
        }
    }
}

void image_line_show::image_line_show_button()
{
    ui->image_line_show_k_pushButton->setText(tr("K线图"));
    ui->image_line_show_real_pushButton->setText(tr("分时图"));
    ui->image_line_show_5min_pushButton->setText(tr("5分钟"));
    ui->image_line_show_30min_pushButton->setText(tr("30分钟"));
    ui->image_line_show_60min_pushButton->setText("60分钟");
    ui->image_line_show_kDay_pushButton->setText(tr("日线"));
    ui->image_line_show_kWeek_pushButton->setText(tr("周线"));
    ui->image_line_show_kMonth_pushButton->setText(tr("月线"));
    ui->image_line_show_bfq_pushbutton->setText(tr("不复权"));
    ui->image_line_show_qfq_pushbutton->setText(tr("前复权"));
    this->setStyleSheet("QPushButton{color:darkGray;background-color:rgba(34,34,34,0);border:1px solid darkGray;}:hover{color:yellow}:checked{background-color:red};");
    ui->image_line_show_beganday_label->setText(tr("开始时间"));
    ui->image_line_show_beganday_label->setStyleSheet("color:white");
    ui->image_line_show_beganday_dateEdit->setStyleSheet("color:white");
    ui->image_line_show_beganday_dateEdit->setDate(QDate::currentDate());

    ui->image_line_show_KDJ_pushButton->setText((tr("KDJ")));
    ui->image_line_show_MACD_pushButton->setText(tr("MACD"));
    ui->image_line_show_BRAR_pushButton->setText(tr("BRAR"));
    ui->image_line_show_RSI_pushButton->setText(tr("RSI"));
    this->indexButtonGroupLoc();
}

void image_line_show::buttonChangeRepaint()
{
    QString code = this->searchInt;
    qDebug() << "code: " << code;
    if(code != 0)
    {
         // WebGets *web;
         QWebKLine *qWeb;
         // web = new WebRealtimeK(this->dateString(),code,this->timeString(),this->fqString());

         qWeb = new QWebKLine(code, "2018-08-01", "2018-09-01"); //code,this->timeString(),this->dateString(),this->fqString());
         this->string = qWeb->getResponse();
         qDebug() << this->string;


         // this->MAstring = qwebstockindexline->getResponse();
         this->repaint();
         // web->destroyed();
         qWeb->destroyed();
         delete qWeb;
    }
}

void image_line_show::kButtonState()
{
    ui->image_line_show_5min_pushButton->setCheckable(true);
    ui->image_line_show_5min_pushButton->blockSignals(false);
    ui->image_line_show_30min_pushButton->setCheckable(true);
    ui->image_line_show_30min_pushButton->blockSignals(false);
    ui->image_line_show_60min_pushButton->setCheckable(true);
    ui->image_line_show_60min_pushButton->blockSignals(false);
    ui->image_line_show_kDay_pushButton->setCheckable(true);
    ui->image_line_show_kDay_pushButton->blockSignals(false);
    ui->image_line_show_kWeek_pushButton->setCheckable(true);
    ui->image_line_show_kWeek_pushButton->blockSignals(false);
    ui->image_line_show_kMonth_pushButton->setCheckable(true);
    ui->image_line_show_kMonth_pushButton->blockSignals(false);
    ui->image_line_show_bfq_pushbutton->setCheckable(true);
    ui->image_line_show_bfq_pushbutton->blockSignals(false);
    ui->image_line_show_qfq_pushbutton->setCheckable(true);
    ui->image_line_show_qfq_pushbutton->blockSignals(false);
}
void image_line_show::rButtonState()
{
    ui->image_line_show_5min_pushButton->setCheckable(false);
    ui->image_line_show_5min_pushButton->blockSignals(true);
    ui->image_line_show_30min_pushButton->setCheckable(false);
    ui->image_line_show_30min_pushButton->blockSignals(true);
    ui->image_line_show_60min_pushButton->setCheckable(false);
    ui->image_line_show_60min_pushButton->blockSignals(true);
    ui->image_line_show_kDay_pushButton->setCheckable(false);
    ui->image_line_show_kDay_pushButton->blockSignals(true);
    ui->image_line_show_kWeek_pushButton->setCheckable(false);
    ui->image_line_show_kWeek_pushButton->blockSignals(true);
    ui->image_line_show_kMonth_pushButton->setCheckable(false);
    ui->image_line_show_kMonth_pushButton->blockSignals(true);
    ui->image_line_show_bfq_pushbutton->setCheckable(false);
    ui->image_line_show_bfq_pushbutton->blockSignals(true);
    ui->image_line_show_qfq_pushbutton->setCheckable(false);
    ui->image_line_show_qfq_pushbutton->blockSignals(true);
}

void image_line_show::on_image_line_show_beganday_dateEdit_editingFinished()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_30min_pushButton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_5min_pushButton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_k_pushButton_clicked()
{
    this->kButtonState();
    ui->imagelineshow_value_widget->show();
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_60min_pushButton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_kDay_pushButton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_kWeek_pushButton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_kMonth_pushButton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_bfq_pushbutton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_qfq_pushbutton_clicked()
{
    this->buttonChangeRepaint();
}

void image_line_show::on_image_line_show_real_pushButton_pressed()
{
    this->krButtonBackedup = ui->k_real_buttonGroup->checkedButton();
    this->fqButtonBackedup = ui->bqfq_buttonGroup->checkedButton();
    this->rButtonState();
    ui->imagelineshow_value_widget->hide();
    QString code = this->searchInt;
    if(code!= 0)
    {
        QWebKLine *qWeb;
        qWeb = new QWebKLine(code, "2018-08-01", "2018-09-01"); //code,this->timeString(),this->dateString(),this->fqString());
        this->string = qWeb->getResponse();
        this->repaint();
        qWeb->destroyed();
        delete qWeb;
    }
}


void image_line_show::mouseMoveEvent(QMouseEvent *event)
{

    if(event->pos().x()<620&&event->pos().y()<40)
    {
        mouseMoveKInfo->close();
        mouseMoveRInfo->close();
    }else
    {
        if(krButtonString() =="分时图")
        {
            mouseMoveRInfo->close();
            jsonvalue = stockDataGet::stockTimeLine(string);
            if(jsonvalue.isUndefined())
            {
                mouseMoveRInfo->close();
            }else{
                mouseMoveRInfo->show();
            }
            //获取数据
            arrayFirst  = jsonvalue.toObject().value("dataList").toArray();
            if(arrayFirst.size()>0)
            {
                array =arrayFirst[0].toObject().value("minuteList").toArray();
                if(event->pos().x()>(this->width()-60)*array.size()/242+30)
                {
                    mouseMoveRInfo->close();
                }else
                {
                    for(int i =0;i<array.size()-1;i++)
                    {
                        int x_nowPrice_loc =(this->width()-60)*i/242+30;
                        int x_nowPrice_next_loc =(this->width()-60)*(i+1)/242+30;
                        int x_d = (x_nowPrice_next_loc-x_nowPrice_loc)/2;
                        QRect mouseRect(x_nowPrice_loc-x_d,0,x_nowPrice_loc+x_d,this->height());
                        if(mouseRect.contains(event->pos()))
                        {
                            QString nowPrice =  array[i].toObject().value("nowPrice").toString();
                            QString time =  array[i].toObject().value("time").toString();
                            QString avgPrice =  array[i].toObject().value("avgPrice").toString();
                            QString volume =  array[i].toObject().value("volume").toString();
                            mouseMoveRTime->setText("时    间:"+time[0]+time[1]+"时"+time[2]+time[3]+"分");
                            mouseMoveRNowPrice->setText("当前价格:"+nowPrice+"元");
                            mouseMoveRAvgPrice->setText("平均价格:"+avgPrice+"元");
                            mouseMoveRVolume->setText("成 交 量:"+volume);
                            if(event->pos().x()<=180&&event->pos().y()<=150)
                            {
                                mouseMoveRInfo->setGeometry(this->width()-170,30,140,80);

                            }else{
                                mouseMoveRInfo->setGeometry(30,30,140,80);
                            }
                        }
                    }
                }
            }
        }
        if(krButtonString() =="K线图")
        {
            mouseMoveRInfo->close();
            jsonvalue = stockDataGet::stockKLine(string);
            if(jsonvalue.isUndefined())
            {
                mouseMoveKInfo->close();
            }else{
                mouseMoveKInfo->show();
            }
            //获取窗口长宽
            int width = this->width();
            int height = this->height();

            //获取数据
            array =jsonvalue.toObject().value("list").toArray();
            if(array.size()<40)
            {
                 width = (width-30)/40*array.size();
            }
            if(event->pos().x()>width)
            {
                mouseMoveKInfo->close();
            }
            else
            {
                //数据点距离
                for(int i = array.size()-1;i>=0;i--)
                {
                    int x_width = (width-30)/array.size();
                    int x_d = x_width/2-1;
                    int x_loc = x_width/2-1+(i*(width-30)/array.size());

                    QRect mouseRect(x_loc-x_d,0,x_d*2,height);
                    if(mouseRect.contains(event->pos()))
                    {
                        QString max = array[array.size()-1-i].toObject().value("max_price").toString();
                        QString min = array[array.size()-1-i].toObject().value("min_price").toString();
                        QString close = array[array.size()-1-i].toObject().value("close_price").toString();
                        QString open = array[array.size()-1-i].toObject().value("open_price").toString();
                        QString volumn = array[array.size()-1-i].toObject().value("trade_num").toString();
                        QString minute = array[array.size()-1-i].toObject().value("minute").toString();
                        QString time = array[array.size()-1-i].toObject().value("time").toString();
                        mouseMoveKTime->setText("时  间:"+time+minute);
                        mouseMoveKMax->setText("最高价:"+max+"元");
                        mouseMoveKMin->setText("最低价:"+min+"元");
                        mouseMoveKOpen->setText("开盘价:"+open+"元");
                        mouseMoveKClose->setText("收盘价:"+close+"元");
                        mouseMoveKVolumn->setText("成交量:"+volumn+"手");
                        if(event->pos().x()<this->width()-140)
                        {
                            mouseMoveKInfo->setGeometry(event->pos().x(),event->pos().y(),140,120);
                        }else{
                            mouseMoveKInfo->setGeometry(event->pos().x()-140,event->pos().y(),140,120);
                        }
                    }
                }
            }
        }
    }
}
void image_line_show::enterEvent(QEvent *event)
{
    if(!searchInt.isEmpty())
    {
    if(krButtonString() =="K线图")
    {
        mouseMoveKInfo->show();
    }
    if(krButtonString() =="分时图")
    {
        mouseMoveRInfo->show();
    }
    }
}
void image_line_show::leaveEvent(QEvent *event)
{
    if(krButtonString() =="K线图")
    {
        mouseMoveKInfo->close();
    }
    if(krButtonString() =="分时图")
    {
        mouseMoveRInfo->close();
    }
}

void image_line_show::indexButtonGroupLoc()
{
    ui->image_line_show_MACD_pushButton->setGeometry(QRect(scrollBarValue,this->height()*17/20,40,20));
    ui->image_line_show_RSI_pushButton->setGeometry(QRect(scrollBarValue+40,this->height()*17/20,40,20));
    ui->image_line_show_KDJ_pushButton->setGeometry(QRect(scrollBarValue+80,this->height()*17/20,40,20));
    ui->image_line_show_BRAR_pushButton->setGeometry(QRect(scrollBarValue+120,this->height()*17/20,40,20));
}
void image_line_show::krButtonGroupLoc()
{
    ui->krbutton_widget->setGeometry(QRect(scrollBarValue,0,40,20));
}

void image_line_show::valueGroupLabel()
{
    ui->imagelineshow_value_widget->setGeometry(QRect(scrollBarValue+scrollArea_width-30,0,28,this->height()*8/10));
    ui->imagelineshow_value_widget->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    ui->imagelineshow_value1_label->setGeometry(QRect(0,this->height()*1/10-8,30,15));
    ui->imagelineshow_value2_label->setGeometry(QRect(0,this->height()*2/10-8,30,15));
    ui->imagelineshow_value3_label->setGeometry(QRect(0,this->height()*3/10-8,30,15));
    ui->imagelineshow_value4_label->setGeometry(QRect(0,this->height()*4/10-8,30,15));
    ui->imagelineshow_value5_label->setGeometry(QRect(0,this->height()*5/10-8,30,15));
    ui->imagelineshow_value6_label->setGeometry(QRect(0,this->height()*6/10-8,30,15));
    ui->imagelineshow_value7_label->setGeometry(QRect(0,this->height()*7/10-8,30,15));
//    QJsonValue indexjsonvalue = stockDataGet::stockKIndexData(MAstring);
    ui->imageindexshow_value_widget->setGeometry(QRect(scrollBarValue+scrollArea_width-30,this->height()*17/20,30,this->height()*3/20));
    ui->imageindexshow_value_widget->setStyleSheet("QLabel{color:white;font-size:12px;font:Microsoft YaHei}");
    ui->imageindexshow_value1_label->setGeometry(QRect(0,(this->height()*3/20-18)*0/4+10,30,15));
    ui->imageindexshow_value2_label->setGeometry(QRect(0,(this->height()*3/20-18)*1/4+10,30,15));
    ui->imageindexshow_value3_label->setGeometry(QRect(0,(this->height()*3/20-18)*2/4+10,30,15));
    ui->imageindexshow_value4_label->setGeometry(QRect(0,(this->height()*3/20-18)*3/4+10,30,15));
    ui->imageindexshow_value5_label->setGeometry(QRect(0,(this->height()*3/20-18)*4/4+6,30,15));
    QJsonValue indexjsonvalue = stockDataGet::stockKIndexData(MAstring);
    QJsonArray jsonarray = indexjsonvalue.toArray();
    if(jsonarray.size()>=150)
    {
        ui->imageindexshow_value_widget->setGeometry(QRect(scrollBarValue+scrollArea_width-30,this->height()*17/20,30,this->height()*3/20-20));
        ui->imageindexshow_value1_label->setGeometry(QRect(0,(this->height()*3/20-38)*0/4+10,30,15));
        ui->imageindexshow_value2_label->setGeometry(QRect(0,(this->height()*3/20-38)*1/4+10,30,15));
        ui->imageindexshow_value3_label->setGeometry(QRect(0,(this->height()*3/20-38)*2/4+10,30,15));
        ui->imageindexshow_value4_label->setGeometry(QRect(0,(this->height()*3/20-38)*3/4+10,30,15));
        ui->imageindexshow_value5_label->setGeometry(QRect(0,(this->height()*3/20-38)*4/4+6,30,15));
    }



}

void image_line_show::resizeEvent(QResizeEvent *event)
{
    indexButtonGroupLoc();
    valueGroupLabel();
}

void image_line_show::scrollBar()
{
    indexButtonGroupLoc();
    krButtonGroupLoc();
    valueGroupLabel();
}
