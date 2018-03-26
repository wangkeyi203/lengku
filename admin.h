#ifndef ADMIN_H
#define ADMIN_H

#include <QMainWindow>
#include "list.h"


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

private:
    Ui::Admin *ui;
    List *list;
    QSqlDatabase db;
};

#endif // ADMIN_H
