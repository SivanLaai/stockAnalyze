#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QImage>
#include <QPainter>



void MainWindow::title_logo()
{
    //绘制LOGO（单图，图片位于资源文件）
    QImage logo_image;
    logo_image.load(":/icon/img/LOGO.png");
    QPainter logo_Pai(&logo_image);
    QImage logo_ResultImg = logo_image.scaled(ui->title_logo_label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->title_logo_label->setPixmap(QPixmap::fromImage(logo_ResultImg));
}
