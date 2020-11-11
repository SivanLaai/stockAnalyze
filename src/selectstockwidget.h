#ifndef SELECTSTOCKWIDGET_H
#define SELECTSTOCKWIDGET_H

#include <QWidget>
#include "qwebdata.h"
#include <QTableWidget>
#include <QMenu>
#include <QAction>

namespace Ui {
class selectStockWidget;
}

class selectStockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit selectStockWidget(QWidget *parent = 0);
    ~selectStockWidget();
    QString getUserName();
    void setUserName(QString userName);
    //实现从服务器读取相应的自选股信息到本地
    void updateStockTable(QTableWidget *tableWidget,QWebOptionalStock &optStock);
protected:
    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_updateToolButton_clicked();  
    void on_addStockButton_clicked();

    void on_nextPageButton_clicked();

    void on_prePageButton_clicked();

private:
    Ui::selectStockWidget *ui;
    QString userName;
    QWebOptionalStock optStock;//已经初始化好了用户名，用来获取自选股列表
    //菜单
    QPoint mousePos;//呼出菜单的时候鼠标的位置
    QMenu *rightMenu;//右键菜单
    QAction *openAction;
    QAction *delAction;//删除按钮
signals:
    void showStock();
    void showOptStock(QString code);//主界面显示股票K线的信号
};

#endif // SELECTSTOCKWIDGET_H
