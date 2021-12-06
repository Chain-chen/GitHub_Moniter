#ifndef CANTHREAD_H
#define CANTHREAD_H

#include <QThread>
#include "ControlCAN.h"
#include <QDebug>
#include <array>
#include <windows.h>
#include <QMutex>
#include <QWaitCondition>

#define REG_INFIX_LENGTH 0x07
//寄存器地址
#define REG_MOTOR_POLES		   0x1000                  //电机极对数
#define REG_MOTOR_RATEDCURRENT 0x1004                  //电机额定电流
#define REG_APPLICATION		   0x1100                  //电机运行配置参数

#define REG_CURRENTLOOP_P	0x2000
#define REG_CURRENTLOOP_I	0x2004
#define REG_CURRENTLOOP_D	0x2008

#define REG_VELOCITYLOOP_P	0x200C
#define REG_VELOCITYLOOP_I	0x2010
#define REG_VELOCITYLOOP_D	0x2014

#define REG_POSITIONLOOP_P	0x2018
#define REG_POSITIONLOOP_I	0x201C
#define REG_POSITIONLOOP_D	0x2020

#define REG_DEVICE_STATUS	0x1200                  //电机及驱动器状态

#define REG_DEVICE_REAL		0x1206
#define REG_DEVICE_REAL_NUM	16

#define REG_DEVICE_INFO		0x2100
#define REG_DEVICE_STATIC	0x2122                  //驱动器静态参数

#define REG_MATCH_FLAG		0x2200                  //驱动器匹配参数

#define REG_RUN_MODE		0x2300                  //驱动器运行参数
#define REG_RUN_CURRENT		0x2301
#define REG_RUN_VELOCITY	0x2305
#define REG_RUN_POSITION	0x2309

#define REG_CURRENTLOOP_TEST_FLAG	0x2400
#define REG_CURRENTLOOP_TEST		0x2401
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

#define REG_DEVICE_ERRORCODE	0x1202

#define REG_USER_DEF		0x3000
#define REG_USER_DEF_NUM	0x0064

//错误码
#define ERRORCODE_NORESPONCE		0xF0
#define ERRORCODE_CRC				0xF1
#define ERRORCODE_FUNCTION			0xF2
#define ERRORCODE_ADDRESS			0xF3
#define ERRORCODE_COMMAND			0xF4
#define ERRORCODE_COMMANDVALUE		0xF5
#define ERRORCODE_INVALIDE			0xF6
#define ERRORCODE_MATCH				0xF7
#define ERRORCODE_READONLY			0xF8
#define ERRORCODE_ENABLE			0xF9
#define ERRORCODE_OVERFLOW			0xFA

//bootloader
#define BOOTREADY 0x7000
#define BOOTACK 0x7001
#define BOOTCOMPLETE 0x7002

class CANThread:public QThread
{
    Q_OBJECT
public:
    CANThread();

    BYTE m_BYTE_DataRxArray[1024];
    int deviceType;  //设备类型
    int devicIndex;  //设备索引
    int baundRate;   //波特率
    int devicCom;    //CAN通道
    bool stopped;
    QVector<QString> devinfo;
    UINT CANID = 301;
    uint8_t registerErrorCode=0;  //错误码

    //*******************//

    // 电机静态参数
    INT8 motorPolePairs;                    //电机极对数
    UINT8 encoderType;                      //电机位置反馈类型
    uint16_t encoderResolution;             //电机编码器线数
    float ratedCurrent;                     //电机额定电流
    float stallCurrent;                     //电机堵转电流
    uint8_t hallOrder;
    int32_t ratedSpeed;                     //电机额定转速
    uint32_t pulsePerRevolution;            //每圈 脉冲数   P/s = P/r * r/m  / 60

    // 电机运行配置参数
    uint8_t motionDirection;     //电机转动正方向
    uint8_t uint8_initialPosition;
    float appRatedCurrent;      //电机运行持续电流
    float appStallCurrent;      //电机运行堵转电流
    int32_t AppMaxSpeed;        //电机运行最大速度
    int32_t Acceleration;       //电机运行加速度
    int32_t Deceleration;       //电机正常停止减速度
    int32_t stopDeceleration;   //电机紧急停止减速度
    int32_t appMaxPosition;     //电机运行最大位置
    int32_t appMinPosition;     //电机运行最小位置
    int32_t initialPosition;
    float appMaxAngle;
    float appMinAngle;

    // 电机及驱动器状态
    uint8_t deviceStatus;       //设备状态
    uint8_t motorStatus;        //电机状态
    uint16_t errorStatus;       //故障值
    uint16_t statusErrorStatus; //故障值备份
    ///实时显示
    int32_t velocityStatus;     //电机当前运行速度
    int32_t positionStatus;     //电机当前绝对位置
    float phaseCurrentStatus;   //电机当前电流
    float busVoltageStatus;     //驱动器母线电压
    int32_t jointPositionStatus; //关节位置

    //PID参数
    float currentLoopP;
    float currentLoopI;
    float currentLoopD;
    float velocityLoopP;
    float velocityLoopI;
    float velocityLoopD;
    float positionLoopP;
    float positionLoopI;
    float positionLoopD;

    //PID测试
    uint8_t currentTestStatu;               //电流环测试状态
    uint8_t currentTestType;                //电流环测试类型
    float currentTestAmplitude;             //电流幅值
    uint16_t currentTestFrequency;          //正弦电流频率
    uint16_t currentTestLength;             //电流环测试数据长度

    uint8_t velocityTestStatu;              //速度环测试状态
    int32_t velocityTestVelocity;           //速度环测试速度
    uint16_t velocityTestLength;            //速度环测试数据长度

    uint8_t positionTestStatu;              //位置环测试状态
    int32_t positionTestAcceleration;       //位置环测试加速度值
    int32_t positionTestDeceleration;       //位置环测试减速度值
    int32_t positionMaxSpeed;               //位置环测试最大速度值
    int32_t positionTargetPosition;         //位置环测试目标位置值
    uint16_t positionTestLength;            //位置环测试数据长度

    UINT8 pTestData[0x2000];


    int testDataIndex;
    int testDataLength;

    //驱动器静态参数
    QString driverInfo;                     //驱动器识别号
    QString driverType;                     //驱动器型号
    float driverMinVoltage;                 //驱动器最小输入电压
    float driverMaxVoltage;                 //驱动器最大输入电压
    float driverRatedCurrent;               //驱动器输出持续电流
    float driverStallCurrent;               //驱动器输出峰值电流
    int32_t driverMinVelocity;              //驱动器输出最小速度
    int32_t driverMaxVelocity;              //驱动器输出最大速度
    float driverMaxSpeed;
    float driverMinSpeed;

    //驱动器匹配参数
    uint8_t matchFlag;                      //驱动器匹配标志
    uint8_t phaseOrder;                     //电机相线线序
    uint8_t gethallOrder;                   //电机霍尔线序
    int8_t hallDirection;                   //电机霍尔角度方向
    int8_t encoderDirection;                //编码器角度方向
    int16_t defaultDegree;                  //初始电角度

    //驱动器运行参数
    uint8_t workMode;                       //运行模式
    float setCurrent;                       //电流模式设置的电流
    int32_t setSpeed;                       //速度模式设置的速度
    uint8_t setPositionType;                //位置模式设置的模式
    int32_t setPosition;                    //位置模式设置的位置
    int32_t setPositionSpeed;               //位置模式设置的速度

    //自定义参数
    UINT8 m_UINT8_UserDef[32];
    INT32 m_INT32_UserDef[24];


    QMutex mutex;
    QWaitCondition condition;
    QWaitCondition ACKcondition;
    QWaitCondition Readycondition;
    QWaitCondition Comcondition;

    void stop();
    bool openCAN();
    void closeCAN();
    void sendData(unsigned int ID, unsigned char *ch,int isremote, int num);
    void receiveData();
    void processErrorCode(uint8_t);
    void sleep(unsigned int msec);
    void dataProcess(BYTE*);
    float hexstrToFloat(QString);

    UINT8 BitToArray(UINT8* pArray,UINT8* data,UINT8 position);
    UINT8 Hex32ToArray(UINT8 * pArray, UINT32 data);
    UINT8 Hex16ToArray(UINT8 * pArray, UINT16 data);
    UINT8 Hex8ToArray(UINT8 * pArray, UINT8 data);
    UINT8 ArrayToHex32(UINT8 * pArray, UINT32 * pdata);
    UINT8 ArrayToHex32(UINT8 * pArray, INT32 * pdata);
    UINT8 ArrayToHex32(UINT8 * pArray, DWORD * pdata);
    UINT8 ArrayToHex16(UINT8 * pArray, UINT16 * pdata);
    UINT8 ArrayToHex16(UINT8 * pArray, INT16 * pdata);
    UINT8 ArrayToHex8(UINT8 * pArray, UINT8 * pdata);
    UINT8 ArrayToHex8(UINT8 * pArray, INT8 * pdata);
    UINT8 ArrayToBit(UINT8* pArray, UINT8* pdata, UINT8 position);
    QString uncharToQstring(unsigned char * id,int len);

    WORD CRC16(BYTE * pByteArray, BYTE Length);
    static const BYTE auchCRCHi[256];
    static const BYTE auchCRCLo[256];

    //界面测试显示
    QList<unsigned char> showdata;

    //bootloader下载接受
    bool isBootLoader = false;

signals:
    void showCANError(const QString title,const QString text);
    void showCANInfo(const QString title,const QString text);
    void recvError();
    void recvLoopTestDataEnding(QVector<QVector<double>>);
    void sendToMainRecvData(const QVector<QVector<uint8_t>>, int count);
    void showRecvData(QList<unsigned char>);

private:
    void run();


};

#endif // CANTHREAD_H
