#include "login.h"
#include "ui_login.h"
#include"registering.h"
#include "globals.h"

#include <QMessageBox>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::painEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/images/shu.jpeg"));
}

int verifyUser(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT readerpassword,type FROM reader WHERE readername = :username");
    // 通过占位符名称绑定参数
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        QByteArray storedPassword = query.value(0).toByteArray();
        QByteArray TYPE = query.value(1).toByteArray();
        // 对输入的密码进行哈希加密
        QByteArray passwordBytes = password.toUtf8();
        QByteArray hashedPassword = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256).toHex();
        if (hashedPassword == storedPassword) {
            if(TYPE=="reader"){
                return 0;
            }
            else{
                return 1;
            }
        }
    }
    return 2;  // 用户名或密码错误
}

void Login::on_registerButton_clicked()
{
    regForm = new registering();
    regForm->setAttribute(Qt::WA_DeleteOnClose, true);
    regForm->show();
}

void Login::on_loginButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    int key1=verifyUser(username, password);
    controling2 = new control2();
    controling1 = new control1();
    if (key1 == 0) {
        user = username;
        login1->close();
        controling1->show();
        controling1->setUsername(user);
        controling1->showrecommend();
        controling1->showBorrowedBooks();
    }
    else if(key1==1){
        user = username;
        login1->close();
        controling2->show();
        controling2->initializeLibraryChart();
        }
    else {
        QMessageBox::warning(this, "错误", "用户名或密码错误");
    }
}
