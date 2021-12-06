#ifndef USERDEFINED_H
#define USERDEFINED_H

#include <QWidget>
#include <canthread.h>

namespace Ui {
class UserDefined;
}

class UserDefined : public QWidget
{
    Q_OBJECT

public:
    explicit UserDefined(CANThread* c,QWidget *parent = nullptr);
    ~UserDefined();
    Ui::UserDefined *ui;
signals:
    void sendUserDefParams();

private slots:
    void on_sureBtn_clicked();

private:
    CANThread *canthread;
    void init();
    UINT8 setUserDef();

};

#endif // USERDEFINED_H
