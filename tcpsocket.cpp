#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include "my_sqlite.h"
TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    con = QString::number(socketDescriptor,10);
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
    last_time.start();
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
    auto data = this->readAll();
    //this->write(data);
    QString card_we = data;
    QByteArray bytedata = data;
    QString flag;
    sqlite->open_database(con);
    qDebug()<<"auto data is "<<data;
    qDebug()<<"byte data is "<<bytedata;
    QByteArrayList datalist = bytedata.split(',');
    qDebug()<<"di san duan is "<<datalist[2].toHex().data();
    QStringList list=card_we.split(',');
    for(int i=0;i<3;i++)
    {
        if(list[i].isEmpty())
        {
            qDebug()<<"list"<<list[i];
            this->write("n");
            return;
        }
    }


    flag=list[0];

    QString weight= list[1];
    float w=weight.toFloat();
    w=w/1000;
    weight = QString("%1").arg(w);
    QString worker_name;
    QString kind;
    sqlite->get_kind(kind);
    sqlite->get_worker_name(datalist[2].toHex().data(),worker_name);

    if(worker_name.isEmpty())
    {
        this->write("n");
        return;
    }

    if("0" == flag)//毛料
    {
        qDebug()<<last_worker<<worker_name;
        if(last_worker == worker_name)
        {
            if(last_time.elapsed() < 60000)
            {
                qDebug()<<"小于一分钟";
                this->write("n");
                return;
            }
            qDebug()<<"大于一分钟";
            sqlite->add_weight_1(worker_name,kind,weight);
            sqlite->set_flag(worker_name,1);
            sqlite->set_card(worker_name,list[2]);
            card_last=list[2];
            last_worker=worker_name;
            last_time.restart();
            this->write("y");
            return;
        }
        else //换人了
        {
            sqlite->add_weight_1(worker_name,kind,weight);
            sqlite->set_flag(worker_name,1);
            sqlite->set_card(worker_name,list[2]);
            card_last=list[2];
            last_worker=worker_name;
            last_time.restart();
            this->write("y");
            return;

        }


    }
    else //半成品
    {
        if(last_worker == worker_name)
        {
            if(last_time.elapsed() < 60000)
            {
                qDebug()<<"小于一分钟";
                this->write("n");
                return;
            }
            else
            {
                if(sqlite->check_card(worker_name,list[2])==false)
                {
                    //卡号对不上
                  //  sqlite->add_weight_1(worker_name,kind,weight);
                   // sqlite->set_flag(worker_name,1);
                   // sqlite->set_card(worker_name,list[2]);
                    //card_last= list[2];
                    this->write("n");
                    return;
                }
                else
                {
                    sqlite->add_weight_2(worker_name,weight);
                    sqlite->set_flag(worker_name,0);
                    card_last = list[2];
                    this->write("y");
                    return;
                }
            }
        }
        else
        {
            sqlite->add_weight_2(worker_name,weight);
            sqlite->set_flag(worker_name,0);
            card_last = list[2];
            this->write("y");
            return;
        }
    }

    /*
    QStringList list=card_we.split(',');
    QString weight= list[0];
    float w=weight.toFloat();
    w=w/1000;
    weight = QString("%1").arg(w);

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
    }*/


    //data1 =data;
   // QString num = QString::number(this->socketID,10);

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
