#include "client_w.h"

static inline QByteArray IntToArray(qint32 source);
static inline qint32 ArrayToInt(QByteArray source);

Client_W::Client_W(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

bool Client_W::connectToHost(QString host)
{
    socket->connectToHost(host, 1024);
    connect(socket, SIGNAL(readyRead()), SLOT(readData()));
    return socket->waitForConnected();
}

bool Client_W::writeData(QString data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray code = IntToArray(1);
        socket->write(IntToArray(code.size()));
        socket->write(code);
        socket->write(IntToArray(data.toUtf8().size()));
        socket->write(data.toUtf8());
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

bool Client_W::writeData(int data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray temp = IntToArray(data);
        QByteArray code = IntToArray(0);
        socket->write(IntToArray(code.size()));
        socket->write(code);
        socket->write(IntToArray(code.size()));
        socket->write(temp);
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

void Client_W::readData()
{
    while (socket->bytesAvailable() > 0)
    {
        buffer.append(socket->readAll());
        while ((size == 0 && buffer.size() >= 4) || (size > 0 && buffer.size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer.size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer.mid(0, 4));
                buffer.remove(0, 4);
            }
            if (size > 0 && buffer.size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                QByteArray coding = buffer.mid(0, size);
                buffer.remove(0, size);
                size = 0;
                int code = ArrayToInt(coding);
                switch(code)
                {
                case 0:
                {
                    int p = readInt();
                    emit dataReceived(p);
                    break;
                }
                case 1:
                {
                    QString p = readString();
                    emit dataReceived(p);
                    break;
                }
                case 2:
                    QString q = readString();
                    int num = readInt();
                    emit dataReceived(q, num);
                    break;
                }
            }
        }
    }
}

int Client_W::readInt()
{
    if (size == 0 && buffer.size() >= 4) //if size of data has received completely, then store it on our global variable
    {
        size = ArrayToInt(buffer.mid(0, 4));
        buffer.remove(0, 4);
    }
    if (size > 0 && buffer.size() >= size) // If data has received completely, then emit our SIGNAL with the data
    {
        QByteArray data = buffer.mid(0, size);
        buffer.remove(0, size);
        size = 0;
        return ArrayToInt(data);
    }
}

QString Client_W::readString()
{
    if (size == 0 && buffer.size() >= 4) //if size of data has received completely, then store it on our global variable
    {
        size = ArrayToInt(buffer.mid(0, 4));
        buffer.remove(0, 4);
    }
    if (size > 0 && buffer.size() >= size) // If data has received completely, then emit our SIGNAL with the data
    {
        QByteArray data = buffer.mid(0, size);
        buffer.remove(0, size);
        size = 0;
        QString text(data);
        return text;
    }
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}
