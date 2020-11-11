#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QDialog>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


namespace Ui {
class register_dialog;
}

class register_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit register_dialog(QWidget *parent = 0);
    ~register_dialog();

private:
    Ui::register_dialog *ui;
    void register_info();//注册要求

};

#endif // REGISTER_DIALOG_H
