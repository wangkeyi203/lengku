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
    QTcpServer *kind_server;
    QTcpSocket *kind_socket;
    ~Kind();
public slots:
    void new_cardid_connect();
    void read_cardid();
private slots:
    void on_refresh_clicked();

    void on_add_kind_clicked();

    void on_changekind_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Kind *ui;
    QStandardItemModel *model;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlTableModel *tablemodel;
};

#endif // KIND_H
