#include "selectstockwidget.h"
#include "ui_selectstockwidget.h"
#include "frameless.h"
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>

selectStockWidget::selectStockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectStockWidget)
{
    ui->setupUi(this);
    setWindowTitle("自选股窗口");
    //背景样式
    setStyleSheet("background-color:rgb(20,20,20)");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);
    ui->stock_label->setStyleSheet("color:white");
    ui->stock_label->setFont(QFont("微软雅黑"));
    ui->stock_label->setAttribute(Qt::WA_Disabled);
    FramelessHelper *pHelper = new FramelessHelper(this);
    pHelper->activateOn(this);  //激活当前窗体
    pHelper->setWidgetMovable(true);  //设置窗体可移动
    pHelper->setWidgetResizable(false);  //设置窗体可缩放
    pHelper->setRubberBandOnMove(false);  //设置橡皮筋效果-可移动
    pHelper->setRubberBandOnResize(false);  //设置橡皮筋效果-可缩放

    //退出按钮
    QPixmap quitIcon = QPixmap(tr(":/icon/img/quit20.png"));
    ui->quitToolButton->setIcon(quitIcon);
    ui->quitToolButton->setStyleSheet("background-color:transparent");
    //
    ui->quitToolButton->setToolTip(tr("退出登录"));
    connect(ui->quitToolButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    //上下页按钮
    ui->prePageButton->setStyleSheet("color:white");
    ui->nextPageButton->setStyleSheet("color:white");
    //刷新按钮
    ui->updateToolButton->setStyleSheet("color:white");
    //添加自选股按钮
    ui->addStockButton->setStyleSheet("color:white");
    //自选股输入框背景
    ui->codeLineEdit->setStyleSheet("background-color:white");

    //窗口的表格样式
    ui->tableWidget->setStyleSheet("QHeaderView::section{"
                                   "color:white;"
                                   "background-color:rgb(30,30,30);}");
    ui->tableWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->resizeColumnToContents(7);
    ui->tableWidget->horizontalHeader()->setStyleSheet("background-color: rgb(20,20,20);");
    ui->tableWidget->verticalHeader()->setStyleSheet("background-color: rgb(20,20,20);");
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);  //少这句，右键没有任何反应的。
    //创建右键菜单
    rightMenu = new QMenu;
    openAction = new QAction("打开");
    delAction = new QAction("删除");

    rightMenu->addAction(openAction);
    rightMenu->addAction(delAction);

    //
    //处理右键信号
    connect(ui->tableWidget,&QTableWidget::customContextMenuRequested,
            [=](QPoint pos)
    {
        mousePos = pos;
        int currentXpos = geometry().x()+pos.x();
        int currentYpos = geometry().y()+pos.y()+65;
        rightMenu->move(currentXpos,currentYpos);
        rightMenu->exec();
    });
    //处理菜单按钮的操作
    //打开按钮
    connect(openAction,&QAction::triggered,
            [=]()
    {
        int currentRow = ui->tableWidget->itemAt(mousePos)->row();
        QString code = ui->tableWidget->item(currentRow,0)->text();//得到选中的股票代码
        emit showOptStock(code);
    });
    //删除按钮
    //初始化删除提示框
    QMessageBox *warnDlg = new QMessageBox(QMessageBox::Information,"提示","<font color=#FFFFFF>确定删除该自选股吗？</font>",NULL,NULL);
    warnDlg->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);
    warnDlg->setStyleSheet("background-color:rgb(60,60,60)");
    QAbstractButton *okButton = (QAbstractButton *)warnDlg->addButton(QMessageBox::Ok);
    QAbstractButton *cancelButton = (QAbstractButton *)warnDlg->addButton(QMessageBox::Cancel);
    okButton->setText("确定");
    cancelButton->setText("取消");
    okButton->setStyleSheet("QPushButton{color:white}");
    cancelButton->setStyleSheet("QPushButton{color:white}");

    connect(delAction,&QAction::triggered,
            [=]()
    {
        warnDlg->move(this->geometry().x()+300,this->geometry().y()+200);

        QModelIndexList selectedRowsIndex = ui->tableWidget->selectionModel()->selectedRows();//得到当前选中行
        QTableWidgetItem *currentItem = ui->tableWidget->item(selectedRowsIndex[0].row(),0);
        QString code;
        if(currentItem&&QMessageBox::Ok == warnDlg->exec())//点按按钮进行删除处理
        {
            foreach(const QModelIndex& index,selectedRowsIndex)//遍历选中的所有行
            {
                currentItem = ui->tableWidget->item(index.row(),0);
                if(currentItem)
                {
                    code = currentItem->text();//得到选中的股票代码
                    optStock.deleteOptionalStock(code);
                }
            }
            updateStockTable(ui->tableWidget,optStock);
        }

    });


    //处理显示自选股窗口的信号
    connect(this,&selectStockWidget::showStock,
            [=]()
    {
        updateStockTable(ui->tableWidget,optStock);
    });

    //处理删除按钮
    connect(ui->tableWidget,&QTableWidget::itemClicked,
            [=](const QTableWidgetItem *item)
    {
        warnDlg->move(this->geometry().x()+300,this->geometry().y()+200);

        if(item->column()==7&&QMessageBox::Ok == warnDlg->exec())//点按按钮进行删除处理
        {
            QString code = ui->tableWidget->item(item->row(),0)->text();
            optStock.deleteOptionalStock(code);
            updateStockTable(ui->tableWidget,optStock);
        }

    });

    connect(ui->tableWidget,&QTableWidget::itemDoubleClicked,
            [=](const QTableWidgetItem *item)
    {
        if(item->column()!=7)//双击不是7的列
        {
            QString code = ui->tableWidget->item(item->row(),0)->text();
            emit showOptStock(code);
        }
    });

}
void selectStockWidget::mousePressEvent(QMouseEvent *e)
{
    ui->tableWidget->clearSelection();
}

void selectStockWidget::updateStockTable(QTableWidget *tableWidget,QWebOptionalStock &optStock)
{
    //初始化好自选股的个数
    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(optStock.getResponse().toUtf8(), &jsonError);
    QString returnStr("");
    QStringList optStockList;
    if(jsonError.error == QJsonParseError::NoError)
    {
    if(json.isArray())
    {
        QJsonArray optStockArr = json.array();//返回的数据是json数组
        int arrSize = optStockArr.size();
        for(int i=0; i<arrSize; i++)
        {
            returnStr.clear();
            returnStr.append(optStockArr[i].toObject()["code"].toString()+",");
            returnStr.append(optStockArr[i].toObject()["name"].toString()+",");
            returnStr.append(optStockArr[i].toObject()["nowPrice"].toString()+",");
            returnStr.append(optStockArr[i].toObject()["diff_rate"].toString()+",");
            returnStr.append(optStockArr[i].toObject()["todayMax"].toString()+",");
            returnStr.append(optStockArr[i].toObject()["todayMin"].toString()+",");
            returnStr.append(optStockArr[i].toObject()["openPrice"].toString());
            optStockList.append(returnStr);
        }

    }
    }
    int realRowCount = optStockList.size();
    int viewRowCount = tableWidget->rowCount();
    if(0 == realRowCount)
    {
        tableWidget->setRowCount(0);
        tableWidget->setRowCount(10);
    }
    else if(0 == realRowCount%10)
    {
        tableWidget->setRowCount(realRowCount);
    }
    else
    {
        if(realRowCount<viewRowCount)
            tableWidget->setRowCount(realRowCount);//消除已删除的条目
        tableWidget->setRowCount(10*(realRowCount/10+1));
    }
    QString stockCodeStr;
    QString stockNameStr;
    QString latestPriceStr;
    QString diffRateStr;
    QString maxPriceStr;
    QString minPriceStr;
    QString openPriceStr;
    for(int i = 0; i < realRowCount; i++)//根据拿到的自选股信息
    {
        if(optStockList[i].split(",").size()==7)
        {
            stockCodeStr = (QString)(optStockList[i]).split(",")[0];
            stockNameStr = (QString)(optStockList[i]).split(",")[1];
            latestPriceStr = (QString)(optStockList[i]).split(",")[2];
            diffRateStr = (QString)(optStockList[i]).split(",")[3];
            maxPriceStr = (QString)(optStockList[i]).split(",")[4];
            minPriceStr = (QString)(optStockList[i]).split(",")[5];
            openPriceStr = (QString)(optStockList[i]).split(",")[6];
        }
        else
        {
            stockNameStr = "-";
            latestPriceStr = "-";
            diffRateStr = "-";
            maxPriceStr = "-";
            minPriceStr = "-";
            openPriceStr = "-";
        }

        QTableWidgetItem *stockCode = new QTableWidgetItem(stockCodeStr);
        QTableWidgetItem *stockName = new QTableWidgetItem(stockNameStr);
        QTableWidgetItem *latestPrice = new QTableWidgetItem(latestPriceStr);
        QTableWidgetItem *diffRate = new QTableWidgetItem(diffRateStr);
        QTableWidgetItem *maxPrice = new QTableWidgetItem(maxPriceStr);
        QTableWidgetItem *minPrice = new QTableWidgetItem(minPriceStr);
        QTableWidgetItem *openPrice = new QTableWidgetItem(openPriceStr);
        QTableWidgetItem *delButton = new QTableWidgetItem(QIcon(QPixmap(tr(":/icon/img/delete1.png"))),"");

        stockCode->setBackgroundColor(QColor(0,0,0));
        stockName->setBackgroundColor(QColor(0,0,0));
        latestPrice->setBackgroundColor(QColor(0,0,0));
        diffRate->setBackgroundColor(QColor(0,0,0));
        maxPrice->setBackgroundColor(QColor(0,0,0));
        minPrice->setBackgroundColor(QColor(0,0,0));
        openPrice->setBackgroundColor(QColor(0,0,0));
        delButton->setBackgroundColor(QColor(0,0,0));
        delButton->setFlags(Qt::ItemIsEnabled);
        //设置表格字体颜色

        //涨跌幅正负时颜色有所变化
        if(diffRateStr.toFloat()>0.0)
        {
            diffRate->setTextColor(QColor(255,0,0));
            latestPrice->setTextColor(QColor(255,0,0));
            maxPrice->setTextColor(QColor(255,0,0));
            minPrice->setTextColor(QColor(255,0,0));
            openPrice->setTextColor(QColor(255,0,0));
            delButton->setTextColor(QColor(255,0,0));
            stockCode->setTextColor(QColor(255,0,0));//股票代码
            stockName->setTextColor(QColor(255,0,0));//股票名称
        }
        else if(diffRateStr.toFloat()<0.0)
        {
            diffRate->setTextColor(QColor(0,255,0));
            latestPrice->setTextColor(QColor(0,255,0));
            maxPrice->setTextColor(QColor(0,255,0));
            minPrice->setTextColor(QColor(0,255,0));
            openPrice->setTextColor(QColor(0,255,0));
            delButton->setTextColor(QColor(0,255,0));
            stockCode->setTextColor(QColor(0,255,0));//股票代码
            stockName->setTextColor(QColor(0,255,0));//股票名称
        }
        else
        {
            diffRate->setTextColor(QColor(255,255,255));
            latestPrice->setTextColor(QColor(255,255,255));
            maxPrice->setTextColor(QColor(255,255,255));
            minPrice->setTextColor(QColor(255,255,255));
            openPrice->setTextColor(QColor(255,255,255));
            delButton->setTextColor(QColor(255,255,255));
            stockCode->setTextColor(QColor(255,255,255));//股票代码
            stockName->setTextColor(QColor(255,255,255));//股票名称
        }

        stockCode->setTextAlignment(Qt::AlignCenter);
        stockName->setTextAlignment(Qt::AlignCenter);
        latestPrice->setTextAlignment(Qt::AlignCenter);
        diffRate->setTextAlignment(Qt::AlignCenter);
        maxPrice->setTextAlignment(Qt::AlignCenter);
        minPrice->setTextAlignment(Qt::AlignCenter);
        openPrice->setTextAlignment(Qt::AlignCenter);
        delButton->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 0, stockCode);
        tableWidget->setItem(i, 1, stockName);
        tableWidget->setItem(i, 2, latestPrice);
        tableWidget->setItem(i, 3, diffRate);
        tableWidget->setItem(i, 4, maxPrice);
        tableWidget->setItem(i, 5, minPrice);
        tableWidget->setItem(i, 6, openPrice);
        tableWidget->setItem(i, 7, delButton);
    }
}

QString selectStockWidget::getUserName()
{
    return userName;
}

void selectStockWidget::setUserName(QString userName)
{
    this->userName = userName;
    optStock.setUserName(userName);
}

selectStockWidget::~selectStockWidget()
{
    delete ui;
}

void selectStockWidget::on_updateToolButton_clicked()
{
    updateStockTable(ui->tableWidget,optStock);
}

void selectStockWidget::on_addStockButton_clicked()
{
    optStock.addOptionalStock(ui->codeLineEdit->text());
    updateStockTable(ui->tableWidget,optStock);
}

void selectStockWidget::on_nextPageButton_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值
    int nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

    if(nCurScroller<maxValue)
        ui->tableWidget->verticalScrollBar()->setSliderPosition(10 + nCurScroller);
    else
        ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}

void selectStockWidget::on_prePageButton_clicked()
{
    int nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

    int pos = nCurScroller - 10;
    if(pos>=0)
        ui->tableWidget->verticalScrollBar()->setSliderPosition(pos);
    else
        ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}
