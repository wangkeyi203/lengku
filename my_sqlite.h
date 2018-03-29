#ifndef MY_SQLITE_H
#define MY_SQLITE_H
#include "config.h"

class My_Sqlite
{

public:
    explicit My_Sqlite(QString con_num);
    QSqlQuery *query;
    QSqlDatabase db;
    QString write_time;
    QDateTime time;
    //QString id; //list的id
    bool get_worker_name(QString card_num,QString &worker_name);
    int check_flag(QString worker_name);
    bool set_flag(QString worker_name);
    bool add_weight_1(QString worker_name,QString kind,QString weight);//新加一条，再写入毛料重量,时间
    bool add_weight_2(QString worker_name,QString weight);//成品重量加时间.id用来在list中检索
    bool get_kind(QString &kind);
    bool add_date();
    bool test_add(QString test);
};

#endif // MY_SQLITE_H
