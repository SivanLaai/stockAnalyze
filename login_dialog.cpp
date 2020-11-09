#include "mainwindow.h"
#include "login_dialog.h"
#include "ui_login_dialog.h"
#include <QPixmap>
#include "qwebdata.h"
// #include "webgets.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <QFormLayout>
#include <QDesktopServices>
#include <frameless.h>

login_dialog::login_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_dialog)
{
    ui->setupUi(this);
    //登陆框
    setWindowTitle("股票登录界面");
    ui->login_label->setText(tr("股票登陆界面"));
    ui->login_label->setAlignment(Qt::AlignCenter);
    ui->login_label->setStyleSheet("QLabel{color:green;font:30px}");
    ui->loginin_label->setText(tr("账号："));
    ui->loginin_label->setStyleSheet("color:white");
    ui->login_password_label->setText(tr("密码："));
    ui->login_password_label->setStyleSheet("color:white");
    ui->login_showpassword_radioButton->setText(tr("显示密码"));
    ui->login_showpassword_radioButton->setStyleSheet("QRadioButton{color:white}:hover{color:yellow}");
    ui->login_signin_pushButton->setText(tr("登陆"));
    ui->login_signin_pushButton->setStyleSheet("QPushButton{color:white}:hover{color:yellow}");
    ui->login_findpassword_pushButton->setText(tr("找回密码"));
    ui->login_findpassword_pushButton->setStyleSheet("QPushButton{color:white}:hover{color:yellow}");
    ui->login_regaccount_pushButton->setText(tr("注册账号"));
    ui->login_regaccount_pushButton->setStyleSheet("QPushButton{color:white}:hover{color:yellow}");
    ui->loginin_lineEdit->setStyleSheet("background-color:white");
    ui->login_passwaord_lineEdit->setEchoMode(QLineEdit::Password);
    ui->login_passwaord_lineEdit->setStyleSheet("background-color:white");
    this->setStyleSheet("background-color:rgb(20,20,20)");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    FramelessHelper *pHelper = new FramelessHelper(this);
    pHelper->activateOn(this);  //激活当前窗体
    pHelper->setWidgetMovable(true);  //设置窗体可移动
    pHelper->setWidgetResizable(false);  //设置窗体可缩放
    pHelper->setRubberBandOnMove(false);  //设置橡皮筋效果-可移动
    pHelper->setRubberBandOnResize(false);  //设置橡皮筋效果-可缩放

    //密码输入错误警告窗口
    warnDlg = new QMessageBox(QMessageBox::Warning,"警告",NULL,NULL,NULL);
    warnDlg->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint |Qt::WindowStaysOnTopHint);
    warnDlg->setStyleSheet("background-color:rgb(20,20,20)");
    QAbstractButton *okButton = warnDlg->addButton(QMessageBox::Ok);
    okButton->setText("确定");
    okButton->setStyleSheet("QPushButton{color:white}");

    //退出登录
    QPixmap quitIcon = QPixmap(tr(":/icon/img/quit20.png"));
    ui->login_quit_toolButton->setIcon(quitIcon);
    ui->login_quit_toolButton->setStyleSheet("background-color:transparent");
    ui->login_quit_toolButton->setToolTip(tr("退出登录"));

    //处理登录事件信号
    connect(ui->login_quit_toolButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->login_signin_pushButton,&QPushButton::clicked,
            [=]()
    {
        QWebLogin *pLogin = new QWebLogin(Username,Password);
        if(pLogin->isLogin())
        {
            accept();
        }
        else if(""==Username||""==Password)
        {
            warnDlg->setText("<font color='white'>帐号或密码不能为空！</font>");
            warnDlg->exec();
        }
        else
        {
            warnDlg->setText("<font color='white'>帐号或密码输入错误！</font>");
            warnDlg->exec();
        }

    });

    //显示密码
    connect(ui->login_showpassword_radioButton,SIGNAL(clicked(bool)),this,SLOT(showPassword()));
    //显示界面的时候清空密码
    connect(this,&login_dialog::accepted,
            [=]()
    {
        ui->login_passwaord_lineEdit->clear();
        ui->loginin_lineEdit->clear();
    });

}

login_dialog::~login_dialog()
{
    delete ui;
}



void login_dialog::on_login_signin_pushButton_clicked()//按下登录按钮
{
    Username = ui->loginin_lineEdit->text();
    Password = ui->login_passwaord_lineEdit->text();
}

void login_dialog::on_login_findpassword_pushButton_clicked()
{

}
QString login_dialog::getUserName()
{
    return Username;
}

void login_dialog::on_login_regaccount_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://47.94.146.176:8080/SHH3/register.jsp"));
}
