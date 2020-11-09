#ifndef COMPLETELINEEDIT_H
#define COMPLETELINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QListView>
#include <QStringListModel>
#include <QModelIndex>
#include <QKeyEvent>
#include <QDebug>
#include "opstring.h"


class CompleteLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CompleteLineEdit(QWidget *parent = 0);
    explicit CompleteLineEdit(QStringList words,QWidget *parent = 0);

    void setCompleter(const QString &text); // 动态的显示完成列表
    void completeText(const QModelIndex &index); // 点击完成列表中的项，使用此项自动完成输入的单词
    void setList(const QStringList &list);
    QListView* getListView();
 protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
 private:
    QStringList list; // 整个匹配列表的列表
    QListView *listView; // 匹配列表
    int listWidth;
    QStringListModel *model; // 匹配列表的model

signals:

public slots:
};

#endif // COMPLETELINEEDIT_H
