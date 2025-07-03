#include "control1.h"
#include "ui_control1.h"
#include "globals.h"
#include <QGroupBox>
#include <QMessageBox>
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
void control1::setUsername(const QString& username)
{
    ui->username->setText(username); // 设置用户名到相应的控件上
}
void clearLayout1(QLayout* layout) {
    if (!layout) return;
    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        } else if (item->layout()) {
            clearLayout1(item->layout());
        }
        delete item;
    }
}
void control1::showrecommend(){
    clearLayout1(ui->verticalLayout_2);
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
            ui->verticalLayout_2->addLayout(currentRowLayout);
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
        QPushButton *button = new QPushButton("借阅", groupBox);
        boxLayout->addWidget(button);
        connect(button, &QPushButton::clicked, [this, result]() {
            QSqlQuery query;
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
            showrecommend();
            showBorrowedBooks();
        });
        currentRowLayout->addWidget(groupBox);
        count++;
    }
    for(int i=0; i < (2 - count%2) % 2;i++){
        QGroupBox *groupBox = new QGroupBox(this);
        groupBox->setFixedSize(groupBoxSize); // 设置固定大小
        currentRowLayout->addWidget(groupBox);
        groupBox->setStyleSheet("QGroupBox { background-color: transparent; border: none; }");
    }
}
void control1::showBorrowedBooks() {
    clearLayout1(ui->verticalLayout_5);
    QHBoxLayout *currentRowLayout = nullptr;
    int count = 0;
    const QSize groupBoxSize(200, 350); // 调整尺寸以显示归还时间
    QList<BorrowInfo> borrowedBooks; // 定义借阅信息结构体
    QSqlQuery query;

    // 从borrowing表查询当前用户的借阅记录
    query.prepare("SELECT b.bookname, b.author, b.intro, b.num, b.bookID, "
                  "br.borrowkey, br.borrowedtime, br.returnedtime , br.returned "
                  "FROM books b "
                  "JOIN borrowing br ON b.bookID = br.bookID "
                  "WHERE br.readername = :username");
    query.bindValue(":username", user);

    if (query.exec()) {
        while (query.next()) {
            // 解析查询结果并存储
            BorrowInfo book;
            book.bookName = query.value("bookname").toString();
            book.author = query.value("author").toString();
            book.intro = query.value("intro").toString();
            book.num = query.value("num").toInt();
            book.id = query.value("bookID").toString();
            book.borrowkey = query.value("borrowkey").toString(); // 获取borrowkey
            book.borrowedDate = query.value("borrowedtime").toDate();
            book.returnDate = query.value("returnedtime").toDate();
            book.returned=query.value("returned").toBool();
            borrowedBooks.append(book);
        }
    } else {
        QMessageBox::warning(this, "错误", "查询借阅记录失败: " + query.lastError().text());
        return;
    }
    for (const BorrowInfo& book : borrowedBooks) {
        if (count % 2 == 0) {
            currentRowLayout = new QHBoxLayout();
            ui->verticalLayout_5->addLayout(currentRowLayout);
        }

        // 创建书籍信息组
        QGroupBox *groupBox = new QGroupBox(this);
        groupBox->setFixedSize(groupBoxSize);
        QVBoxLayout *boxLayout = new QVBoxLayout(groupBox);

        // 显示书名
        QString title = QString("书名：%1").arg(book.bookName);
        QLabel *titleLabel = new QLabel(title, groupBox);
        titleLabel->setWordWrap(true);
        boxLayout->addWidget(titleLabel);

        // 显示作者
        QString author = QString("作者：%1").arg(book.author);
        QLabel *authorLabel = new QLabel(author, groupBox);
        authorLabel->setWordWrap(true);
        boxLayout->addWidget(authorLabel);

        // 显示简介
        QString intro = QString("简介：%1").arg(book.intro);
        QLabel *introLabel = new QLabel(intro, groupBox);
        introLabel->setWordWrap(true);
        boxLayout->addWidget(introLabel);

        // 显示借阅日期和应归还日期
        QString borrowDate = QString("借阅日期：%1").arg(book.borrowedDate.toString("yyyy-MM-dd"));
        QLabel *borrowDateLabel = new QLabel(borrowDate, groupBox);
        borrowDateLabel->setWordWrap(true);
        boxLayout->addWidget(borrowDateLabel);

        QString returnDate = QString("应归还日期：%1").arg(book.returnDate.toString("yyyy-MM-dd"));
        QLabel *returnDateLabel = new QLabel(returnDate, groupBox);
        returnDateLabel->setWordWrap(true);
        boxLayout->addWidget(returnDateLabel);

        QString status = book.returned ? "已归还" : "未归还";
        QLabel *statusLabel = new QLabel("状态：" + status, groupBox);
        statusLabel->setWordWrap(true);
        boxLayout->addWidget(statusLabel);
        // 创建归还按钮
        if(!book.returned){
        QPushButton *returnButton = new QPushButton("归还", groupBox);
        boxLayout->addWidget(returnButton);
        QPushButton *reborrowButton = new QPushButton("续借", groupBox);
        boxLayout->addWidget(reborrowButton);
        // 连接归还按钮事件
        connect(returnButton, &QPushButton::clicked, [this, book]() {
            QSqlQuery query;
            QDate currDate = QDate::currentDate();

            // 1. 更新borrowing表，标记为已归还
            query.prepare("UPDATE borrowing SET returned = true, returntime = :rDate "
                          "WHERE borrowkey = :bkey");
            query.bindValue(":rDate", currDate.toString("yyyy-MM-dd"));
            query.bindValue(":bkey", book.borrowkey); // 使用borrowkey
            if (!query.exec()) {
                QMessageBox::warning(this, "错误", "归还记录更新失败：" + query.lastError().text());
                return;
            }
            query.prepare("UPDATE books SET num = num + 1 WHERE bookID = :bid");
            query.bindValue(":bid", book.id);
            if (!query.exec()) {
                QMessageBox::warning(this, "错误", "库存更新失败：" + query.lastError().text());
                return;
            }

            // 3. 提示用户归还成功
            QMessageBox::information(this, "成功",
                                     QString("归还成功！\n"
                                             "书籍：%1\n"
                                             "实际归还日期：%2")
                                         .arg(book.bookName)
                                         .arg(currDate.toString("yyyy-MM-dd")));

            showrecommend();
            showBorrowedBooks();
        });
        connect(reborrowButton, &QPushButton::clicked, [this, book](){
            QSqlQuery query;
            // 1. 从globe表读取借阅天数配置
            QSqlQuery daysQuery;
            daysQuery.prepare("SELECT days FROM globe ORDER BY id LIMIT 1");
            if (!daysQuery.exec() || !daysQuery.next()) {
                QMessageBox::warning(this, "错误", "获取借阅天数失败，使用默认值14天");
                return;
            }
            int borrowDays = daysQuery.value(0).toInt();
            if (borrowDays <= 0) borrowDays = 14;  // 默认14天

            // 2. 获取当前日期（忽略时间）
            QDate currDate = QDate::currentDate();
            // 3. 计算新的应归还日期
            QDate newReturnDate = currDate.addDays(borrowDays);

            // 4. 更新借阅记录的应归还日期
            query.prepare("UPDATE borrowing SET returnedtime = :newReturnDate WHERE borrowkey = :borrowKey");
            query.bindValue(":newReturnDate", newReturnDate.toString("yyyy-MM-dd"));
            query.bindValue(":borrowKey", book.id);

            if (query.exec()) {
                QMessageBox::information(this, "成功", "续借成功，新的应归还日期为: " + newReturnDate.toString("yyyy-MM-dd"));
                showBorrowedBooks();
            } else {
                QMessageBox::warning(this, "错误", "续借失败: " + query.lastError().text());
            }

        });}
        currentRowLayout->addWidget(groupBox);
        count++;
    }

    // 补齐最后一行
    for (int i = 0; i < (2 - count % 2) % 2; i++) {
        QGroupBox *groupBox = new QGroupBox(this);
        groupBox->setFixedSize(groupBoxSize);
        currentRowLayout->addWidget(groupBox);
        groupBox->setStyleSheet("QGroupBox { background-color: transparent; border: none; }");
    }
}
void control1::on_search_clicked()
{

        QString searchText = ui->lineEdit->text(); // 获取搜索框中的文本
        QString selectedText = ui->comboBox->currentText();
        // 这里添加具体的搜索逻辑，例如从某个数据源中查找匹配的信息
        QList<BookInfo> searchResults;
        // 假设这里有一个数据源 QStringList dataSource
        if(selectedText=="书名"){
            QSqlQuery query;
            // 准备SQL语句，使用:keyword占位符
            query.prepare("SELECT * FROM books WHERE bookname LIKE :keyword");
            // 拼接通配符，%表示任意字符序列
            query.bindValue(":keyword", "%" + searchText + "%");
            if (query.exec()) {
                while (query.next()) {
                    searchResults.append(BookInfo(query.value("bookname").toString(),query.value("author").toString(), query.value("intro").toString(),query.value("num").toInt(),query.value("bookID").toString()));
                }
            } else {
                qDebug() << "查询失败:" << query.lastError().text();
            }
        }
        else{
            QSqlQuery query;
            // 准备SQL语句，使用:keyword占位符
            query.prepare("SELECT * FROM books WHERE author LIKE :keyword");
            // 拼接通配符，%表示任意字符序列
            query.bindValue(":keyword", "%" + searchText + "%");
            if (query.exec()) {
                while (query.next()) {
                    searchResults.append(BookInfo(query.value("bookname").toString(),query.value("author").toString(), query.value("intro").toString(),query.value("num").toInt(),query.value("bookID").toString()));
                }
            } else {
                qDebug() << "查询失败:" << query.lastError().text();
            }}
        if (!Searching) {
            Searching = new Search();
        }
        Searching->showSearchResults(searchResults,"借阅"); // 显示搜索结果
        Searching->show(); // 显示搜索结果窗口
}
void control1::highlight1(){
    ui->pushButton->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_2->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_3->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
}

void control1::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    highlight1();
    ui->pushButton->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}

void control1::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    highlight1();
    ui->pushButton_2->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}


void control1::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    highlight1();
    ui->pushButton_3->setStyleSheet("background-color: black;color: white; border-radius: 5px;");

}

