#include "registering.h"
#include "ui_registering.h"
#include "globals.h"
#include <QMessageBox>
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

void registering::on_pushButton_clicked()
{
    QString admin_name=ui->admin->text();
    QString password1=ui->Password->text();
    QString password2=ui->Password2->text();
    if(admin_name==nullptr){
        QMessageBox::warning(this,"错误","用户名为空");
        return;
    }
    else{
        if(password1.isEmpty()){
            QMessageBox::warning(this,"错误","密码为空");
            return;
        }
        else{
            if(password2.isEmpty()){
                QMessageBox::warning(this,"错误","请再次输入密码");
                return;
            }
            else{
                if(password1!=password2){
                    QMessageBox::warning(this,"错误","两次密码不一致");
                    return;
                }
                else{
                    QMessageBox::information(this,"","注册成功");
                    regForm->close();
                    delete regForm;
                }
            }
        }
    }
}

