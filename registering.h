#ifndef REGISTERING_H
#define REGISTERING_H

#include <QWidget>
namespace Ui {
class registering;
}

class registering : public QWidget
{
    Q_OBJECT

public:
    explicit registering(QWidget *parent = nullptr);
    ~registering();

private slots:
    void on_pushButton_clicked();

private:
    Ui::registering *ui;
};
#endif // REGISTERING_H
