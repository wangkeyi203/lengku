#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include "config.h"

namespace Ui {
class List;
}

class List : public QWidget
{
    Q_OBJECT

public:
    explicit List(QWidget *parent = 0);
    ~List();

private:
    Ui::List *ui;
    QStandardItemModel *model;


};

#endif // LIST_H
