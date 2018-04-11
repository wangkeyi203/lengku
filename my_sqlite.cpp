#include "my_sqlite.h"
#include <QDebug>

My_Sqlite::My_Sqlite(QString con_num)
{
}

bool My_Sqlite::open_database(QString con_num)
{
    if(this->db.isOpen())
    {
        qDebug()<<this->db.isOpen();
        return true;
    }
    this->db=QSqlDatabase::addDatabase("QSQLITE",con_num);
    this->db.setDatabaseName(DATABASE);
    //qDebug()<< QSqlDatabase::contains("test");
    if (this->db.open())
    {
        qDebug() << "open success";
    }
    else
    {
        qDebug() << "open failed";
        return false;
    }
   this->query = new QSqlQuery(this->db);
   // query =QSqlQuery::QSqlQuery(db);
    return true;
}
bool My_Sqlite::close_database(QString con_num)
{
    QSqlDatabase::removeDatabase(con_num);
    return true;
}
bool My_Sqlite::test_add(QString test)
{
    QString test2 = "黄花";
    QString test3 = "10";
    QString con;
    if(this->add_weight_1(test,test2,test3) == false)
    {
        qDebug() << " add_weight_1 failed";
    }
    return true;
}
bool My_Sqlite::add_weight_1(QString worker_name,QString kind,QString weight)
{
    query->exec("select max(id) from list");
    int id;
    while(query->next())
    {
        id = query->value(0).toInt()+1;
    }
    write_time = time.currentDateTime().toString("hh:mm:ss");
    //再将id写入worker表中,用来第二次写入的时候查询
    query->prepare("update worker set listid=:id where name=:worker_name");
    query->bindValue(":id",QString::number(id,10));
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }

    query->prepare("insert into list (id,name,kind,weight1,time1) values(:list_id,:worker_name,:list_kind,:list_weight1,:list_time1)");
    query->bindValue(":list_id",QString::number(id,10));
    query->bindValue(":worker_name",worker_name);
    query->bindValue(":list_kind",kind);
    query->bindValue(":list_weight1",weight);
    query->bindValue(":list_time1",write_time);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }

    unsigned int write_date_id =0;
    float list_weight=weight.toFloat();
    int num1=0;
    QString write_date=time.currentDateTime().toString("yyyy.MM.dd");
    query->prepare("select * from list_date where name=:worker_name and date=:write_date");
    query->bindValue(":worker_name",worker_name);
    query->bindValue(":write_date",write_date);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    while(query->next())
    {
        write_date_id=query->value(0).toInt();
        num1=query->value(3).toInt()+1;

    }

    if (0 == write_date_id)
    {
        //添加该工人当天的行
        num1 =1;
        query->exec("select max(id) from list_date");
        while(query->next())
        {
            write_date_id = query->value(0).toInt()+1;
        }
        query->prepare("insert into list_date (id,name,num1,num2,date) values (:list_id,:worker_name,:list_num1,:list_num2,:list_date)");
        query->bindValue(":list_id",write_date_id);
        query->bindValue(":worker_name",worker_name);
        query->bindValue(":list_num1",0);
        query->bindValue(":list_num2",0);
        query->bindValue(":list_date",write_date);
        query->exec();
        qDebug()<<query->lastError();
    }

    query->prepare("select * from list_date where id=:list_id");
    query->bindValue(":list_id",write_date_id);
    query->exec();
    while(query->next())
    {
        qDebug()<<list_weight;
        qDebug()<<query->value(2).toString();
        list_weight=list_weight+query->value(2).toFloat();
        qDebug()<<list_weight;
    }
    weight=QString("%1").arg(list_weight);
    qDebug()<<weight;
    query->prepare("update list_date set weight1=:weight,num1=:list_num where id=:list_id");
    query->bindValue(":weight",weight);
    query->bindValue(":list_num",num1);
    query->bindValue(":list_id",write_date_id);
    query->exec();

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

    unsigned int write_date_id =0;
    float weight1,weight2,persent;
    unsigned int con1,con2;

    query->prepare("select * from list where id=:list_id");
    query->bindValue(":list_id",id);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    while (query->next())
    {
        weight1=query->value(3).toFloat();
    }
    weight2=weight.toFloat();
    persent=weight2/weight1;

    write_time = time.currentDateTime().toString("hh:mm:ss");
    write_date = time.currentDateTime().toString("yyyy.MM.dd");
    query->prepare("update list set weight2=:weight,time2=:time,percent=:list_persent,date=:write_date where id=:list_id");
    query->bindValue(":weight",weight);
    query->bindValue(":time",write_time);
    query->bindValue(":list_persent",QString("%1").arg(persent));
    query->bindValue(":list_id",QString::number(id,10));
    query->bindValue(":write_date",write_date);
    query->exec();

    if(!query->isActive())
    {
        return false;
    }


    QString write_date=time.currentDateTime().toString("yyyy.MM.dd");
    weight2=weight.toFloat();
    query->prepare("select * from list_date where name=:worker_name and date=:write_date");
    query->bindValue(":worker_name",worker_name);
    query->bindValue(":write_date",write_date);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    while (query->next()) {
        write_date_id=query->value(0).toInt();
        weight1=query->value(2).toFloat();
        weight2=weight2+query->value(4).toFloat();
        con1=query->value(3).toInt()+1;
        con2=query->value(5).toInt()+1;
    }
    persent=weight2/weight1;

    query->prepare("update list_date set weight2=:weight ,num2=:num,persent=:list_persent where id=:list_id");
    query->bindValue(":weight",QString("%1").arg(weight2));
    query->bindValue(":num",QString::number(con2,10));
    query->bindValue(":list_persent",QString("%1").arg(persent));
    query->bindValue(":list_id",write_date_id);
    query->exec();

    return true;
}

bool My_Sqlite::get_worker_name(QString card_num, QString &worker_name)
{
    query->prepare("select * from worker where card2 =:card or card3 =:card or card4 =:card or card5 =:card or card6 =:card or card7 =:card or card8 =:card or card9 =:card or card10 =:card or card11 =:card or card12 =:card or card13 =:card or card14 =:card or card15 =:card or card16 =:card or card17 =:card or card18 =:card or card19 =:card or card20 =:card or card21 =:card");
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
        qDebug()<<"flag is ";
        qDebug()<<flag_n;
    }
    return flag_n;
}

bool My_Sqlite::set_flag(QString worker_name,int flag)
{
        query->prepare("update worker set flag=:flag_n where name=:worker_name");
        query->bindValue(":flag_n",flag);
        query->bindValue(":worker_name",worker_name);
        query->exec();
        if(!query->isActive())
        {
            return false;
        }

    return true;
}

bool My_Sqlite::get_kind(QString &kind)
{
    int kind_num;
    query->exec("select * from kind_now where id=1");
    if(!query->isActive())
    {
        return false;
    }
    while(query->next())
    {
        kind_num = query->value(1).toInt();
    }

    query->prepare("select * from kind where id=:kind_id");
    query->bindValue(":kind_id",kind_num);
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

bool My_Sqlite::set_card(QString worker_name, QString card)
{
    query->prepare("update worker set card1=:card_num where name=:worker_name");
    query->bindValue(":card_num",card);
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    return true;

}

bool My_Sqlite::check_card(QString worker_name, QString card1)
{
    QString card2;
    query->prepare("select * from worker where name=:worker_name");
    query->bindValue(":worker_name",worker_name);
    query->exec();
    if(!query->isActive())
    {
        return false;
    }
    while(query->next())
    {
        card2=query->value(4).toString();
    }

    if(card1!=card2)
    {
        return false;
    }
    return true;

}

