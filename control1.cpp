#include "control1.h"
#include "ui_control1.h"

control1::control1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::control1)
{
    ui->setupUi(this);
}

control1::~control1()
{
    delete ui;
}
