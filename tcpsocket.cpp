#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include "my_sqlite.h"
TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    qDebug() << socketDescriptor;
    con = QString::number(socketDescriptor,10);
    //qDebug() << con;
    //qDebug() << this->socketID;
    sqlite = new My_Sqlite(con);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect" ;
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);

    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{
}


void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
        qDebug() << "test" ;
    }
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::readData()
{
//    datas.append(this->readAll());
    //auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->peerPort());
    auto data = this->readAll();
    this->write(data);
    qDebug() << data;
    QString card_we = data;
    sqlite->open_database(con);

    QStringList list=card_we.split(',');
    QString weight= list[0];
    float w=weight.toFloat();
    w=w/1000;
    weight =QString("%1").arg(w);

    QString worker_name;
    QString kind;
    sqlite->get_kind(kind);
    sqlite->get_worker_name(list[1],worker_name);
    qDebug()<<worker_name;
    if (sqlite->check_flag(worker_name) == 0)
    {
        //原料
        sqlite->add_weight_1(worker_name,kind,weight);
        sqlite->set_flag(worker_name,1);
        sqlite->set_card(worker_name,list[1]);
    }
    else {
        //半成品
        if(sqlite->check_card(worker_name,list[1])==false)
        {
            //卡号对不上
            sqlite->add_weight_1(worker_name,kind,weight);
            sqlite->set_flag(worker_name,1);
            sqlite->set_card(worker_name,list[1]);
        }
        else {
            sqlite->add_weight_2(worker_name,weight);
            sqlite->set_flag(worker_name,0);
        }
    }


    data1 =data;
    QString num = QString::number(this->socketID,10);

//    if (!watcher.isRunning())//放到异步线程中处理。
//    {
//        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
//    }
}

QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
    QTime time;
    time.start();

    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}
