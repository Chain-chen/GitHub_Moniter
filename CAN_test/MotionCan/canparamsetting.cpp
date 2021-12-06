#include "canparamsetting.h"
#include "ui_canparamsetting.h"
#include <QFile>
#include <QDebug>
#include<QMessageBox>

CANParamSetting::CANParamSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CANParamSetting)
{
    ui->setupUi(this);

    connect(ui->saveBtn,&QPushButton::clicked,this,&CANParamSetting::saveCanParam);
    connect(ui->CANType, QOverload<int>::of(&QComboBox::activated),[&](int index){
        if(index==1){
            ui->CANCom->setCurrentIndex(0);
            ui->CANCom->setEnabled(false);
        }
        else{
            ui->CANCom->setEnabled(true);
        }

    });
}

CANParamSetting::~CANParamSetting()
{
    delete ui;
}

void CANParamSetting::saveCanParam()
{
    //can设备类型
    int type = ui->CANType->currentIndex();
    switch (type) {
        case 0:
            CANtype=3;
            break;
        case 1:
            CANtype=4;
            break;
        case 2:
            CANtype=4;
            break;
        case 3:
            CANtype=4;
            break;
        case 4:
            CANtype=4;
            break;
        default:
            break;
    }

    index = ui->CANIndex->currentIndex();
    baundRate = (int)ui->CANBaundRate->currentText().remove("Kbps").toFloat();
    devicCOM = ui->CANCom->currentIndex();
    QMessageBox::information(this,tr(u8"设置"),tr(u8"CAN参数设置成功!"),"OK");
    this->hide();
}

void CANParamSetting::on_quitBtn_clicked()
{
    this->hide();
}
