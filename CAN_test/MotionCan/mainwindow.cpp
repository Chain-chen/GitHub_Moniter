#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "motormatch.h"
#include "ui_motormatch.h"
#include "ui_loopparam.h"
#include "ui_driverparams.h"
#include "ui_userdefined.h"
#include <QThread>
#include <QRegExp>
#include <array>
#include <windows.h>
#include <QMessageBox>
#include <QTime>
#include <QtGlobal>
#include <QList>
#include <QMetaType>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //测试区



    //注册（在使用信号槽传递参数时，类型如何Qt识别不了，则需要先注册
    qRegisterMetaType<QList<double>>("QList<double>");
    qRegisterMetaType<QVector<QVector<double>>>("QVector<QVector<double>>");
    qRegisterMetaType<QVector<QVector<uint8_t>>>("QVector<QVector<uint8_t> >");

    //初始化函数
    init();

    //角度与位置转换
    connect(ui->setAngelLineEdit,&QLineEdit::textChanged,[=](QString angel){
        int position_float =  angel.toFloat() * 22444.0f;
        ui->setPositionLineEdit->setText(QString::number(position_float));
    });

    //实时状态更新定时器
    realDataUpdataTimer = new QTimer();
    connect(realDataUpdataTimer,&QTimer::timeout,[=](){
        timerProcess(RealDataUpdataTimer);
    });

    //状态栏更新定时器
    deviceStatusUpdataTimer = new QTimer();
    connect(deviceStatusUpdataTimer,&QTimer::timeout,[=](){
        timerProcess(DeviceStatusUpdataTimer);
    });

    //状态栏错误码定时器
    statusErrorProcess = new QTimer();
    connect(statusErrorProcess,&QTimer::timeout,[=](){
        timerProcess(StatusErrorProcess);
    });

    //电机匹配定时器
    motorMatchTimer = new QTimer();
    connect(motorMatchTimer,&QTimer::timeout,[=](){
        timerProcess(MotorMatchTimer);
    });

    //电流环测试定时器
    currentTestTimer = new QTimer();
    connect(currentTestTimer,&QTimer::timeout,[=](){
        timerProcess(CurrentTestTimer);
    });

    //速度环测试定时器
    velocityTestTimer = new QTimer();
    connect(velocityTestTimer,&QTimer::timeout,[=](){
        timerProcess(VelocityTestTimer);
    });

    //位置环测试定时器
    positionTestTimer = new QTimer();
    connect(positionTestTimer,&QTimer::timeout,[=](){
        timerProcess(PositionTestTimer);
    });

    //CAN参数设置
    CANsetting = new CANParamSetting();

    //CAN通信线程
    canthread = new CANThread();

    //将接收到的数据显示
    connect(canthread,&CANThread::recvError,[=](){
        ui->CanparamSettingPbt->setEnabled(true);
        ui->CanRunPtn->setText(tr(u8"CAN启动"));
    });

    //CAN线程 错误信息以及提示信息显示
    connect(canthread,&CANThread::showCANError,this,&MainWindow::showError);
    connect(canthread,&CANThread::showCANInfo,this,&MainWindow::showInfo);

    //按钮 设备信息
    connect(ui->deviceinfo_btn,&QPushButton::clicked,[=](){
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }
        UINT8 result = 0;
        result = getRegValue(REG_DEVICE_STATUS, 6);
        if(result){
            return;
        }
        result = getRegValue(REG_DEVICE_INFO, 34);
        if(result){
            return;
        }
        QMessageBox::information(this,tr(u8"设备信息"),tr(u8"CAN设备型号：[%1]\nCAN设备序列号：[%2]\nCAN通道数：[%3]").arg(canthread->devinfo[2]).arg(canthread->devinfo[1]).arg(canthread->devinfo[0]),"OK");
    });

    //按钮 清除波形
    connect(ui->clearWavefrom,&QPushButton::clicked,[&](){
        ui->waveformwidget1->clearData();
        ui->waveformwidget2->clearData();
        ui->waveformwidget3->clearData();
        ui->waveformwidget4->clearData();
        ui->waveformwidget5->clearData();
        ui->waveformwidget6->clearData();
    });

    //按钮 重置横轴长度
    connect(ui->resetXrange,&QPushButton::clicked,[&](){
        ui->waveformwidget1->resetXrange();
        ui->waveformwidget2->resetXrange();
        ui->waveformwidget3->resetXrange();
        ui->waveformwidget4->resetXrange();
        ui->waveformwidget5->resetXrange();
    });

    //按钮 保存数据
    connect(ui->dataSave,&QPushButton::clicked,[&](){
        if(!isDataRecording()){
            if(openDataRecordFileAndStart()){
                ui->dataSave->setText(tr(u8"停止保存数据"));
                ui->dataSave->setStyleSheet("QPushButton:enabled { background-color: rgb(0,149,255); font-weight: normal; color:white; border:2px solid rgb(0,110,255);}\
                                                    QPushButton:enabled:hover {  background-color: rgb(0,149,255); font-weight: bold;}\
                                                    QPushButton:enabled:pressed { background-color: rgb(0,149,255); }");
            }
        }
        else{
            if(stopAndCloseDataRecordFile()){
                ui->dataSave->setText(tr(u8"开始保存数据"));
                ui->dataSave->setStyleSheet("QPushButton:enabled { background-color: #019b48; font-weight: normal; color:white; }\
                                                    QPushButton:enabled:hover { background-color: #019b48;font-weight: bold; }\
                                                    QPushButton:enabled:pressed { background-color: #019b48; }");
            }
        }
    });

    //按钮 打开保存文件夹
    connect(ui->openSavedataFile,&QPushButton::clicked,[&](){
        QDir dir(QDir::currentPath()+QDir::separator()+tr(u8"user/dataRecord"));
        if(!dir.exists()){
            dir.mkpath(tr(u8"."));
        }
        QDesktopServices::openUrl(QUrl("file:///"+dir.absolutePath(), QUrl::TolerantMode));
    });

    //按钮 实时更新按钮
    connect(ui->realShowDataBtn,&QPushButton::clicked,[&](){
        if(!isRealShow){
            isRealShow=!isRealShow;
            realDataUpdataTimer->start(100);
            deviceStatusUpdataTimer->stop();
            ui->realShowDataBtn->setStyleSheet("QPushButton:enabled { background-color: rgb(0,149,255); font-weight: normal; color:white; border:2px solid rgb(0,110,255);}\
                                                QPushButton:enabled:hover {  background-color: rgb(0,149,255); font-weight: bold;}\
                                                QPushButton:enabled:pressed { background-color: rgb(0,149,255); }");
        }
        else{
            isRealShow=!isRealShow;
            deviceStatusUpdataTimer->start(500);
            realDataUpdataTimer->stop();
            ui->realShowDataBtn->setStyleSheet("QPushButton:enabled { background-color: #019b48; font-weight: normal; color:white; }\
                                                QPushButton:enabled:hover { background-color: #019b48;font-weight: bold; }\
                                                QPushButton:enabled:pressed { background-color: #019b48; }");
        }
    });

    //复选框 工作模式选择
    connect(ui->workModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index){
        if(index == 0){                                                 //电流模式
            ui->setCurrentLineEdit->setEnabled(true);
            ui->setAccelerationLineEdit->setEnabled(false);
            ui->setSpeedLineEdit->setEnabled(false);
            ui->setPositionLineEdit->setEnabled(false);
            ui->positionTypeComboBox->setEnabled(false);
            ui->setAngelLineEdit->setEnabled(false);
        }
        else if(index == 1){                                            //速度模式
            ui->setCurrentLineEdit->setEnabled(false);
            ui->setSpeedLineEdit->setEnabled(true);
            ui->setAccelerationLineEdit->setEnabled(true);

            ui->setPositionLineEdit->setEnabled(false);
            ui->positionTypeComboBox->setEnabled(false);
            ui->setAngelLineEdit->setEnabled(false);
        }
        else if(index ==2){                                             //位置模式
            ui->setCurrentLineEdit->setEnabled(false);
            ui->setSpeedLineEdit->setEnabled(true);
            ui->setAccelerationLineEdit->setEnabled(true);
            ui->setPositionLineEdit->setEnabled(false);
            ui->positionTypeComboBox->setEnabled(true);
            ui->setAngelLineEdit->setEnabled(true);
        }
        else{
            return;
        }
    });

    //菜单栏
    //菜单栏子栏点击
    //文件 下载
    connect(ui->fileactions,&QAction::triggered,[&](){
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }

        if(isMotorEnabled == true){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机已使能"),QMessageBox::Close);
            return;
        }

        if(isRealShow == true){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙，请先关闭实时显示"),QMessageBox::Close);
            return;
        }

        canthread->Hex16ToArray(DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_CPARAM_SAVE);
        if (setRegValue(REG_COMMAND, 0x02) == 1)
        {
            return;
        }
        canthread->sleep(500);
        QMessageBox::information(this,tr(u8"下载"),tr(u8"参数下载完成"),"OK");
    });

    //电机 
    connect(ui->motoraction,&QAction::triggered,[&](){
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }
        UINT8 result = 0;
        result = getRegValue(REG_MOTOR_POLES, 17);
        if (result)
        {
            canthread->processErrorCode(result);
            return;
        }
        result = getRegValue(REG_MATCH_FLAG, 7);
        if (result)
        {
            canthread->processErrorCode(result);
            return;
        }

        motormatch = new MotorMatch(canthread);
        menuMotorMatch();
        //匹配电机
        connect(motormatch,&MotorMatch::motormatchstart,this,&MainWindow::processMotorMatch);
        //取消匹配电机
        connect(motormatch,&MotorMatch::motormatchcancel,this,&MainWindow::processMotorMatchCancel);
        //电机参数保存
        connect(motormatch,&MotorMatch::saveMotorParams,this,&MainWindow::setMotorPoles);
    });

    //驱动器
    connect(ui->driveraction,&QAction::triggered,[&](){
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }
        UINT8 result = 0;
        result = getRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
        if(result)
        {
            canthread->processErrorCode(result);
            return;
        }
        result = getRegValue(REG_DEVICE_STATIC, REG_DEVICE_STATIC_NUM);
        if (result)
        {
            canthread->processErrorCode(result);
            return;
        }
        driverparams = new DriverParams(canthread);
        menuDriverparam();
        connect(driverparams,&DriverParams::setDriverParam,this,&MainWindow::setApplication);    
    });

    //环路参数
    connect(ui->loopaction,&QAction::triggered,[&](){
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }
        UINT8 result = getRegValue(REG_CURRENTLOOP_P, REG_CURRENTLOOP_NUM);
        if(result)
        {
            canthread->processErrorCode(result);
            return;
        }
        result = getRegValue(REG_VELOCITYLOOP_P, REG_VELOCITYLOOP_NUM);
        if(result)
        {
            canthread->processErrorCode(result);
            return;
        }
        result = getRegValue(REG_POSITIONLOOP_P, REG_POSITIONLOOP_NUM);
        if (result)
        {
            canthread->processErrorCode(result);
            return;
        }
        loopparam = new LoopParam(canthread);
        menuLoopParam();
        //环路测试与取消测试
        connect(loopparam,&LoopParam::currentTest,this,&MainWindow::processCurrentTest);
        connect(loopparam,&LoopParam::cancelCurrentTest,this,&MainWindow::processCurrentTestCancel);

        connect(loopparam,&LoopParam::velocityTest,this,&MainWindow::processVelocityTest);
        connect(loopparam,&LoopParam::cancelVelocityTest,this,&MainWindow::processVelocityTestCancel);

        connect(loopparam,&LoopParam::positionTest,this,&MainWindow::processPositionTest);
        connect(loopparam,&LoopParam::cancelPositionTest,this,&MainWindow::processPositionTestCancel);

        //驱动器参数保存
        connect(loopparam,&LoopParam::saveLoopParams,[&](){
            setCurrentLoop();
            setVelocityLoop();
            setPositionLoop();
        });
    });

    //自定义参数
    connect(ui->selfsettingaction,&QAction::triggered,[&](){
        if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
            return;
        }
        UINT8 result = 0;
        result = getRegValue(REG_USER_DEF, REG_USER_DEF_NUM);
        if (result)
        {
            canthread->processErrorCode(result);
            return;
        }

        userdefined = new UserDefined(canthread);
        menuUserDefined();
        connect(userdefined,&UserDefined::sendUserDefParams,this,&MainWindow::setsetUserDef);

    });

    //显示接收数据
    connect(canthread,&CANThread::showRecvData,[=](QList<unsigned char> showdata){

        QByteArray DataContent;
        foreach(unsigned char value, showdata){
            DataContent.append(value);
        }
        ui->recv_textEdit->append(tr(u8"接收")+DataContent.toHex());
    });

    //显示发送数据
    connect(this,&MainWindow::showSendData,[=](BYTE *data2){
        QByteArray Datasend;
        Datasend.resize(sizeof(data2));
        memcpy(Datasend.data(), data2, sizeof(data2));
        ui->recv_textEdit->append(tr(u8"发送")+Datasend.toHex());
    });
}

//初始:波形标题、状态栏
void MainWindow::init()
{
    //输入框限制
    QRegExp regExp("^(-?\\d+)(\\.\\d+)?$");
    QRegExpValidator *pRegExpValidator = new QRegExpValidator(regExp,this);
    ui->setCurrentLineEdit->setValidator(pRegExpValidator);
    ui->setAngelLineEdit->setValidator(pRegExpValidator);
    ui->setSpeedLineEdit->setValidator(pRegExpValidator);
    ui->setPositionLineEdit->setValidator(pRegExpValidator);
    ui->setAccelerationLineEdit->setValidator(pRegExpValidator);

    ui->waveformwidget1->setWaveformTitle(tr(u8"母线电压"));
    ui->waveformwidget2->setWaveformTitle(tr(u8"相电流幅值"));
    ui->waveformwidget3->setWaveformTitle(tr(u8"当前速度"));
    ui->waveformwidget4->setWaveformTitle(tr(u8"当前位置"));
    ui->waveformwidget5->setWaveformTitle(tr(u8"关节位置"));
    ui->waveformwidget6->setWaveformTitle(tr(u8"关节角度"));

    //状态栏添加
    QStatusBar *sBar = statusBar();
    deviceInfo = new QLabel(this);
    deviceInfo->setText(tr(u8"设备信息"));
    sBar->addWidget(deviceInfo);

    deviceType = new QLabel(this);
    deviceType->setText(tr(u8"设备类型"));
    sBar->addWidget(deviceType);

    deviceStatu = new QLabel(this);
    deviceStatu->setText(tr(u8"设备状态"));
    sBar->addWidget(deviceStatu);

    motorStatu = new QLabel(this);
    motorStatu->setText(tr(u8"电机状态"));
    sBar->addWidget(motorStatu);

    errorCode = new QLabel(this);
    errorCode->setText(tr(u8"错误码"));
    sBar->addWidget(errorCode);

    bootLoaderStatus = new QLabel(this);
    bootLoaderStatus->setText(tr(u8"bootLoader下载"));
    sBar->addWidget(bootLoaderStatus);

    ui->setCurrentLineEdit->setEnabled(false);
    ui->setSpeedLineEdit->setEnabled(false);
    ui->setPositionLineEdit->setEnabled(false);
    ui->setAccelerationLineEdit->setEnabled(false);
    ui->positionTypeComboBox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//参数设置界面显示
void MainWindow::on_CanparamSettingPbt_clicked()
{
    CANsetting->show();
}

//启动CAN分析仪
void MainWindow::on_CanRunPtn_clicked()
{
    if(ui->CanRunPtn->text() == tr(u8"CAN启动"))
    {
        canthread->deviceType = CANsetting->CANtype;
        canthread->devicIndex = CANsetting->index;
        canthread->baundRate = CANsetting->baundRate;
        canthread->devicCom = CANsetting->devicCOM;
        bool dev = canthread->openCAN();

        if(dev == true){
             isRealShow = false;
             canthread->start();
             ui->CanparamSettingPbt->setEnabled(false);
             ui->CanRunPtn->setText(tr(u8"CAN关闭"));

             //读取 匹配参数
             UINT8 result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);
             if (result)
             {
                 canthread->processErrorCode(result);
                 return;
             }

             //读取 应用参数
             result = getRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
             if (result)
             {
                 canthread->processErrorCode(result);
                 return;
             }

             result = getRegValue(REG_DEVICE_STATIC, REG_DEVICE_STATIC_NUM);
             if (result)
             {
                 canthread->processErrorCode(result);
                 return;
             }

             //读取 电机默认参数
             result = getRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
             if (result)
             {
                 canthread->processErrorCode(result);
                 return;
             }

             //读取 设备运行模式
              result = getRegValue(REG_RUN_MODE, 18);
             if (result)
             {
                 canthread->processErrorCode(result);
                 return;
             }

             //读取 驱动器型号
             result = getRegValue(REG_DEVICE_INFO, 34);
             if (result)
             {
                canthread->processErrorCode(result);
                return;
             }

             //读取 设备状态
             if(canthread->driverType == "ACS306"){                             //关节模组驱动器 多读一个 关节位置寄存器
                 result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);
             }
             else{
                 result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);
             }

             if (result)
             {
                 canthread->processErrorCode(result);
             }


             deviceStatusUpdataTimer->start(500);
        }
        else{
            ui->CanparamSettingPbt->setEnabled(true);
            ui->CanRunPtn->setText(tr(u8"CAN启动"));
        }
    }
    else if(ui->CanRunPtn->text() == tr(u8"CAN关闭"))
    {
        if(isRealShow==true){
            ui->realShowDataBtn->click();
            realDataUpdataTimer->stop();
        }
        deviceStatusUpdataTimer->stop();
        ui->CanparamSettingPbt->setEnabled(true);
        ui->CanRunPtn->setText(tr(u8"CAN启动"));
        canthread->stop();
        canthread->closeCAN();
    }
}

//**************菜单栏******************//
//************************电机匹配*************************
void MainWindow::menuMotorMatch()
{
    //0新建 1打开
    openDlgFileMotor(1);
    motormatch->setWindowTitle(tr(u8"电机参数-修改"));
    motormatch->ui->nextStepBtn->setText(tr(u8"保存"));
}

        //函数 打开 电机对话框 //0 无效  F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 MainWindow::openDlgFileMotor(UINT8 mode)
{
    motormatch->ui->nextStepBtn->setEnabled(!(isMotorEnabled || isRealShow));
    if(mode==1){
        motormatch->isRealShow = isRealShow;
        motormatch->isMotorEnabled = isMotorEnabled;
        motormatch->update();
        motormatch->show();
    }
    else{
        motormatch->show();
    }
    return 0;
}

        // 电机匹配响应函数
void MainWindow::processMotorMatch()
{
    UINT8 result = 0;
    result = setMotorPoles();
    if (result)
    {
        canthread->processErrorCode(result);
    }

    result = setCommand(COMMAND_MOTOR_MATCH);
    if (result)
    {
        canthread->processErrorCode(result);
    }
    motormatchCount = 0;         // 匹配最多次数置位0
    deviceStatusUpdataTimer->stop();
    motorMatchTimer->start(500); //启动匹配电机定时器

}

        //函数 写入电机参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setMotorPoles()
{
    int ratedCurrent = printBinaryDigit(&canthread->ratedCurrent);
    int stallCurrent = printBinaryDigit(&canthread->stallCurrent);

    canthread->Hex8ToArray(DataTxArray + 7 + 0, canthread->motorPolePairs);
    canthread->Hex8ToArray(DataTxArray + 7 + 1, canthread->encoderType);
    canthread->Hex16ToArray(DataTxArray + 7 + 2, canthread->encoderResolution);
    canthread->Hex32ToArray(DataTxArray + 7 + 4, ratedCurrent);
    canthread->Hex32ToArray(DataTxArray + 7 + 8, stallCurrent);
    canthread->Hex32ToArray(DataTxArray + 7 + 12, canthread->ratedSpeed);
    canthread->Hex8ToArray(DataTxArray + 7 + 16, canthread->hallOrder);

    UINT8 result = setRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
    return result;
}

        // 取消电机匹配函数
void MainWindow::processMotorMatchCancel()
{
    motorMatchTimer->stop();   //停止电机匹配
    UINT8 result = setCommand(COMMAND_MOTOR_NOMATCH);
    motormatch->ui->matchStatusComboBox->setCurrentIndex(0);
    if (result)
    {
        canthread->processErrorCode(result);
    }
}

//******************环路参数******************
void MainWindow::menuLoopParam()
{
    //0新建 1打开
    openDlgFileLoop(1);
    loopparam->setWindowTitle(tr(u8"环路参数-修改"));
    loopparam->ui->nextStepBtn->setText(tr(u8"保存"));
    loopparam->ui->backBtn->hide();
}

        //函数 打开 环路对话框 //0 无效  F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 MainWindow::openDlgFileLoop(UINT8 mode)
{
    if(mode==1){
        loopparam->ui->nextStepBtn->setEnabled(!(isMotorEnabled || isRealShow));
        loopparam->isMotorEnabled = isMotorEnabled;
        loopparam->isRealShow = isRealShow;
        loopparam->loopOpenMode=1;
        loopparam->update();
        loopparam->show();
    }
    else{
        loopparam->loopOpenMode=0;
        loopparam->show();
    }
    return 0;
}

        //电流环测试响应函数
void MainWindow::processCurrentTest()
{
    UINT8 result = 0;
    result = setCurrentLoop();
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }

    result = setCurrentTest();
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }

    result = setCommand(COMMAND_CURRENT_TEST_BG);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
    motormatchCount = 0;         // 匹配最多次数置位0
    currentTestTimer->start(500);
}

        //函数 写入电流环参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setCurrentLoop()
{
    int currentP = printBinaryDigit(&canthread->currentLoopP);
    int currentI = printBinaryDigit(&canthread->currentLoopI);
    int currentD = printBinaryDigit(&canthread->currentLoopD);

    canthread->Hex32ToArray(DataTxArray + 7 + 0, currentP);
    canthread->Hex32ToArray(DataTxArray + 7 + 4, currentI);
    canthread->Hex32ToArray(DataTxArray + 7 + 8, currentD);

    UINT8 result = setRegValue(REG_CURRENTLOOP_P, REG_CURRENTLOOP_NUM);
    return result;
}

        //函数 写入电流环测试参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setCurrentTest()
{
    int currentTestAmplitude = printBinaryDigit(&canthread->currentTestAmplitude);

    canthread->Hex8ToArray(DataTxArray + 7 + 0, canthread->currentTestType);
    canthread->Hex32ToArray(DataTxArray + 7 + 1, currentTestAmplitude);
    canthread->Hex16ToArray(DataTxArray + 7 + 5, canthread->currentTestFrequency);

    UINT8 result = setRegValue(REG_CURRENTLOOP_TEST, REG_CURRENTLOOP_TEST_NUM);
    return result;
}

        //获取电流环测试参数
UINT8 MainWindow::getCurrentTestData()
{
    UINT8 result = getRegValue(REG_CURRENTLOOP_TEST_LENGTH, 2);
    if(result)
        return result;

    canthread->testDataIndex = 0;
    canthread->testDataLength = canthread->currentTestLength;
    int testDataLength =  canthread->currentTestLength;
    while (canthread->testDataLength > 0)
    {
        if (canthread->testDataLength  > 128)
        {
            result = getRegValue(REG_LOOP_TESTDATA + canthread->testDataIndex, 128);
            if (result)
                return result;
        }
        else
        {
            result = getRegValue(REG_LOOP_TESTDATA + canthread->testDataIndex, testDataLength);
            if (result)
                return result;
        }
    }
    return 0;
}

        //取消电流环测试函数
void MainWindow::processCurrentTestCancel()
{
    currentTestTimer->stop();
    UINT8 result = setCommand(COMMAND_CURRENT_TEST_ST);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
}

        //速度环测试响应函数
void MainWindow::processVelocityTest()
{
    UINT8 result = 0;
    result = setVelocityLoop();
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }

    result = setVelocityTest();
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }

    result = setCommand(COMMAND_VELOCITY_TEST_BG);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
    motormatchCount = 0;         // 匹配最多次数置位0
    velocityTestTimer->start(500);
}

        //函数 写入速度环参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setVelocityLoop()
{
    int velocityP = printBinaryDigit(&canthread->velocityLoopP);
    int velocityI = printBinaryDigit(&canthread->velocityLoopI);
    int velocityD = printBinaryDigit(&canthread->velocityLoopD);

    canthread->Hex32ToArray(DataTxArray + 7 + 0, velocityP);
    canthread->Hex32ToArray(DataTxArray + 7 + 4, velocityI);
    canthread->Hex32ToArray(DataTxArray + 7 + 8, velocityD);

    UINT8 result = setRegValue(REG_VELOCITYLOOP_P, REG_VELOCITYLOOP_NUM);
    return result;
}

        //函数 写入速度环测试参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setVelocityTest()
{
    int velocityTestSpeed = canthread->velocityTestVelocity;
    canthread->Hex32ToArray(DataTxArray + 7,velocityTestSpeed);
    UINT8 result = setRegValue(REG_VELOCITYLOOP_TEST_VELOCITY, 4);
    return result;
}

        //获取速度环测试参数
UINT8 MainWindow::getVelocityTestData()
{
    UINT8 result = getRegValue(REG_VELOCITYLOOP_TEST_LENGTH, 2);
    if(result)
        return result;

    canthread->testDataIndex = 0;
    canthread->testDataLength = canthread->velocityTestLength;

    while (canthread->testDataLength)
    {
        if (canthread->testDataLength > 38)
        {
            result = getRegValue(REG_LOOP_TESTDATA + canthread->testDataIndex, 38);
            if (result)
                return result;
        }
        else
        {
            result = getRegValue(REG_LOOP_TESTDATA + canthread->testDataIndex, (UINT8)canthread->testDataLength);
            if (result)
                return result;
        }
    }
    return 0;
}

        //取消速度环测试函数
void MainWindow::processVelocityTestCancel()
{
    velocityTestTimer->stop();
    UINT8 result = setCommand(COMMAND_VELOCITY_TEST_ST);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
}

        //位置环测试响应函数
void MainWindow::processPositionTest()
{
    UINT8 result = 0;
    result = setPositionLoop();
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }

    result = setPositionTest();
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }

    result = setCommand(COMMAND_POSITION_TEST_BG);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
    motormatchCount = 0;         // 匹配最多次数置位0
    positionTestTimer->start(500);
}

        //函数 写入位置环参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setPositionLoop()
{
    int positionP = printBinaryDigit(&canthread->positionLoopP);
    int positionI = printBinaryDigit(&canthread->positionLoopI);
    int positionD = printBinaryDigit(&canthread->positionLoopD);

    canthread->Hex32ToArray(DataTxArray + 7 + 0, positionP);
    canthread->Hex32ToArray(DataTxArray + 7 + 4, positionI);
    canthread->Hex32ToArray(DataTxArray + 7 + 8, positionD);

    UINT8 result = setRegValue(REG_POSITIONLOOP_P, REG_POSITIONLOOP_NUM);
    return result;
}

        //函数 写入位置环测试参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setPositionTest()
{
    int positionTestAcceleration = canthread->positionTestAcceleration;
    int positionTestDeceleration = canthread->positionTestDeceleration;
    int positionTestSpeed = canthread->positionMaxSpeed;
    int positionTestPosition = canthread->positionTargetPosition;

    canthread->Hex32ToArray(DataTxArray + 7 + 0, positionTestAcceleration);
    canthread->Hex32ToArray(DataTxArray + 7 + 4, positionTestDeceleration);
    canthread->Hex32ToArray(DataTxArray + 7 + 8, positionTestSpeed);
    canthread->Hex32ToArray(DataTxArray + 7 + 12, positionTestPosition);

    UINT8 result = setRegValue(REG_POSITIONLOOP_TEST, 16);
    return result;
}

        //获取位置环测试参数
UINT8 MainWindow::getPositionTestData()
{
    UINT8 result = getRegValue(REG_POSITIONLOOP_TEST_LENGTH, 2);
    if(result)
        return result;

    canthread->testDataIndex = 0;
    canthread->testDataLength = canthread->positionTestLength;

    while (canthread->testDataLength)
    {
        if (canthread->testDataLength > 38)
        {
            result = getRegValue(REG_LOOP_TESTDATA + canthread->testDataIndex, 38);
            if (result)
                return result;
        }
        else
        {
            result = getRegValue(REG_LOOP_TESTDATA + canthread->testDataIndex, (UINT8)canthread->testDataLength);
            if (result)
                return result;
        }
    }
    return 0;
}

        //取消位置环测试函数
void MainWindow::processPositionTestCancel()
{
    positionTestTimer->stop();
    UINT8 result = setCommand(COMMAND_POSITION_TEST_ST);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
}

//*********************驱动器************************
void MainWindow::menuDriverparam()
{
    //0新建 1打开
    openDlgFileDriver(1);
    driverparams->setWindowTitle(tr(u8"应用参数-修改"));
    driverparams->ui->nextBtn->setText(tr(u8"保存"));
    driverparams->ui->backBtn->hide();
}

        //函数 打开 驱动器对话框 //0 无效 1确认2取消3上一步 F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 MainWindow::openDlgFileDriver(UINT8 mode)
{
    if(mode == 1){
        driverparams->ui->nextBtn->setEnabled(!(isMotorEnabled || isRealShow));
        driverparams->isRealShow = isRealShow;
        driverparams->isMotorEnabled = isMotorEnabled;
        driverparams->show();
    }
    else{
        driverparams->show();
    }
    return 0;

}

        //函数 写入驱动器应用参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setApplication()
{
    UINT8 motorMotionDirection = (canthread->motionDirection<<0x00) | (canthread->uint8_initialPosition<<0x01);
    DataTxArray[7] = motorMotionDirection;
    int appratedcurrent = printBinaryDigit(&canthread->appRatedCurrent);
    int appstallcurrent = printBinaryDigit(&canthread->appStallCurrent);

    canthread->Hex8ToArray(DataTxArray + 7 + 0, motorMotionDirection);
    canthread->Hex32ToArray(DataTxArray + 7 + 1, appratedcurrent);
    canthread->Hex32ToArray(DataTxArray + 7 + 5, appstallcurrent);
    canthread->Hex32ToArray(DataTxArray + 7 + 9, canthread->AppMaxSpeed);
    canthread->Hex32ToArray(DataTxArray + 7 + 13, canthread->Acceleration);
    canthread->Hex32ToArray(DataTxArray + 7 + 17, canthread->Deceleration);
    canthread->Hex32ToArray(DataTxArray + 7 + 21, canthread->stopDeceleration);
    canthread->Hex32ToArray(DataTxArray + 7 + 25, canthread->appMaxPosition);
    canthread->Hex32ToArray(DataTxArray + 7 + 29, canthread->appMinPosition);
    canthread->Hex32ToArray(DataTxArray + 7 + 33, canthread->initialPosition);
    UINT8 result = setRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
    return result;
}

//******************自定义参数******************
void MainWindow::menuUserDefined()
{
    openDlgFileUser();
    userdefined->setWindowTitle(tr(u8"自定义参数-修改"));
    userdefined->ui->sureBtn->setText(tr(u8"保存"));
}

    //函数 打开 自定义参数对话框 //0 无效 1确认2取消3上一步 F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 MainWindow::openDlgFileUser()
{
    userdefined->show();
    return 0;
}

    //写入自定义参数 RAM
UINT8 MainWindow::setsetUserDef()
{
    //TODO
    for (UINT8 i = 0; i < 8; i++)
    {
        canthread->BitToArray(DataTxArray + REG_INFIX_LENGTH + 0, canthread->m_UINT8_UserDef + i, i);
        canthread->BitToArray(DataTxArray + REG_INFIX_LENGTH + 1, canthread->m_UINT8_UserDef + 8 + i, i);
        canthread->BitToArray(DataTxArray + REG_INFIX_LENGTH + 2, canthread->m_UINT8_UserDef + 16 + i, i);
        canthread->BitToArray(DataTxArray + REG_INFIX_LENGTH + 3, canthread->m_UINT8_UserDef + 24 + i, i);
    }

    for (UINT8 i = 0; i < 24; i++)
    {
        canthread->Hex32ToArray(DataTxArray + REG_INFIX_LENGTH + 4 + 4 * i, canthread->m_INT32_UserDef[i]);
    }
    UINT8 result = setRegValue(REG_USER_DEF, REG_USER_DEF_NUM);
    if(result)
        return result;
    return 0;

//    canthread->BitToArray(DataTxArray + 7 + 0, canthread->m_UINT8_UserDef + 0, 0);
//    canthread->BitToArray(DataTxArray + 7 + 1, canthread->m_UINT8_UserDef + 8 + 0, 0);
//    canthread->BitToArray(DataTxArray + 7 + 2, canthread->m_UINT8_UserDef + 16 + 0, 0);
//    canthread->BitToArray(DataTxArray + 7 + 3, canthread->m_UINT8_UserDef + 24 + 0, 0);
      
//    for (UINT8 i = 0; i < 8; i++)
//	{
//        canthread->Hex32ToArray(DataTxArray + 7 +4 + 4 * i, canthread->m_INT32_UserDef[i]);
//	}

}


//******************************读写寄存器 以及响应数据处理************************************
    //写入寄存器 返回 无响应 或 processDataWrite的处理结果
    //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setRegValue(uint16_t regAddr, uint16_t reglength)
{
    DataTxArray[0] = 0x5A;                                              //帧头
    DataTxArray[1] = 0xA5;
    DataTxArray[2] = 0x04 + reglength;                                  //数据域长度
    DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + (0x02 & 0x07);     //功能码
    DataTxArray[4] = regAddr >> 8;                                      //寄存器地址 高字节在前，低字节在后
    DataTxArray[5] = regAddr & 0xFF;
    DataTxArray[6] = reglength;                                         //数据长度

    WORD CRCValue = canthread->CRC16(DataTxArray, reglength+ REG_INFIX_LENGTH);    //CRC16校验码

    DataTxArray[reglength + REG_INFIX_LENGTH] = (CRCValue & 0xFF);
    DataTxArray[reglength + REG_INFIX_LENGTH + 1] = (CRCValue >> 8);
    int num ;   //帧数
    if( (reglength + REG_INFIX_LENGTH + 2) % 8 == 0){
        num = (reglength + REG_INFIX_LENGTH + 2) / 8;
    }
    else if((reglength + REG_INFIX_LENGTH + 2) % 8 >0){
        num = (reglength + REG_INFIX_LENGTH + 2) / 8 + 1;
    }
    else{
        return ERRORCODE_NORESPONCE;
    }

    for(int i = reglength + REG_INFIX_LENGTH + 1;i<num*8;i++){
        DataTxArray[i] = 0x00;                                          //补码
    }

    QByteArray ba;
    QDataStream stream(&ba,QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);  
    for(const auto &value: DataTxArray){
        stream<<value;
    }

    unsigned char *sendData = (unsigned char*)ba.data();
    canthread->sendData(ID,sendData,0,num);

    //锁 等待接收线程数据返回
    canthread->mutex.lock();
    if(canthread->condition.wait(&canthread->mutex,5000)){
        canthread->mutex.unlock();
        return processDataWrite(regAddr);
    }
    else{
        canthread->mutex.unlock();
        return ERRORCODE_NORESPONCE;
    }
}

    //读取寄存器返回 无响应 或 processDataRead的处理结果
    //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::getRegValue(uint16_t regAddr, uint16_t reglength)
{
    DataTxArray[0] = 0x5A;                                              //帧头
    DataTxArray[1] = 0xA5;
    DataTxArray[2] = 0x04 + reglength;                                  //数据域长度
    DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + (0x01 & 0x07);     //功能码
    DataTxArray[4] = regAddr >> 8;                                      //寄存器地址 高字节在前，低字节在后
    DataTxArray[5] = regAddr & 0xFF;
    DataTxArray[6] = reglength;                                         //数据长度

    WORD CRCValue = canthread->CRC16(DataTxArray, REG_INFIX_LENGTH);    //CRC16校验码
    DataTxArray[REG_INFIX_LENGTH] = (CRCValue & 0xFF);
    DataTxArray[REG_INFIX_LENGTH + 1] = (CRCValue >> 8);

    for(int i=9;i<15;i++){                                   //补码 0x00
        DataTxArray[i] = 0x00;
    }

    QByteArray ba;
    QDataStream stream(&ba,QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);  
    for(const auto &value: DataTxArray){
        stream<<value;
    }
    unsigned char *sendData = (unsigned char*)ba.data();

    canthread->sendData(ID,sendData,0,2);
    canthread->mutex.lock();

    if(canthread->condition.wait(&canthread->mutex,5000)){
        canthread->mutex.unlock();
        return processDataRead(regAddr);
    }
    else{
        canthread->mutex.unlock();
        return ERRORCODE_NORESPONCE;
    }
}

    // 写入寄存器 显示处理 //0 处理正常 1 无响应 F0 + ErrCode 访问寄存器错误
UINT8 MainWindow::processDataWrite(uint16_t regAddr)
{
    if(canthread->registerErrorCode != 0){
        return 0xF0 + (canthread->registerErrorCode>>4);
    }
    switch (regAddr) {
        case REG_DEVICE_STATUS:
            break;
        case REG_DEVICE_ERRORCODE:
            canthread->errorStatus = 0;
            errorCode->setText(tr(u8"无错误"));
            break;
        case REG_DEVICE_INFO:
            break;
        case REG_RUN_MODE:
            break;
        case REG_CURRENTLOOP_P:
            break;
        case REG_CURRENTLOOP_TEST:
            break;
        case REG_CURRENTLOOP_TEST_FLAG:
            break;
        case REG_COMMAND:
            if (CommandWord == COMMAND_MOTOR_ENABLE)
            {
                ui->enableButton->setText(tr(u8"禁用"));
                ui->workModeComboBox->setEnabled(false);
                isMotorEnabled = 1;
            }
            else if (CommandWord == COMMAND_MOTOR_DISABLE)
            {
                ui->enableButton->setText(tr(u8"使能"));
                ui->workModeComboBox->setEnabled(true);
                isMotorEnabled = 0;
            }
            else
            {
            }
        break;
    }
    return 0;
}

    // 读取寄存器 显示处理 //0 处理正常 1 无响应 F0 + ErrCode 访问寄存器错误
UINT8 MainWindow::processDataRead(uint16_t regAddr)
{
    if(canthread->registerErrorCode != 0){
        return 0xF0 + (canthread->registerErrorCode>>4);
    }

    switch (regAddr) {
        case REG_DEVICE_STATUS:
            //显示设备状态
            switch (canthread->deviceStatus) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    deviceStatu->setText(tr(u8"设备状态：[%1]").arg(canthread->deviceStatus));
                default:
                break;
            }
            //显示电机状态
            switch (canthread->motorStatus) {
                case 0:
                    motorStatu->setText(tr(u8"电机状态：电机禁用"));
                    ui->workModeComboBox->setEnabled(true);
                    isMotorEnabled = false;
                break;
                case 1:
                case 2:
                    motorStatu->setText(tr(u8"电机状态：电机使能"));
                    ui->workModeComboBox->setEnabled(false);
                    isMotorEnabled = true;
                break;
                default:
                break;
            }
            //显示错误状态
            errorCodeCurrent = canthread->errorStatus;
            if (errorCodeCurrent != errorCodeFront) {
                index = 0;
                statusErrorProcess->stop();
                parseErrorCode(errorCodeCurrent);
                errorCodeFront = errorCodeCurrent;
            }

            //errorCode->setText(tr(u8"错误码：[%1]").arg(canthread->errorStatus));

            //显示实时信息
            if(isRealShow){
                QString dataRecordString;
                ui->recvdata1lineEdit->setText(QString::number(canthread->busVoltageStatus));
                ui->recvdata2lineEdit->setText(QString::number(canthread->phaseCurrentStatus));
                ui->recvdata3lineEdit->setText(QString::number(canthread->velocityStatus));
                ui->recvdata4lineEdit->setText(QString::number(canthread->positionStatus));
                ui->recvdata5lineEdit->setText(QString::number(canthread->jointPositionStatus));
                ui->recvdata6lineEdit->setText(QString::number(canthread->jointPositionStatus*360/131072.0));

                list.clear();
                QVector<double> temp1;
                temp1.append(canthread->velocityStatus);
                list.append(temp1);
                QVector<double> temp2;
                temp2.append(canthread->positionStatus);
                list.append(temp2);
                QVector<double> temp3;
                temp3.append(canthread->phaseCurrentStatus);
                list.append(temp3);
                QVector<double> temp4;
                temp4.append(canthread->busVoltageStatus);
                list.append(temp4);
                QVector<double> temp5;
                temp5.append(canthread->jointPositionStatus);
                list.append(temp5);
                QVector<double> temp6;
                temp6.append(canthread->jointPositionStatus*360/131072.0);
                list.append(temp6);

                 for(int i=0;i<6;++i){
                    dataRecordString.append(QString::fromLocal8Bit("%1\t").arg(list[i][0]));
                 }
                 dataRecordString.append("\r\n");

                 if(dataRecording){
                     dataRecordFile.write(dataRecordString.toLocal8Bit());
                 }

                 addPlotData(list);
            }
        break;

        case REG_DEVICE_INFO:
            deviceType->setText(tr(u8"设备类型：[%1]").arg(canthread->driverType));
            deviceInfo->setText(tr(u8"设备信息：[%1]").arg(canthread->driverInfo));
        break;

        case REG_DEVICE_REAL:
        break;

        case REG_RUN_MODE:
            ui->workModeComboBox->setCurrentIndex(canthread->workMode);
            //工作模式
            switch (canthread->workMode) {
                //电流模式
                case 0:
                    ui->setCurrentLineEdit->setText(QString::number(canthread->setCurrent));
                break;
                //速度模式
                case 1:

                    ui->setSpeedLineEdit->setText(QString::number(canthread->setSpeed));
                break;
                //位置模式
                case 2:

                    ui->setPositionLineEdit->setText(QString::number(canthread->setPosition));
                    ui->setSpeedLineEdit->setText(QString::number(canthread->setSpeed));
                    ui->positionTypeComboBox->setCurrentIndex(canthread->setPositionType);
                break;
                default:
                    QMessageBox::critical(this,tr(u8"错误"),tr(u8"工作模式无效"),QMessageBox::Close);
                break;
            }
        break;

        case REG_RUN_CURRENT:
            ui->setCurrentLineEdit->setText(QString::number(canthread->setCurrent));
        break;

        case REG_RUN_VELOCITY:
            edit_float_setSpeed = edit_setSpeed * 60.0F / canthread->pulsePerRevolution;
            if (edit_float_setSpeed >= 0)
                edit_float_setSpeed += 0.5F;
            else
                edit_float_setSpeed -= 0.5F;
            ui->setSpeedLineEdit->setText(QString::number(edit_float_setSpeed));
            ui->recvdata1lineEdit->setText(QString::number(canthread->busVoltageStatus));
            ui->recvdata2lineEdit->setText(QString::number(canthread->phaseCurrentStatus));
        break;

        case REG_RUN_POSITION:
            ui->setPositionLineEdit->setText(QString::number(canthread->setPosition));
            ui->setSpeedLineEdit->setText(QString::number(canthread->setSpeed));
            ui->positionTypeComboBox->setCurrentIndex(canthread->setPositionType);
        break;

        //电机匹配
        case REG_MATCH_FLAG:
            if(canthread->matchFlag == 0x01){
                //TODO
            }
            else if(canthread->matchFlag == 0x02){
                //TODO
            }
        break;

        case REG_CURRENTLOOP_TEST_FLAG:
        break;
        case REG_COMMAND:
        break;
        default:
        break;

    }
    return 0;
}

//******************************设置工作模式***************************************
    //设置工作模式
UINT8 MainWindow::setWorkMode()
{
    setworkmode = ui->workModeComboBox->currentIndex();
    DataTxArray[7] = setworkmode;
    uint8_t result = setRegValue(REG_RUN_MODE,1);
    return result;
}

    //设定电流模式 返回 setRegValue的处理结果 //0 0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setCurrentMode(int8_t direction)
{
    edit_setCurrent = ui->setCurrentLineEdit->text().toFloat();
    if(edit_setCurrent>canthread->appRatedCurrent || edit_setCurrent<-(canthread->appRatedCurrent)){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设定电流，输入超限"),QMessageBox::Close);
        return 1;
    }
    m_unParam_setCurrent.param_float = FLOAT(edit_setCurrent) *direction;
    canthread->Hex32ToArray(DataTxArray + 7, m_unParam_setCurrent.param_int);
    UINT8 result = setRegValue(REG_RUN_CURRENT, 4);
    return result;
}

    //设定速度模式 返回 setRegValue的处理结果 //0 0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setSpeedMode(int8_t direction)
{
    edit_float_setSpeed = ui->setSpeedLineEdit->text().toFloat();
    edit_setAcceleration = ui->setAccelerationLineEdit->text().toInt();
    if(edit_setSpeed > canthread->AppMaxSpeed || edit_setSpeed < -(canthread->AppMaxSpeed)){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设定速度，输入超限"),QMessageBox::Close);
        return 1;
    }
    edit_float_setSpeed = edit_float_setSpeed*direction;
    edit_setSpeed = (INT32)edit_float_setSpeed;
    canthread->Hex32ToArray(DataTxArray + 7 + 0, edit_setSpeed);
    canthread->Hex32ToArray(DataTxArray + 7 + 4, edit_setAcceleration);
    UINT8 result = setRegValue(REG_RUN_VELOCITY, 8);
    return result;
}

    //设定位置模式 返回 setRegValue的处理结果 //0 0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 MainWindow::setPositionMode(int8_t direction)
{
    edit_setSpeed = ui->setSpeedLineEdit->text().toInt();
    edit_setAngel = ui->setAngelLineEdit->text().toFloat();
    edit_setAcceleration = ui->setAccelerationLineEdit->text().toInt();
    edit_setPosition = ui->setPositionLineEdit->text().toInt()*direction;
    combox_setPositionType = ui->positionTypeComboBox->currentIndex();

    if(edit_setSpeed > canthread->AppMaxSpeed || edit_setSpeed < -(canthread->AppMaxSpeed)){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设定速度，输入超限"),QMessageBox::Close);
        return 1;
    }

    if(edit_setAngel > canthread->appMaxAngle || edit_setSpeed < -(canthread->appMinAngle)){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设定位置，输入超限"),QMessageBox::Close);
        return 1;
    }
    canthread->Hex8ToArray(DataTxArray + 7 + 0, combox_setPositionType);
    canthread->Hex32ToArray(DataTxArray + 7 + 1, edit_setPosition);
    canthread->Hex32ToArray(DataTxArray + 7 + 5, edit_setSpeed);
    canthread->Hex32ToArray(DataTxArray + 7 + 9, edit_setAcceleration);

    UINT8 result = setRegValue(REG_RUN_POSITION, 13);
    return result;
}

    //设置命令
UINT8 MainWindow::setCommand(uint16_t command)
{
    DataTxArray[7] = uint8_t(command>>8);
    DataTxArray[8] = uint8_t(command & 0xFF);

    UINT8 result = setRegValue(REG_COMMAND, 2);
    return result;
}

//=************运动控制 按键操作************//
    //使能
void MainWindow::on_enableButton_clicked()
{
    UINT8 result = 0;
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(!isMotorEnabled){
        if(!canthread->matchFlag){
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机未匹配"),QMessageBox::Close);
            return;
        }
        result = setWorkMode();


        if (result)
        {
            canthread->processErrorCode(result);
            return;
        }
        CommandWord = COMMAND_MOTOR_ENABLE;
        result = setCommand(COMMAND_MOTOR_ENABLE);
        if (result)
        {
            canthread->processErrorCode(result);
        }
    }
    else
    {
        CommandWord = COMMAND_MOTOR_DISABLE;
        result = setCommand(COMMAND_MOTOR_DISABLE);
        if (result){
            canthread->processErrorCode(result);
        }
    }
}

    //启动
void MainWindow::on_runButton_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(!isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机未使能"),QMessageBox::Close);
        return;
    }
    uint8_t workMode = ui->workModeComboBox->currentIndex();

    UINT8 result = 0;

    switch (workMode)
    {
        case 0:
            result = setCurrentMode(1);
            if (result)
            {
                canthread->processErrorCode(result);
                return;
            }
        break;
        case 1:
            result = setSpeedMode(1);
            if (result)
            {
                canthread->processErrorCode(result);
                return;
            }
        break;
        case 2:
            result = setPositionMode(1);
            if (result)
            {
                canthread->processErrorCode(result);
                return;
            }
        break;
        default:
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("工作模式无效"),QMessageBox::Close);
        break;
    }

    CommandWord = COMMAND_MOTOR_RUN;
    result = setCommand(COMMAND_MOTOR_RUN);
    if (result){
        canthread->processErrorCode(result);
        return;
    }
//    backRunButtonDisenable();
//    runButtonEnable();
}

    //正常停止
void MainWindow::on_stopButton_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(!isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机未使能"),QMessageBox::Close);
        return;
    }

    UINT8 result = 0;
    CommandWord = COMMAND_MOTOR_STOP;
    result = setCommand(COMMAND_MOTOR_STOP);
    if (result){
        canthread->processErrorCode(result);
        return;
    }
//    runButtonDisenable();
//    backRunButtonDisenable();
}

    //紧急停止
void MainWindow::on_emergencyStopButton_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(!isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机未使能"),QMessageBox::Close);
        return;
    }

    UINT8 result = 0;
    CommandWord = COMMAND_MOTOR_ESTOP;
    result = setCommand(COMMAND_MOTOR_ESTOP);
    if (result){
        canthread->processErrorCode(result);
        return;
    }
//    runButtonDisenable();
//    backRunButtonDisenable();
}

    //反向
void MainWindow::on_reverseButton_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(!isMotorEnabled){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("电机未使能"),QMessageBox::Close);
        return;
    }
    uint8_t workMode = ui->workModeComboBox->currentIndex();

    UINT8 result = 0;
    switch (workMode)
    {
        case 0:
            result = setCurrentMode(-1);
            if (result)
            {
                canthread->processErrorCode(result);
                return;
            }
            break;
        case 1:
            result = setSpeedMode(-1);
            if (result)
            {
                canthread->processErrorCode(result);
                return;
            }
            break;
        case 2:
            result = setPositionMode(-1);
            if (result)
            {
                canthread->processErrorCode(result);
                return;
            }
            break;
        default:
            QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("工作模式无效"),QMessageBox::Close);
            break;
    }

    CommandWord = COMMAND_MOTOR_RUN;
    result = setCommand(COMMAND_MOTOR_RUN);
    if (result){
        canthread->processErrorCode(result);
        return;
    }
//    runButtonDisenable();
//    backRunButtonEnable();

}

    //清除故障
void MainWindow::on_clearErrorBtn_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }

    UINT8 result = setRegValue(REG_DEVICE_ERRORCODE, REG_DEVICE_ERRORCODE_NUM);
    if (result)
    {
        canthread->processErrorCode(result);
        return;
    }
    else{
        QMessageBox::information(this,tr(u8""),tr(u8"清除成功"),"OK");
    }
}

    //校准
void MainWindow::on_adjustBtn_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(isRealShow){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }
    edit_positionAdjust = ui->positionAdjustingLineEdit->text().toInt();
    if(edit_positionAdjust>canthread->appMaxPosition || edit_positionAdjust<canthread->appMinPosition){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("位置校准，输入超限"),QMessageBox::Close);
        return;
    }

    DataTxArray[7] = edit_positionAdjust >> 24;
    DataTxArray[8] = edit_positionAdjust >> 16;
    DataTxArray[9] = edit_positionAdjust >> 8;
    DataTxArray[10] = edit_positionAdjust & 0xFF;

    UINT8 result = setRegValue(REG_DEVICE_REAL + 4, 4);
    if (result)
        canthread->processErrorCode(result);

    if (canthread->driverType == "ACS306")
        result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM + 4);				//关节模组驱动器 多读一个 关节位置寄存器
    else
        result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);
    if (result)
        canthread->processErrorCode(result);
}
//**************************************************************//

//********定时器处理******//
void MainWindow::timerProcess(UINT8 timer)
{
    UINT8 result = 0;
    switch (timer) {
        //实时数据更新
        case RealDataUpdataTimer:
            if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
                realDataUpdataTimer->stop();
                ui->realShowDataBtn->click();
                QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
                return;
            }

            if (canthread->driverInfo == "ASC306"){                                        // 2   ACS306
                result = getRegValue(REG_DEVICE_STATUS, 16 + 6 + 4);				//关节模组驱动器 多读一个 关节位置寄存器
            }
            else{
                result = getRegValue(REG_DEVICE_STATUS, 16 + 6);
            }

            if (result)
            {
                realDataUpdataTimer->stop();
                ui->realShowDataBtn->click();
                canthread->processErrorCode(result);
            }
        break;

        //状态栏
        case DeviceStatusUpdataTimer:
            result = getRegValue(REG_DEVICE_STATUS, 6);
            if (result)
            {
                QMessageBox::critical(this,tr(u8"错误"),tr(u8"设备无响应，请重新连接……"),QMessageBox::Close);
            }
        break;

        //电机匹配
        case MotorMatchTimer:
            //电机匹配500ms一次，最多请求5次
            if(motormatchCount>=4){
                motormatch->m_box->close();
                QMessageBox::critical(this,tr(u8"错误"),tr(u8"电机匹配超时，请重试……"),QMessageBox::Close);
                motorMatchTimer->stop();
                motormatch->matchStatus = 0;
                motormatch->ui->matchStatusComboBox->setCurrentIndex(2);
                return;
            }
            result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);

            if (result)
            {
                motormatch->m_box->close();
                motorMatchTimer->stop();
                canthread->processErrorCode(result);
            }
            else
            {
                motormatch->m_box->setText(tr(u8"匹配结束"));
                motormatch->cancel->hide();
                motormatch->close->show();
                if (canthread->matchFlag>=0 && canthread->matchFlag<=2){
                    deviceStatusUpdataTimer->start(500);
                    motormatch->ui->matchStatusComboBox->setCurrentIndex(canthread->matchFlag);             
                    motorMatchTimer->stop();
                }
            }
            motormatchCount++;
        break;

        //电流环测试
        case CurrentTestTimer:
            if(motormatchCount>=10){
                loopparam->m_box->close();
                QMessageBox::critical(this,tr(u8"错误"),tr(u8"测试超时，请重试……"),QMessageBox::Close);
                currentTestTimer->stop();
                return;
            }
            result = getRegValue(REG_CURRENTLOOP_TEST_FLAG, 1);
            if (result)
            {
                loopparam->m_box->close();
                currentTestTimer->stop();
                canthread->processErrorCode(result);
            }
            else{
                if(canthread->currentTestStatu == 3){
                    currentTestTimer->stop();
                    loopparam->m_box->setText(tr(u8"读取数据..."));
                    loopparam->cancel->hide();
                    loopparam->close->show();
                    result = getCurrentTestData();
                    if(result !=0)
                    {
                        loopparam->m_box->setText(tr(u8"测试出错，请关闭……"));
                        return;
                    }
                    else{
                        loopparam->m_box->setText(tr(u8"读取完成"));
                    }
                }
            }
            motormatchCount++;
        break;

        //速度环测试
        case VelocityTestTimer:
            if(motormatchCount>=10){
                loopparam->m_box->close();
                QMessageBox::critical(this,tr(u8"错误"),tr(u8"测试超时，请重试……"),QMessageBox::Close);
                velocityTestTimer->stop();
                return;
            }
            result = getRegValue(REG_VELOCITYLOOP_TEST_FLAG, 1);
            if (result)
            {
                loopparam->m_box->close();
                velocityTestTimer->stop();
                canthread->processErrorCode(result);
            }
            else{
                if(canthread->velocityTestStatu == 3){
                    velocityTestTimer->stop();
                    loopparam->m_box->setText(tr(u8"读取数据..."));
                    loopparam->cancel->hide();
                    loopparam->close->show();
                    result = getVelocityTestData();
                    if(result)
                    {
                        loopparam->m_box->setText(tr(u8"测试出错，请关闭……"));
                    }
                    else{
                        //TODO:环路测试界面波形显示
                        loopparam->m_box->setText(tr(u8"读取完成"));
                    }
                }
            }
            motormatchCount++;
        break;

            //位置环测试
            case PositionTestTimer:
                if(motormatchCount>=10){
                    loopparam->m_box->close();
                    QMessageBox::critical(this,tr(u8"错误"),tr(u8"测试超时，请重试……"),QMessageBox::Close);
                    positionTestTimer->stop();
                    return;
                }
                result = getRegValue(REG_POSITIONLOOP_TEST_FLAG, 1);
                if (result)
                {
                    loopparam->m_box->close();
                    positionTestTimer->stop();
                    canthread->processErrorCode(result);
                }
                else{
                    if(canthread->positionTestStatu == 3){
                        positionTestTimer->stop();
                        loopparam->m_box->setText(tr(u8"读取数据..."));
                        loopparam->cancel->hide();
                        loopparam->close->show();
                        result = getPositionTestData();
                        if(result)
                        {
                            loopparam->m_box->setText(tr(u8"测试出错，请关闭……"));
                        }
                        else{
                            //TODO:环路测试界面波形显示
                            loopparam->m_box->setText(tr(u8"读取完成"));
                        }
                    }
                }
                motormatchCount++;
            break;
        case StatusErrorProcess:
                errorCode->setText(tr(u8"错误码：[%1]").arg(codeValue[index]));
                index++;
                if (index == listLength) {
                    index = 0;
                }
                break;
        default:
        break;
    }
}

//状态栏错误码对应说明
void MainWindow::parseErrorCode(UINT16 errorValue) {
    codeValue.clear();
    if (errorValue == 0x00) {
        errorCode->setText(tr(u8"错误码：无错误"));
        return;
    }
    else {
        if ((errorValue & (1 << 0)) != 0) {
            codeValue.push_back(tr(u8"硬件过流复位"));
        }
        if ((errorValue & (1 << 1)) != 0) {
            codeValue.push_back(tr(u8"堵转错误"));
        }
        if ((errorValue & (1 << 2)) != 0) {
            codeValue.push_back(tr(u8"欠压错误"));
        }
        if ((errorValue & (1 << 3)) != 0) {
            codeValue.push_back(tr(u8"过压错误"));
        }
        if ((errorValue & (1 << 4)) != 0) {
            codeValue.push_back(tr(u8"电机过流"));
        }
        if ((errorValue & (1 << 5)) != 0) {
            codeValue.push_back(tr(u8"保留"));
        }
        if ((errorValue & (1 << 6)) != 0) {
            codeValue.push_back(tr(u8"电源输入过流"));
        }
        if ((errorValue & (1 << 7)) != 0) {
            codeValue.push_back(tr(u8"外部电池故障"));
        }
        if ((errorValue & (1 << 8)) != 0) {
            codeValue.push_back(tr(u8"超速错误"));
        }
        if ((errorValue & (1 << 9)) != 0) {
            codeValue.push_back(tr(u8"位置超限"));
        }
        if ((errorValue & (1 << 10)) != 0) {
            codeValue.push_back(tr(u8"内部参考电压错误"));
        }
        if ((errorValue & (1 << 11)) != 0) {
            codeValue.push_back(tr(u8"内部温度错误"));
        }
        if ((errorValue & (1 << 12)) != 0) {
            codeValue.push_back(tr(u8"电机温度错误"));
        }
        if ((errorValue & (1 << 13)) != 0) {
            codeValue.push_back(tr(u8"编码器校准错误"));
        }
    }

    listLength = codeValue.size();
    if (codeValue.empty()) {
        return;
    }
    statusErrorProcess->start(1000);
    return;
}

//添加波形
void MainWindow::addPlotData(const QVector<QVector<double>> &values)
{
    ui->waveformwidget1->addOneData(values[3]);
    ui->waveformwidget2->addOneData(values[2]);
    ui->waveformwidget3->addOneData(values[0]);
    ui->waveformwidget4->addOneData(values[1]);
    ui->waveformwidget5->addOneData(values[4]);
    ui->waveformwidget6->addOneData(values[5]);
}

//打开数据记录文件
bool MainWindow::openDataRecordFileAndStart()
{
    QDir dir(QDir::currentPath()+QDir::separator()+tr(u8"user/dataRecord"));
    if(!dir.exists()){
        dir.mkpath(tr(u8"."));
    }

    QString fileName=tr(u8"user/dataRecord/RobotRecordData %1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
    dataRecordFile.setFileName(fileName);
    if (!dataRecordFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr(u8"文件错误"), tr(u8"打开 [%1] 数据保存文件失败！").arg(fileName), QMessageBox::Close);
        return false;
    }
    dataRecording=true;
    return true;
}

//停止记录并关闭文件
bool MainWindow::stopAndCloseDataRecordFile()
{
    if(dataRecordFile.isOpen()){
        dataRecording=false;
        dataRecordFile.close();
        return true;
    }else{
        dataRecording=false;
        QMessageBox::critical(this, tr(u8"文件错误"), tr(u8"数据保存出现异常！\n关闭 [%1] 数据保存文件失败！").arg(dataRecordFile.fileName()), QMessageBox::Close);
        return false;
    }
}

bool MainWindow::isDataRecording()
{
    return dataRecording;
}

// messagebox 信息显示
void MainWindow::showInfo(const QString& title, const QString& text)
{
    QMessageBox::information(this,title,text,"OK");
}

// messagebox 错误信息显示
void MainWindow::showError(const QString& title, const QString& text)
{
    QMessageBox::critical(this,title,text,QMessageBox::Close);
}

//十六进制str转有符号十进制
int MainWindow::hex_str_to_int(unsigned char *ch)
{
    if(ch[0]>='0'&&ch[0]<='9')
    {
        if(ch[1]>='0'&&ch[1]<='9')
            return (((ch[0]-'0')*16)+(ch[1]-'0'));
        if(ch[1]>='a'&&ch[1]<='f')
            return (((ch[0]-'0')*16)+(ch[1]-'a'+10));
        if(ch[1]>='A'&&ch[1]<='F')
            return (((ch[0]-'0')*16)+(ch[1]-'A'+10));
    }
    else if(ch[0]>='a'&&ch[0]<='f')
    {
        if(ch[1]>='0'&&ch[1]<='9')
            return (((ch[0]-'a'+10)*16)+(ch[1]-'0'));
        if(ch[1]>='a'&&ch[1]<='f')
            return (((ch[0]-'a'+10)*16)+(ch[1]-'a'+10));
        if(ch[1]>='A'&&ch[1]<='F')
            return (((ch[0]-'a'+10)*16)+(ch[1]-'A'+10));
    }
    else if(ch[0]>='A'&&ch[0]<='F')
    {
        if(ch[1]>='0'&&ch[1]<='9')
            return (((ch[0]-'A'+10)*16)+(ch[1]-'0'));
        if(ch[1]>='a'&&ch[1]<='f')
            return (((ch[0]-'A'+10)*16)+(ch[1]-'a'+10));
        if(ch[1]>='A'&&ch[1]<='F')
            return (((ch[0]-'A'+10)*16)+(ch[1]-'A'+10));
    }
}

//十六进制str转无符号十进制
unsigned int MainWindow::hex_str_to_uint(char*ch_id)
{
    int sum = 0,num=1;
    for(int i=0; i<8; i++,num=1)
    {
        if('0'==ch_id[i])
            continue;

        for(int j=i;j < 7;j++)
            num = num*16;

        if (ch_id[i]>='0'&&ch_id[i]<='9')
        {
            int n = ch_id[i]-'0';
            sum = sum + n*num;
        }
        else if(ch_id[i]>='a'&&ch_id[i]<='f')
            sum += ((int)(ch_id[i]-'a'+10))*num;
        else if(ch_id[i]>='A'&&ch_id[i]<='F')
            sum += ((int)(ch_id[i]-'A'+10))*num;
    }
    return sum;
}

//float转二进制转十进制
int MainWindow::printBinaryDigit(float *pf)
{
    QList<QString> list;
    int n = *(int *)pf;
    int sizeOfBits = sizeof(int) * 8;
    int i = sizeOfBits - 1;
    for(i = sizeOfBits - 1; i >= 0; i--)
    {
        if(n & (1 << i))
        {
            list.append("1");
        }
        else
        {
            list.append("0");
        }
    }
    QString str = list.join("");
    bool ok;
    int c = str.toInt(&ok,2);//先转为10进制
    return c;
}

// CANID设置
void MainWindow::on_setIDBtn_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(isRealShow == true){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }
    ID = ui->CANIDComboBox->currentText().toUInt();
    canthread->CANID = ID;
    QMessageBox::information(this,tr(u8"设置"),tr(u8"ID设置成功"),"OK");
}

void MainWindow::runButtonEnable(){
    ui->workModeComboBox->setEnabled(false);
    ui->runButton->setText(tr(u8"运行中"));
    ui->runButton->setStyleSheet("QPushButton { background-color: rgb(0,149,255); font-weight: normal; color:white; border:2px solid rgb(0,110,255);}\
                                        QPushButton:enabled:hover {  background-color: rgb(0,149,255); font-weight: bold;}\
                                        QPushButton:enabled:pressed { background-color: rgb(0,149,255); }");
    ui->runButton->setEnabled(false);
}

void MainWindow::runButtonDisenable(){
    ui->workModeComboBox->setEnabled(true);
    ui->runButton->setText(tr(u8"启动"));
    ui->runButton->setStyleSheet("QPushButton:enabled { background-color: #019b48; font-weight: normal; color:white; }\
                                        QPushButton:enabled:hover { background-color: #019b48;font-weight: bold; }\
                                        QPushButton:enabled:pressed { background-color: #019b48; }");
    ui->runButton->setEnabled(true);
}

void MainWindow::backRunButtonEnable(){
    ui->workModeComboBox->setEnabled(false);
    ui->reverseButton->setText(tr(u8"反向运行中"));
    ui->reverseButton->setStyleSheet("QPushButton { background-color: rgb(0,149,255); font-weight: normal; color:white; border:2px solid rgb(0,110,255);}\
                                        QPushButton:enabled:hover {  background-color: rgb(0,149,255); font-weight: bold;}\
                                        QPushButton:enabled:pressed { background-color: rgb(0,149,255); }");
    ui->reverseButton->setEnabled(false);
}

void MainWindow::backRunButtonDisenable(){
    ui->workModeComboBox->setEnabled(true);
    ui->reverseButton->setText(tr(u8"反向"));
    ui->reverseButton->setStyleSheet("QPushButton:enabled { background-color: #019b48; font-weight: normal; color:white; }\
                                        QPushButton:enabled:hover { background-color: #019b48;font-weight: bold; }\
                                        QPushButton:enabled:pressed { background-color: #019b48; }");

    ui->reverseButton->setEnabled(true);
}


//选择文件
void MainWindow::on_openfile_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);  //QSettings能记录一些程序中的信息，下次再打开时可以读取出来；将设置存储在INI文件中。请注意，从INI文件读取设置时，不会保留类型信息;所有值都将作为QString返回。
    QString lastPath = setting.value("LastFilePath").toString();  //获取上次的打开路径

    filename = QFileDialog::getOpenFileName(this,tr(u8"打开文件"),lastPath,"files (*);;files (*.elf);;files (*.hex);;files (*.txt);;files (*.bin)");
    ui->filenameedit->setText(filename);
}

//启动程序
void MainWindow::on_runBootLoader_clicked()
{
    if(ui->CanRunPtn->text()==tr(u8"CAN启动")){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备未打开"),QMessageBox::Close);
        return;
    }
    if(isRealShow == true){
        QMessageBox::critical(this,tr(u8"错误"),QString::fromLocal8Bit("设备正忙"),QMessageBox::Close);
        return;
    }

    QDataStream stream(&downloadRun,QIODevice::WriteOnly);
    std::array<quint8,8> stopvalue={0x5A,0xA5,0x04,0x02,0x50,0x02,0x08,0xF9};
    for(const auto &value:stopvalue){
        stream<<value;
    }

    unsigned char* bootData;
    bootData =  (unsigned char *)downloadRun.data();
    canthread->sendData(ID,bootData,0,1);

    canthread->mutex.lock();
    if(canthread->Readycondition.wait(&canthread->mutex,25000)){
        bootLoaderStatus->setText(tr(u8"请选择文件开始下载"));
        ui->startBootLoader->setEnabled(true);
        ui->progressBar->setValue(0);
        canthread->mutex.unlock();
    }
    else{
        canthread->mutex.unlock();
        bootLoaderStatus->setText(tr(u8"响应超时"));
        QMessageBox::critical(this,"bootLoader下载",tr(u8"响应超时"),QMessageBox::Close);
        return;
    }
}

//程序下载
void MainWindow::on_startBootLoader_clicked()
{
    deviceStatusUpdataTimer->stop();
    //各个控件禁用
    ui->horizontalLayout_Top->setEnabled(false);
    ui->menuBar->setEnabled(false);
    ui->MoveControlgroupBox->setEnabled(false);
    ui->OthergroupBox->setEnabled(false);
    ui->realShowDataBtn->setEnabled(false);

    bootLoaderStatus->setText(tr(u8"正在下载中"));
    ui->runBootLoader->setEnabled(false);
    ui->startBootLoader->setEnabled(false);

    sumSendPacket = 0;
    quint16 sum=0;
    QFile localfile (filename);
    int size = localfile.size();
    int linesize=0;
    address[0] = 0x30;
    int length;
    if(!localfile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"choose file first";
        return;
    }
    while(!localfile.atEnd())
    {
        sum = 0;
        QByteArray line = localfile.readLine();
        linesize += line.length();
        // 设置进度条
        ui->progressBar->setMaximum(size);
        ui->progressBar->setValue(linesize);

        //去冒号
        line.remove(0,1);
        frame = QByteArray::fromHex(line);
        length = frame.length();
        //去校验码
        frame.remove(length-1,1);

        // 取数据字节
        int framehead= frame.at(0);
        QString num = QString::number(framehead,10);

        //判断类型是否为扩展地址开始
        if(frame.at(3) == 0x04)
        {
            address[0] = frame.at(5);
            continue;
        }
        // 判断是否结束程序下载
        else if(frame.at(3) == 0x01){
            finishDownload();
            break;
        }

        address[1] = frame.at(1);
        address[2] = frame.at(2);

        if(frame.at(0) == 0x20){
            firstframe = frame.mid(4,32);
            firstframe.insert(0,address);
            firstframe.insert(0,0x10);
            firstframe.insert(0,0x53);
            firstframe.insert(firstframe.length(),0x45);

           //加入校验
           uchar *sumframe = (uchar *)(firstframe.data());
           for(int i=0;i<firstframe.length();i++){
               sum = sum + sumframe[i];
           }
           firstframe.insert(firstframe.length()-1,sum & 0xFF);
           firstframe.insert(firstframe.length()-1,sum >>8);
           sendBootData(firstframe);
           sumSendPacket += 1;

           canthread->mutex.lock();
           if(canthread->ACKcondition.wait(&canthread->mutex,5000)){
               canthread->mutex.unlock();
           }
           else{
               canthread->mutex.unlock();
               bootLoaderStatus->setText(tr(u8"响应超时"));
               QMessageBox::critical(this,"",tr(u8"响应超时,请重新下载文件"),QMessageBox::Close);
               ui->runBootLoader->setEnabled(false);
               ui->startBootLoader->setEnabled(true);
               ui->progressBar->setValue(0);
               break;
           }
        }

        //不需要分帧；加入帧头、地址、帧尾、数据字长度、补齐八个字数据
        else
        {
            sum = 0;
            int head1 = num.toUInt()/2;
            QByteArray head = QByteArray::fromHex(QByteArray::number(head1,16));
            frame.remove(0,4);
            frame.insert(0,address);
            frame.insert(0,head);
            frame.insert(0,0x53);

            for(uint i=0;i<(32-num.toUInt());i++)
            {
                frame.insert(frame.length(),0x01);
            }
            frame.insert(frame.length(),0x45);

            uchar *sumframe3 = (uchar *)(frame.data());
            for(int i=0;i<frame.length();i++){
                sum = sum + sumframe3[i];
            }
            frame.insert(frame.length()-1,sum & 0xFF);
            frame.insert(frame.length()-1,sum >> 8);

            sendBootData(frame);
            sumSendPacket += 1;

            canthread->mutex.lock();
            if(canthread->ACKcondition.wait(&canthread->mutex,3000)){
                canthread->mutex.unlock();
            }
            else{
                canthread->mutex.unlock();
                bootLoaderStatus->setText(tr(u8"响应超时"));
                QMessageBox::critical(this,"",tr(u8"响应超时,请重新下载文件"),QMessageBox::Close);
                ui->runBootLoader->setEnabled(false);
                ui->startBootLoader->setEnabled(true);
                ui->progressBar->setValue(0);

                break;
            }
        }
    }
    localfile.close();
    deviceStatusUpdataTimer->start(500);
    ui->horizontalLayout_Top->setEnabled(true);
    ui->menuBar->setEnabled(true);
    ui->MoveControlgroupBox->setEnabled(true);
    ui->OthergroupBox->setEnabled(true);
    ui->realShowDataBtn->setEnabled(true);
}

//  结束程序下载
void MainWindow::finishDownload()
{
    QDataStream stream(&finishArray,QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    quint8 value=0;
    quint16 sum1=0;
    value=0x53;
    sum1+=value;
    stream << value;

    value=0x45;
    sum1+=value;
    stream << value;

    value=sumSendPacket & 0xFF;
    sum1+=value;
    stream << value;

    value=sumSendPacket >> 8;
    sum1+=value;
    stream << value;

    for(size_t i=0;i<33;i++){
        value = 0x00;
        sum1 += value;
        stream << value;
    }

    value = 0x45;
    sum1+=value;
    //写入校验码
    quint8 sumLow = sum1  & 0xFF;
    quint8 sumHigh = sum1 >>8;
    stream << sumLow;
    stream << sumHigh;
    value = 0x45;
    stream << value;

    sendBootData(finishArray);

     canthread->mutex.lock();
     if(canthread->Comcondition.wait(&canthread->mutex,15000)){
         canthread->mutex.unlock();
         bootLoaderStatus->setText(tr(u8"下载完成"));
         QMessageBox::information(this,"",tr(u8"下载完成"),"OK");
         ui->runBootLoader->setEnabled(true);
     }
     else{
         canthread->mutex.unlock();
         bootLoaderStatus->setText(tr(u8"响应超时"));
         QMessageBox::critical(this,"",tr(u8"响应超时,请重新下载文件"),QMessageBox::Close);
         ui->runBootLoader->setEnabled(false);
         ui->startBootLoader->setEnabled(true);
         ui->progressBar->setValue(0);
     }
}

//发送Boot相关数据QByteArray
void MainWindow::sendBootData(QByteArray data)
{
    unsigned char* bootData;
    bootData =  (unsigned char *)data.data();
    canthread->sendData(ID,bootData,0,5);
}


void MainWindow::on_pushButton_clicked()
{
    ui->recv_textEdit->clear();
}



