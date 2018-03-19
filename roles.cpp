#include "roles.h"
#include "ui_roles.h"

Roles::Roles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Roles)
{
    ui->setupUi(this);
    ui->pushButtonOK->setEnabled(false);
}

Roles::~Roles()
{
    delete ui;
}

void Roles::on_pushButton1_clicked()
{
    choise = ui->pushButton1->objectName().remove("pushButton").toInt();
    ui->pushButtonOK->setEnabled(true);
    emit choiseReady(choise);
}

void Roles::on_pushButtonOK_clicked()
{
    emit RolesFinished();
    this->close();
}

