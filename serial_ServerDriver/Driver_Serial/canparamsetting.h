#ifndef CANPARAMSETTING_H
#define CANPARAMSETTING_H

#include <QDialog>
#include <canthread.h>
#include <QSerialPort>

namespace Ui {
class CANParamSetting;
}

class CANParamSetting : public QDialog
{
    Q_OBJECT

public:
    explicit CANParamSetting(CANThread* c,QWidget *parent = 0);
    ~CANParamSetting();


private slots:


    void on_connectSerialButton_clicked();

    void on_quitBtn_clicked();

private:
    QList<QString> portNames;
    Ui::CANParamSetting *ui;
    CANThread *canthread;
    QSerialPort *mSerialPort;
    int BaudRate;
signals:
    void serialPortOpened();
};

#endif // CANPARAMSETTING_H
