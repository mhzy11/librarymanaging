#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include "BookInfo.h"
namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();
     void showSearchResults(QList<BookInfo>& results,QString Type); // 显示搜索结果
private:
    Ui::Search *ui;
};

#endif // SEARCH_H
