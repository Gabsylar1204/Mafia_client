#ifndef EMCEE_H
#define EMCEE_H
#include "player.h"
#include <QtCore>
#include <vector>

using namespace std;

enum STATE { FIRST_NIGHT, DEAD_PLAYER, DISCUSSION, VOTING, SHOOTING, CHECKING };

class Emcee : public QObject
{
    Q_OBJECT

public:
    Player player;
    int dead_player;
    bool daytime;
    STATE s;
    vector<int> playersOnVote;
    explicit Emcee(QObject *parent = 0);
signals:
    void PlayerReady(int, int);
    void SpeechAvailable();
    void SpeechNVote(QString, int);
    void VotingDone(QString, int);
    void ActiveRole(bool);
    void CheckResponse(QString);
    void NegativeRole();
    void NegativeRole(QString);
    void IamDead();
    void Victory(QString);
    void JustSpeech(QString);
    void MafiaFinished(QString);
public slots:
    void settingPlayer(int);
    void processResponse(int);
    void processResponse(QString, int);
    void processResponse(QString);
    void MafiaOrders(QString);
    void PlayerVoted();
    void switchState();
//private:
};

#endif // EMCEE_H
