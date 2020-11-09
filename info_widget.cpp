#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>



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
