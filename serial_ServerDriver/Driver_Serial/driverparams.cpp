#include "driverparams.h"
#include "ui_driverparams.h"
#include <QMessageBox>
#include <QRegExp>

DriverParams::DriverParams(CANThread* c,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriverParams),
    canthread(c)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_ShowModal);             //设置为模态对话框

    ///输入框输入限制
    /// 浮点数
    QRegExp regExp("^(-?\\d+)(\\.\\d+)?$");
    QRegExpValidator *pRegExpValidator = new QRegExpValidator(regExp,this);
    ui->AppratedCurrentLineEdit->setValidator(pRegExpValidator);
    ui->AppstallCurrenLineEdit->setValidator(pRegExpValidator);
    ui->AppmaxPositionLineEdit->setValidator(pRegExpValidator);
    ui->AppminPositionLineEdit->setValidator(pRegExpValidator);

    ///整数
    QRegExp regExp2("^-?\\d+$");
    QRegExpValidator *pRegExpValidator2 = new QRegExpValidator(regExp2,this);
    ui->AppmaxSpeedLineEdit->setValidator(pRegExpValidator2);
    ui->AppaccelerationLineEdit->setValidator(pRegExpValidator2);
    ui->AppdeceleratitionLineEdit->setValidator(pRegExpValidator2);
    ui->AppstopDecelerationLineEdit->setValidator(pRegExpValidator2);
    ui->initialPositionLineEdit->setValidator(pRegExpValidator2);

    init();
}

void DriverParams::init()
{
    ui->motorDirectionCombpBox->setCurrentIndex(canthread->motionDirection);    //电机正方向

    edit_appRatedCurrent = canthread->appRatedCurrent;
    ui->AppratedCurrentLineEdit->setText(QString::number(edit_appRatedCurrent));

    edit_appStallCurrent = canthread->appStallCurrent;
    ui->AppstallCurrenLineEdit->setText(QString::number(edit_appStallCurrent));

    edit_appMaxSpeed = canthread->AppMaxSpeed;
    ui->AppmaxSpeedLineEdit->setText(QString::number(edit_appMaxSpeed));

    edit_acceleration = canthread->Acceleration;
    ui->AppaccelerationLineEdit->setText(QString::number(edit_acceleration));

    edit_deceleration = canthread->Deceleration;
    ui->AppdeceleratitionLineEdit->setText(QString::number(edit_deceleration));

    edit_stopDeceleration = canthread->stopDeceleration;
    ui->AppstopDecelerationLineEdit->setText(QString::number(edit_stopDeceleration));

    edit_maxPosition = canthread->appMaxAngle;
    ui->AppmaxPositionLineEdit->setText(QString::number(edit_maxPosition));

    edit_minPosition = canthread->appMinAngle;
    ui->AppminPositionLineEdit->setText(QString::number(edit_minPosition));

    edit_initialPostion = canthread->initialPosition;
    ui->initialPositionLineEdit->setText(QString::number(edit_initialPostion));

    //静态参数
    edit_driverRatedCurrent = canthread->driverRatedCurrent;
    ui->driverRatedCurrent->setText(QString::number(edit_driverRatedCurrent));

    edit_driverStallCurrent = canthread->driverStallCurrent;
    ui->driverStallCurrent->setText(QString::number(edit_driverStallCurrent));

    edit_driverMaxSpeed = canthread->driverMaxSpeed;
    ui->driverMaxVelocity->setText(QString::number(edit_driverMaxSpeed));

    edit_driverMinSpeed = canthread->driverMinSpeed;
    ui->DriverMinVelocity->setText(QString::number(edit_driverMinSpeed));

    edit_driverMaxVoltage = canthread->driverMaxVoltage;
    ui->driverMaxVoltage->setText(QString::number(edit_driverMaxVoltage));

    edit_driverMinVoltage = canthread->driverMinVoltage;
    ui->driverMinVoltage->setText(QString::number(edit_driverMinVoltage));

}

UINT8 DriverParams::setApplication()
{
    edit_appRatedCurrent = ui->AppratedCurrentLineEdit->text().toFloat();
    edit_appStallCurrent = ui->AppstallCurrenLineEdit->text().toFloat();
    edit_appMaxSpeed = ui->AppmaxSpeedLineEdit->text().toInt();
    edit_acceleration = ui->AppaccelerationLineEdit->text().toInt();
    edit_deceleration = ui->AppdeceleratitionLineEdit->text().toInt();
    edit_stopDeceleration = ui->AppstopDecelerationLineEdit->text().toInt();
    edit_maxPosition = ui->AppmaxPositionLineEdit->text().toFloat();
    edit_minPosition = ui->AppminPositionLineEdit->text().toFloat();
    edit_initialPostion = ui->initialPositionLineEdit->text().toInt();

    if(edit_appRatedCurrent<0 || edit_appRatedCurrent>min(canthread->driverRatedCurrent,canthread->ratedCurrent)){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"额定电流，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_appStallCurrent<0 || edit_appRatedCurrent>min(canthread->driverStallCurrent,canthread->stallCurrent)){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"堵转电流，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_appMaxSpeed<0 || edit_appMaxSpeed>canthread->ratedSpeed){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"最大速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_acceleration<0 || edit_acceleration>0x7FFFFFFF){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"加速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_deceleration<0 || edit_deceleration>0x7FFFFFFF){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"减速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_stopDeceleration<0 || edit_stopDeceleration>0x7FFFFFFF){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"停止减速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_maxPosition<0 || edit_maxPosition>0x0001FFFF){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"最大位置，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_minPosition<0 || edit_minPosition>edit_maxPosition){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"最小位置，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_initialPostion<(INT32)0x80000000|| edit_initialPostion>(INT32)0x7FFFFFFFc){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"初始位置，输入超限"),QMessageBox::Close);
        return 1;
    }

    if(ui->motorDirectionCombpBox->currentIndex() == 0){
        canthread->motionDirection = 0x00;
    }
    else{
        canthread->motionDirection = 0x01;
    }
    if(ui->currentlyCheckBox->isChecked()){
        canthread->uint8_initialPosition = 0x01;
    }
    else{
        canthread->uint8_initialPosition = 0x00;
    }

    canthread->appRatedCurrent = edit_appRatedCurrent;
    canthread->appStallCurrent = edit_appStallCurrent;
    if(edit_appMaxSpeed > 0){
        edit_appMaxSpeed += 0.5f;
    }
    else{
        edit_appMaxSpeed -= 0.5f;
    }
    canthread->AppMaxSpeed = edit_appMaxSpeed;
    canthread->Acceleration = edit_acceleration;
    canthread->Deceleration = edit_deceleration;
    canthread->stopDeceleration = edit_stopDeceleration;
    canthread->appMaxAngle = edit_maxPosition;
    canthread->appMinAngle = edit_minPosition;
    canthread->initialPosition = edit_initialPostion;
    canthread->appMaxPosition = canthread->appMaxAngle;
    canthread->appMinPosition = canthread->appMinAngle;

    return 0;
}

DriverParams::~DriverParams()
{
    delete ui;
}

void DriverParams::on_nextBtn_clicked()
{
    if (setApplication() == 1)
        return;
    else
        QMessageBox::information(this,tr(u8""),tr(u8"保存成功"),"OK");
    emit(setDriverParam());
}
