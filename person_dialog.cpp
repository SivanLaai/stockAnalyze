#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login_dialog.h"
#include "ui_login_dialog.h"
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

void MainWindow::person_dialog()
{
    //登陆窗口
    d.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);
    d.exec();
}

void login_dialog::register_show_dialog()
{
    //注册窗口
    rd.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);
    rd.exec();

}
