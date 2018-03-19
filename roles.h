#ifndef ROLES_H
#define ROLES_H

#include <QWidget>

namespace Ui {
class Roles;
}

class Roles : public QWidget
{
    Q_OBJECT

public:
    explicit Roles(QWidget *parent = 0);
    ~Roles();
signals:
    void choiseReady(int);
    void RolesFinished();
private slots:
    void on_pushButton1_clicked();
    void on_pushButtonOK_clicked();
private:
    Ui::Roles *ui;
    int choise;
};

#endif // ROLES_H
