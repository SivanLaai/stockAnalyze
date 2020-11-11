#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>


void MainWindow::title_quitbox_widget()
{
    //设置退出栏背景（rgb（34，34，34））
    ui->title_quitbox_widget->setStyleSheet("background-color:rgb(34,34,34)");
    //退出栏图标载入
    minIcon = QPixmap(tr(":/icon/img/min20.png"));
    maxIcon = QPixmap(tr(":/icon/img/max20.png"));
    quitIcon = QPixmap(tr(":/icon/img/quit20.png"));
    returnIcon = QPixmap(tr(":/icon/img/return20.png"));
    //设置最小化、关闭按钮图标
    ui->title_quitbox_min_toolButton->setIcon(minIcon);
    ui->title_quitbox_quit_toolButton->setIcon(quitIcon);
    if(!this->isMaximized())//判断是否最大化
    ui->title_quitbox_max_toolButton->setIcon(maxIcon);
    else
    ui->title_quitbox_max_toolButton->setIcon(returnIcon);
    //设置最小化，最大化，退出，样式
    ui->title_quitbox_min_toolButton->setStyleSheet("background-color:transparent");
    ui->title_quitbox_max_toolButton->setStyleSheet("background-color:transparent");
    ui->title_quitbox_quit_toolButton->setStyleSheet("background-color:transparent");
    //最小化，最大化，退出提示
    ui->title_quitbox_min_toolButton->setToolTip(tr("最小化"));
    ui->title_quitbox_max_toolButton->setToolTip(tr("最大化"));
    ui->title_quitbox_quit_toolButton->setToolTip(tr("退出"));
    //最小化，最大化，退出功能实现
    connect(ui->title_quitbox_quit_toolButton, SIGNAL(clicked()), this, SLOT(QuitWindows()));
    connect(ui->title_quitbox_min_toolButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui->title_quitbox_max_toolButton,SIGNAL(clicked()),this,SLOT(MaxAndNormal()));


}
