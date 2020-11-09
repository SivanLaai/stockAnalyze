#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPalette>
#include "frameless.h"
#include "selectstockwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);
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
    pHelper->activateOn(this);  //激活当前窗体
    pHelper->setTitleHeight(ui->title_widget->height());  //设置窗体的标题栏高度
    pHelper->setWidgetMovable(true);  //设置窗体可移动
    pHelper->setWidgetResizable(true);  //设置窗体可缩放
    pHelper->setRubberBandOnMove(false);  //设置橡皮筋效果-可移动
    pHelper->setRubberBandOnResize(false);  //设置橡皮筋效果-可缩放

    //加载登录界面
    if(d.exec()==QDialog::Accepted)
    {
        //自选股按钮处理

        ui->info_currentUser_label->setText(QString("当前用户：%1").arg(d.getUserName()));
        selectStock_widget();
        show();
    }
    else
    {
        exit(0);//退出程序
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
