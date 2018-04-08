#ifndef LIST_DATE_H
#define LIST_DATE_H

#include <QWidget>
#include "config.h"

namespace Ui {
class List_date;
}

class List_date : public QWidget
{
    Q_OBJECT

public:
    explicit List_date(QSqlDatabase &db,QWidget *parent = 0);
    ~List_date();

private slots:
//#ifdef Q_OS_WIN
    void on_export_xls_clicked();
//#endif
    void on_export_xls_2_clicked();

private:
    QDateTime time;
    QString date;
    Ui::List_date *ui;
    QStandardItemModel *model;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlTableModel *tablemodel;
};

#endif // LIST_DATE_H
