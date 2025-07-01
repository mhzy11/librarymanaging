#include "control2.h"
#include "ui_control2.h"

control2::control2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::control2)
{
    ui->setupUi(this);
}

control2::~control2()
{
    delete ui;
}
void control2::highlight1(){
    ui->pushButton->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_2->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_3->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
}
void control2::highlight2(){
    ui->pushButton_4->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_5->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
}
void control2::on_pushButton_clicked()
{
    ui->stackedWidget1->setCurrentIndex(0);
    highlight1();
    ui->pushButton->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}

void control2::on_pushButton_2_clicked()
{
    ui->stackedWidget1->setCurrentIndex(1);
    highlight1();
    ui->pushButton_2->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}


void control2::on_pushButton_3_clicked()
{
    ui->stackedWidget1->setCurrentIndex(2);
    highlight1();
    ui->pushButton_3->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}


void control2::on_pushButton_4_clicked()
{
    ui->stackedWidget2->setCurrentIndex(0);
    highlight2();
    ui->pushButton_4->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}



void control2::on_pushButton_5_clicked()
{
    ui->stackedWidget2->setCurrentIndex(1);
    highlight2();
    ui->pushButton_5->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}

