#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "admin.h"
#include "config.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:


    void on_login_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase db;
};

#endif // LOGIN_H
