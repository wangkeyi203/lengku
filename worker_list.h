#ifndef WORKER_LIST_H
#define WORKER_LIST_H

#include <QWidget>
#include "config.h"

namespace Ui {
class Worker_list;
}

class Worker_list : public QWidget
{
    Q_OBJECT

public:
    explicit Worker_list(QSqlDatabase &db,QWidget *parent = 0);
    ~Worker_list();

private:
    Ui::Worker_list *ui;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlTableModel *tablemodel;
};

#endif // WORKER_LIST_H
