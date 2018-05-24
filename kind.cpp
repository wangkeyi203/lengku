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
    tablemodel->setHeaderData(0,Qt::Orientation::Horizontal,"种类编号");
    tablemodel->setHeaderData(1,Qt::Orientation::Horizontal,"种类名称");
    tablemodel->setHeaderData(2,Qt::Orientation::Horizontal,"价格");
    tablemodel->setHeaderData(3,Qt::Orientation::Horizontal,"卡号");
    ui->tableView->setModel(tablemodel);
    ui->kind_id->hide();


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
}

void Kind::on_add_kind_clicked()
{
    if(ui->kind_name->text().isEmpty() || ui->kind_price->text().isEmpty())
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
    query.prepare("insert into kind (id,name,price) values (:kind_id,:kind_name,:kind_price)");
    query.bindValue(":kind_id",maxid);
    query.bindValue(":kind_name",ui->kind_name->text());
    query.bindValue(":kind_price",ui->kind_price->text());
    query.exec();
    if(!query.isActive())
    {
        QMessageBox::warning(this,tr("提示"),tr("添加种类错误"));
        return;
    }

    this->tablemodel->select();
    ui->tableView->setModel(this->tablemodel);

    this->tablemodel->select();
    ui->tableView->setModel(this->tablemodel);
    int id_now;
    query.exec("select * from kind_now");
    while(query.next())
    {
        id_now  = query.value(1).toInt();
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
    QString kind_name2=ui->kind_name->text();
    QString kind_price=ui->kind_price->text();
    query.prepare("update kind set name=:kind_name,price=:kind_price where id=:kind_id");
    query.bindValue(":kind_name",kind_name2);
    query.bindValue(":kind_price",kind_price);
    query.bindValue(":kind_id",kind_id);
    query.exec();
    qDebug()<<"change kind "<<query.lastError();
    if(!query.isActive())
    {
        QMessageBox::warning(this,tr("提示"),tr("更改种类错误"));
        return;
    }

    this->tablemodel->select();
    ui->tableView->setModel(this->tablemodel);
    int id_now;
    query.exec("select * from kind_now");
    while(query.next())
    {
        id_now  = query.value(1).toInt();
    }

    /*
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
*/
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
    QByteArray write_back="0,";
    QByteArray data;
    data = kind_socket->readAll();
    //cardid = data.toStdString();
    qDebug()<<"bytedata is " <<data;
    cardid = data.toHex().data();
    qDebug()<<"cardid is "<<cardid;

    QString id=ui->kind_id->text();
    uint l = id.length();
    for(l;l<2;l++)
    {
        write_back.append("0");

    }
    write_back.append(id);
    kind_socket->write(write_back);

    //先从worker中查找是否有当前卡
    /*
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
*/

}

void Kind::on_tableView_clicked(const QModelIndex &index)
{
    int row=ui->tableView->currentIndex().row();
    int kind_id=ui->tableView->model()->index(row,0).data().toInt();
    query.prepare("select * from kind where id=:kind_id");
    query.bindValue(":kind_id",kind_id);
    query.exec();
    while(query.next())
    {
        ui->kind_name->setText(query.value(1).toString());
        ui->kind_id->setText(ui->tableView->model()->index(row,0).data().toString());
        ui->kind_price->setText(query.value(0).toString());
    }
}
