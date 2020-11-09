#include "completelineedit.h"
#include <QMessageBox>

CompleteLineEdit::CompleteLineEdit(QWidget *parent): QLineEdit(parent)
{
    OptString opStr;
    QList<int> index;
    index.append(1);//需要上面字符串的第1位
    index.append(4);//需要上面字符串的第1位
    index.append(5);//需要上面字符串的第5位
    QStringList allList;//创建String集合
    QFile file(stocklistPath);//从本地文件读取股票列表
    if(file.open(QFile::ReadOnly))//打开成功
    {
        QTextStream stream(&file);//创建一个流
        stream.readLine();//读取第一行数据
        while(!stream.atEnd())//每读一行往allList集合里面加入这行字符
        {
            allList.append(stream.readLine());
        }
    }
    else
    {
        qDebug()<<"打开失败!";
    }
    QStringList *showlist = opStr.getStringList(allList,"\t",index);
    setList(*showlist);


    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    listWidth = width();
    connect(this, &QLineEdit::textChanged, this, &CompleteLineEdit::setCompleter);
    connect(listView, &QListView::clicked, this,&CompleteLineEdit::completeText);
}

CompleteLineEdit::CompleteLineEdit(QStringList words, QWidget *parent)     : QLineEdit(parent), list(words)
{
    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    connect(this, &QLineEdit::textChanged, this, &CompleteLineEdit::setCompleter);
    connect(listView, &QListView::clicked, this,&CompleteLineEdit::completeText);
}

void CompleteLineEdit::focusOutEvent(QFocusEvent *e)
{
    //listView->hide();
}

void CompleteLineEdit::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if (!listView->isHidden())//
    {
        int count = listView->model()->rowCount();
        QModelIndex currentIndex = listView->currentIndex();
        if (Qt::Key_Down == key)
        {
            // 按向下方向键时，移动光标选中下一个完成列表中的项
            int row = currentIndex.row() + 1;
            if (row >= count)
            {
                row = 0;
            }
            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        }
        else if(Qt::Key_Up == key)
        {
           //按向下方向键时，移动光标选中上一个完成列表中的项
           int row = currentIndex.row() - 1;
           if(row < 0)
            {
                row = count - 1;
            }
            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        }
        else if (Qt::Key_Escape == key)
        {
            // 按下Esc键时,隐藏完成列表
            listView->hide();
        }
        else if (Qt::Key_Enter == key || Qt::Key_Return == key)
        {
            // 按下回车键时,使用完成列表中选中的项,并隐藏完成列表.
            if (currentIndex.isValid())
            {
                QString text = listView->currentIndex().data().toString().split("\t").at(0);
                setText(text);
            }
            listView->hide();
        }
        else
        {
            //其他情况，隐藏完成列表，并使用QLineEdit的键盘按下事件
            listView->hide();
            QLineEdit::keyPressEvent(e);
        }
    }
    else
    {
        QLineEdit::keyPressEvent(e);
    }

    if (Qt::Key_Right == key||Qt::Key_Left == key)
    {
        // 按左右方向键时，移动光标选中下一个完成列表中的项
          setCompleter(text());
    }
}

void CompleteLineEdit::setList(const QStringList &list)
{
    this->list = list;
}

void CompleteLineEdit::setCompleter(const QString&text)
{
    if (text.isEmpty())
    {
        listView->hide();
        return;
    }
    if ((text.length() > 1) && (!listView->isHidden()))
    {
        return;
    }
    // 如果完整的完成列表中的某个单词包含输入的文本，则加入要显示的完成列表串中
    QStringList sl;
    foreach(QString word, list)
    {
        if (word.contains(text))
        {
            sl<< word;
        }
    }
    model->setStringList(sl);
    listView->setModel(model);
    if (model->rowCount() == 0)
    {
        return;
    }
    // Position the text edit
    listView->setMinimumWidth(240);
    listView->setMaximumWidth(240);
    QPoint p(0, height());
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y() + 1;
    listView->move(x, y);
    listView->show();
}
QListView* CompleteLineEdit::getListView()
{
    return listView;
}

void CompleteLineEdit::completeText(const QModelIndex&index)
{
        QString text = index.data().toString().split("\t").at(0);
        setText(text);
        listView->hide();
}
