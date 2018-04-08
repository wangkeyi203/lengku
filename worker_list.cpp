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
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"卡5");
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"卡6");
    ui->tableView->setModel(tablemodel);
    list_server = new QTcpServer(this);
    list_socket = new QTcpSocket(this);
    list_server->listen(QHostAddress::Any,6667);
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
    QString data;
    data = list_socket->readAll();
    //先从worker中查找是否有当前卡
    //循环看那个卡位置是空的
    //update

    qDebug()<<data;
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

        query.prepare("insert into worker (id,name,flag) values (:worker_id,:worker_name,:worker_flag)");
        query.bindValue(":worker_id",maxid);
        query.bindValue(":worker_name",ui->worker_name->text());
        query.bindValue(":worker_flag",0);
        query.exec();


        QString worker_name = ui->worker_name->text();
        int row =ui->tableView->currentIndex().row();
        //QModelIndex test = tablemodel->index(row,3).data().toString();

        qDebug()<< tablemodel->index(row,4).data().toString();
        //tablemodel->index(row,4).model()->setData("123123");

    }
}

void Worker_list::on_tableView_clicked(const QModelIndex &index)
{
    ui->card_num->setText("11111111");
}
