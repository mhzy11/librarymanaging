#include "search.h"
#include "ui_search.h"

Search::Search(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Search)
{
    ui->setupUi(this);
}

Search::~Search()
{
    delete ui;
}
