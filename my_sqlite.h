#ifndef MY_SQLITE_H
#define MY_SQLITE_H
#include "config.h"

class My_Sqlite
{

public:
    QSqlQuery *q;
    My_Sqlite();
    void initsql();
    bool add_heave1();
    bool add_heave2();
    bool add_date();
};

#endif // MY_SQLITE_H
