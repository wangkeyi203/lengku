#include "admin.h"
#include "ui_admin.h"

Admin::Admin(QSqlDatabase db,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    this->db = db;
}

Admin::~Admin()
{
    delete ui;
}



void Admin::on_show_list_clicked()
{
    list = new List(this->db,this);
    setCentralWidget(list);
}

void Admin::on_worker_list_clicked()
{
    worker_list = new Worker_list(this->db,this);
    setCentralWidget(worker_list);
}
