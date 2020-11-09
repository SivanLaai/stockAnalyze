#include "register_dialog.h"
#include "ui_register_dialog.h"
#include <QLabel>

register_dialog::register_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::register_dialog)
{
    ui->setupUi(this);

    //注册框
    ui->register_uesr_label->setText(tr("XX股票注册界面"));
    ui->register_uesr_label->setAlignment(Qt::AlignCenter);
    ui->register_uesr_label->setStyleSheet("QLabel{color:green;font:20px}");

    ui->register_uesrid_label->setText(tr("账号："));
    ui->register_uesrid_label->setStyleSheet("color:white");
    ui->register_uesrid_lineEdit->setMaxLength(16);
    ui->register_uesrid_lineEdit->setStyleSheet("background-color:white");
    ui->register_uesrpassword_label->setText(tr("密码："));
    ui->register_uesrpassword_label->setStyleSheet("color:white");
    ui->register_uesrpassword_lineEdit->setMaxLength(18);
     ui->register_uesrpassword_lineEdit->setEchoMode(QLineEdit::Password);
    ui->register_uesrpassword_lineEdit->setStyleSheet("background-color:white");
    ui->register_uesrpasswordconfim_label->setText(tr("确认密码："));
    ui->register_uesrpasswordconfim_label->setStyleSheet("color:white");
    ui->register_uesrpasswordconfim_lineEdit->setMaxLength(18);
    ui->register_uesrpasswordconfim_lineEdit->setEchoMode(QLineEdit::Password);
    ui->register_uesrpasswordconfim_lineEdit->setStyleSheet("background-color:white");
    ui->register_uesrname_label->setText(tr("昵称："));
    ui->register_uesrname_label->setStyleSheet("color:white");
    ui->register_uesrname_lineEdit->setMaxLength(16);
    ui->register_uesrname_lineEdit->setStyleSheet("background-color:white");
    ui->register_uesrphone_label->setText(tr("手机号："));
    ui->register_uesrphone_label->setStyleSheet("color:white");
    ui->register_uesrphone_lineEdit->setMaxLength(11);
    ui->register_uesrphone_lineEdit->setStyleSheet("background-color:white");
    ui->register_uesr_pushButton->setText(tr("注册"));
    ui->register_uesr_pushButton->setStyleSheet("QPushButton{color:white}:hover{color:yellow}");

    this->setStyleSheet("background-color:rgb(20,20,20)");
    //注册要求
    register_info();



    //退出注册
    QPixmap quitIcon = QPixmap(tr(":/icon/img/quit20.png"));
    ui->register_quit_toolButton->setIcon(quitIcon);
    ui->register_quit_toolButton->setStyleSheet("background-color:transparent");
    ui->register_quit_toolButton->setToolTip(tr("退出登录"));
    connect(ui->register_quit_toolButton,SIGNAL(clicked(bool)),this,SLOT(close()));




}

register_dialog::~register_dialog()
{
    delete ui;
}

void register_dialog::register_info()
{
    //注册提示信息
    ui->register_uesrid_info_label->setText(tr("6-16个字符(英文,数字)"));
    ui->register_uesrid_info_label->setStyleSheet("color:white");
    ui->register_uesrpassword_info_label->setText(tr("6-18个字符(数字,字母,符号)"));
    ui->register_uesrpassword_info_label->setStyleSheet("color:white");
    ui->register_uesrpasswordconfim_info_label->setText(tr("请再输入一遍密码"));
    ui->register_uesrpasswordconfim_info_label->setStyleSheet("color:white");
    ui->register_uesrname_info_label->setText(tr("4-16个字符(中英文,数字)"));
    ui->register_uesrname_info_label->setStyleSheet("color:white");
    ui->register_uesrphone_info_label->setText(tr("请输入11位真实手机号码"));
    ui->register_uesrphone_info_label->setStyleSheet("color:white");



}

