#ifndef ADMIN_H
#define ADMIN_H

#include <QMainWindow>
#include "list.h"
#include "worker_list.h"
#include "list_date.h"
#include "kind.h"

namespace Ui {
class Admin;
}

class Admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin(QSqlDatabase db, QWidget *parent = 0);
    ~Admin();

private slots:

    void on_show_list_clicked();

    void on_worker_list_clicked();

    void on_list_date_clicked();

    void on_kind_clicked();

private:
    Ui::Admin *ui;
    List *list;
    List_date *list_date;
    Worker_list *worker_list;
    Kind *kind;
    QSqlDatabase db;
};

#endif // ADMIN_H
