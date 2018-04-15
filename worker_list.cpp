#include "worker_list.h"
#include "ui_worker_list.h"

Worker_list::Worker_list(QSqlDatabase &db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Worker_list)
{
    ui->setupUi(this);
    this->db = db;
    tablemodel = new QSqlTableModel(this);
    tablemodel->setTable("worker");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->select();
    tablemodel->setHeaderData(0,Qt::Orientation::Horizontal,"编号");
    tablemodel->setHeaderData(1,Qt::Orientation::Horizontal,"姓名");
    tablemodel->setHeaderData(2,Qt::Orientation::Horizontal,"标志位");
    tablemodel->setHeaderData(3,Qt::Orientation::Horizontal,"清单号");
    tablemodel->setHeaderData(4,Qt::Orientation::Horizontal,"当前卡");
    tablemodel->setHeaderData(5,Qt::Orientation::Horizontal,"卡1");
    tablemodel->setHeaderData(6,Qt::Orientation::Horizontal,"卡2");
    tablemodel->setHeaderData(7,Qt::Orientation::Horizontal,"卡3");
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"卡4");
    tablemodel->setHeaderData(9,Qt::Orientation::Horizontal,"卡5");
    tablemodel->setHeaderData(10,Qt::Orientation::Horizontal,"卡6");
    tablemodel->setHeaderData(11,Qt::Orientation::Horizontal,"卡7");
    tablemodel->setHeaderData(12,Qt::Orientation::Horizontal,"卡8");
    tablemodel->setHeaderData(13,Qt::Orientation::Horizontal,"卡9");
    tablemodel->setHeaderData(14,Qt::Orientation::Horizontal,"卡10");
    tablemodel->setHeaderData(15,Qt::Orientation::Horizontal,"卡11");
    tablemodel->setHeaderData(16,Qt::Orientation::Horizontal,"卡12");
    tablemodel->setHeaderData(17,Qt::Orientation::Horizontal,"卡13");
    tablemodel->setHeaderData(18,Qt::Orientation::Horizontal,"卡14");
    tablemodel->setHeaderData(19,Qt::Orientation::Horizontal,"卡15");
    tablemodel->setHeaderData(20,Qt::Orientation::Horizontal,"卡16");
    tablemodel->setHeaderData(21,Qt::Orientation::Horizontal,"卡17");
    tablemodel->setHeaderData(22,Qt::Orientation::Horizontal,"卡18");
    tablemodel->setHeaderData(23,Qt::Orientation::Horizontal,"卡19");
    tablemodel->setHeaderData(24,Qt::Orientation::Horizontal,"卡20");
    ui->tableView->setModel(tablemodel);
    list_server = new QTcpServer(this);
    list_socket = new QTcpSocket(this);
    list_server->listen(QHostAddress::Any,6668);
    connect(list_server,SIGNAL(newConnection()),this,SLOT(new_cardid_connect()));

}

Worker_list::~Worker_list()
{
    delete ui;
}

void Worker_list::new_cardid_connect()
{
    qDebug()<<"new list connect";
    list_socket=list_server->nextPendingConnection();
    connect(list_socket,SIGNAL(readyRead()),this,SLOT(read_cardid()));
    connect(list_socket,SIGNAL(disconnected()),list_socket,SLOT(deleteLater()));


}

void Worker_list::read_cardid()
{
    QString cardid;
    QString test_empty ;
    QByteArray data;
    data = list_socket->readAll();
    //cardid = data.toStdString();
    qDebug()<<"bytedata is " <<data;
    cardid = data.toHex().data();
    qDebug()<<"cardid is "<<cardid;

    //cardid= list_socket->readAll();
    qDebug()<<"cardid is "<<cardid;
    //先从worker中查找是否有当前卡

    query.prepare("select * from worker where card2 =:card or card3 =:card or card4 =:card or card5 =:card or card6 =:card or card7 =:card or card8 =:card or card9 =:card or card10 =:card or card11 =:card  or card12 =:card or card13 =:card or card14 =:card or card15 =:card or card16 =:card or card17 =:card or card18 =:card or card19 =:card or card20 =:card or card21 =:card");
    query.bindValue(":card",cardid);
    query.exec();
    while(query.next())
    {
        test_empty = query.value(1).toString();
    }
    if(!test_empty.isEmpty())
    {
        qDebug()<<"youka";
        QMessageBox::warning(this,tr("错误"),tr("该卡已经添加进数据库中"));
        return;
    }

    //循环看那个卡位置是空的
    QList<QLineEdit *> card_list;
    card_list <<ui->card_num_1<<ui->card_num_2<<ui->card_num_3<<ui->card_num_4<<ui->card_num_5<<ui->card_num_6<<ui->card_num_7       \
              <<ui->card_num_8<<ui->card_num_9<<ui->card_num_10<<ui->card_num_11<<ui->card_num_12<<ui->card_num_13<<ui->card_num_14  \
              <<ui->card_num_15<<ui->card_num_16<<ui->card_num_17<<ui->card_num_18<<ui->card_num_19<<ui->card_num_20;
    int j=0;
    for(int i=0;i<20;i++)
    {
        if(card_list[i]->text().isEmpty())
        {
            for(j=0;j<20;j++)
            {
                if(card_list[j]->text() == cardid)
                {
                    QMessageBox::warning(this,tr("错误"),tr("该卡在列表中已经存在"));
                    return;
                }
            }
            card_list[i]->setText(cardid);
            break;
        }
    }
    //update


}

void Worker_list::on_refresh_clicked()
{
    this->tablemodel->select();
    ui->tableView->setModel(this->tablemodel);
}

void Worker_list::on_add_worker_clicked()
{
    if(ui->worker_name->text().isEmpty())
    {
        QMessageBox::warning(this,tr("错误"),tr("工人姓名不能为空"));
        return;
    }
    else
    {
        int maxid;
        query.exec("select max(id) from worker");
        while(query.next())
        {
            maxid = query.value(0).toInt() +1;
        }

        //query.prepare("insert into worker (id,name,flag,card2,card3,card4,card5,card6,card7,card8,card9,card10,card11,card12,card13,card14,card15,card16,card17,card18,card19,card20,card21) values (:worker_id,:worker_name,:worker_flag,:card_2,:card_3:card_4:card_5:card_6:card_7:card_8:card_9:card_10:card_11:card_12:card_13:card_14:card_15:card_16:card_17:card_18:card_19:card_20:card_21)");
        query.prepare("insert into worker (id,name,flag) values (:worker_id,:worker_name,:worker_flag)");
        query.bindValue(":worker_id",maxid);
        query.bindValue(":worker_name",ui->worker_name->text());
        query.bindValue(":worker_flag",0);
        query.exec();
        qDebug()<<"add worker error"<<query.lastError();
        if(!query.isActive())
        {
            QMessageBox::warning(this,tr("错误"),tr("添加新工人失败！"));
            return ;
        }

        ui->worker_name->clear();
        ui->card_1->clear();
        ui->card_2->clear();
        ui->card_3->clear();
        ui->card_4->clear();
        ui->card_5->clear();
        ui->card_6->clear();
        ui->card_7->clear();
        ui->card_8->clear();
        ui->card_9->clear();
        ui->card_10->clear();
        ui->card_11->clear();
        ui->card_12->clear();
        ui->card_13->clear();
        ui->card_14->clear();
        ui->card_15->clear();
        ui->card_16->clear();
        ui->card_17->clear();
        ui->card_18->clear();
        ui->card_19->clear();
        ui->card_20->clear();

    }
}

void Worker_list::on_tableView_clicked(const QModelIndex &index)
{
    int row =ui->tableView->currentIndex().row();
    //QStandardItemModel this_id;
    //qDebug()<<ui->tableView->model()->data(this_id.index(row,0)).toString();
    int worker_id =ui->tableView->model()->index(row,0).data().toInt();
    query.prepare("select * from worker where id=:worker_id");
    query.bindValue(":worker_id",worker_id);
    query.exec();
    while(query.next())
    {
        id_now = query.value(0).toInt();
        //ui->worker_id_now->setText(query.value(0).toString());
        ui->worker_name->setText(query.value(1).toString());
        ui->card_num_1->setText(query.value(5).toString());
        ui->card_num_2->setText(query.value(6).toString());
        ui->card_num_3->setText(query.value(7).toString());
        ui->card_num_4->setText(query.value(8).toString());
        ui->card_num_5->setText(query.value(9).toString());
        ui->card_num_6->setText(query.value(10).toString());
        ui->card_num_7->setText(query.value(11).toString());
        ui->card_num_8->setText(query.value(12).toString());
        ui->card_num_9->setText(query.value(13).toString());
        ui->card_num_10->setText(query.value(14).toString());
        ui->card_num_11->setText(query.value(15).toString());
        ui->card_num_12->setText(query.value(16).toString());
        ui->card_num_13->setText(query.value(17).toString());
        ui->card_num_14->setText(query.value(18).toString());
        ui->card_num_15->setText(query.value(19).toString());
        ui->card_num_16->setText(query.value(20).toString());
        ui->card_num_17->setText(query.value(21).toString());
        ui->card_num_18->setText(query.value(22).toString());
        ui->card_num_19->setText(query.value(23).toString());
        ui->card_num_20->setText(query.value(24).toString());

    }

}

void Worker_list::on_add_card_clicked()
{
    query.prepare("update worker set name=:worker_name,card2=:card_2,card3=:card_3,card4=:card_4,card5=:card_5,card6=:card_6,card7=:card_7,card8=:card_8,card9=:card_9,card10=:card_10,card11=:card_11,card12=:card_12,card13=:card_13,card14=:card_14,card15=:card_15,card16=:card_16,card17=:card_17,card18=:card_18,card19=:card_19,card20=:card_20,card21=:card_21 where id=:worker_id");
    query.bindValue(":worker_name",ui->worker_name->text());
    query.bindValue(":card_2",ui->card_num_1->text());
    query.bindValue(":card_3",ui->card_num_2->text());
    query.bindValue(":card_4",ui->card_num_3->text());
    query.bindValue(":card_5",ui->card_num_4->text());
    query.bindValue(":card_6",ui->card_num_5->text());
    query.bindValue(":card_7",ui->card_num_6->text());
    query.bindValue(":card_8",ui->card_num_7->text());
    query.bindValue(":card_9",ui->card_num_8->text());
    query.bindValue(":card_10",ui->card_num_9->text());
    query.bindValue(":card_11",ui->card_num_10->text());
    query.bindValue(":card_12",ui->card_num_11->text());
    query.bindValue(":card_13",ui->card_num_12->text());
    query.bindValue(":card_14",ui->card_num_13->text());
    query.bindValue(":card_15",ui->card_num_14->text());
    query.bindValue(":card_16",ui->card_num_15->text());
    query.bindValue(":card_17",ui->card_num_16->text());
    query.bindValue(":card_18",ui->card_num_17->text());
    query.bindValue(":card_19",ui->card_num_18->text());
    query.bindValue(":card_20",ui->card_num_19->text());
    query.bindValue(":card_21",ui->card_num_20->text());
    query.bindValue(":worker_id",id_now);
    query.exec();
    qDebug()<<"update worker_list "<<query.lastError();
    if(!query.isActive())
    {
        QMessageBox::warning(this,tr("错误"),tr("更新工人信息失败！"));
    }


}
