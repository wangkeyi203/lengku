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
    tablemodel->setHeaderData(3,Qt::Orientation::Horizontal,"清单");
    tablemodel->setHeaderData(4,Qt::Orientation::Horizontal,"卡1");
    tablemodel->setHeaderData(5,Qt::Orientation::Horizontal,"卡2");
    tablemodel->setHeaderData(6,Qt::Orientation::Horizontal,"卡3");
    tablemodel->setHeaderData(7,Qt::Orientation::Horizontal,"卡4");
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"卡5");
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"卡6");
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"卡7");
    ui->tableView->setModel(tablemodel);

}

Worker_list::~Worker_list()
{
    delete ui;
}
