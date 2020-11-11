#include "mainwindow.h"
#include "ui_mainwindow.h"




void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)//鼠标双击 全屏
{
    if(event->button()==Qt::LeftButton)
    {
        //判断是否在标题栏
        QRect title_widget = QRect(ui->centralWidget->pos()+ui->title_widget->pos(),
                                   ui->title_widget->size());
        //在标题栏则可以双击放大
        if(title_widget.contains(event->pos()))
        {
            if(windowState()!=Qt::WindowMaximized)
            {
                setWindowState(Qt::WindowMaximized);
                ui->title_quitbox_max_toolButton->setIcon(returnIcon);
            }
            else
            {
                setWindowState(Qt::WindowNoState);//恢复正常模式
                ui->title_quitbox_max_toolButton->setIcon(maxIcon);
            }
        }
    }
    event->ignore();
}




