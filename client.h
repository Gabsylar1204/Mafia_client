#ifndef CLIENT_H
#define CLIENT_H

#include <QTimer>
#include <QDialog>
#include "client_w.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
signals:
    void Speech(QString);
    void Check(int);
    void Shoot(int);
    void ShootingFinished(QString);
    void VotingDone();
    void MafiaDone(QString);
    void Vote(int);
    void warning();
public slots:
    void setAvatar(int, int);
    void ReadyToShow();
    void SpeechAvailable();
    void ChatUpdate(QString);
    void SpeechNVote(QString, int);
    void EmceeSpeaks(QString);
    void DeadPlayer();
    void MafiaOrders(QString);
    void SendPushed();
    void NegativeRole();
    void ClearCommands();
    void ActiveRole(bool);
    void ShootingDone();
    void Shooting();
    void DonCommands();
    void Victory(QString);
private slots:
    void on_votepushButton_clicked();
    void on_speechpushButton_clicked();
    void on_shootpushButton_clicked();
    void on_checkpushButton_clicked();
    void updateTime();
    void Warning();

private:
    Ui::Client *ui;
    QTimer* timer;
};

#endif // CLIENT_H
