#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPalette>
#include "frameless.h"
#include "selectstockwidget.h"
#include "mainwindow.h"
#include "ui_image_line_show.h"
#include "opstring.h"
#include "stockDataGet.h"
#include "QWidget"
#include "image_line_show.h"

#include <QTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    //载入各窗口部件
    //标题栏窗口载入
    title_widget();
    title_menu();
    title_logo();
    title_quitbox_widget();

    //选项栏窗口载入
    option_widget();

    //搜索栏窗口载入
    search_widget();

    //图表栏窗口载入
    image_widget();
    image_info();

    //资讯栏窗口载入
    info_widget();

    //检测鼠标状态,设置在不按鼠标的情况下也触发鼠标移动事件
    this->setMouseTracking(true);

    //主窗口界面 边缘拉伸，标题拖动
    FramelessHelper *pHelper = new FramelessHelper(this);
    pHelper->activateOn(this);                           //激活当前窗体
    pHelper->setTitleHeight(ui->title_widget->height()); //设置窗体的标题栏高度
    pHelper->setWidgetMovable(true);                     //设置窗体可移动
    pHelper->setWidgetResizable(true);                   //设置窗体可缩放
    pHelper->setRubberBandOnMove(false);                 //设置橡皮筋效果-可移动
    pHelper->setRubberBandOnResize(false);               //设置橡皮筋效果-可缩放

    //加载登录界面
    if (d.exec() == QDialog::Accepted)
    {
        //自选股按钮处理

        ui->info_currentUser_label->setText(QString("当前用户：%1").arg(d.getUserName()));
        selectStock_widget();
        show();
    }
    else
    {
        exit(0); //退出程序
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::search_widget()
{
    //载入登陆栏按钮
    search_person_widget();
    //载入搜索栏背景色（rgb(15,15,15)）
    ui->search_widget->setStyleSheet("background-color:rgb(15,15,15)");

    //搜索文本框
    ui->search_lineEdit->setStyleSheet("background-color:white");
    ui->search_lineEdit->setPlaceholderText(tr("请输入搜索信息"));
    ui->search_lineEdit->setToolTip(tr("可输入股票代码，股票名称，板块名称"));

    //搜索栏按钮
    /*自选股*/
    ui->search_optionalStock_pushButton->setText(tr("自选股"));
    ui->search_optionalStock_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*全景图*/
    ui->search_panorama_pushButton->setText(tr(NULL));
    ui->search_panorama_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*沪深个股*/
    ui->search_HuShenGeGu_pushButton->setText(tr(NULL));
    ui->search_HuShenGeGu_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*沪深板块*/
    ui->search_HuShenBanKuai_pushButton->setText(tr(NULL));
    ui->search_HuShenBanKuai_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*资讯中心*/
    ui->search_infocentral_pushButton->setText(tr(NULL));
    ui->search_infocentral_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    //搜索槽

    connect(ui->search_lineEdit, SIGNAL(returnPressed()), this, SLOT(search_getIn_slot()));
}

void MainWindow::search_getIn_slot()
{
    QString code = ui->search_lineEdit->text();
    imageLineShow.searchInt = code;
    imageInfoShow.searchInt = code;

    if (imageLineShow.krButtonString() == "分时图")
    {
        if (imageLineShow.realTimeStock != NULL && code != imageLineShow.realTimeStock->getCode())
        {
            imageLineShow.realTimeStock->workerThread.exit();
            imageLineShow.realTimeStock = NULL;
        }
        if (imageLineShow.realTimeStock == NULL)
        {
            imageLineShow.realTimeStock = new QWebRealTimeStock(code);
        }
        imageLineShow.realTimeStock->setString(&imageLineShow);
    }
    if (imageLineShow.krButtonString() == "K线图")
    {
        QWebData *web;
        web = new QWebKLine("600004", "2015-08-01", "2015-09-01");
        imageLineShow.string = web->getResponse();
    }
    //    if (web == NULL)
    //    {
    //        return;
    //    }
    //    web->getResponse();
    // imageLineShow.string = web->getResponse();
    // imageLineShow.setParent(ui->image_show_scrollArea);
    //     web->destroyed();
    // WebGets *web3;
    // web3 = new WebRealStockinfo(ui->search_lineEdit->text(),"","");
    // imageInfoShow.string = web3->getResponse();
    // imageInfoShow.setParent(ui->image_show_info_widget);
    // web3->destroyed();
    // imageInfoShow.show();
    // imageLineShow.show();
}

void MainWindow::info_widget()
{
    //载入搜索栏背景色（rgb(35,35,35)）
    ui->info_widget->setStyleSheet("background-color:rgb(35,35,35)");

    //时间
    ui->info_time_info_label->setText(tr("当前时间："));
    ui->info_time_info_label->setStyleSheet("color:green");
    ui->info_currentUser_label->setStyleSheet("color:green");

    //时间计时
    timerEvent(0);
    startTimer(1000);
    ui->info_time_label->setStyleSheet("color:green");
}
