#include "my_sqlite.h"
#include <QDebug>

My_Sqlite::My_Sqlite(QString con_num)
{
    db = QSqlDatabase::addDatabase("QSQLITE",con_num);
    db.setDatabaseName("DATABASE");
    query = new QSqlQuery(con_num);
    if (db.open())
    {
        qDebug() << "open success";
    }
    else
    {
        qDebug() << "open failed";
    }

}

bool My_Sqlite::test_add(QString test)
{

    query->exec("select max(id) from test");
    while(query->next())
    {
        id = "4";
        qDebug() << query->value(0).toInt();
    }
    query->prepare("insert into test values (:test_id,:test,:test)");
    query->bindValue(":test_id",id.toInt());
    query->bindValue(":test",test);
    query->exec();
    if(!query->isActive())
    {
        qDebug() << "add test error";
        return false;
    }
    else {
        return true;
    }
}
bool My_Sqlite::add_weight_1(QString &id, QString weight)
{

}
