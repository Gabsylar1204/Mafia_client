#include "roles.h"
#include "client_w.h"
#include "client.h"
#include "emcee.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client_W w;
    Emcee e;
    Roles r;
    Client c;

    QObject::connect(&r, SIGNAL(choiseReady(int)), &w, SLOT(writeData(int)));
    QObject::connect(&r, SIGNAL(RolesFinished()), &c, SLOT(ReadyToShow()));
    QObject::connect(&c, SIGNAL(Speech(QString)), &w, SLOT(writeData(QString)));
    QObject::connect(&c, SIGNAL(Shoot(int)), &w, SLOT(writeData(int)));
    QObject::connect(&c, SIGNAL(Check(int)), &w, SLOT(writeData(int)));
    QObject::connect(&c, SIGNAL(Vote(int)), &w, SLOT(writeData(int)));
    QObject::connect(&c, SIGNAL(warning()), &c, SLOT(Warning()));
    QObject::connect(&c, SIGNAL(ShootingFinished(QString)), &e, SLOT(processResponse(QString)));
    QObject::connect(&c, SIGNAL(MafiaDone(QString)), &e, SLOT(processResponse(QString)));
    QObject::connect(&w, SIGNAL(dataReceived(int)), &e, SLOT(processResponse(int)));
    QObject::connect(&w, SIGNAL(dataReceived(QString)), &e, SLOT(processResponse(QString)));
    QObject::connect(&w, SIGNAL(dataReceived(QString,int)), &e, SLOT(processResponse(QString,int)));
    QObject::connect(&e, SIGNAL(PlayerReady(int,int)), &c, SLOT(setAvatar(int,int)));
    QObject::connect(&e, SIGNAL(SpeechAvailable()), &c, SLOT(SpeechAvailable()));
    QObject::connect(&e, SIGNAL(SpeechNVote(QString,int)), &c, SLOT(SpeechNVote(QString,int)));
    QObject::connect(&e, SIGNAL(ActiveRole(bool)), &c, SLOT(ActiveRole(bool)));
    QObject::connect(&e, SIGNAL(CheckResponse(QString)), &c, SLOT(EmceeSpeaks(QString)));
    QObject::connect(&e, SIGNAL(NegativeRole()), &c, SLOT(NegativeRole()));
    QObject::connect(&e, SIGNAL(NegativeRole(QString)), &c, SLOT(MafiaOrders(QString)));
    QObject::connect(&e, SIGNAL(IamDead()), &c, SLOT(DeadPlayer()));
    QObject::connect(&e, SIGNAL(JustSpeech(QString)), &c, SLOT(ChatUpdate(QString)));
    QObject::connect(&e, SIGNAL(VotingDone(QString,int)), &c, SLOT(EmceeSpeaks(QString)));
    QObject::connect(&e, SIGNAL(MafiaFinished(QString)), &w, SLOT(writeData(QString)));
    QObject::connect(&e, SIGNAL(Victory(QString)), &c, SLOT(Victory(QString)));

    w.connectToHost("localhost");
    r.show();

    return a.exec();
}
