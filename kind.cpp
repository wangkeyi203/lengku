#include "kind.h"
#include "ui_kind.h"

Kind::Kind(QSqlDatabase &db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Kind)
{
    ui->setupUi(this);
    this->db=db;
    tablemodel = new QSqlTableModel(this);
    tablemodel->setTable("kind");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->select();
    tablemodel->setHeaderData(0,Qt::Orientation::Horizontal,"编号");
    tablemodel->setHeaderData(1,Qt::Orientation::Horizontal,"种类");
    tablemodel->setHeaderData(2,Qt::Orientation::Horizontal,"价格");
    tablemodel->setHeaderData(3,Qt::Orientation::Horizontal,"卡号");
    ui->tableView->setModel(tablemodel);

    int id_now;
    query.exec("select * from kind_now where id=1");
    while(query.next())
    {
        id_now  = query.value(1).toInt();
    }
    query.prepare("select * from kind where id=:kind_id");
    query.bindValue(":kind_id",id_now);
    query.exec();
    while(query.next())
    {
        ui->kind_now_2->setText(query.value(1).toString());
    }
    kind_server = new QTcpServer(this);
    kind_socket = new QTcpSocket(this);
    kind_server->listen(QHostAddress::Any,6667);
    connect(kind_server,SIGNAL(newConnection()),this,SLOT(new_cardid_connect()));

}

Kind::~Kind()
{
    delete ui;
}

void Kind::on_refresh_clicked()
{
    this->tablemodel->select();
    ui->tableView->setModel(this->tablemodel);
    int id_now;
    query.exec("select * from kind_now");
    while(query.next())
    {
        id_now  = query.value(1).toInt();
    }
    query.prepare("select * from kind where id=:kind_id");
    query.bindValue(":kind_id",id_now);
    query.exec();
    while(query.next())
    {
        ui->kind_now_2->setText(query.value(1).toString());
    }

}

void Kind::on_add_kind_clicked()
{
    if(ui->kind_name->text().isEmpty() || ui->kind_price->text().isEmpty() || ui->kind_card->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请填入种类和价格和卡号"));
        return;
    }
    int maxid;
    query.exec("select max(id) from kind");
    while (query.next())
    {
        maxid = query.value(0).toInt()+1;
    }
    qDebug()<<maxid;
    query.prepare("insert into kind (id,name,price,cardnum) values (:kind_id,:kind_name,:kind_price,:card_num)");
    query.bindValue(":kind_id",maxid);
    query.bindValue(":kind_name",ui->kind_name->text());
    query.bindValue(":kind_price",ui->kind_price->text());
    query.bindValue(":card_num",ui->kind_card->text());
    query.exec();
    int id_now;
    query.exec("select * from kind_now");
    while(query.next())
    {
        id_now  = query.value(1).toInt();
    }
    query.prepare("select * from kind where id=:kind_id");
    query.bindValue(":kind_id",id_now);
    query.exec();
    while(query.next())
    {
        ui->kind_now_2->setText(query.value(1).toString());
    }

}

void Kind::on_changekind_clicked()
{
    int row = ui->tableView->currentIndex().row();
    int kind_id = ui->tableView->model()->index(row,0).data().toInt();
    QString kind_name=ui->tableView->model()->index(row,1).data().toString();
    if(kind_name.isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请从表中选择一个种类"));
        return;
    }
    query.prepare("update kind_now set kind=:kind_now where id=1");
    query.bindValue(":kind_now",kind_id);
    query.exec();
    qDebug()<<"change kind "<<query.lastError();
    if(!query.isActive())
    {
        QMessageBox::warning(this,tr("提示"),tr("更改种类错误"));
        return;
    }
    ui->kind_now_2->setText(kind_name);
    //int change_id=ui->tableView->currentIndex();

    //query.exec("select * from ")
}
void Kind::new_cardid_connect()
{
    qDebug()<<"new list connect";
    kind_socket=kind_server->nextPendingConnection();
    connect(kind_socket,SIGNAL(readyRead()),this,SLOT(read_cardid()));
    connect(kind_socket,SIGNAL(disconnected()),kind_socket,SLOT(deleteLater()));
}

void Kind::read_cardid()
{
    QString cardid;
    QString test_empty ;
    QByteArray data;
    data = kind_socket->readAll();
    //cardid = data.toStdString();
    qDebug()<<"bytedata is " <<data;
    cardid = data.toHex().data();
    qDebug()<<"cardid is "<<cardid;


    //先从worker中查找是否有当前卡

    query.prepare("select * from kind where cardnum=:card");
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
    kind_socket->write("y,01,12");

    ui->kind_card->setText(cardid);
    //update


}
