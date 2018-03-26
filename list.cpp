#include "list.h"
#include "ui_list.h"

List::List(QSqlDatabase &db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List)
{
    ui->setupUi(this);
    this->db = db;
    tablemodel = new QSqlTableModel(this);
    tablemodel->setTable("list");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->select();
    tablemodel->setHeaderData(0,Qt::Orientation::Horizontal,"编号");
    tablemodel->setHeaderData(1,Qt::Orientation::Horizontal,"姓名");
    tablemodel->setHeaderData(2,Qt::Orientation::Horizontal,"种类");
    tablemodel->setHeaderData(3,Qt::Orientation::Horizontal,"毛料");
    tablemodel->setHeaderData(4,Qt::Orientation::Horizontal,"时间");
    tablemodel->setHeaderData(5,Qt::Orientation::Horizontal,"成品");
    tablemodel->setHeaderData(6,Qt::Orientation::Horizontal,"时间");
    tablemodel->setHeaderData(7,Qt::Orientation::Horizontal,"出成");
    tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"日期");
    ui->tableView->setModel(tablemodel);
    /*
    model = new QStandardItemModel(this);
    model->insertColumns(0,8);
    model->setHeaderData(0,Qt::Horizontal,tr("姓名"));
    model->setHeaderData(1,Qt::Horizontal,tr("种类"));
    model->setHeaderData(2,Qt::Horizontal,tr("毛料"));
    model->setHeaderData(3,Qt::Horizontal,tr("时间"));
    model->setHeaderData(4,Qt::Horizontal,tr("成品"));
    model->setHeaderData(5,Qt::Horizontal,tr("时间"));
    model->setHeaderData(6,Qt::Horizontal,tr("出成"));
    model->setHeaderData(7,Qt::Horizontal,tr("日期"));

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);*/

}

List::~List()
{
    delete ui;
}
