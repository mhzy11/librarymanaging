#include "registering.h"
#include "ui_registering.h"
#include "globals.h"
#include <QMessageBox>
#include <QString>

registering::registering(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::registering)
{
    ui->setupUi(this);
}

registering::~registering()
{
    delete ui;
}

bool isUsernameExists(const QString& username) {
    QSqlQuery query;
    query.prepare("SELECT readerpassword FROM reader WHERE readername = :username");
    // 通过占位符名称绑定参数
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "SQL执行失败:" << query.lastError().text();
        return false;
    }
    return query.next();
}

void registering::on_pushButton_clicked()
{
    QString admin_name = ui->admin->text();
    QString password1 = ui->Password->text();
    QString password2 = ui->Password2->text();

    if (admin_name.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名为空");
        return;
    }
    if (password1.isEmpty()) {
        QMessageBox::warning(this, "错误", "密码为空");
        return;
    }
    if (password2.isEmpty()) {
        QMessageBox::warning(this, "错误", "请再次输入密码");
        return;
    }
    if (password1 != password2) {
        QMessageBox::warning(this, "错误", "两次密码不一致");
        return;
    }

    if (isUsernameExists(admin_name)) {
        QMessageBox::warning(this, "错误", "该用户名已存在");
        return;
    }

    // 对密码进行哈希加密
    QByteArray passwordBytes = password1.toUtf8();
    QByteArray hashedPassword = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256).toHex();
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO reader (readername, readerpassword, type) VALUES (?, ?, ?)");
    insertQuery.bindValue(0, admin_name);
    insertQuery.bindValue(1, hashedPassword);
    insertQuery.bindValue(2, "reader");
    if (insertQuery.exec()) {
        QMessageBox::information(this, "", "注册成功");
        regForm->close();
        delete regForm;
    } else {
        QMessageBox::warning(this, "错误", "注册失败: " + insertQuery.lastError().text());
        qDebug() << "SQL错误:" << insertQuery.lastError().text();
        qDebug() << "SQL查询:" << insertQuery.lastQuery();
        qDebug() << "绑定参数:" << insertQuery.boundValues();
    }
}
