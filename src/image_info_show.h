#ifndef IMAGE_INFO_SHOW_H
#define IMAGE_INFO_SHOW_H

#include <QWidget>

class image_info_show : public QWidget
{
    Q_OBJECT
public:
    explicit image_info_show(QWidget *parent = 0);
    QString searchInt;
    QString string;//json字符串

signals:

public slots:

private:
    void paintEvent(QPaintEvent *event);//绘制事件
};

#endif // IMAGE_INFO_SHOW_H
