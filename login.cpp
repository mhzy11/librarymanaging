#include "login.h"
#include "ui_login.h"
#include"registering.h"
#include "globals.h"
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

void Login::on_registerButton_clicked()
{
    regForm = new registering();
    regForm->setAttribute(Qt::WA_DeleteOnClose,true);
    regForm->show();
}


void Login::on_loginButton_clicked()
{
    //检测登录合法if
    controling2 = new control2();
    login1->close();
    controling2->show();

}

