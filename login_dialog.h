#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include "register_dialog.h"
#include <QMessageBox>
#include <QString>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class login_dialog;
}

class login_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit login_dialog(QWidget *parent = 0);
    ~login_dialog();
     register_dialog rd;
     QString getUserName();

private:
     Ui::login_dialog *ui;
     void register_show_dialog();//注册窗口
     QMessageBox *warnDlg;
     QString Username;
     QString Password;


private slots:
    void slotRegister();//注册账号槽
    void showPassword();//显示密码槽
    void on_login_signin_pushButton_clicked();
    void on_login_findpassword_pushButton_clicked();

    void on_login_regaccount_pushButton_clicked();
};

#endif // LOGIN_DIALOG_H
