#ifndef CONTROL1_H
#define CONTROL1_H

#include <QWidget>
#include <QDate>
namespace Ui {
class control1;
}

class control1 : public QWidget
{
    Q_OBJECT

public:
    explicit control1(QWidget *parent = nullptr);
    ~control1();
    void setUsername(const QString& username);
    void showrecommend();
    void showBorrowedBooks() ;

private slots:
    void on_search_clicked();
    void highlight1();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::control1 *ui;
};

struct BorrowInfo {
    QString bookName;
    QString author;
    QString intro;
    int num;
    QString id;          // bookID
    QString borrowkey;       // 新增borrowkey字段
    QDate borrowedDate;
    QDate returnDate;
    bool returned;
};
#endif // CONTROL1_H
