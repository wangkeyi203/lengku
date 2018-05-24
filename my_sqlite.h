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
    QString write_date;
    QDateTime time;
    //QString id; //list的id
    bool open_database(QString con_num);
    bool close_database(QString con_num);
    bool get_worker_name(QString work_id,QString &worker_name);
    int  check_flag(QString worker_name);
    bool set_flag(QString worker_name,int flag);
    bool set_card(QString worker_name,QString card);
    bool check_card(QString worker_name,QString card1);
    bool add_weight_1(QString worker_name,QString kind,QString weight);//新加一条，再写入毛料重量,时间
    bool add_weight_2(QString worker_name,QString weight);//成品重量加时间.id用来在list中检索
    bool add_weight_3(QString worker_name,QString kind,QString weight1,QString weight2,QString time2);
    bool search_from_kind(QString &kind,int &kindid,QString cardid);
    bool get_kind(QString kind_id,QString &kind);
    bool add_date();
    bool test_add(QString test);
};

#endif // MY_SQLITE_H
