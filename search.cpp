#include "search.h"
#include "ui_search.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMap>
#include "globals.h"
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
void clearLayout(QLayout* layout) {
    if (!layout) return;
    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        } else if (item->layout()) {
            clearLayout(item->layout());
        }
        delete item;
    }
}
void Search::showSearchResults(QList<BookInfo>& results,QString Type)
{
    // 清空之前的搜索结果
    clearLayout(ui->verticalLayout);
    QHBoxLayout *currentRowLayout = nullptr;
    int count = 0;
    const QSize groupBoxSize(200, 300); // 可根据需要调整
    const QSize buttonSize(50, 80);
    for (const BookInfo& result : results) {
        if (count % 3 == 0) {
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
        QPushButton *button = new QPushButton(Type, groupBox);
        boxLayout->addWidget(button);
        connect(button, &QPushButton::clicked, [this, result,Type]() {
            QSqlQuery query;
            QSqlDatabase::database().transaction();  // 开启事务

            try {
                if (Type == "借阅") {
                    if (result.num <= 0) {
                        throw QString("库存不足，当前可借数量：" + QString::number(result.num));
                    }
                    // 1. 从globe表读取借阅天数配置
                    QSqlQuery daysQuery;
                    daysQuery.prepare("SELECT days FROM globe ORDER BY id LIMIT 1");
                    if (!daysQuery.exec() || !daysQuery.next()) {
                        throw QString("获取借阅天数失败，使用默认值14天");
                    }
                    int borrowDays = daysQuery.value(0).toInt();
                    if (borrowDays <= 0) borrowDays = 14;  // 默认14天
                    // 2. 获取当前日期（忽略时间）
                    QDate currDate = QDate::currentDate();
                    // 3. 计算应归还日期
                    QDate returnDate = currDate.addDays(borrowDays);

                    // 4. 插入借阅记录（仅存储日期）
                    query.prepare("INSERT INTO borrowing (borrowkey,readername, bookID, borrowedtime, returnedtime,returned) "
                                  "VALUES (:key,:name, :bid, :bDate, :rDate,:boolre)");
                    QUuid uuid = QUuid::createUuid();
                    QString uuidStr = uuid.toString(QUuid::WithoutBraces);
                    query.bindValue(":key", uuidStr);
                    query.bindValue(":name", user);
                    query.bindValue(":bid", result.id);
                    query.bindValue(":bDate", currDate.toString("yyyy-MM-dd"));
                    query.bindValue(":rDate", returnDate.toString("yyyy-MM-dd"));
                    query.bindValue(":boolre",false);
                    if (!query.exec()) {
                        throw QString("借阅记录失败：" + query.lastError().text());
                    }

                    // 5. 更新图书库存
                    query.prepare("UPDATE books SET num = num - 1 WHERE bookID = :bid");
                    query.bindValue(":bid", result.id);
                    if (!query.exec()) {
                        throw QString("库存更新失败：" + query.lastError().text());
                    }
                    // 6. 提示用户
                    QMessageBox::information(this, "成功",
                                             QString("借阅成功！\n"
                                                     "当前库存：%1\n"
                                                     "借阅日期：%2\n"
                                                     "应归还日期：%3")
                                                 .arg(result.num - 1)
                                                 .arg(currDate.toString("yyyy-MM-dd"))
                                                 .arg(returnDate.toString("yyyy-MM-dd")));
                    controling1->showBorrowedBooks();
                    controling1->showrecommend();
                    Searching->close();
                } else if (Type == "推荐") {


                    // 插入推荐记录（避免重复推荐）
                    query.prepare("INSERT OR IGNORE INTO recommend (recommendBOOKID) VALUES (:bid)");
                    query.bindValue(":bid", result.id);
                    if (!query.exec()) {
                        throw QString("推荐失败：" + query.lastError().text());
                    }
                    QMessageBox::information(this, "成功", "推荐成功");
                } else if (Type == "删除") {
                    query.prepare("DELETE FROM books WHERE bookID = :bid");
                    query.bindValue(":bid", result.id);
                    if (!query.exec()) {
                        throw QString("删除图书失败：" + query.lastError().text());

                    }
                    QMessageBox::information(this, "成功", "图书删除成功");
                    Searching->close();
                } else {
                    throw QString("未知操作类型：" + Type);
                }
                QSqlDatabase::database().commit();  // 提交事务
            } catch (const QString& err) {
                QSqlDatabase::database().rollback();  // 回滚事务
                QMessageBox::warning(this, "操作失败", err);
            }
        });
        currentRowLayout->addWidget(groupBox);

        count++;

    }
    for(int i=0; i < (3 - count%3) % 3;i++){
        QGroupBox *groupBox = new QGroupBox(this);
        groupBox->setFixedSize(groupBoxSize); // 设置固定大小
        currentRowLayout->addWidget(groupBox);
        groupBox->setStyleSheet("QGroupBox { background-color: transparent; border: none; }");
    }
    ui->countings->setText(QString::number(count));
}
