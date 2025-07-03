#include "globals.h"
class UserManager {
public:
    static bool verifyUser(const QString& username, const QString& password) {
        QSqlQuery query;
        query.prepare("SELECT id FROM users WHERE username = :username AND password = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        return query.exec() && query.next();
    }

    static bool createUser(const QString& username, const QString& password) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", hashPassword(password)); // 使用加密后的密码

        return query.exec();
    }

private:
    static QString hashPassword(const QString& password) {
        QByteArray hash = QCryptographicHash::hash(
            password.toUtf8(),
            QCryptographicHash::Sha256
            );
        return hash.toHex();
    }
};
