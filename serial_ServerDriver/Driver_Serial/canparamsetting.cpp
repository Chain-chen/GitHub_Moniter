#include "canparamsetting.h"
#include "ui_canparamsetting.h"
#include <QFile>
#include <QDebug>
#include<QMessageBox>
#include <QSerialPortInfo>
#include <synchapi.h>

CANParamSetting::CANParamSetting(CANThread* c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CANParamSetting),
    canthread(c)
{
    ui->setupUi(this);
    mSerialPort = new QSerialPort();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            serial.setDataTerminalReady(true);
            portNames.append(serial.portName());
            ui->serialPortComboBox->addItem(serial.portName());
            serial.close();
        }
    }
}


CANParamSetting::~CANParamSetting()
{
    delete ui;
}


void CANParamSetting::on_connectSerialButton_clicked()
{
    if(ui->connectSerialButton->text()==tr(u8"连接")){
        BaudRate = ui->BaudRateComboBox->currentText().toInt();
        if(canthread->openSerialPort(ui->serialPortComboBox->currentText(),BaudRate)){
            ui->connectSerialButton->setText(tr(u8"断开"));
            QMessageBox::information(this,tr(u8"打开串口"),tr(u8"串口打开成功！"),"OK");
            emit serialPortOpened();
            this->hide();
        }
        else{
            QMessageBox::critical(this,tr(u8"打开串口"),tr(u8"串口打开失败！"),QMessageBox::Close);
        }
    }
    else{
        canthread->closeSerialPort();
    }
}

void CANParamSetting::on_quitBtn_clicked()
{
    this->close();
}
