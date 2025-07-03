#include "recommended.h"
#include "ui_recommended.h"
#include "BookInfo.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMap>
#include "globals.h"
#include <QLabel>
recommended::recommended(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::recommended)
{
    ui->setupUi(this);
}

recommended::~recommended()
{
    delete ui;
}
void clearLayout2(QLayout* layout) {
    if (!layout) return;
    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        } else if (item->layout()) {
            clearLayout2(item->layout());
        }
        delete item;
    }
}
void recommended::showrecommend1(){
    clearLayout2(ui->verticalLayout);
    QHBoxLayout *currentRowLayout = nullptr;
    int count = 0;
    const QSize groupBoxSize(200, 300); // 可根据需要调整
    const QSize buttonSize(50, 80);
    QList<BookInfo> recommendResults;
    QSqlQuery query;
    // 编写 SQL 查询，从 recommend 表中获取 recommendBOOKID，然后从 books 表中选取对应的书籍信息
    query.prepare("SELECT b.bookname, b.author, b.intro, b.num, b.bookID "
                  "FROM books b "
                  "JOIN recommend r ON b.bookID = r.recommendBOOKID");

    if (query.exec()) {
        while (query.next()) {
            // 将查询结果存入 recommendResults 中
            recommendResults.append(BookInfo(query.value("bookname").toString(),
                                             query.value("author").toString(),
                                             query.value("intro").toString(),
                                             query.value("num").toInt(),
                                             query.value("bookID").toString()));
        }
    } else {
        QMessageBox::warning(this,"错误","查询失败:");
    }
    for (const BookInfo& result : recommendResults) {
        if (count % 2 == 0) {
            currentRowLayout = new QHBoxLayout();
            ui->verticalLayout->addLayout(currentRowLayout);
        }
        // 分割搜索结果为书名、作者和简介
        QString title=QString("书名：%1").arg(result.bookName);
        QString author=QString("作者：%1").arg(result.author);
        QString introduction=QString("简介：%1").arg(result.intro);
        QString number=QString("数量：%1").arg(result.num);
        QGroupBox *groupBox = new QGroupBox(this);
        groupBox->setFixedSize(groupBoxSize); // 设置固定大小
        QVBoxLayout *boxLayout = new QVBoxLayout(groupBox);
        // 创建书名标签
        QLabel *titleLabel = new QLabel(title, groupBox);
        titleLabel->setWordWrap(true); // 允许自动换行
        boxLayout->addWidget(titleLabel);

        // 创建作者标签
        QLabel *authorLabel = new QLabel(author, groupBox);
        authorLabel->setWordWrap(true); // 允许自动换行
        boxLayout->addWidget(authorLabel);

        QLabel *numLabel = new QLabel(number, groupBox);
        numLabel->setWordWrap(true); // 允许自动换行
        boxLayout->addWidget(numLabel);
        // 创建简介标签
        QLabel *introLabel = new QLabel(introduction, groupBox);
        introLabel->setWordWrap(true); // 允许自动换行
        boxLayout->addWidget(introLabel);

        // 创建按钮
        QPushButton *button = new QPushButton("取消推荐", groupBox);
        boxLayout->addWidget(button);
        connect(button, &QPushButton::clicked, [this, result]() {
            QSqlQuery query;
            query.prepare("DELETE FROM recommend WHERE recommendBOOKID = :bid");
            query.bindValue(":bid", result.id);
            if (!query.exec()) {
                throw QString("取消失败：" + query.lastError().text());

            }
            QMessageBox::information(this, "成功","取消成功");
            showrecommend1();
        });
        currentRowLayout->addWidget(groupBox);
        count++;
    }
    if (currentRowLayout) { // 加非空判断
        for(int i=0; i < (2 - count%2) % 2;i++){
            QGroupBox *groupBox = new QGroupBox(this);
            groupBox->setFixedSize(groupBoxSize);
            currentRowLayout->addWidget(groupBox);
            groupBox->setStyleSheet("QGroupBox { background-color: transparent; border: none; }");
        }
    }

}
