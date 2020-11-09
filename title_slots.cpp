#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QRect>
#include <QWidget>
//退出程序槽
void MainWindow::QuitWindows()
{
    //绘制退出对话框
    QDialog *quitDialog = new QDialog(this);
    quitDialog->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    quitDialog->setMinimumSize(300,200);
    quitDialog->setMaximumSize(300,200);
    quitDialog->setStyleSheet("background-color:black");

    //退出确认
    QPushButton *quitOk = new QPushButton(quitDialog);
    quitOk->setStyleSheet("QPushButton{color:white;background-color:rgb(34,34,34)}:hover{color:red}");
    quitOk->setText(tr("确认"));
    quitOk->setGeometry(150,150,60,20);

    //点按推出对话框的确认按钮
    connect(quitOk,&QPushButton::clicked,
            [=]()
    {
        exit(0);//退出窗口
    });//this,SLOT(close()));

    //退出取消
    QPushButton *quitCancel = new QPushButton(quitDialog);
    quitCancel->setStyleSheet("QPushButton{color:white;background-color:rgb(34,34,34)}:hover{color:red}");
    quitCancel->setText(tr("取消"));
    quitCancel->setGeometry(210,150,60,20);
    connect(quitCancel,SIGNAL(clicked(bool)),quitDialog,SLOT(close()));

    //对话框标题栏
    QWidget *quitTitleWidget = new QWidget(quitDialog);
    quitTitleWidget->setGeometry(0,0,300,20);

    QPushButton *quitQuit = new QPushButton(quitTitleWidget);
    quitIcon = QPixmap(tr(":/icon/img/quit20.png"));
    quitQuit->setIcon(quitIcon);
    quitQuit->setMaximumSize(20,20);
    quitQuit->setGeometry(280,0,20,20);
    connect(quitQuit,SIGNAL(clicked(bool)),quitDialog,SLOT(close()));

    QLabel *quitTitleLabel = new QLabel(quitTitleWidget);
    quitTitleLabel->setGeometry(0,0,240,20);
    quitTitleLabel->setText("确认退出");

    quitTitleWidget->setStyleSheet("color:white;background-color:rgb(34,34,34)");

    QLabel *quitInfoLabel = new QLabel(quitDialog);
    quitInfoLabel->setGeometry(60,80,200,50);
    quitInfoLabel->setStyleSheet("color:white;font-size:20px");
    quitInfoLabel->setText(tr("是否确认退出程序？"));
    quitDialog->exec();
}

//最大化还原槽
void MainWindow::MaxAndNormal()
{
    if(!this->isMaximized())
    {
        this->showMaximized();
        ui->title_quitbox_max_toolButton->setIcon(returnIcon);
        ui->title_quitbox_max_toolButton->setToolTip(tr("向下还原"));
    }
    else
    {
        this->showNormal();
        ui->title_quitbox_max_toolButton->setIcon(maxIcon);
        ui->title_quitbox_max_toolButton->setToolTip(tr("最大化"));
    }
}
