#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    ui->speechpushButton->setEnabled(false);
    ui->votepushButton->setEnabled(false);
    ui->checkpushButton->setEnabled(false);
    ui->shootpushButton->setEnabled(false);
    ui->shootpushButton->setVisible(false);
    ui->checklineEdit->setReadOnly(true);
    ui->checklineEdit->setVisible(false);
    ui->checkpushButton->setVisible(false);
    ui->shootlcdNumber->setVisible(false);
    ui->speechtextEdit->setReadOnly(true);
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
}

Client::~Client()
{
    delete ui;
    delete timer;
}

void Client::on_votepushButton_clicked()
{
    ui->votepushButton->setEnabled(false);
    int vote = ui->votelabel->text().toInt();
    emit Vote(vote);
}

void Client::on_speechpushButton_clicked()
{
    SendPushed();
}

void Client::setAvatar(int n, int r)
{
    ui->playerlabel->setText("Player #" + QString::number(n));
    switch (r)
    {
    case 1:
        ui->checklineEdit->setVisible(true);
        ui->checkpushButton->setVisible(true);
        ui->avatarlabel->setText("sheriff");
        break;
    case -1:
    {
        ui->shootpushButton->setVisible(true);
        ui->avatarlabel->setText("mafia");
        break;
    }
    case -2:
        ui->checklineEdit->setVisible(true);
        ui->checkpushButton->setVisible(true);
        ui->shootpushButton->setVisible(true);
        ui->avatarlabel->setText("don");
        DonCommands();
        break;
    default:
        ui->avatarlabel->setText("citizen");
        break;
    }
}
void Client::ReadyToShow()
{
    this->show();
}

void Client::on_shootpushButton_clicked()
{
    int shot = ui->shootlcdNumber->intValue();
    emit Shoot(shot);
}

void Client::on_checkpushButton_clicked()
{
    ui->checkpushButton->setEnabled(false);
    int check = ui->checklineEdit->text().toInt();
    ui->checklineEdit->clear();
    ui->checklineEdit->setReadOnly(true);
    emit Check(check);
}

void Client::SpeechAvailable()
{
    ui->timerlcdNumber->display(60);
    QTimer::singleShot(60000, this, SLOT(SendPushed()));
    timer->start();
    ui->speechtextEdit->setReadOnly(false);
    ui->speechpushButton->setEnabled(true);
}
void Client::ChatUpdate(QString speech)
{
    ui->chattextBrowser->append(speech);
}
void Client::SpeechNVote(QString speech, int num)
{
    ui->chattextBrowser->append(speech);
    ui->onvotelabel->text().append(" " + QString::number(num));
}
void Client::EmceeSpeaks(QString result)
{
    ui->emceetextBrowser->append(result);
}
void Client::DeadPlayer()
{
    ui->timerlcdNumber->display(30);
    QTimer::singleShot(30000, this, SLOT(SendPushed()));
    timer->start();
    ui->speechtextEdit->setReadOnly(false);
    ui->speechpushButton->setEnabled(true);
}
void Client::MafiaOrders(QString orders)
{
    ui->speechtextEdit->append(orders);
    ui->timerlcdNumber->display(5);
    QTimer::singleShot(5000, this, SLOT(ClearCommands()));
    timer->start();
}
void Client::updateTime()
{
    if(ui->timerlcdNumber->intValue() == 10)
        emit Warning();
    int num = ui->timerlcdNumber->intValue() - 1;
    int shot = ui->shootlcdNumber->intValue() + 1;
    ui->timerlcdNumber->display(num);
    ui->shootlcdNumber->display(shot);
}
void Client::Warning()
{
    ui->emceetextBrowser->append("\nYou have 10 seconds left!\n");
}

void Client::SendPushed()
{
    timer->stop();
    ui->speechpushButton->setEnabled(false);
    ui->timerlcdNumber->display(0);
    QString text = ui->speechtextEdit->toPlainText();
    ui->speechtextEdit->clear();
    ui->speechtextEdit->setReadOnly(true);
    emit Speech(text);
}

void Client::NegativeRole()
{
    EmceeSpeaks("\nGet ready to shoot!\n");
    ui->shootlcdNumber->display(0);
    ui->shootpushButton->setEnabled(true);
    QTimer::singleShot(3000, this, SLOT(Shooting()));
}
void Client::Shooting()
{
    ui->shootlcdNumber->setVisible(true);
    ui->timerlcdNumber->setVisible(false);
    QTimer::singleShot(10000, this, SLOT(ShootingDone()));
    timer->start();
}
void Client::ClearCommands()
{
    timer->stop();
    ui->timerlcdNumber->display(0);
    ui->speechtextEdit->clear();
    emit MafiaDone("Switch.\n");
}
void Client::ActiveRole(bool b)
{
    if(b == true)
    {
        EmceeSpeaks("\nWhich player you'll be checking?\n");
        ui->checklineEdit->setReadOnly(false);
        ui->checkpushButton->setEnabled(true);
    }
    else
        EmceeSpeaks("\nJust keep sleeping:)\n");
}
void Client::ShootingDone()
{
    timer->stop();
    ui->shootpushButton->setEnabled(false);
    ui->shootlcdNumber->display(0);
    ui->shootlcdNumber->setVisible(false);
    ui->timerlcdNumber->setVisible(true);
    emit ShootingFinished("Switch.\n");
}

void Client::DonCommands()
{
    ui->speechtextEdit->setReadOnly(false);
    ui->speechpushButton->setEnabled(true);
    EmceeSpeaks("\nYou are don, order to your mafias!\n");
}

void Client::Victory(QString team)
{
    EmceeSpeaks(team);
}
