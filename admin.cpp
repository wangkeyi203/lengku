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
