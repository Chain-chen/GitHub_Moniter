#include "motormatch.h"
#include "ui_motormatch.h"
#include <QMessageBox>
#include <QRegExp>
#include <QTimer>
#include <QPushButton>

MotorMatch::MotorMatch(CANThread* c,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorMatch),
    canthread(c)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_ShowModal);             //设置为模态对话框
    init();
}

void MotorMatch::init()
{
    //读取各个参数
    ui->polePairsLineEdit->setText(QString::number(canthread->motorPolePairs,10));          // 极对数
    ui->feekbackTypeComboBox->setCurrentIndex(canthread->encoderType);                      // 反馈类型（编码器类型）
    ui->encoderlineLineEdit->setText(QString::number(canthread->encoderResolution,10));     //编码器线
    ui->ratedCurrentLineEdit->setText(QString("%1").arg(canthread->ratedCurrent));          //额定电流
    ui->locked_rotorCurrentLineEdit->setText(QString("%1").arg(canthread->stallCurrent));   //堵转电流
    ui->speedLineEdit->setText(QString::number(canthread->ratedSpeed));                     //机械转速
    ui->hallOrderComboBox->setCurrentIndex(canthread->hallOrder-1);                         //霍尔线序
    ui->matchStatusComboBox->setCurrentIndex(canthread->matchFlag);                         //匹配状态

    //电机极对数0x00-0xff
    QRegExp regExp2("^(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])$");
    QRegExpValidator *pRegExpValidator2 = new QRegExpValidator(regExp2,this);
    ui->polePairsLineEdit->setValidator(pRegExpValidator2);

    //编码器线0x0000-0xffff
    QRegExp regExp3("^([0-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])$");
    QRegExpValidator *pRegExpValidator3 = new QRegExpValidator(regExp3,this);
    ui->encoderlineLineEdit->setValidator(pRegExpValidator3);

    //电机额定电流、堵转电流 正浮点数
    QRegExp regExp4("^(([0-9]+\\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
    QRegExpValidator *pRegExpValidator4 = new QRegExpValidator(regExp4,this);
    ui->ratedCurrentLineEdit->setValidator(pRegExpValidator4);
    ui->locked_rotorCurrentLineEdit->setValidator(pRegExpValidator4);

    //电机额定转速大于等于0 整数
    QRegExp regExp5("^\\d+$");
    QRegExpValidator *pRegExpValidator5 = new QRegExpValidator(regExp5,this);
    ui->speedLineEdit->setValidator(pRegExpValidator5);
}

MotorMatch::~MotorMatch()
{
    delete ui;
}

// 电机匹配
void MotorMatch::on_startMatchBtn_clicked()
{
    if(isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机使能中"),QMessageBox::Close);
        return;
    }

    if(isRealShow){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }

    if (setMotorPoles() == 0){
        return;
    }
    emit(motormatchstart());    //发送匹配电机信号至主函数

    cancel = new QPushButton(tr(u8"取消匹配"));
    close = new QPushButton(tr(u8"关闭"));
    m_box = new QMessageBox(this);

    m_box->setIcon(QMessageBox::Information);
    m_box->setWindowTitle(tr(u8"电机匹配"));
    m_box->setText(tr(u8"电机匹配中..."));
    m_box->addButton(cancel, QMessageBox::AcceptRole);
    m_box->addButton(close, QMessageBox::RejectRole);
    close->hide();
    m_box->show();

//    m_box->exec();//阻塞等待用户输入
    if (m_box->clickedButton()==cancel)//点击了OK按钮
    {
        emit(motormatchcancel());
    }
    else if(m_box->clickedButton()==close){
        m_box->close();
    }


}

UINT8 MotorMatch::setMotorPoles()
{
    if(ui->polePairsLineEdit->text()=="" || ui->encoderlineLineEdit->text()=="" || ui->ratedCurrentLineEdit->text()=="" ||ui->locked_rotorCurrentLineEdit->text()=="" ||ui->speedLineEdit->text()=="")
    {
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("请将数据填写完整"),QMessageBox::Close);
        return 0;
    }
    //获取输入框值 检查是否超限 保存至全局变量
    edit_polePairs = ui->polePairsLineEdit->text().toUInt();
    edit_encoderLines = ui->encoderlineLineEdit->text().toUInt();
    edit_feekType = ui->feekbackTypeComboBox->currentIndex();
    edit_ratedCurrent = ui->ratedCurrentLineEdit->text().toFloat();
    edit_stallCurrent = ui->locked_rotorCurrentLineEdit->text().toFloat();
    edit_ratedSpeed = ui->speedLineEdit->text().toUInt();

    if(edit_stallCurrent < edit_ratedCurrent){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"额定电流，输入超限\n允许范围 0 到 [%1]").arg(edit_stallCurrent),QMessageBox::Close);
        return 0;
    }

    //根据电机计算 脉冲数每圈
    if(edit_feekType == 0){
        canthread->pulsePerRevolution = edit_polePairs * 6;
    }
    else{
        canthread->pulsePerRevolution = edit_encoderLines * 4;
    }

    if(edit_ratedSpeed > canthread->driverMaxSpeed || edit_ratedSpeed < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"机械转速，输入超限"),QMessageBox::Close);
        return 0;
    }
    canthread->motorPolePairs = edit_polePairs;
    canthread->encoderType = edit_feekType;
    canthread->encoderResolution = edit_encoderLines;
    canthread->ratedCurrent = edit_ratedCurrent;
    canthread->ratedSpeed = edit_ratedSpeed;
    canthread->stallCurrent = edit_stallCurrent;
    canthread->hallOrder = com_hallOrder;
    return 1;
}

// 下一步 /保存
void MotorMatch::on_nextStepBtn_clicked()
{
    if (setMotorPoles() == 0){
        return;
    }
    else{
        QMessageBox::information(this,tr(u8""),tr(u8"保存成功"),"OK");
    }
}
