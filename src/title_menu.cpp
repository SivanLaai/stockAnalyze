#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>


void MainWindow::title_menu()
{
    //设置窗口背景色为RGB（34，34，34）
    ui->title_menu_widget->setStyleSheet("background-color:rgb(34,34,34)");

    //绘制顶层菜单栏（位于底部LOGO栏与退出栏中间  大小剩余空间）[暂只做放置，具体功能以后添加]
    //系统菜单
    ui->title_menu_system_toolButton->setText(tr("系统(&S)"));
    QMenu *title_meny_system_toolButton_menu = new QMenu(this);
    QAction *changeUser = title_meny_system_toolButton_menu->addAction(tr("切换用户"));
    QAction *exitSys = title_meny_system_toolButton_menu->addAction(tr("退出"));

    connect(exitSys,&QAction::triggered,this,&MainWindow::QuitWindows);
    connect(changeUser,&QAction::triggered,[=]()
    {
        this->hide();
        stockWidget.hide();
        if(d.exec()==QDialog::Accepted)
        {
            //自选股按钮处理
            ui->info_currentUser_label->setText(QString("当前用户：%1").arg(d.getUserName()));
            stockWidget.setUserName(d.getUserName());
            show();
        }
        else
        {
            exit(0);//退出程序
        }
    });

    ui->title_menu_system_toolButton->setMenu(title_meny_system_toolButton_menu);
    ui->title_menu_system_toolButton->setStyleSheet("QToolButton{color:white;background-color:rgba(34,34,34,0)}:hover{color:yellow}:menu-indicator{image: None}");
    //行情菜单
//    ui->title_menu_news_toolButton->setText(tr("行情(&Q)"));
//    QMenu *title_menu_news_toolButton_menu = new QMenu(this);
//    title_menu_news_toolButton_menu->addAction(tr("沪深指数"));
//    title_menu_news_toolButton_menu->addAction(tr("沪深个股"));

//    ui->title_menu_news_toolButton->setMenu(title_menu_news_toolButton_menu);
//    ui->title_menu_news_toolButton->setStyleSheet("QToolButton{color:white;background-color:rgba(34,34,34,0)}:hover{color:yellow}:menu-indicator{image: None}");

//    ui->title_menu_info_toolButton->setText(tr("咨询(&I)"));
//    QMenu *title_menu_info_toolButton_menu = new QMenu(this);
//    title_menu_info_toolButton_menu->addAction(tr("财经热点"));
//    title_menu_info_toolButton_menu->addAction(tr("市场动态"));
//    ui->title_menu_info_toolButton->setStyleSheet("QToolButton{color:white;background-color:rgba(34,34,34,0)}:hover{color:yellow}:menu-indicator{image: None}");

//   ui->title_menu_info_toolButton->setMenu(title_menu_info_toolButton_menu);

}
