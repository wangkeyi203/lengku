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
    void initsql(QString con_num);
    bool add_heave1();
    bool add_heave2();
    bool add_date();
    bool test_add(QString test,QString con_num);
};

#endif // MY_SQLITE_H
