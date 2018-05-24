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
    this_kind="未定义";
    this_kindid=0;
}

TcpSocket::~TcpSocket()
{
}


void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data) ;
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
    QString send_data;
    //this->write(data);
    QString card_we = data;
    QByteArray bytedata = data;
    QString flag;
    sqlite->open_database(con);
    qDebug()<<"auto data is "<<data;
    qDebug()<<"byte data is "<<bytedata;
    QByteArrayList datalist = bytedata.split(',');

    QStringList list=card_we.split(',');

    //new
    //work_num,weight1,kind,weight2,time

    qDebug()<<"di san duan is "<<datalist[0];
    qDebug()<<"di san duan is "<<datalist[1];
    qDebug()<<"di san duan is "<<datalist[2];
    qDebug()<<"di san duan is "<<datalist[3];
    qDebug()<<"di san duan is "<<datalist[4];

    QString worker_name;
    QString weight1,weight2;
    QString kind;
    weight1 = datalist[1];
    weight2 = datalist[3];
    float w1 =weight1.toFloat();
    float w2 = weight2.toFloat();
    w1=w1/100;
    w2=w2/100;
    if(sqlite->get_worker_name(datalist[0],worker_name)==false)
    {
        send_data="1";
        this->write(send_data.toLatin1());
        return;
    }
    sqlite->get_kind(datalist[2],kind);

    weight1 = QString("%1").arg(w1);
    weight2 = QString("%1").arg(w2);

    if(sqlite->add_weight_3(worker_name,kind,weight1,weight2,datalist[4]))
    {
        send_data="0";
        this->write(send_data.toLatin1());
        return;
    }
    else
    {
        send_data="1";
        this->write(send_data.toLatin1());
        return;
    }



   /*

    send_data="n,";
    send_data.append(QString::number(this_kindid,10));
    send_data.append(",000");
    for(int i=0;i<3;i++)
    {
        if(list[i].isEmpty())
        {
            qDebug()<<"list"<<list[i];
            this->write(send_data.toLatin1());
            send_data.clear();
            return;
        }
    }

    QString kind;
    int kindid;
    QString card_now=datalist[2].toHex().data();
    //QString card_now=datalist[2];
    if(sqlite->search_from_kind(kind,kindid,card_now))
    {
        this_kind=kind;
        this_kindid=kindid;
        send_data="y,";
        send_data.append(QString::number(this_kindid,10));
        send_data.append(",000");
        this->write(send_data.toLatin1());//
        return;
    }



    flag=list[0];

    QString weight= list[1];
    float w=weight.toFloat();
    w=w/100;
    weight = QString("%1").arg(w);
    QString worker_name;
    QString worker_id;
    //sqlite->get_kind(kind);
    qDebug()<<"kind is "<<this_kind;
    qDebug()<<"weight is "<<weight;
    sqlite->get_worker_name(card_now,worker_name,worker_id);
    qDebug()<<"worker name is "<<worker_name;
    qDebug()<<"flag is "<<flag;

    if(worker_name.isEmpty())
    {
        send_data="n,";
        send_data.append(QString::number(this_kindid,10));
        send_data.append(",000");
        this->write(send_data.toLatin1());
        send_data.clear();
        return;
    }

    if("1" == flag)//毛料
    {
        qDebug()<<last_worker<<worker_name;
        if(last_worker == worker_name)
        {
            if(last_time.elapsed() < 60000)
            {
                qDebug()<<"小于一分钟";
                send_data="n,";
                send_data.append(QString::number(this_kindid,10));
                send_data.append(",");
                send_data.append(worker_id);
                this->write(send_data.toLatin1());
                send_data.clear();
                return;
            }
            qDebug()<<"大于一分钟";
            sqlite->add_weight_1(worker_name,this_kind,weight);
            sqlite->set_flag(worker_name,1);
            sqlite->set_card(worker_name,card_now);
            card_last=card_now;
            last_worker=worker_name;
            last_time.restart();
            send_data="y,";
            send_data.append(QString::number(this_kindid,10));
            send_data.append(",");
            send_data.append(worker_id);
            this->write(send_data.toLatin1());
            send_data.clear();
            return;
        }
        else //换人了
        {
            sqlite->add_weight_1(worker_name,this_kind,weight);
            sqlite->set_flag(worker_name,1);
            sqlite->set_card(worker_name,card_now);
            card_last=card_now;
            last_worker=worker_name;
            last_time.restart();
            send_data="y,";
            send_data.append(QString::number(this_kindid,10));
            send_data.append(",");
            send_data.append(worker_id);
            this->write(send_data.toLatin1());
            send_data.clear();
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
                send_data="n,";
                send_data.append(QString::number(this_kindid,10));
                send_data.append(",");
                send_data.append(worker_id);
                this->write(send_data.toLatin1());
                send_data.clear();
                return;
            }
            else
            {
                if(sqlite->check_card(worker_name,card_now)==false)
                {
                    //卡号对不上
                  //  sqlite->add_weight_1(worker_name,kind,weight);
                   // sqlite->set_flag(worker_name,1);
                   // sqlite->set_card(worker_name,list[2]);
                    //card_last= list[2];
                    send_data="n,";
                    send_data.append(QString::number(this_kindid,10));
                    send_data.append(",");
                    send_data.append(worker_id);
                    this->write(send_data.toLatin1());
                    send_data.clear();
                    return;
                }
                else
                {
                    sqlite->add_weight_2(worker_name,weight);
                    sqlite->set_flag(worker_name,0);
                    card_last = card_now;
                    last_worker = worker_name;
                    send_data="y,";
                    send_data.append(QString::number(this_kindid,10));
                    send_data.append(",");
                    send_data.append(worker_id);
                    this->write(send_data.toLatin1());
                    send_data.clear();
                    return;
                }
            }
        }
        else
        {
            sqlite->add_weight_2(worker_name,weight);
            sqlite->set_flag(worker_name,0);
            card_last = card_now;
            last_worker=worker_name;
            send_data="y,";
            send_data.append(QString::number(this_kindid,10));
            send_data.append(",");
            send_data.append(worker_id);
            this->write(send_data.toLatin1());
            send_data.clear();
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
