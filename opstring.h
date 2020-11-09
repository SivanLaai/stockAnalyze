#ifndef OPSTRING_H
#define OPSTRING_H
#include <QString>
#include <QDebug>
#include <QCompleter>
#include <QTimer>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <typeinfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QFile>


const QString stocklistPath = QString("./lib/stocklist.txt");


class OptString: public QObject//处理常规字符串
{
public:

public:
    OptString()
    {

    }
    ~OptString()
    {

    }
   //
   QStringList* getStringList(QStringList list,QString split,QList<int> index) const
   //list是你要传入的QStringlist集合
   //split为list集合里每个QString元素的分割符
   //index为你想要获取的QString字符串里的某几个元素，例如QString为"654654 abc 213k",分隔符为' ',index里包含1,3两个元素，则表示你需要第一个和第三个元素分别为654654和213k
   {
       QStringList *showList = new QStringList();//用来显示的字符串集合
       QString strBuf;

       showList->clear();
       for(int i=0;i<list.length();i++)
       {//遍历QList<QStringList>
           QStringList orStrList = list.at(i).split(split);//得到一个QStringList list.at(i)
           strBuf.clear();
           for(int j=0;j<index.length();j++)//对选中的一个QStringList进行字符筛选，筛选的下标存放在index 这个List链表里
           {
               strBuf.append(orStrList.at(index.at(j)-1));//正向字符串
               if(j!=index.length()-1)//如果是最后一个了就不添加空位符了
               {
                   strBuf.append("\t");
               }
           }
           showList->append(strBuf);//把字符串添加到showList中
       }
       return showList;

   }
   //给设备设置下拉框
void static setListByText()
   {
       QList<int> index;/*000002	sz	1991-01-29	wkａ	万 科Ａ
                             1      2      3         4    5*/
       index.append(1);//需要上面字符串的第1位
       //index.append(4);//需要上面字符串的第1位
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
       //这里把股票列表的每行字符串如"000002	sz	1991-01-29	wkａ	万 科Ａ"都加入到allList里面
       //第二个参数" "表示字符串的分隔符为" "
       //第三个参数index表示需要的字符串下标元素，这里需要的是第一个和第五个分别为000002和万 科Ａ
   }


   //从数据库获取数据
   template<class T> void static setListBySql(T *inputDevice)
   {
       QList<int> index;/*000002	sz	1991-01-29	wkａ	万 科Ａ
                             1      2      3         4    5*/
       index.append(1);//需要上面字符串的第1位
       index.append(5);//需要上面字符串的第5位
       QStringList allList;//创建String集合
       QSqlError err;
       QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
       // 设置相关参数
       db.setDatabaseName("stock");    // 需要与数据库中的一致
       db.setHostName("127.0.0.1");    // 主机名(地址)
       db.setPort(3306);        // 端口
       db.exec("SET NAMES 'UTF-8'"); //直接使用数据库的latin1编码

       // 打开数据库
       if (!db.open("root", "232245")) {
           err = db.lastError();
           db = QSqlDatabase();
           QSqlDatabase::removeDatabase("MySQL");
       }
       else
       {
           qDebug()<<"success!";
       }
       QSqlQuery query(db);
       query.exec("select * from stocklist");
       while(query.next())
       {
           QString str = QString(query.value(0).toString().toUtf8());
           QString str1 = QString(query.value(1).toString().toUtf8());
           QString str2 = QString(query.value(2).toString().toUtf8());
           QString str3 = QString(query.value(3).toString().toUtf8());
           QString str4 = QString(query.value(4).toString().toUtf8());
           QString strBuf = QString("%1\t%2\t%3\t%4\t%5").arg(str).arg(str1).arg(str2).arg(str3).arg(str4);
           qDebug()<<strBuf;
           allList.append(strBuf);
       }
       query.exec(QString("insert into stocklist values('1','2','3','4','阿斯顿')").toUtf8().data());


   }
};
#endif // OPSTRING_H
//-----------------------------------------------------------------------------------------------------
