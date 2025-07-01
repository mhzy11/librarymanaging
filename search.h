#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

private:
    Ui::Search *ui;
};

#endif // SEARCH_H
