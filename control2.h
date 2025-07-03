#ifndef CONTROL2_H
#define CONTROL2_H

#include <QWidget>
#include <QStackedWidget>
namespace Ui {
class control2;
}

class control2 : public QWidget
{
    Q_OBJECT

public:
    explicit control2(QWidget *parent = nullptr);
    ~control2();
    void initializeLibraryChart();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void highlight1();

    void highlight2();

    void highlight3();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::control2 *ui;
};

#endif // CONTROL2_H
