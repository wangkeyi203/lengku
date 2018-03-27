#ifndef MY_SQLITE_H
#define MY_SQLITE_H
#include "config.h"

class My_Sqlite
{

public:
    explicit My_Sqlite(QString con_num);
    QSqlQuery *query;
    QSqlDatabase db;
    QString id;
    QString get_worker_name(QString card_num);
    bool check_flag(QString worker_name);
    bool set_flag(QString worker_name);
    bool add_weight_1(QString &id,QString weight);//毛料重量加时间
    bool add_weight_2(QString &id,QString weight);//成品重量加时间.id用来在list中检索
    bool add_date();
    bool test_add(QString test);
};

#endif // MY_SQLITE_H
