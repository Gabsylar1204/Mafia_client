#ifndef CLIENT_W_H
#define CLIENT_W_H

#include <QtCore>
#include <QtNetwork>
#include <vector>

class Client_W : public QObject
{
    Q_OBJECT
public:
    explicit Client_W(QObject *parent = 0);

signals:
    void dataReceived(int);
    void dataReceived(QString);
    void dataReceived(QString, int);
public slots:
    bool connectToHost(QString host);
    bool writeData(QString data);
    bool writeData(int data);
    void readData();
    int readInt();
    QString readString();

private:
    QTcpSocket* socket;
    QByteArray buffer; //We need a buffer to store data until block has completely received
    qint32 size = 0; //We need to store the size to verify if a block has received completely
};


#endif // CLIENT_W_H
