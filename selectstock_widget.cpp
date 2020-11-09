#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "selectstockwidget.h"
// #include "webgets.h"

void MainWindow::selectStock_widget()
{
    stockWidget.setUserName(d.getUserName());//设置用户名
    connect(ui->option_optionalStock_toolButton,&QToolButton::clicked,this,&MainWindow::selectStockSlot);
    connect(ui->search_optionalStock_pushButton,&QPushButton::clicked,this,&MainWindow::selectStockSlot);
    //connect(ui->search_optionalStock_pushButton,&QPushButton::clicked,[](){});
    connect(&stockWidget,&selectStockWidget::showOptStock,
            [=](QString code){
        imageLineShow.searchInt  = code;
        imageInfoShow.searchInt = code;
        qDebug() << "selectStock_widget" << code;
        // WebGets *web;
        // if(imageLineShow.krButtonString()=="分时图")
        // {
        //     web = new WebTimeLine(code,"");
        // }
        // if(imageLineShow.krButtonString()=="K线图")
        // {
        //    web = new WebRealtimeK(imageLineShow.dateString(),code,imageLineShow.timeString(),imageLineShow.fqString());
        // }
        // imageLineShow.string = web->getResponse();
        // imageLineShow.setParent(ui->image_show_scrollArea);
        // web->destroyed();
        // WebGets *web3;
        // web3 = new WebRealStockinfo(code,"","");
        // imageInfoShow.string = web3->getResponse();
        // imageInfoShow.setParent(ui->image_show_info_widget);
        // web3->destroyed();
        // imageInfoShow.show();
        // imageLineShow.show();
    });
}
void MainWindow::selectStockSlot()
{
    if(!stockWidget.isVisible())
    {
        stockWidget.show();
        emit stockWidget.showStock();
        stockWidget.move(frameGeometry().x()+200,frameGeometry().y()+150);
    }
    else
    {
        stockWidget.hide();
    }
}
