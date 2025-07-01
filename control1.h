#ifndef CONTROL1_H
#define CONTROL1_H

#include <QWidget>

namespace Ui {
class control1;
}

class control1 : public QWidget
{
    Q_OBJECT

public:
    explicit control1(QWidget *parent = nullptr);
    ~control1();

private:
    Ui::control1 *ui;
};

#endif // CONTROL1_H
