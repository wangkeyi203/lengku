#include "login.h"
#include <QApplication>
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TcpServer ser;
    ser.listen(QHostAddress::Any,6666);

    Login w;
    w.show();

    return a.exec();
}
