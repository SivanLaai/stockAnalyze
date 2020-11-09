#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

void MainWindow::timerEvent(QTimerEvent *event)
{
    //咨询栏时间
    Q_UNUSED(event);
    ui->info_time_label->setText(QTime::currentTime().toString("hh:mm:ss"));
}
