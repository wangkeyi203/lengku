#include "login.h"
#include "ui_login.h"

#include <iostream>

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(DATABASE);
}

Login::~Login()
{
    delete ui;
}


void Login::on_login_clicked()
{
    if(ui->passwd->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("密码不能为空！"));
        return;
    }
    if(db.open())
    {
        QMessageBox::warning(this,tr("错误"),tr("数据库打开错误，检查数据库"));
    }

    QSqlQuery query;
    query.prepare("select * from admin where name=:admin");
    query.bindValue(":admin",ui->name->text());
    query.exec();
    while(query.next())
    {
       if(query.value(2).toString() == (ui->passwd->text()))
       {
           this->close();
           Admin *AD = new Admin;
           AD->show();
       }
       ui->passwd->text()=query.value(2).toString();
    }
    QMessageBox::warning(this,tr("错误"),tr("错误"));

}
