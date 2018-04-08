#ifndef KIND_H
#define KIND_H

#include <QWidget>
#include "config.h"

namespace Ui {
class Kind;
}

class Kind : public QWidget
{
    Q_OBJECT

public:
    explicit Kind(QSqlDatabase &db,QWidget *parent = 0);
    ~Kind();

private slots:
    void on_refresh_clicked();

private:
    Ui::Kind *ui;
    QStandardItemModel *model;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlTableModel *tablemodel;
};

#endif // KIND_H
