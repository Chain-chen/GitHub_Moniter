#include "userdefined.h"
#include "ui_userdefined.h"
#include <QMessageBox>

UserDefined::UserDefined(CANThread* c,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDefined),
    canthread(c)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_ShowModal);             //设置为模态对话框
    init();
}

UserDefined::~UserDefined()
{
    delete ui;
}

void UserDefined::init()
{
    ui->checkBox_hall->setChecked(canthread->m_UINT8_UserDef[0]);
    ui->checkBox_1->setChecked(canthread->m_UINT8_UserDef[1]);
    ui->checkBox_2->setChecked(canthread->m_UINT8_UserDef[2]);
    ui->checkBox_3->setChecked(canthread->m_UINT8_UserDef[3]);
    ui->checkBox_4->setChecked(canthread->m_UINT8_UserDef[4]);
    ui->checkBox_5->setChecked(canthread->m_UINT8_UserDef[5]);
    ui->checkBox_6->setChecked(canthread->m_UINT8_UserDef[6]);
    ui->checkBox_7->setChecked(canthread->m_UINT8_UserDef[7]);
    ui->checkBox_8->setChecked(canthread->m_UINT8_UserDef[8]);
    ui->checkBox_9->setChecked(canthread->m_UINT8_UserDef[9]);
    ui->checkBox_10->setChecked(canthread->m_UINT8_UserDef[10]);
    ui->checkBox_11->setChecked(canthread->m_UINT8_UserDef[11]);
    ui->checkBox_12->setChecked(canthread->m_UINT8_UserDef[12]);
    ui->checkBox_13->setChecked(canthread->m_UINT8_UserDef[13]);
    ui->checkBox_14->setChecked(canthread->m_UINT8_UserDef[14]);
    ui->checkBox_15->setChecked(canthread->m_UINT8_UserDef[15]);
    ui->checkBox_16->setChecked(canthread->m_UINT8_UserDef[16]);

    ui->checkBox_17->setChecked(canthread->m_UINT8_UserDef[17]);
    ui->checkBox_18->setChecked(canthread->m_UINT8_UserDef[18]);
    ui->checkBox_19->setChecked(canthread->m_UINT8_UserDef[19]);
    ui->checkBox_20->setChecked(canthread->m_UINT8_UserDef[20]);
    ui->checkBox_21->setChecked(canthread->m_UINT8_UserDef[21]);
    ui->checkBox_22->setChecked(canthread->m_UINT8_UserDef[22]);
    ui->checkBox_23->setChecked(canthread->m_UINT8_UserDef[23]);
    ui->checkBox_24->setChecked(canthread->m_UINT8_UserDef[24]);
    ui->checkBox_25->setChecked(canthread->m_UINT8_UserDef[25]);
    ui->checkBox_26->setChecked(canthread->m_UINT8_UserDef[26]);
    ui->checkBox_27->setChecked(canthread->m_UINT8_UserDef[27]);
    ui->checkBox_28->setChecked(canthread->m_UINT8_UserDef[28]);
    ui->checkBox_29->setChecked(canthread->m_UINT8_UserDef[29]);
    ui->checkBox_30->setChecked(canthread->m_UINT8_UserDef[30]);
    ui->checkBox_31->setChecked(canthread->m_UINT8_UserDef[31]);


    ui->lineEdit->setText(QString::number(canthread->m_INT32_UserDef[0]));
    ui->lineEdit_2->setText(QString::number(canthread->m_INT32_UserDef[1]));
    ui->lineEdit_3->setText(QString::number(canthread->m_INT32_UserDef[2]));
    ui->lineEdit_4->setText(QString::number(canthread->m_INT32_UserDef[3]));
    ui->lineEdit_5->setText(QString::number(canthread->m_INT32_UserDef[4]));
    ui->lineEdit_6->setText(QString::number(canthread->m_INT32_UserDef[5]));
    ui->lineEdit_7->setText(QString::number(canthread->m_INT32_UserDef[6]));
    ui->lineEdit_8->setText(QString::number(canthread->m_INT32_UserDef[7]));
    ui->lineEdit_9->setText(QString::number(canthread->m_INT32_UserDef[8]));
    ui->lineEdit_10->setText(QString::number(canthread->m_INT32_UserDef[9]));
    ui->lineEdit_11->setText(QString::number(canthread->m_INT32_UserDef[10]));
    ui->lineEdit_12->setText(QString::number(canthread->m_INT32_UserDef[11]));
    ui->lineEdit_13->setText(QString::number(canthread->m_INT32_UserDef[12]));
    ui->lineEdit_14->setText(QString::number(canthread->m_INT32_UserDef[13]));
    ui->lineEdit_15->setText(QString::number(canthread->m_INT32_UserDef[14]));
    ui->lineEdit_16->setText(QString::number(canthread->m_INT32_UserDef[15]));
    ui->lineEdit_17->setText(QString::number(canthread->m_INT32_UserDef[16]));
    ui->lineEdit_18->setText(QString::number(canthread->m_INT32_UserDef[17]));
    ui->lineEdit_19->setText(QString::number(canthread->m_INT32_UserDef[18]));
    ui->lineEdit_20->setText(QString::number(canthread->m_INT32_UserDef[19]));
    ui->lineEdit_21->setText(QString::number(canthread->m_INT32_UserDef[20]));
    ui->lineEdit_22->setText(QString::number(canthread->m_INT32_UserDef[21]));
    ui->lineEdit_23->setText(QString::number(canthread->m_INT32_UserDef[22]));
    ui->lineEdit_24->setText(QString::number(canthread->m_INT32_UserDef[23]));

}

UINT8 UserDefined::setUserDef()
{
    canthread->m_UINT8_UserDef[0] = ui->checkBox_hall->checkState();
    canthread->m_UINT8_UserDef[1] = ui->checkBox_1->checkState();
    canthread->m_UINT8_UserDef[2] = ui->checkBox_2->checkState();
    canthread->m_UINT8_UserDef[3] = ui->checkBox_3->checkState();
    canthread->m_UINT8_UserDef[4] = ui->checkBox_4->checkState();
    canthread->m_UINT8_UserDef[5] = ui->checkBox_5->checkState();
    canthread->m_UINT8_UserDef[6] = ui->checkBox_6->checkState();
    canthread->m_UINT8_UserDef[7] = ui->checkBox_7->checkState();
    canthread->m_UINT8_UserDef[8] = ui->checkBox_8->checkState();
    canthread->m_UINT8_UserDef[9] = ui->checkBox_9->checkState();
    canthread->m_UINT8_UserDef[10] = ui->checkBox_10->checkState();
    canthread->m_UINT8_UserDef[11] = ui->checkBox_11->checkState();
    canthread->m_UINT8_UserDef[12] = ui->checkBox_12->checkState();
    canthread->m_UINT8_UserDef[13] = ui->checkBox_13->checkState();
    canthread->m_UINT8_UserDef[14] = ui->checkBox_14->checkState();
    canthread->m_UINT8_UserDef[15] = ui->checkBox_15->checkState();
    canthread->m_UINT8_UserDef[16] = ui->checkBox_16->checkState();
    canthread->m_UINT8_UserDef[17] = ui->checkBox_17->checkState();
    canthread->m_UINT8_UserDef[18] = ui->checkBox_18->checkState();
    canthread->m_UINT8_UserDef[19] = ui->checkBox_19->checkState();
    canthread->m_UINT8_UserDef[20] = ui->checkBox_20->checkState();
    canthread->m_UINT8_UserDef[21] = ui->checkBox_21->checkState();
    canthread->m_UINT8_UserDef[22] = ui->checkBox_22->checkState();
    canthread->m_UINT8_UserDef[23] = ui->checkBox_23->checkState();
    canthread->m_UINT8_UserDef[24] = ui->checkBox_24->checkState();
    canthread->m_UINT8_UserDef[25] = ui->checkBox_25->checkState();
    canthread->m_UINT8_UserDef[26] = ui->checkBox_26->checkState();
    canthread->m_UINT8_UserDef[27] = ui->checkBox_27->checkState();
    canthread->m_UINT8_UserDef[28] = ui->checkBox_28->checkState();
    canthread->m_UINT8_UserDef[29] = ui->checkBox_29->checkState();
    canthread->m_UINT8_UserDef[30] = ui->checkBox_30->checkState();
    canthread->m_UINT8_UserDef[31] = ui->checkBox_31->checkState();

    canthread->m_INT32_UserDef[0] = ui->lineEdit->text().toInt();
    canthread->m_INT32_UserDef[1] = ui->lineEdit_2->text().toInt();
    canthread->m_INT32_UserDef[2] = ui->lineEdit_3->text().toInt();
    canthread->m_INT32_UserDef[3] = ui->lineEdit_4->text().toInt();
    canthread->m_INT32_UserDef[4] = ui->lineEdit_5->text().toInt();
    canthread->m_INT32_UserDef[5] = ui->lineEdit_6->text().toInt();
    canthread->m_INT32_UserDef[6] = ui->lineEdit_7->text().toInt();
    canthread->m_INT32_UserDef[7] = ui->lineEdit_8->text().toInt();
    canthread->m_INT32_UserDef[8] = ui->lineEdit_9->text().toInt();
    canthread->m_INT32_UserDef[9] = ui->lineEdit_10->text().toInt();
    canthread->m_INT32_UserDef[10] = ui->lineEdit_11->text().toInt();
    canthread->m_INT32_UserDef[11] = ui->lineEdit_12->text().toInt();
    canthread->m_INT32_UserDef[12] = ui->lineEdit_13->text().toInt();
    canthread->m_INT32_UserDef[13] = ui->lineEdit_14->text().toInt();
    canthread->m_INT32_UserDef[14] = ui->lineEdit_15->text().toInt();
    canthread->m_INT32_UserDef[15] = ui->lineEdit_16->text().toInt();

    canthread->m_INT32_UserDef[16] = ui->lineEdit_17->text().toInt();
    canthread->m_INT32_UserDef[17] = ui->lineEdit_18->text().toInt();
    canthread->m_INT32_UserDef[18] = ui->lineEdit_19->text().toInt();
    canthread->m_INT32_UserDef[19] = ui->lineEdit_20->text().toInt();
    canthread->m_INT32_UserDef[20] = ui->lineEdit_21->text().toInt();
    canthread->m_INT32_UserDef[21] = ui->lineEdit_22->text().toInt();
    canthread->m_INT32_UserDef[22] = ui->lineEdit_23->text().toInt();
    canthread->m_INT32_UserDef[23] = ui->lineEdit_24->text().toInt();

    return 0;
}

void UserDefined::on_sureBtn_clicked()
{
    if (setUserDef())
        return;
    QMessageBox::information(this,tr(u8""),tr(u8"保存成功"),"OK");
    emit(sendUserDefParams());
}
