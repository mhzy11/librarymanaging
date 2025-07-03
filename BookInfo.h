#ifndef BOOKINFO_H
#define BOOKINFO_H
#include <QWidget>
struct BookInfo {
    QString bookName;
    QString author;
    QString intro;
    int num;  // 可能是库存数量？
    QString id;

    // 构造函数
    BookInfo(const QString& name, const QString& auth, const QString& desc, const int& count,const QString& ID)
        : bookName(name), author(auth), intro(desc), num(count) ,id(ID){}
};
#endif // BOOKINFO_H
