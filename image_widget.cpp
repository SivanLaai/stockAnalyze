#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_line_show.h"
#include <QApplication>
#include "painter.h"
#include <QDebug>
#include <QScrollBar>
void MainWindow::image_widget()
{
    imageLineShow.scrollArea_heigth= ui->image_show_scrollArea->height();
    imageLineShow.scrollArea_width= ui->image_show_scrollArea->width();
    ui->image_show_scrollArea->setWidgetResizable(true);
    ui->image_show_scrollArea->setWidget(&imageLineShow);
    imageLineShow.scrollBarValue = ui->image_show_scrollArea->horizontalScrollBar()->value();
    connect(ui->image_show_scrollArea->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(scrollBarValue()));
    //载入搜索栏背景色（rgb(7,7,7)）
    ui->image_widget->setStyleSheet("background-color:rgb(7,7,7)");

    //图表框测试
    imageLineShow.setParent(ui->image_show_scrollArea);
    imageLineShow.setMouseTracking(true);

    imageLineShow.setGeometry(0,0,ui->image_show_scrollArea->width(),ui->image_show_scrollArea->height());

}


void MainWindow::resizeEvent(QResizeEvent *size)
{
    imageLineShow.destroyed();
    image_widget();
    imageInfoShow.destroyed();
    image_info();
}

void MainWindow::image_info()
{
    imageInfoShow.setParent(ui->image_show_info_widget);
    imageInfoShow.setGeometry(0,0,ui->image_show_info_widget->width(),ui->image_show_info_widget->height());

}

void MainWindow::scrollBarValue()
{
    imageLineShow.scrollBarValue = ui->image_show_scrollArea->horizontalScrollBar()->value();
    emit imageLineShow.scrollBarValueChange();
}

