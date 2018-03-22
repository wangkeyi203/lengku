#include "admin.h"
#include "ui_admin.h"

Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}



void Admin::on_show_list_clicked()
{
    list = new List;
    setCentralWidget(list);
}
