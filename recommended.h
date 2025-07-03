#ifndef RECOMMENDED_H
#define RECOMMENDED_H

#include <QWidget>

namespace Ui {
class recommended;
}

class recommended : public QWidget
{
    Q_OBJECT

public:
    explicit recommended(QWidget *parent = nullptr);
    ~recommended();
    void showrecommend1();
private:
    Ui::recommended *ui;
};

#endif // RECOMMENDED_H
