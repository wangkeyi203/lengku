#include "list.h"
#include "ui_list.h"

List::List(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List)
{
    ui->setupUi(this);
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
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

List::~List()
{
    delete ui;
}
