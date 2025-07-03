#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QPainter>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void painEvent(QPaintEvent * event);


private slots:
    void on_registerButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
