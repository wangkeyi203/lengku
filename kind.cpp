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
    if(ui->kind_name->text().isEmpty() ||ui->kind_price->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请填入种类和价格"));
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
