#include "control2.h"
#include "ui_control2.h"
#include "globals.h"
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
struct ChartConfig {
    QString title = "图书馆数据统计";      // 图表标题
    QColor barColor = QColor(51, 153, 255); // 柱状图颜色
    QStringList categories = {"图书总量", "已借数量", "未借数量", "读者数量"}; // 分类标签
    QFont axisFont = QFont("SimHei", 10);   // 坐标轴字体
    bool showGrid = true;                   // 是否显示网格线
    bool enableAnimation = true;            // 是否启用动画
    bool showLegend = true;                 // 是否显示图例
};
void control2::initializeLibraryChart() {

    QSqlQuery query;

    // 1. 查询books表中所有图书的存量总和
    int totalStock = 0;
    query.prepare("SELECT SUM(num) FROM books");
    if (query.exec() && query.next()) {
        totalStock = query.value(0).toInt();
    } else {
        QMessageBox::warning(this, "数据查询失败",
                             "无法获取图书存量总和:\n" + query.lastError().text());
        totalStock = 0;
    }
    // 2. 查询borrowing表中未归还的记录总数（当前外借数量）
    int borrowedCount = 0;
    query.prepare("SELECT COUNT(*) FROM borrowing WHERE returned = false");
    if (query.exec() && query.next()) {
        borrowedCount = query.value(0).toInt();
    } else {
        QMessageBox::warning(this, "数据查询失败",
                             "无法获取当前外借图书数量:\n" + query.lastError().text());
        borrowedCount = 0;
    }

    // 3. 计算图书总量和未借数量
    int totalBooks = totalStock + borrowedCount;
    int unborrowedBooks = totalStock;
    // 4. 查询reader表中的读者数量
    int readerCount = 0;
    query.prepare("SELECT COUNT(*) FROM reader");
    if (query.exec() && query.next()) {
        readerCount = query.value(0).toInt();
    } else {
        QMessageBox::warning(this, "数据查询失败",
                             "无法获取读者数量:\n" + query.lastError().text());
        readerCount = 0;
    }

    // ------------------- 创建柱状图 -------------------
    // 创建数据集合
    QBarSet *set = new QBarSet("数量");
    *set << totalBooks << borrowedCount << unborrowedBooks << readerCount;
    set->setColor(ChartConfig().barColor);

    // 创建系列并添加数据
    QBarSeries *series = new QBarSeries();
    series->append(set);
    // 创建图表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(ChartConfig().title);
    chart->setBackgroundBrush(QBrush(QColor(245, 245, 245)));
    if (ChartConfig().enableAnimation)
        chart->setAnimationOptions(QChart::SeriesAnimations);

    // 设置坐标轴
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(ChartConfig().categories);
    axisX->setLabelsFont(ChartConfig().axisFont);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    int maxValue = std::max({totalBooks, borrowedCount, unborrowedBooks, readerCount});
    axisY->setRange(0, maxValue > 0 ? maxValue * 1.1 : 10);
    axisY->setTitleText("数量");
    axisY->setTitleFont(ChartConfig().axisFont);
    axisY->setLabelsFont(ChartConfig().axisFont);
    axisY->setGridLineVisible(ChartConfig().showGrid);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // 设置图例
    chart->legend()->setVisible(ChartConfig().showLegend);
    if (ChartConfig().showLegend) {
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->legend()->setFont(ChartConfig().axisFont);
    }

    // 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);
    // ------------------- 集成到StackedWidget -------------------
    // 创建容器并添加图表视图
    // 获取page_5的布局
    QVBoxLayout *layout = new QVBoxLayout(ui->page_5);
    layout->addWidget(chartView);
    }
void control2::highlight1(){
    ui->pushButton->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_2->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_3->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
}
void control2::highlight2(){
    ui->pushButton_4->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_5->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_12->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
}
void control2::highlight3(){
    ui->pushButton_8->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
    ui->pushButton_9->setStyleSheet("background-color: white; color: black; border-radius: 5px;");

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
void control2::on_pushButton_12_clicked()
{
    ui->stackedWidget2->setCurrentIndex(2);
    highlight2();
    ui->pushButton_12->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}

void control2::on_pushButton_6_clicked()
{
    QString BOOKNAME=ui->BOOKNAMEedit->text();
    QString AUTHOR=ui->AUTHORedit->text();
    QString INTRO=ui->INTROedit->text();
    int NUM = (ui->NUMedit->text()).toInt();
    QUuid uuid = QUuid::createUuid();
    QString uuidStr = uuid.toString(QUuid::WithoutBraces);
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO books (bookID, bookname, author,intro,num) VALUES (?, ?, ?,?,?)");
    insertQuery.bindValue(0, uuidStr);
    insertQuery.bindValue(1, BOOKNAME);
    insertQuery.bindValue(2, AUTHOR);
    insertQuery.bindValue(3,INTRO );
    insertQuery.bindValue(4,NUM );
    if (insertQuery.exec()) {
        QMessageBox::information(this, "", "添加成功");
        initializeLibraryChart();
    } else {
        QMessageBox::warning(this, "错误", "添加失败:" );
    }
}


void control2::on_pushButton_8_clicked()
{
    ui->stackedWidgetsettings->setCurrentIndex(0);
    highlight3();
    ui->pushButton_8->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}


void control2::on_pushButton_9_clicked()
{
    ui->stackedWidgetsettings->setCurrentIndex(1);
    highlight3();
    ui->pushButton_9->setStyleSheet("background-color: black;color: white; border-radius: 5px;");
}



void control2::on_pushButton_10_clicked()
{
    int MONEY = (ui->money->text()).toInt();
    int DAYS = (ui->days->text()).toInt();
    QSqlQuery query;

    query.prepare("UPDATE globe SET money = ?, days = ? WHERE id = (SELECT id FROM globe ORDER BY id LIMIT 1)");
    query.bindValue(0, MONEY);
    query.bindValue(1, DAYS);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "数据更新成功");
    } else {
        // 显示完整的错误信息
        QString errorMsg = "更新失败:\n" + query.lastError().text();
        QMessageBox::warning(this, "错误", errorMsg);

        // 在调试输出中显示更详细的错误
        qDebug() << "SQL 错误:" << query.lastError().databaseText();
        qDebug() << "驱动错误:" << query.lastError().driverText();
        qDebug() << "SQL 语句:" << query.lastQuery();
    }
}


void control2::on_pushButton_11_clicked()
{
    QString searchText = ui->recommend->text(); // 获取搜索框中的文本
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
    Searching->showSearchResults(searchResults,"推荐"); // 显示搜索结果
    Searching->show(); // 显示搜索结果窗口
    initializeLibraryChart();
}


void control2::on_pushButton_7_clicked()
{
    QString searchText = ui->lineEdit_4->text(); // 获取搜索框中的文本
    QString selectedText = ui->comboBox_2->currentText();
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
    Searching->showSearchResults(searchResults,"删除"); // 显示搜索结果
    Searching->show(); // 显示搜索结果窗口
    initializeLibraryChart();
}

void control2::on_pushButton_13_clicked()
{
    QString bookId = ui->lineEdit->text();
    QString bookName = ui->lineEdit_2->text();
    QString author = ui->lineEdit_3->text();
    QString intro = ui->lineEdit_5->text();
    int num = (ui->lineEdit_6->text()).toInt();
    if (bookId.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入要修改的图书 ID");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE books SET bookname = :bookname, author = :author, intro = :intro, num = :num WHERE bookID = :bookId");
    query.bindValue(":bookname", bookName);
    query.bindValue(":author", author);
    query.bindValue(":intro", intro);
    query.bindValue(":num", num);
    query.bindValue(":bookId", bookId);
    if (query.exec()) {
        QMessageBox::information(this, "成功", "图书信息修改成功");
        initializeLibraryChart();
    } else {
        QMessageBox::warning(this, "错误", "图书信息修改失败: " + query.lastError().text());
    }
}


void control2::on_pushButton_14_clicked()
{
    recommending = new recommended();
    recommending->show(); // 显示搜索结果窗口
    recommending->showrecommend1() ;// 显示搜索结果
}

