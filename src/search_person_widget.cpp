#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>

void MainWindow::search_person_widget()
{
    //图标载入
    personIcon = QPixmap(":/icon/img/person.png");
    ui->search_person_pushButton->setIcon(personIcon);
    ui->search_person_pushButton->setIconSize(QSize(30,30));
    connect(ui->search_person_pushButton,SIGNAL(clicked(bool)),this,SLOT(person_dialog_slot()));

    personGroupIcon = QPixmap(":/icon/img/personGroup.png");
    ui->search_personGroup_pushButton->setIcon(personGroupIcon);
    ui->search_personGroup_pushButton->setIconSize(QSize(30,30));

    extraFunctionIcon = QPixmap(":/icon/img/extraFunction.png");
    ui->search_extraFunction_pushButton->setIcon(extraFunctionIcon);
    ui->search_extraFunction_pushButton->setIconSize(QSize(30,30));

}
