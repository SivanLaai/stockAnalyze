#include <mainwindow.h>
#include <ui_mainwindow.h>
#include "register_dialog.h"
#include "ui_login_dialog.h"
#include <QDialog>

//登陆框槽
void MainWindow::person_dialog_slot()
{
    person_dialog();
}

//注册框槽
void login_dialog::slotRegister()
{
    register_show_dialog();

}

void login_dialog::showPassword()
{
    if(ui->login_showpassword_radioButton->isChecked())
    {
    ui->login_passwaord_lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui->login_passwaord_lineEdit->setEchoMode(QLineEdit::Password);
    }


}
