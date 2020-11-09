#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "login_dialog.h"
#include "image_line_show.h"
#include "image_info_show.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMouseEvent>
#include <QRect>
#include <QPushButton>
#include <QPixmap>
#include <QPoint>
#include <QtGui>
#include "register_dialog.h"
#include "selectstockwidget.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    login_dialog d;
    selectStockWidget stockWidget;
    image_line_show imageLineShow;
    image_info_show imageInfoShow;


    //获取窗口大小
    int window_wide;
    int window_heigh;

    //获取图标
    QPixmap minIcon;
    QPixmap maxIcon;
    QPixmap quitIcon;
    QPixmap returnIcon;
    QPixmap personIcon;
    QPixmap personGroupIcon;
    QPixmap extraFunctionIcon;
    //标题栏菜单
    QMenu* title_qmenu[10];
    QAction* title_act[10];
    QMenuBar* title_menuBar ;
    QStatusBar* title_status ;


    //搜索栏数据
    static QString searchInt;

private:
    Ui::MainWindow *ui;

    void title_widget();//标题栏窗口
    void title_quitbox_widget();//退出栏窗口
    void title_logo();//标题栏LOGO
    void title_menu();//标题栏菜单
    void option_widget();//选项栏窗口
    void search_widget();//搜索栏窗口
    void search_person_widget();//搜索栏登陆窗口
    void image_widget();//图线框窗口
    void info_widget();//资讯栏窗口
    void person_dialog();//登陆窗口
    void image_info();//图线框股票信息窗口
    void selectStock_widget();//自选股处理


protected:
    void mouseDoubleClickEvent(QMouseEvent *event);//双击事件，暂用于标题窗口双击放大还原
    void timerEvent(QTimerEvent *event);//时间事件
    void resizeEvent(QResizeEvent *size);//窗口大小改变事件
public slots:
    void scrollBarValue();
private slots:
    void QuitWindows();//退出槽
    void MaxAndNormal();//最大化还原槽
    void person_dialog_slot();//登陆窗口槽
    void search_getIn_slot();//搜索框槽
    void selectStockSlot();//自选股槽函数
};



#endif // MAINWINDOW_H
