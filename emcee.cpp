#include "emcee.h"

Emcee::Emcee(QObject* parent) : QObject(parent)
{
    s = FIRST_NIGHT;
    player.number = 0;
    player.role = 0;
    dead_player = 0;
    daytime = false;
}

void Emcee::switchState() //дописать voting!!!
{
    if(s == FIRST_NIGHT)
    {
        s = DISCUSSION;
        daytime = true;
    }
    else if (s == DISCUSSION)
    {
        if(playersOnVote.empty())
        {
            s = SHOOTING;
            daytime = false;
            if (player.role < 0 && player.is_dead == false)
                emit NegativeRole();
        }
        else
            s = VOTING;
    }
    else if (s == VOTING)
    {
        if (dead_player != 0)
        {
            s = DEAD_PLAYER;
            if (player.number == dead_player)
            {
                player.is_dead = true;
                emit IamDead();
            }
        }
        else
        {
            s = SHOOTING;
            daytime = false;
            if (player.role < 0 && player.is_dead == false)
                emit NegativeRole();
        }
    }
    else if (s == SHOOTING)
    {
        s = CHECKING;
        if (player.role == 1 || player.role == -2)
            emit ActiveRole(!(player.is_dead));
    }
    else if (s == CHECKING)
    {
        if(dead_player != 0)
        {
            s = DEAD_PLAYER;
            if (player.number == dead_player)
            {
                player.is_dead = true;
                emit IamDead();
            }
        }
        else
        {
            emit CheckResponse("\nNo one is dead!\n");
            s = DISCUSSION;
            daytime = true;
        }
    }
    else if (s == DEAD_PLAYER)
    {
        if(daytime == true)
        {
            s = SHOOTING;
            daytime = false;
            if (player.role < 0 && player.is_dead == false)
                emit NegativeRole();
        }
        else
        {
            s = DISCUSSION;
            daytime = true;
        }

    }
}

void Emcee::settingPlayer(int num)
{
    if (player.number == 0)
        player.number = num;
    else
    {
        player.role = num;
        int n = player.number;
        int r = player.role;
        emit PlayerReady(n, r);
    }
}

void Emcee::processResponse(int num)
{
    switch(s)
    {
    case FIRST_NIGHT:
        settingPlayer(num);
        break;
    case DISCUSSION:
        if(player.number == num)
        {
            emit SpeechAvailable();
        }
        break;
    case CHECKING:
        if (player.role == 1)
        {
            if (num < 0)
                emit CheckResponse("This player is mafia!\n");
            else
                emit CheckResponse("This player is not mafia.\n");
        }
        if (player.role == -2)
        {
            if (num == 1)
                emit CheckResponse("This player is a sheriff!\n");
            else
                emit CheckResponse("This player is not a sheriff.\n");
        }
        break;
    case SHOOTING:
        dead_player = num;
        switchState();
        break;
    }
}

void Emcee::processResponse(QString speech, int num)
{
    switch(s)
    {
    case DISCUSSION:
        playersOnVote.push_back(num);
        emit SpeechNVote(speech, num);
        break;
    case VOTING:
        dead_player = num;
        emit VotingDone(speech, num);
        switchState();
        break;
    }
}

void Emcee::processResponse(QString message)
{
    if(message == "\nFailure. Mafia has won.\n")
        emit Victory(message);
    else if (message == "\nVictory! City has won.\n")
        emit Victory(message);
    else if (message == "Switch.\n")
    {
        if(player.role == -2 && (s == SHOOTING || s == FIRST_NIGHT))
            emit MafiaFinished(message);
        if (s == FIRST_NIGHT || s == DISCUSSION || s == CHECKING)
        switchState();
    }
    else if (s == FIRST_NIGHT)
        MafiaOrders(message);
    else
        emit JustSpeech(message);
}

void Emcee::MafiaOrders(QString orders)
{
    if (player.role < 0)
        emit NegativeRole(orders);
}

void Emcee::PlayerVoted()
{
    player.has_voted = true;
}
