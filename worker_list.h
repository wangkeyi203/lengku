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
    QTcpServer *list_server;
    QTcpSocket *list_socket;
    ~Worker_list();

public slots:
    void new_cardid_connect();
    void read_cardid();
    //void sendMessage();

private slots:
    void on_refresh_clicked();

    void on_add_worker_clicked();

private:
    Ui::Worker_list *ui;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlTableModel *tablemodel;

};

#endif // WORKER_LIST_H
