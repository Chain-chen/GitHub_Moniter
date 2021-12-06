#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ControlCAN.h"
#include <QDebug>
#include "canparamsetting.h"
#include "canthread.h"
#include "loopparam.h"
#include "motormatch.h"
#include "driverparams.h"
#include "userdefined.h"
#include "canparamsetting.h"
#include <QTimer>
#include <windows.h>
#include <array>
#include <QTimer>
#include "waveformwidget.h"

//定时器
#define RealDataUpdataTimer 1                          //实时更新
#define DeviceStatusUpdataTimer 2                      //设备信息
#define MotorMatchTimer 3                              //电机匹配
#define CurrentTestTimer 4                             //电流环测试
#define VelocityTestTimer 5                            //速度环测试
#define PositionTestTimer 6                            //位置环测试

//寄存器地址
#define REG_MOTOR_POLES		   0x1000                  //电机极对数
#define REG_MOTOR_POLES_NUM	17
#define REG_MOTOR_RATEDCURRENT 0x1004                  //电机额定电流

#define REG_APPLICATION		   0x1100                  //电机运行配置参数
#define REG_APPLICATION_NUM	37

#define REG_CURRENTLOOP_P	0x2000
#define REG_CURRENTLOOP_I	0x2004
#define REG_CURRENTLOOP_D	0x2008
#define REG_CURRENTLOOP_NUM	12

#define REG_VELOCITYLOOP_P	0x200C
#define REG_VELOCITYLOOP_I	0x2010
#define REG_VELOCITYLOOP_D	0x2014
#define REG_VELOCITYLOOP_NUM 12

#define REG_POSITIONLOOP_P	0x2018
#define REG_POSITIONLOOP_I	0x201C
#define REG_POSITIONLOOP_D	0x2020
#define REG_POSITIONLOOP_NUM 12

#define REG_DEVICE_STATUS	0x1200                  //电机及驱动器状态
#define REG_DEVICE_STATUS_NUM 6

#define REG_DEVICE_REAL		0x1206
#define REG_DEVICE_REAL_NUM	16

#define REG_DEVICE_ERRORCODE	0x1202
#define REG_DEVICE_ERRORCODE_NUM 2

#define REG_DEVICE_INFO		0x2100

#define REG_DEVICE_STATIC	0x2122                  //驱动器静态参数
#define REG_DEVICE_STATIC_NUM 24

#define REG_MATCH_FLAG		0x2200                  //驱动器匹配参数
#define REG_MATCH_FLAG_NUM	7

#define REG_RUN_MODE		0x2300                  //驱动器运行参数
#define REG_RUN_CURRENT		0x2301
#define REG_RUN_VELOCITY	0x2305
#define REG_RUN_POSITION	0x2309

#define REG_CURRENTLOOP_TEST_FLAG	0x2400
#define REG_CURRENTLOOP_TEST		0x2401
#define REG_CURRENTLOOP_TEST_NUM	7
#define REG_CURRENTLOOP_TEST_LENGTH	0x2408

#define REG_VELOCITYLOOP_TEST_FLAG	        0x2500
#define REG_VELOCITYLOOP_TEST_VELOCITY		0x2501
#define REG_VELOCITYLOOP_TEST_LENGTH		0x2505

#define REG_POSITIONLOOP_TEST_FLAG		0x2600
#define REG_POSITIONLOOP_TEST			0x2601
#define REG_POSITIONLOOP_TEST_LENGTH	0x2611

#define REG_COMMAND			   0x6000
#define COMMAND_MOTOR_MATCH	   0x0101
#define COMMAND_MOTOR_NOMATCH  0x0100
#define COMMAND_MOTOR_ENABLE   0x0201
#define COMMAND_MOTOR_DISABLE  0x0200
#define COMMAND_MOTOR_RUN	   0x0300
#define COMMAND_MOTOR_STOP	   0x0301
#define COMMAND_MOTOR_ESTOP	   0x0302
#define COMMAND_CPARAM_SAVE	   0x0401

#define COMMAND_DPARAM_SAVE	     0x0400
#define COMMAND_CURRENT_TEST_BG  0x0501
#define COMMAND_CURRENT_TEST_ST  0x0500
#define COMMAND_VELOCITY_TEST_BG 0x0601
#define COMMAND_VELOCITY_TEST_ST 0x0600
#define COMMAND_POSITION_TEST_BG 0x0701
#define COMMAND_POSITION_TEST_ST 0x0700

#define REG_LOOP_TESTDATA		 0x8000

#define REG_INFIX_LENGTH	0x07


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    WaveformWidget *waveformwidget;
    ///添加波形图
    void addPlotData(const QVector<QVector<double>> &values);
    ///十六进制str转十进制
    int hex_str_to_int(unsigned char *ch);
    unsigned int hex_str_to_uint(char*ch_id);

    void init();
    bool openDataRecordFileAndStart();
    bool stopAndCloseDataRecordFile();
    bool isDataRecording();

    void showError(const QString& title,const QString& text);
    void showInfo(const QString& title,const QString& text);

    BYTE DataTxArray[128];                //发送数据

    uint16_t CommandWord;                //控制命令

    //状态栏信息
    QLabel *deviceInfo;
    QLabel *deviceType;
    QLabel *deviceStatu;
    QLabel *motorStatu;
    QLabel *errorCode;

signals:


private slots:
    void on_CanparamSettingPbt_clicked();

    void on_enableButton_clicked();

    void on_runButton_clicked();

    void on_stopButton_clicked();

    void on_emergencyStopButton_clicked();

    void on_reverseButton_clicked();

    void on_clearErrorBtn_clicked();

    void on_adjustBtn_clicked();

    ///***匹配电机处理
    void processMotorMatch();
    ///取消电机匹配
    void processMotorMatchCancel();
    ///函数 写入电机参数 RAM
    UINT8 setMotorPoles();

    //***三环测试***
    ///电流环测试
    void processCurrentTest();
    ///取消电流环测试
    void processCurrentTestCancel();
    ///函数 写入电流环参数 RAM
    UINT8 setCurrentLoop();
    ///写入电流环测试参数 RAM
    UINT8 setCurrentTest();
    ///获取电流环测试数据
    UINT8 getCurrentTestData();
    ///速度环测试
    void processVelocityTest();
    ///取消速度环测试
    void processVelocityTestCancel();
    ///函数 写入速度环参数 RAM
    UINT8 setVelocityLoop();
    ///写入速度环测试参数 RAM
    UINT8 setVelocityTest();
    ///获取速度环测试数据
    UINT8 getVelocityTestData();
    ///位置环测试
    void processPositionTest();
    ///取消位置环测试
    void processPositionTestCancel();
    ///函数 写入位置环参数 RAM
    UINT8 setPositionLoop();
    ///写入位置环测试参数 RAM
    UINT8 setPositionTest();
    ///获取位置环测试数据
    UINT8 getPositionTestData();
    //***驱动器参数
    ///写入驱动器应用参数ARM
    UINT8 setApplication();
    //***自定义参数
    UINT8 setsetUserDef();

    void on_openfile_clicked();

    void on_datasendPtn_clicked();

    void on_remoteSend_clicked();

    void initdevice();

private:
    Ui::MainWindow *ui;

    CANParamSetting *CANsetting;
    LoopParam *loopparam;
    MotorMatch *motormatch;
    DriverParams *driverparams;
    UserDefined *userdefined;
    CANThread *canthread;
    CANParamSetting *canparamsetting;
    QThread thread;

    bool isMotorEnabled=false;        //电机使能
    bool dataRecording=false;         //文件保存
    bool isRealShow=false;            //实时显示
    QFile dataRecordFile;

    float edit_setCurrent;            //输入框设置电流
    float edit_setAngel;              //输入框设置角度
    int32_t edit_setSpeed;            //输入框设置速度
    float edit_float_setSpeed;
    int32_t edit_setAcceleration;     //输入框设置加速度
    int32_t edit_setPosition;         //输入框设置位置
    int8_t combox_setPositionType;    //复选框设置位置类型
    int32_t edit_positionAdjust;      //输入框设置位置校准

    uint8_t setworkmode;              //工作模式

    QVector<QVector<double>> list;

    ///写入寄存器
    UINT8 setRegValue(uint16_t regAddr,uint16_t reglength);
    ///读取寄存器
    UINT8 getRegValue(uint16_t regAddr,uint16_t dataLength);
    ///读取寄存器数据处理：显示
    UINT8 processDataRead(uint16_t regAddr);
    ///写入寄存器数据处理：显示
    UINT8 processDataWrite(uint16_t regAddr);
    ///设置工作模式
    UINT8 setWorkMode();
    UINT8 setCurrentMode(int8_t direction);
    UINT8 setSpeedMode(int8_t direction);
    UINT8 setPositionMode(int8_t direction);

    ///写入命令
    UINT8 setCommand(uint16_t);

    //各个定时器超时检测次数
    int motormatchCount=0;
    ///定时器处理函数
    void timerProcess(UINT8);
    //实时数据显示定时器
    QTimer *realDataUpdataTimer;
    //电机匹配定时器
    QTimer *motorMatchTimer;
    //电流环测试定时器
    QTimer *currentTestTimer;
    //速度环测试定时器
    QTimer *velocityTestTimer;
    //位置环测试定时器
    QTimer *positionTestTimer;

    ///float转十进制
    int printBinaryDigit(float *pf);
    //****电机****
    void menuMotorMatch();
    UINT8 openDlgFileMotor(UINT8 mode);
    //****环路****
    void menuLoopParam();
    UINT8 openDlgFileLoop(UINT8 mode);
    //****驱动器****
    void menuDriverparam();
    UINT8 openDlgFileDriver(UINT8 mode);
    //****自定义参数****
    void menuUserDefined();
    UINT8 openDlgFileUser();

    ///启动状态
    void runButtonEnable();
    ///启动停止状态
    void runButtonDisenable();
    ///反向启动状态
    void backRunButtonEnable();
    ///方向停止状态
    void backRunButtonDisenable();

    QString filename;
    QFile localfile;
};

#endif // MAINWINDOW_H
