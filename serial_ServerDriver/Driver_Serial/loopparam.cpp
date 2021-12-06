#include "loopparam.h"
#include "ui_loopparam.h"
#include <QRegExp>

LoopParam::LoopParam(CANThread* c,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoopParam),
    canthread(c)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_ShowModal);             //设置为模态对话框

    //电流环类型选择与正弦频率相关联
    connect(ui->currentTypeComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){
        if(index==0){
            ui->sinusoidalfrequencyLineEdit->setEnabled(false);
        }
        else{
            ui->sinusoidalfrequencyLineEdit->setEnabled(true);
        }
    });

    init();
    //测试数据波形显示
    connect(ui->currentTestDataPlotBtn,&QPushButton::clicked,this,&LoopParam::addCurrentPlotData);
    connect(ui->velocityTestDataPlotBtn,&QPushButton::clicked,this,&LoopParam::addVelocityPlotData);
    connect(ui->positionTestDataPlotBtn,&QPushButton::clicked,this,&LoopParam::addPositionPlotData);
}

void LoopParam::init()
{
    ///输入框输入限制
    /// PID  浮点数
    QRegExp regExp("^(-?\\d+)(\\.\\d+)?$");
    QRegExpValidator *pRegExpValidator = new QRegExpValidator(regExp,this);
    ui->currentPValue->setValidator(pRegExpValidator);
    ui->currentIValue->setValidator(pRegExpValidator);
    ui->currentDValue->setValidator(pRegExpValidator);
    ui->currentamplitudeLineEdit->setValidator(pRegExpValidator);           //电流幅值

    ui->speedLoopPValue->setValidator(pRegExpValidator);
    ui->speedLoopIValue->setValidator(pRegExpValidator);
    ui->speedLoopDValue->setValidator(pRegExpValidator);

    ui->positionLoopPValue->setValidator(pRegExpValidator);
    ui->positionLoopIValue->setValidator(pRegExpValidator);
    ui->positionLoopDValue->setValidator(pRegExpValidator);

    ///整数
    QRegExp regExp2("^-?\\d+$");
    QRegExpValidator *pRegExpValidator2 = new QRegExpValidator(regExp2,this);
    ui->setSpeedLineEdit->setValidator(pRegExpValidator2);
    ui->targetPositionLineEdit->setValidator(pRegExpValidator2);

    ///非负整数
    QRegExp regExp3("^\\d+$");
    QRegExpValidator *pRegExpValidator3 = new QRegExpValidator(regExp3,this);
    ui->accelerationLineEdit->setValidator(pRegExpValidator3);
    ui->decelerationLineEdit->setValidator(pRegExpValidator3);
    ui->MaxSpeedLineEdit->setValidator(pRegExpValidator3);

    ui->sinusoidalfrequencyLineEdit->setEnabled(false);

    ui->waveformwidget1->setWaveformTitle(tr(u8"响应波形"));
    ui->waveformwidget2->setWaveformTitle(tr(u8"响应波形"));

    getCurrentLoop();
    getVelocityLoop();
    getPositionLoop();
}

LoopParam::~LoopParam()
{
    delete ui;
}

void LoopParam::getCurrentLoop()
{

    edit_currentLoopP = canthread->currentLoopP;
    edit_currentLoopI = canthread->currentLoopI;
    edit_currentLoopD = canthread->currentLoopD;
    ui->currentPValue->setText(QString::number(edit_currentLoopP));
    ui->currentIValue->setText(QString::number(edit_currentLoopI));
    ui->currentDValue->setText(QString::number(edit_currentLoopD));
    ui->currentTypeComboBox->setCurrentIndex(0);
    ui->currentamplitudeLineEdit->setText("1");
}

void LoopParam::getVelocityLoop()
{
    edit_velocityLoopP = canthread->velocityLoopP;
    edit_velocityLoopI = canthread->velocityLoopI;
    edit_velocityLoopD = canthread->velocityLoopD;
    ui->speedLoopPValue->setText(QString::number(edit_velocityLoopP));
    ui->speedLoopIValue->setText(QString::number(edit_velocityLoopI));
    ui->speedLoopDValue->setText(QString::number(edit_velocityLoopD));
    ui->setSpeedLineEdit->setText("1000");
}

void LoopParam::getPositionLoop()
{
    edit_positionLoopP = canthread->positionLoopP;
    edit_positionLoopI = canthread->positionLoopI;
    edit_positionLoopD = canthread->positionLoopD;
    ui->positionLoopPValue->setText(QString::number(edit_positionLoopP));
    ui->positionLoopIValue->setText(QString::number(edit_positionLoopI));
    ui->positionLoopDValue->setText(QString::number(edit_positionLoopD));
    ui->accelerationLineEdit->setText("0");
    ui->decelerationLineEdit->setText("0");
    ui->MaxSpeedLineEdit->setText("1000");
    ui->targetPositionLineEdit->setText("20");
}

UINT8 LoopParam::setCurrentLoop()
{
    if(ui->currentPValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电流环P值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }

    if(ui->currentIValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电流环I值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }

    if(ui->currentDValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电流环D值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }
    canthread->currentLoopP = ui->currentPValue->text().toFloat();
    canthread->currentLoopI = ui->currentIValue->text().toFloat();
    canthread->currentLoopD = ui->currentDValue->text().toFloat();
    return 0;
}

UINT8 LoopParam::setVelocityLoop()
{
    if(ui->speedLoopPValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"速度环P值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }

    if(ui->speedLoopIValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"速度环I值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }

    if(ui->speedLoopDValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"速度环D值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }
    canthread->velocityLoopP = ui->speedLoopPValue->text().toFloat();
    canthread->velocityLoopI = ui->speedLoopIValue->text().toFloat();
    canthread->velocityLoopD = ui->speedLoopDValue->text().toFloat();
    return 0;
}

UINT8 LoopParam::setPositionLoop()
{
    if(ui->positionLoopPValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"位置环P值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }

    if(ui->positionLoopIValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"位置环I值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }

    if(ui->positionLoopDValue->text().toFloat() < 0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"位置环D值，输入超限\n允许范围 大于0"),QMessageBox::Close);
        return 1;
    }
    canthread->positionLoopP = ui->positionLoopPValue->text().toFloat();
    canthread->positionLoopI = ui->positionLoopIValue->text().toFloat();
    canthread->positionLoopD = ui->positionLoopDValue->text().toFloat();
    return 0;
}

UINT8 LoopParam::setCurrentLoopTest()
{
    edit_currentamplitude = ui->currentamplitudeLineEdit->text().toFloat();
    if(edit_currentamplitude>canthread->appRatedCurrent || edit_currentamplitude<0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电流幅值，输入超限"),QMessageBox::Close);
        return 1;
    }

    canthread->currentTestAmplitude = edit_currentamplitude;
    canthread->currentTestType = ui->currentTypeComboBox->currentIndex();
    canthread->currentTestFrequency = ui->sinusoidalfrequencyLineEdit->text().toUInt();
    return 0;
}

UINT8 LoopParam::setVelocityLoopTest()
{
    edit_speed = ui->setSpeedLineEdit->text().toInt();
    if(edit_speed>canthread->AppMaxSpeed || edit_speed<0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"设定速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    qDebug()<<"edit_speed"<<edit_speed<<canthread->velocityTestVelocity;
    canthread->velocityTestVelocity = edit_speed;
    return 0;
}

UINT8 LoopParam::setPositionLoopTest()
{
    edit_acceleration = ui->accelerationLineEdit->text().toInt();
    edit_deceleration = ui->decelerationLineEdit->text().toInt();
    edit_targetAngel = ui->targetPositionLineEdit->text().toFloat();
    edit_maxVelocity = ui->MaxSpeedLineEdit->text().toInt();
    if(edit_acceleration>canthread->Acceleration || edit_acceleration<0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"加速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_deceleration>canthread->Deceleration || edit_deceleration<0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"减速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_maxVelocity>canthread->AppMaxSpeed || edit_maxVelocity<0){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"最大速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    if(edit_targetAngel>canthread->appMaxPosition || edit_targetAngel<(-canthread->appMinPosition)){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"目标位置，输入超限"),QMessageBox::Close);
        return 1;
    }

    canthread->positionTestAcceleration = edit_acceleration;
    canthread->positionTestDeceleration = edit_deceleration;
    canthread->positionMaxSpeed = edit_maxVelocity;
    canthread->positionTargetPosition = edit_targetAngel* 22444.0f;
    return 0;
}

void LoopParam::on_nextStepBtn_clicked()
{
    if (setCurrentLoop()==1)
        return;

    if (setVelocityLoop()==1)
        return;

    if (setPositionLoop()==1)
        return;
    QMessageBox::information(this,tr(u8""),tr(u8"保存成功"),"OK");
}

void LoopParam::on_currentLoopTestBtn_clicked()
{
    if(canthread->matchFlag != 1){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电机未匹配"),QMessageBox::Close);
        return;
    }
    if(isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机使能中"),QMessageBox::Close);
        return;
    }

    if(isRealShow){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }
    //写入 环路参数
    if (setCurrentLoop()==1)
        return;
    if (setCurrentLoopTest()==1)
        return;

    emit(currentTest());

    cancel = new QPushButton(tr(u8"取消测试"));
    close = new QPushButton(tr(u8"关闭"));
    m_box = new QMessageBox(this);
    m_box->setIcon(QMessageBox::Information);
    m_box->setWindowTitle(tr(u8"电流环测试"));
    m_box->setText(tr(u8"正在测试..."));
    m_box->addButton(cancel, QMessageBox::AcceptRole);
    m_box->addButton(close, QMessageBox::RejectRole);
    close->hide();
    m_box->show();
    if (m_box->clickedButton()==cancel)//点击了OK按钮
    {
        emit(cancelCurrentTest());
    }
    else if(m_box->clickedButton()==close){
        m_box->close();
    }

}

void LoopParam::on_speedLoopTestBtn_clicked()
{
    if(canthread->matchFlag != 1){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电机未匹配"),QMessageBox::Close);
        return;
    }
    if(isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机使能中"),QMessageBox::Close);
        return;
    }

    if(isRealShow){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }
    if (setVelocityLoop()==1)
       return;
    if (setVelocityLoopTest()==1)
       return;
    emit(velocityTest());

    cancel = new QPushButton(tr(u8"取消测试"));
    close = new QPushButton(tr(u8"关闭"));
    m_box = new QMessageBox(this);
    m_box->setIcon(QMessageBox::Information);
    m_box->setWindowTitle(tr(u8"速度环测试"));
    m_box->setText(tr(u8"正在测试..."));
    m_box->addButton(cancel, QMessageBox::AcceptRole);
    m_box->addButton(close, QMessageBox::RejectRole);
    close->hide();
    m_box->show();
    if (m_box->clickedButton()==cancel)//点击了OK按钮
    {
        emit(cancelVelocityTest());
    }
    else if(m_box->clickedButton()==close){
        m_box->close();
    }
}

void LoopParam::on_positionLoopTestBtn_clicked()
{
    if(canthread->matchFlag != 1){
        QMessageBox::critical(this,tr(u8"错误"),tr(u8"电机未匹配"),QMessageBox::Close);
        return;
    }
    if(isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机使能中"),QMessageBox::Close);
        return;
    }

    if(isRealShow){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }
    if (setPositionLoop()==1)
            return;
    if (setPositionLoopTest()==1)
        return;
    emit(positionTest());

    cancel = new QPushButton(tr(u8"取消测试"));
    close = new QPushButton(tr(u8"关闭"));
    m_box = new QMessageBox(this);
    m_box->setIcon(QMessageBox::Information);
    m_box->setWindowTitle(tr(u8"位置环测试"));
    m_box->setText(tr(u8"正在测试..."));
    m_box->addButton(cancel, QMessageBox::AcceptRole);
    m_box->addButton(close, QMessageBox::RejectRole);
    close->hide();
    m_box->show();
    if (m_box->clickedButton()==cancel)
    {
        emit(cancelPositionTest());
    }
    else if(m_box->clickedButton()==close){
        m_box->close();
    }
}

//添加电流环测试数据波形
void LoopParam::addCurrentPlotData()
{
// Iq电流（Float）和反馈Iq电流（Float）

    ui->waveformwidget1->clearData();
    ui->waveformwidget2->clearData();
    ui->waveformwidget1->changeCheckBoxName(tr(u8"Iq电流"),tr(u8"反馈Iq电流"),tr(u8"无效参数"));

    unParam temp1;
    unParam temp2;

    for (UINT16 i=0;i<(canthread->currentTestLength>>3);i++) {
        QVector<QVector<double>> value;
        QVector<double> vector1,vector2;

        canthread->ArrayToHex32(canthread->pTestData + (i << 3),  &temp1.param_int);
        canthread->ArrayToHex32(canthread->pTestData + (i << 3) + 4,  &temp2.param_int);

        vector1.append(temp1.param_float);
        value.append(vector1);
        vector2.append(temp2.param_float);
        value.append(vector2);

        ui->waveformwidget1->addMulData(value[0],value[1],{0});
    }
}

//添加速度环测试数据波形
void LoopParam::addVelocityPlotData()
{
    ui->waveformwidget1->clearData();
    ui->waveformwidget2->clearData();

    ui->waveformwidget1->changeCheckBoxName(tr(u8"速度值"),tr(u8"反馈速度值"),tr(u8"无效参数"));
    ui->waveformwidget2->changeCheckBoxName(tr(u8"反馈Iq电流"),tr(u8"无效参数"),tr(u8"无效参数"));

    int32_t temp_velocityrated;
    int32_t temp_velocityactual;
    uint32_t temp_focindex;
    unParam temp_currentactual;


    for (int i=0;i<(canthread->velocityTestLength>>4);i++) {
        QVector<QVector<double>> value;
        QVector<double> vector1,vector2,vector3,vector4;

        canthread->ArrayToHex32(canthread->pTestData + (i << 4), &temp_velocityrated);
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 4, &temp_velocityactual);
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 8, &temp_focindex);
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 12, &temp_currentactual.param_int);


        vector1.append(temp_velocityrated);
        value.append(vector1);
        vector2.append(temp_velocityactual);
        value.append(vector2);
        vector3.append(temp_focindex);
        value.append(vector3);
        vector4.append(temp_currentactual.param_float);
        value.append(vector4);
        ui->waveformwidget1->addMulData(value[0],value[1],{0});
        ui->waveformwidget2->addMulData(value[3],{0},{0});
    }
}

//添加位置环测试数据波形
void LoopParam::addPositionPlotData()
{
    ui->waveformwidget1->clearData();
    ui->waveformwidget2->clearData();

    ui->waveformwidget1->changeCheckBoxName(tr(u8"设定位置值"),tr(u8"反馈位置值"),tr(u8"位置误差值"));
    ui->waveformwidget2->changeCheckBoxName(tr(u8"反馈速度值"),tr(u8"无效参数"),tr(u8"无效参数"));

    ui->waveformwidget1->clearData();
    ui->waveformwidget2->clearData();


    double difference;

    for (WORD i = 0; i < (canthread->positionTestLength >> 4); i++)
    {
        QVector<QVector<double>> value;
        QVector<double> vector1,vector2,vector3,vector4;

        INT32 temp_positionrated, temp_positionactual;
        UINT32 temp_focindex;
        INT32 temp_velocityactual;

        canthread->ArrayToHex32(canthread->pTestData + (i << 4), &temp_positionrated);
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 4, &temp_positionactual);
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 8, &temp_focindex);
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 12, &temp_velocityactual);
        difference = temp_positionactual - temp_positionrated;

        vector1.append(temp_positionrated);
        value.append(vector1);
        vector2.append(temp_positionactual);
        value.append(vector2);
        vector3.append(temp_focindex);
        value.append(vector3);
        vector4.append(temp_velocityactual);
        value.append(vector4);
        ui->waveformwidget1->addMulData(value[0],value[1],{difference});
        ui->waveformwidget2->addMulData(value[3],{0},{0});
    }
}

//清除波形
void LoopParam::on_clearWaveBtn_clicked()
{
    ui->waveformwidget1->clearData();
    ui->waveformwidget2->clearData();
}

//保存电流环数据
void LoopParam::on_pushButton_3_clicked()
{
    QDir dir(QDir::currentPath()+QDir::separator()+tr(u8"user/dataRecord"));
    if(!dir.exists()){
        dir.mkpath(tr(u8"."));
    }
    QString fileName=tr(u8"user/dataRecord/CurrentTestData %1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
    QFile dataRecordFile;
    dataRecordFile.setFileName(fileName);

    if (!dataRecordFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr(u8"文件错误"), tr(u8"打开 [%1] 数据保存文件失败！").arg(fileName), QMessageBox::Close);
        return;
    }

    unParam WriteData1, WriteData2;
    QString dataRecordString;
    for (WORD i = 0; i < (canthread->currentTestLength >> 3); i++)
    {
        canthread->ArrayToHex32(canthread->pTestData + (i << 3), &WriteData1.param_int);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData1.param_float));
        canthread->ArrayToHex32(canthread->pTestData + (i << 3) + 4, &WriteData2.param_int);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData2.param_float));

        dataRecordString.append("\r\n");
    }
    dataRecordFile.write(dataRecordString.toLocal8Bit());

    dataRecordFile.close();
    QMessageBox::information(this,tr(u8""),tr(u8"电流环数据保存成功"),"OK");
}

//保存速度环数据
void LoopParam::on_pushButton_2_clicked()
{
    QDir dir(QDir::currentPath()+QDir::separator()+tr(u8"user/dataRecord"));
    if(!dir.exists()){
        dir.mkpath(tr(u8"."));
    }
    QString fileName=tr(u8"user/dataRecord/VelocityTestData %1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
    QFile dataRecordFile;
    dataRecordFile.setFileName(fileName);

    if (!dataRecordFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr(u8"文件错误"), tr(u8"打开 [%1] 数据保存文件失败！").arg(fileName), QMessageBox::Close);
        return;
    }

    INT32 WriteData1, WriteData2;
    UINT WriteData3;
    unParam WriteData4;

    QString dataRecordString;
    for (WORD i = 0; i < (canthread->velocityTestLength >> 4); i++)
    {
        canthread->ArrayToHex32(canthread->pTestData + (i << 4), &WriteData1);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData1));
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 4, &WriteData2);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData2));
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 8, &WriteData3);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData3));
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 12, &WriteData4.param_int);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData4.param_float));

        dataRecordString.append("\r\n");
    }
    dataRecordFile.write(dataRecordString.toLocal8Bit());

    dataRecordFile.close();
    QMessageBox::information(this,tr(u8""),tr(u8"速度环数据保存成功"),"OK");
}

//保存位置环数据
void LoopParam::on_pushButton_clicked()
{
    QDir dir(QDir::currentPath()+QDir::separator()+tr(u8"user/dataRecord"));
    if(!dir.exists()){
        dir.mkpath(tr(u8"."));
    }
    QString fileName=tr(u8"user/dataRecord/PositionTestData %1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
    QFile dataRecordFile;
    dataRecordFile.setFileName(fileName);

    if (!dataRecordFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr(u8"文件错误"), tr(u8"打开 [%1] 数据保存文件失败！").arg(fileName), QMessageBox::Close);
        return;
    }

    INT32 WriteData1, WriteData2, WriteData4;
    UINT32 WriteData3;

    QString dataRecordString;
    for (WORD i = 0; i < (canthread->positionTestLength >> 4); i++)
    {
        canthread->ArrayToHex32(canthread->pTestData + (i << 4), &WriteData1);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData1));
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 4, &WriteData2);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData2));
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 8, &WriteData3);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData3));
        canthread->ArrayToHex32(canthread->pTestData + (i << 4) + 12, &WriteData4);
        dataRecordString.append(tr(u8"%1\t").arg(WriteData4));

        dataRecordString.append("\r\n");
    }
    dataRecordFile.write(dataRecordString.toLocal8Bit());

    dataRecordFile.close();
    QMessageBox::information(this,tr(u8""),tr(u8"位置环数据保存成功"),"OK");

}
