#ifndef IMAGE_LINE_SHOW_H
#define IMAGE_LINE_SHOW_H
#include <QWidget>
#include <QAbstractButton>
#include <QLabel>
#include <QJsonValue>
#include <QJsonArray>
#include <QScrollArea>
#include "qwebdata.h"
namespace Ui
{
    class image_line_show;
}

class image_line_show : public QWidget
{
    Q_OBJECT
public:
    QString getCode();

public:
    explicit image_line_show(QWidget *parent = 0);
    ~image_line_show();
    QVector<double> realKlineVec; //分时线
    QWebRealTimeStock *realTimeStock;
    QString string;                    //json字符串
    QString MAstring;                  //MA json字符串
    QString timeButtonString();        //按钮按下状态
    QString krButtonString();          //分K按钮状态
    QString fqButtonString();          //复权按钮状态
    QString indexButtonString();       //指标按钮状态
    QString timeString();              //时间
    QString fqString();                //复权
    QString realdayString();           //分时线时间
    QString dateString();              //起始时间
    void buttonChangeRepaint();        //重绘
    void kButtonState();               //K线图按钮状态
    void rButtonState();               //分时图按钮状态
    QAbstractButton *krButtonBackedup; //分K按钮备份
    QAbstractButton *fqButtonBackedup; //复权按钮备份
    int scrollBarValue;                //滚动条值

    QJsonValue jsonvalue;
    QJsonArray arrayFirst;
    QJsonArray array;

    QString searchInt;

    QWidget *mouseMoveKInfo;
    QLabel *mouseMoveKTime;
    QLabel *mouseMoveKMin;
    QLabel *mouseMoveKMax;
    QLabel *mouseMoveKOpen;
    QLabel *mouseMoveKClose;
    QLabel *mouseMoveKVolumn;

    QWidget *mouseMoveRInfo;
    QLabel *mouseMoveRTime;
    QLabel *mouseMoveRAvgPrice;
    QLabel *mouseMoveRNowPrice;
    QLabel *mouseMoveRVolume;
    void indexButtonGroupLoc();
    void krButtonGroupLoc();
    void valueGroupLabel();

    int scrollArea_width;
    int scrollArea_heigth;
    QScrollArea kScrollArea;
    QWidget kScrollWidget;
    QLabel kScrollLabel;

public slots:
    void scrollBar();
signals:
    void scrollBarValueChange();

private slots:
    void on_image_line_show_beganday_dateEdit_editingFinished();
    void on_image_line_show_30min_pushButton_clicked();
    void on_image_line_show_5min_pushButton_clicked();
    void on_image_line_show_k_pushButton_clicked();
    void on_image_line_show_60min_pushButton_clicked();
    void on_image_line_show_kDay_pushButton_clicked();
    void on_image_line_show_kWeek_pushButton_clicked();
    void on_image_line_show_kMonth_pushButton_clicked();
    void on_image_line_show_bfq_pushbutton_clicked();
    void on_image_line_show_qfq_pushbutton_clicked();
    void on_image_line_show_real_pushButton_pressed();

private:
    Ui::image_line_show *ui;
    void paintEvent(QPaintEvent *event);     //绘制事件
    void image_line_show_button();           //图标框按钮
    void mouseMoveEvent(QMouseEvent *event); //鼠标移动事件
    void enterEvent(QEvent *event);          //进入窗口事件
    void leaveEvent(QEvent *event);          //离开窗口事件
    void resizeEvent(QResizeEvent *event);   //窗口大小改变事件
};

#endif // IMAGE_LINE_SHOW_H
