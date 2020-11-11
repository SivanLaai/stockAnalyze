#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>


void MainWindow::option_widget()
{
    //设置选项栏背景色（背景色为rgb(20,20,20)）
    ui->option_widget->setStyleSheet("background-color:rgb(20,20,20)");
    //设置选项栏布局



    //设置选项栏按钮样式
    /*首页*/
    ui->option_returnHomepage_pushButton->setText(tr("首页"));
    ui->option_returnHomepage_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*自选股*/
    ui->option_optionalStock_toolButton->setText(tr("自选股"));
    ui->option_optionalStock_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    ui->option_optionalStock_toolButton->setStyleSheet("QToolButton{border:none;color:white}:hover{color:yellow}");
    QMenu *optionalStockMenu;
    optionalStockMenu = new QMenu(this);
    optionalStockMenu->addAction(tr("条件选股"));
    optionalStockMenu->addAction(tr("设置自选股"));
    ui->option_optionalStock_toolButton->setMenu(optionalStockMenu);


    /*备用1*/
    ui->option_backup_toolButton_1->setStyleSheet("QToolButton{border:none;color:white}:hover{color:yellow}");

    /*备用2*/
    ui->option_backup_toolButton_2->setStyleSheet("QToolButton{border:none;color:white}:hover{color:yellow}");

    /*备用3*/
    ui->option_backup_toolButton_3->setStyleSheet("QToolButton{border:none;color:white}:hover{color:yellow}");

    /*备用4*/
    ui->option_backup_toolButton_4->setStyleSheet("QToolButton{border:none;color:white}:hover{color:yellow}");




}
