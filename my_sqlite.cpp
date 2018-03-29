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
    QString test1 = "张三";
    QString test2 = "黄花";
    QString test3 = "10";
    if(this->add_weight_1(test1,test2,test3) == false)
    {
        qDebug() << " add_weight_1 failed";
    }
}
bool My_Sqlite::add_weight_1(QString worker_name,QString kind,QString weight)
{
    query->exec("select max(id) from list");
    int id;
    while(query->next())
    {
        id = query->value(0).toInt()+1;
    }
    write_time = time.currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    //再将id写入worker表中,用来第二次写入的时候查询
    query->prepare("update worker set list_id=:id where name=:worker_name");
    query->bindValue(":id",QString::number(id,10));
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    query->prepare("insert into list values (id=:list_id,name=:worker_name,kind=:list_kind,weight1=:list_weight1,time1=:list_time1)");
    query->bindValue(":list_id",QString::number(id,10));
    query->bindValue(":worker_name",worker_name);
    query->bindValue(":list_kind",kind);
    query->bindValue(":list_weight",weight);
    query->bindValue(":list_time1",write_time);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    return true;
}

bool My_Sqlite::add_weight_2(QString worker_name, QString weight)
{
    query->prepare("select * from worker where name=:worker_name");
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    int id;
    while (query->next())
    {
        id = query->value(3).toInt();
    }
    write_time = time.currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    query->prepare("update list set weight2=:weight,time2=:time where id=:list_id");
    query->bindValue(":weight",weight);
    query->bindValue(":time",write_time);
    query->bindValue(":list_id",QString::number(id,10));
    query->exec();
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

bool My_Sqlite::get_kind(QString &kind)
{
    int kind_num;
    query->exec("select * from kind_now");
    while(query->next())
    {
        kind_num = query->value(1).toInt();
    }
    query->prepare("select * from kind where id=:kind_id");
    query->bindValue("kind_id",QString::number(kind_num,10));
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    while(query->next())
    {
        kind = query->value(1).toString();
    }
    return true;
}
