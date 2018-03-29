#include "my_sqlite.h"
#include <QDebug>

My_Sqlite::My_Sqlite(QString con_num)
{
    db = QSqlDatabase::addDatabase("QSQLITE",con_num);
    db.setDatabaseName("DATABASE");
    write_time = new QString;
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
    QString id;
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
bool My_Sqlite::add_weight_1(QString worker_name,QString kind,QString weight)
{
    query->exec("select max(id) from list");
    while(query->next())
    {
        int id = query->value(0).toInt()+1;
    }
    write_time = time.currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    //再将id写入worker表中
    query->prepare("update into worker valude (list_id=:id)");
    query->bindValue(":id",QString::number(id,10));


    query->prepare("insert into list values (id=:list_id,name=:worker_name,kind=:list_kind,weight1=:list_weight1,time1=:list_time1)");
    query->bindValue(":list_id",QString::number(id,10));
    query->bindValue(":worker_name",worker_name);
    query->bindValue(":list_kind",kind);
    query->bindValue(":list_weight",weight);
    query->bindValue(":list_time1",write_time);

    if(!query->isActive())
    {
        return false;
    }
    return true;
}

bool My_Sqlite::get_worker_name(QString card_num, QString &worker_name)
{
    query->prepare("select * from worker where card1 =:card or card2 =:card or card3 =:card or card4 =:card or card5 =:card or card6 =:card or card7 =:card");
    query->bindValue(":card",card_num);
    query->exec();
    if (!query->isActive())
    {
        return false;
    }
    while(query->next())
    {
        worker_name = query->value(1).toString();
    }
    return true;
}

int My_Sqlite::check_flag(QString worker_name)
{
    query->prepare("select * from worker where name =:worker_name");
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if (!query->isActive())
    {
        return -1;
    }
    int flag_n;
    while(query->next())
    {
        flag_n = query->value(2).toInt();
    }
    return flag_n;
}

bool My_Sqlite::set_flag(QString worker_name)
{
    QString flag;
    query->prepare("select * from worker where name =:worker_name");
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    while(query->next())
    {
        flag = query->value(2).toString(); //获取flag值
    }
    if(flag.toInt(0,10) ==1)
    {
        query->prepare("update worker set flag=:flag_n");
        query->bindValue("flag_n",0);
        query->exec();
        if(!query->isActive())
        {
            return false;
        }

    }
    else
    {
        query->prepare("update worker set flag=:flag_n");
        query->bindValue("flag_n",1);
        query->exec();
        if(!query->isActive())
        {
            return false;
        }
    }
    return true;
}
