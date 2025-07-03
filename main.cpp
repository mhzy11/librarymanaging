#include "login.h"
#include "registering.h"
#include <QApplication>
#include "globals.h"
QSqlQuery readquery;
QSqlQuery insertQuery;
registering *regForm;
control2 *controling2;
Login *login1;
Search *Searching;
control1 *controling1;
QString user;
recommended *recommending;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // 添加SQLite数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // 设置数据库文件名，如果文件不存在会自动创建
    db.setDatabaseName("D:/db/library.s3db");
    // 打开数据库
    if (!db.open()) {
        QMessageBox::warning(nullptr, "错误", "连接失败");
        return 1;
    } else {
        QMessageBox::information(nullptr, "成功", "连接成功");
    }
    login1 = new Login();
    login1->show();
    return a.exec();
    return 0;
}
