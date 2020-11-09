#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_image_line_show.h"
#include "opstring.h"
#include "stockDataGet.h"
#include "QWidget"
#include "image_line_show.h"
// #include "webgets.h"
void MainWindow::search_widget()
{
    //载入登陆栏按钮
    search_person_widget();
    //载入搜索栏背景色（rgb(15,15,15)）
    ui->search_widget->setStyleSheet("background-color:rgb(15,15,15)");

    //搜索文本框
    ui->search_lineEdit->setStyleSheet("background-color:white");
    ui->search_lineEdit->setPlaceholderText(tr("请输入搜索信息"));
    ui->search_lineEdit->setToolTip(tr("可输入股票代码，股票名称，板块名称"));

    //搜索栏按钮
    /*自选股*/
    ui->search_optionalStock_pushButton->setText(tr("自选股"));
    ui->search_optionalStock_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*全景图*/
    ui->search_panorama_pushButton->setText(tr(NULL));
    ui->search_panorama_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*沪深个股*/
    ui->search_HuShenGeGu_pushButton->setText(tr(NULL));
    ui->search_HuShenGeGu_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*沪深板块*/
    ui->search_HuShenBanKuai_pushButton->setText(tr(NULL));
    ui->search_HuShenBanKuai_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    /*资讯中心*/
    ui->search_infocentral_pushButton->setText(tr(NULL));
    ui->search_infocentral_pushButton->setStyleSheet("QPushButton{color:white;background-color:transparent}:hover{color:yellow}");

    //搜索槽

    connect(ui->search_lineEdit, SIGNAL(returnPressed()), this, SLOT(search_getIn_slot()));
}

void MainWindow::search_getIn_slot()
{
    imageLineShow.searchInt = ui->search_lineEdit->text();
    imageInfoShow.searchInt = ui->search_lineEdit->text();
    QWebData *web;
    // if(imageLineShow.krButtonString()=="分时图")
    // {
    //     web = new WebTimeLine(ui->search_lineEdit->text(),"");
    // }
    if (imageLineShow.krButtonString() == "K线图")
    {
        web = new QWebKLine("600004", "2015-08-01", "2015-09-01");
    }
    imageLineShow.string = web->getResponse();
    imageLineShow.setParent(ui->image_show_scrollArea);
    // web->destroyed();
    // WebGets *web3;
    // web3 = new WebRealStockinfo(ui->search_lineEdit->text(),"","");
    // imageInfoShow.string = web3->getResponse();
    // imageInfoShow.setParent(ui->image_show_info_widget);
    // web3->destroyed();
    // imageInfoShow.show();
    // imageLineShow.show();
}
