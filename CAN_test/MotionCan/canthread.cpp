#include "canthread.h"
#include "ControlCAN.h"
#include <QTime>
#include <QCoreApplication>
#include <QMetaType>
#include <string.h>
#include <windows.h>
#include <QMessageBox>
#include <QTime>

CANThread::CANThread()
{
    stopped = false;
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    qRegisterMetaType<DWORD>("DWORD");
}

void CANThread::stop()
{
    stopped = true;
}

//打开CAN分析仪
bool CANThread::openCAN()
{
    int nDeviceType = deviceType;
    int nDeviceInd = devicIndex; /* 第1个设备 */
    int nCANInd = devicCom; /* 第1个通道 */
    DWORD dwRel;

    //打开CAN
    dwRel = VCI_OpenDevice(nDeviceType, nDeviceInd, nCANInd);
    if(dwRel != 1)
    {
        showCANError(tr(u8"错误"),tr(u8"设备打开失败！"));
        return false;
    }
    else
        qDebug()<<"open success";

    VCI_INIT_CONFIG vic;
    vic.AccCode=0x00000000;
    vic.AccMask=0xFFFFFFFF;
    //滤波：0/1：接受所有类型；2：只接受标准帧；3：只接受扩展帧
    vic.Filter=1;

    //工作模式：0：正常模式；1：监听模式；2自发自收测试模式
    vic.Mode=0;
    switch (baundRate) {
        case 10:
            vic.Timing0=0x31;
            vic.Timing1=0x1c;
            break;
        case 20:
            vic.Timing0=0x18;
            vic.Timing1=0x1c;
            break;
        case 40:
            vic.Timing0=0x87;
            vic.Timing1=0xff;
            break;
        case 50:
            vic.Timing0=0x09;
            vic.Timing1=0x1c;
            break;
        case 80:
            vic.Timing0=0x83;
            vic.Timing1=0xff;
            break;
        case 100:
            vic.Timing0=0x04;
            vic.Timing1=0x1c;
            break;
        case 125:
            vic.Timing0=0x03;
            vic.Timing1=0x1c;
            break;
        case 200:
            vic.Timing0=0x81;
            vic.Timing1=0xfa;
            break;
        case 250:
            vic.Timing0=0x01;
            vic.Timing1=0x1c;
            break;
        case 400:
            vic.Timing0=0x80;
            vic.Timing1=0xfa;
            break;
        case 500:
            vic.Timing0=0x00;
            vic.Timing1=0x1c;
            break;
        case 666:
            vic.Timing0=0x80;
            vic.Timing1=0xb6;
            break;
        case 800:
            vic.Timing0=0x00;
            vic.Timing1=0x16;
            break;
        case 1000:
            vic.Timing0=0x00;
            vic.Timing1=0x14;
            break;
        case 33:
            vic.Timing0=0x09;
            vic.Timing1=0x6f;
            break;
        case 66:
            vic.Timing0=0x04;
            vic.Timing1=0x6f;
            break;
        case 83:
            vic.Timing0=0x03;
            vic.Timing1=0x6f;
            break;
        default:
            break;
    }

    //环回自检时
    if(vic.Mode==2){
        //清空CAN通道的缓冲区
        dwRel = VCI_ClearBuffer(nDeviceType, nDeviceInd, 0);
        dwRel = VCI_ClearBuffer(nDeviceType, nDeviceInd, 1);
        //初始化CAN通道
        dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, 0, &vic);
        dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, 1, &vic);
    }

    //只打开一个通道时
    else{
        dwRel = VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd);
        dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &vic);
    }

    if(dwRel !=1)
    {
        showCANError(tr(u8"错误"),tr(u8"初始化CAN设备失败！"));
        return false;
    }
    else
        qDebug()<<"init success";

    //获取设备信息
    VCI_BOARD_INFO vbi;
    dwRel = VCI_ReadBoardInfo(nDeviceType, nDeviceInd, &vbi);
    if(dwRel != 1)
    {
        showCANError(tr(u8"错误"),tr(u8"读取设备信息失败！"));
        return false;
    }
    else
    {
        devinfo.append(QString::number(vbi.can_Num,10));  //CAN通道数
        devinfo.append(vbi.str_Serial_Num);               //此板卡的序列号
        devinfo.append(vbi.str_hw_Type);                  //str_hw_Type
    }

    //启动CAN卡的某一个CAN通道
    for(int i=0;i<=nCANInd;i++){
        if(VCI_StartCAN(nDeviceType, nDeviceInd, i) !=1)
        {
            showCANError(tr(u8"错误"),tr(u8"启动CAN卡通道[%1]失败！").arg(i+1));
            return false;
        }
        else
            qDebug()<<"start"<<i<<"success:";
    }
    showCANInfo(tr(u8"启动"),tr(u8"[%1]启动成功!").arg(devinfo[2]));
    return true;
}

//发送数据
void CANThread::sendData(unsigned int ID, unsigned char *ch,int isremote,int num)
{
    DWORD dwRel;
    VCI_CAN_OBJ vco;
    vco.ID = ID;
    //SendType=0时为正常发送（发送失败会自动重发，重发最长时间为1.5-3秒）；1时为单次发送（只发送一次，不自动重发）；
    vco.SendType=1;
    vco.RemoteFlag=isremote;            //远程帧和数据帧
    vco.ExternFlag=0;
    vco.DataLen=8;
    if(isremote==0){
        for(int j=0;j<num;j++){
            for (int i=0; i < 8; i++)
            {
                vco.Data[i]=ch[i+j*8];
            }
            dwRel = VCI_Transmit(deviceType, devicIndex, devicCom, &vco,1);
            if(dwRel!=STATUS_ERR){

            }
            else{
                showCANError(tr(u8"错误"),tr(u8"发送失败！"));
                return;
            }
        }
    }

    else if(isremote==1){
        for (int i=0; i < 8; i++)
        {
            vco.Data[i]=NULL;
        }
        dwRel = VCI_Transmit(deviceType, devicIndex, devicCom, &vco,1);
        if(dwRel!=STATUS_ERR){

        }
        else{
            showCANError(tr(u8"错误"),tr(u8"发送失败！"));
            return;
        }
    }
}

// 接收
void CANThread::receiveData()
{
    DWORD dwRel;
    VCI_CAN_OBJ vco[2500];
    dwRel = VCI_Receive(deviceType, devicIndex, devicCom, vco,2500,0);   //返回实际读取到的帧数    

    switch (CANID) {
        case 301:
            CANID = 641;
        break;
        default:
        break;
    }
//    if(vco->ID == CANID){
    if(1){
        if(dwRel>0){
            if(vco->RemoteFlag == 1){                                        //接收到远程帧
                qDebug()<<tr(u8"远程帧");
                return;
            }                                //数据帧
            for(int i=0;i<int(dwRel);i++){
                for(int j=0;j<8;j++){
                    m_BYTE_DataRxArray[i*8+j] = vco[i].Data[j];
                   // showdata.append( vco[i].Data[j]);
                }
            }
            dataProcess(m_BYTE_DataRxArray);

            //emit showRecvData(showdata);
           // showdata.clear();
        }

        else if(dwRel == -1)
        {
            showCANError(tr(u8"错误"),tr(u8"设备不存在或USB掉线！"));
            VCI_CloseDevice(deviceType,devicIndex);
            emit recvError();
        }
    }
}

//数据处理
void CANThread::dataProcess(BYTE m_BYTE_DataRxArray[1024])
{

    if(m_BYTE_DataRxArray[0]!=0x5A||m_BYTE_DataRxArray[1]!=0xA5){
        return;
    }

    WORD regAddr = 0;                                               //寄存器地址

    registerErrorCode = m_BYTE_DataRxArray[3]&0xF0;                 //错误码
    if(registerErrorCode >= 0xF0){
       processErrorCode(registerErrorCode);
       return;
    }

    BYTE FunctionCode = m_BYTE_DataRxArray[3]&0x07;                 //功能码

    regAddr = m_BYTE_DataRxArray[4]<<8 | m_BYTE_DataRxArray[5];     //寄存器地址

    switch(regAddr){
        //电机参数
        case REG_MOTOR_POLES:
            if (FunctionCode == 0x01)
            {
                int ratedCurrent_int;
                int stallCurrent_int;
                ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &motorPolePairs);
                ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &encoderType);
                ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &encoderResolution);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &ratedCurrent_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &stallCurrent_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 12, &ratedSpeed);
                ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 16, &hallOrder);
                ratedCurrent = hexstrToFloat(QString::number(ratedCurrent_int,16));
                stallCurrent = hexstrToFloat(QString::number(stallCurrent_int,16));

                if(encoderType == 0){
                    pulsePerRevolution = motorPolePairs * 6;
                }
                else{
                    pulsePerRevolution = encoderResolution * 4;
                }
            }

            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

        //驱动器应用参数
        case REG_APPLICATION:
            if (FunctionCode == 0x01)
            {
                int appRatedCurrent_int;
                int appStallCurrent_int;
                ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &motionDirection, 0x00);
                ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &uint8_initialPosition, 0x01);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &appRatedCurrent_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &appStallCurrent_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 9, &AppMaxSpeed);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 13, &Acceleration);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 17, &Deceleration);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 21, &stopDeceleration);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 25, &appMaxPosition);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 29, &appMinPosition);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 33, &initialPosition);
                appRatedCurrent = hexstrToFloat(QString::number(appRatedCurrent_int,16));
                appStallCurrent = hexstrToFloat(QString::number(appStallCurrent_int,16));
                appMaxAngle = appMaxPosition;
                appMinAngle = appMinPosition;
            }
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

        //电机及驱动器状态
        case REG_DEVICE_STATUS:
            int phaseCurrentStatus_int;
            int busVoltageStatus_int;
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &deviceStatus);
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &motorStatus);
            ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &errorStatus);
            ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &statusErrorStatus);
            ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 6, &velocityStatus);
            ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 10, &positionStatus);
            ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 14, &phaseCurrentStatus_int);
            ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 18, &busVoltageStatus_int);

            phaseCurrentStatus = hexstrToFloat(QString::number(phaseCurrentStatus_int,16));
            busVoltageStatus = hexstrToFloat(QString::number(busVoltageStatus_int,16));
            ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 22, &jointPositionStatus);	//关节模组驱动器 多读一个 关节位置寄存器
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

        //电流环PID参数
        case REG_CURRENTLOOP_P:
            if(FunctionCode == 0x01){
                int currentLoopP_int;
                int currentLoopI_int;
                int currentLoopD_int;
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &currentLoopP_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &currentLoopI_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &currentLoopD_int);

                currentLoopP = hexstrToFloat(QString::number(currentLoopP_int,16));
                currentLoopI = hexstrToFloat(QString::number(currentLoopI_int,16));
                currentLoopD = hexstrToFloat(QString::number(currentLoopD_int,16));
            }
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

        //速度环PID
        case REG_VELOCITYLOOP_P:
            if(FunctionCode == 0x01){
                int velocityLoopP_int;
                int velocityLoopI_int;
                int velocityLoopD_int;
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &velocityLoopP_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &velocityLoopI_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &velocityLoopD_int);

                velocityLoopP = hexstrToFloat(QString::number(velocityLoopP_int,16));
                velocityLoopI = hexstrToFloat(QString::number(velocityLoopI_int,16));
                velocityLoopD = hexstrToFloat(QString::number(velocityLoopD_int,16));
            }
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

        //位置环PID
        case REG_POSITIONLOOP_P:
            if(FunctionCode == 0x01){
                int positionLoopP_int;
                int positionLoopI_int;
                int positionLoopD_int;
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &positionLoopP_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &positionLoopI_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &positionLoopD_int);

                positionLoopP = hexstrToFloat(QString::number(positionLoopP_int,16));
                positionLoopI = hexstrToFloat(QString::number(positionLoopI_int,16));
                positionLoopD = hexstrToFloat(QString::number(positionLoopD_int,16));
            }
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

        //驱动器静态参数
        case REG_DEVICE_STATIC:
            if (FunctionCode == 0x01){
                int driverMinVoltage_int;
                int driverMaxVoltage_int;
                int driverRatedCurrent_int;
                int driverStallCurrent_int;

                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &driverMinVoltage_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &driverMaxVoltage_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &driverRatedCurrent_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 12, &driverStallCurrent_int);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 16, &driverMinVelocity);
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 20, &driverMaxVelocity);

                driverMinSpeed = driverMinVelocity * 60.0 / pulsePerRevolution;
                driverMaxSpeed = driverMaxVelocity * 60.0 / pulsePerRevolution;

                driverMinVoltage = hexstrToFloat(QString::number(driverMinVoltage_int,16));
                driverMaxVoltage = hexstrToFloat(QString::number(driverMaxVoltage_int,16));
                driverRatedCurrent = hexstrToFloat(QString::number(driverRatedCurrent_int,16));
                driverStallCurrent = hexstrToFloat(QString::number(driverStallCurrent_int,16));
            }
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;


        case REG_DEVICE_INFO:
            if (FunctionCode == 0x01){
                driverInfo = "";
                driverType = "";

//                for (int i = REG_INFIX_LENGTH; m_BYTE_DataRxArray[i] != '?'; i++)
                for (int i = REG_INFIX_LENGTH; i < REG_INFIX_LENGTH + 15; i++)
                {
                    driverInfo += m_BYTE_DataRxArray[i];
                }
//                for (int i = 16 + REG_INFIX_LENGTH; m_BYTE_DataRxArray[i] != '?'; i++)
                for (int i = 16 + REG_INFIX_LENGTH; i < REG_INFIX_LENGTH + 15 + 7; i++)
                {
                    driverType += m_BYTE_DataRxArray[i];
                }
           }
           mutex.lock();
           condition.wakeAll();
           mutex.unlock();
        break;

        case REG_DEVICE_REAL + 4:
          mutex.lock();
          condition.wakeAll();
          mutex.unlock();
        break;

        case REG_DEVICE_ERRORCODE:
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        break;

    //驱动器匹配参数
    case REG_MATCH_FLAG:
        if(FunctionCode == 0x01){
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &matchFlag);
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &phaseOrder);
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &gethallOrder);
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 3, &hallDirection);
            ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &encoderDirection);
            ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &defaultDegree);
        }
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
    break;

   //驱动器运行参数
   case REG_RUN_MODE:
           int setCurrent_int;
           ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &workMode);
           ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &setCurrent_int);
           ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &setSpeed);
           ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 9, &setPositionType);
           ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 10, &setPosition);
           ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 14, &setPositionSpeed);
           setCurrent = hexstrToFloat(QString::number(setCurrent_int,16));
       mutex.lock();
       condition.wakeAll();
       mutex.unlock();
   break;

    //电流环测试参数
    case REG_CURRENTLOOP_TEST:
      mutex.lock();
      condition.wakeAll();
      mutex.unlock();
    break;

    //电流环测试状态
    case REG_CURRENTLOOP_TEST_FLAG:
         if(FunctionCode == 0x01){
             int currentTestAmplitude_int;
             ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &currentTestStatu);
             ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &currentTestType);
             ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &currentTestAmplitude_int);
             ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 6, &currentTestFrequency);
             ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 6, &currentTestLength);
         }
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
    break;

    //电流环测试参数 测试数据长度
    case REG_CURRENTLOOP_TEST_LENGTH:
        if(FunctionCode == 0x01){
            ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &currentTestLength);
        }
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
    break;

    //速度环测试参数
    case REG_VELOCITYLOOP_TEST_VELOCITY:
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
    break;

    //速度环测试状态
    case REG_VELOCITYLOOP_TEST_FLAG:
       ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &velocityTestStatu);
       ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &velocityTestVelocity);
       ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &velocityTestLength);
       mutex.lock();
       condition.wakeAll();
       mutex.unlock();
    break;

    //速度环测试参数 测试数据长度
    case REG_VELOCITYLOOP_TEST_LENGTH:
        if(FunctionCode == 0x01){
            ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &velocityTestLength);
        }
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
    break;

     //位置环测试参数
     case REG_POSITIONLOOP_TEST:
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
     break;

     //位置环测试状态
     case REG_POSITIONLOOP_TEST_FLAG:
         ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &positionTestStatu);
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &positionTestAcceleration);
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &positionTestDeceleration);
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 9, &positionMaxSpeed);
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 13, &positionTargetPosition);
         ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 17, &positionTestLength);
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
     break;

    //位置环测试参数 测试数据长度
    case REG_POSITIONLOOP_TEST_LENGTH:
        if(FunctionCode == 0x01){
             ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &positionTestLength);
        }
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
    break;


     //工作模式 电流
     case REG_RUN_CURRENT:
         int setCurrent_int2;
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &setCurrent_int2);
         setCurrent = hexstrToFloat(QString::number(setCurrent_int2,16));
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
     break;

     //工作模式 速度
     case REG_RUN_VELOCITY:
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &setSpeed);
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
     break;

     //工作模式 位置
     case REG_RUN_POSITION:
         ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &setPositionType);
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &setPosition);
         ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &setPositionSpeed);
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
     break;

     case REG_COMMAND:
         mutex.lock();
         condition.wakeAll();
         mutex.unlock();
     break;

     //自定义参数
     case REG_USER_DEF:
        if (FunctionCode == 0x01)
        {
            for (UINT8 i = 0; i < 8; i++)
            {
                ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, m_UINT8_UserDef + i, i);
                ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, m_UINT8_UserDef + i + 8, i);
                ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, m_UINT8_UserDef + i + 16, i);
                ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 3, m_UINT8_UserDef + i + 24, i);
            }
            for (UINT8 i = 0; i < 24; i++)
            {
                ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4 + 4 * i, m_INT32_UserDef + i);
            }
        }
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
        break;

    //bootloader
    case BOOTREADY:
        mutex.lock();
        Readycondition.wakeAll();
        mutex.unlock();
        break;
    case BOOTACK:
        mutex.lock();
        ACKcondition.wakeAll();
        mutex.unlock();
        break;
    case BOOTCOMPLETE:
        mutex.lock();
        Comcondition.wakeAll();
        mutex.unlock();
        break;

    //三环测试数据
    default:
        if ((regAddr >= REG_LOOP_TESTDATA) && (regAddr <= REG_LOOP_TESTDATA + 0x2000))
        {
            if (testDataLength > 128)
            {
                memcpy(pTestData + testDataIndex , m_BYTE_DataRxArray + REG_INFIX_LENGTH, sizeof(UINT8) * 128);
                testDataIndex += 128;
                testDataLength -= 128;
            }
            else
            {
                memcpy(pTestData + testDataIndex, m_BYTE_DataRxArray + REG_INFIX_LENGTH, sizeof(UINT8) * (UINT8)testDataLength);
                testDataIndex += (UINT8)testDataLength;
                testDataLength -= (UINT8)testDataLength;
            }
        }
        else
        {
            return;
        }
        mutex.lock();
        condition.wakeAll();
        mutex.unlock();
        break;

    }
}


//错误码处理
void CANThread::processErrorCode(uint8_t errorCode)
{
   switch (errorCode)
       {
       case ERRORCODE_NORESPONCE:
           showCANError(tr(u8"错误"),tr(u8"设备无响应"));
           break;
       case ERRORCODE_CRC:
           showCANError(tr(u8"错误"),tr(u8"错误的CRC校验"));
           break;
       case ERRORCODE_FUNCTION:
           showCANError(tr(u8"错误"),tr(u8"错误的功能码"));
           break;
       case ERRORCODE_ADDRESS:
           showCANError(tr(u8"错误"),tr(u8"错误的参数地址"));
           break;
       case ERRORCODE_COMMAND:
           showCANError(tr(u8"错误"),tr(u8"错误的命令"));
           break;
       case ERRORCODE_COMMANDVALUE:
           showCANError(tr(u8"错误"),tr(u8"错误的命令值"));
           break;
       case ERRORCODE_INVALIDE:
           showCANError(tr(u8"错误"),tr(u8"无法处理命令"));
           break;
       case ERRORCODE_MATCH:
           showCANError(tr(u8"错误"),tr(u8"电机未匹配"));
           break;
       case ERRORCODE_READONLY:
           showCANError(tr(u8"错误"),tr(u8"参数只读"));
           break;
       case ERRORCODE_ENABLE:
           showCANError(tr(u8"错误"),tr(u8"电机未使能"));
           break;
       case ERRORCODE_OVERFLOW:
           showCANError(tr(u8"错误"),tr(u8"参数溢出"));
           break;
       default:
           if (errorCode > 0xF0)
               showCANError(tr(u8"错误"),tr(u8"未知错误"));
           break;
       }
   return;
}

void CANThread::run()
{
   while(!stopped)
   {
       receiveData();
       sleep(50);
   }
   stopped = false;
}

void CANThread::closeCAN()
{
   VCI_CloseDevice(deviceType, devicIndex);
   showCANInfo(tr(u8"关闭"),tr(u8"[%1]关闭成功!").arg(devinfo[2]));
}

//自定义sleep函数
void CANThread::sleep(unsigned int msec)
{
  QTime dieTime = QTime::currentTime().addMSecs(msec);
  while( QTime::currentTime() < dieTime )
      QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//十六进制字符串转为float
float CANThread::hexstrToFloat(QString str)
{
   int hex = str.toUInt(0, 16);
   float value = *(float*)&hex;
   return value;
}

UINT8 CANThread::Hex32ToArray(UINT8 * pArray, UINT32 data)
{
   pArray[0] = (data >> 24) & 0xFF;
   pArray[1] = (data >> 16) & 0xFF;
   pArray[2] = (data >> 8) & 0xFF;
   pArray[3] = (data >> 0) & 0xFF;
   return 0;
}

UINT8 CANThread::Hex16ToArray(UINT8 * pArray, UINT16 data)
{
   pArray[0] = (data >> 8) & 0xFF;
   pArray[1] = (data >> 0) & 0xFF;
   return 0;
}

UINT8 CANThread::Hex8ToArray(UINT8 * pArray, UINT8 data)
{
   pArray[0] = (data >> 0) & 0xFF;
   return 0;
}

UINT8 CANThread::BitToArray(UINT8* pArray,UINT8* data,UINT8 position)
{
   if (*data)
       *pArray |= 0x01 << position;
   else
       *pArray &= ~(0x01 << position);
   return 0;
}

UINT8 CANThread::ArrayToHex32(UINT8 * pArray, UINT32 * pdata)
{
   *pdata = (pArray[0] << 24) | (pArray[1] << 16) | (pArray[2] << 8) | (pArray[3] << 0);
   return 0;
}


UINT8 CANThread::ArrayToHex32(UINT8 * pArray, INT32 * pdata)
{
   *pdata = (pArray[0] << 24) | (pArray[1] << 16) | (pArray[2] << 8) | (pArray[3] << 0);
   return 0;
}

UINT8 CANThread::ArrayToHex32(UINT8 * pArray, DWORD * pdata)
{
   *pdata = (pArray[0] << 24) | (pArray[1] << 16) | (pArray[2] << 8) | (pArray[3] << 0);
   return 0;
}

UINT8 CANThread::ArrayToHex16(UINT8 * pArray, UINT16 * pdata)
{
   *pdata = (pArray[0] << 8) | (pArray[1] << 0);
   return 0;
}

UINT8 CANThread::ArrayToHex16(UINT8 * pArray, INT16 * pdata)
{
   *pdata = (pArray[0] << 8) | (pArray[1] << 0);
   return 0;
}

UINT8 CANThread::ArrayToHex8(UINT8 * pArray, UINT8 * pdata)
{
   *pdata = (pArray[0] << 0);
   return 0;
}

UINT8 CANThread::ArrayToHex8(UINT8 * pArray, INT8 * pdata)
{
   *pdata = (pArray[0] << 0);
   return 0;
}

UINT8 CANThread::ArrayToBit(UINT8* pArray, UINT8* pdata, UINT8 position)
{
   *pdata = (pArray[0] & (0x01<<position))>>position;
   return 0;
}

QString CANThread::uncharToQstring(unsigned char * id,int len)
{
   QString temp,msg;
   int j = 0;

   while (j<len)
   {
       temp = QString("%1").arg((int)id[j], 2, 16, QLatin1Char('0'));
       msg.append(temp);
       j++;
   }

   return msg;
}


//CRC16校验
WORD CANThread::CRC16(BYTE * pByteArray, BYTE Length)
{
    BYTE uchCRCHi = 0xFF; /* CRC 的高字节初始化*/
    BYTE uchCRCLo = 0xFF; /* CRC 的低字节初始化*/
    BYTE uIndex; /* CRC 查询表索引*/
    BYTE i = 0;
    while (Length--) /* 完成整个报文缓冲区*/
    {
        uIndex = uchCRCLo ^ (pByteArray[i++]);// uchCRCLo ^ (*(pByteArray++)); /* 计算CRC */
        uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
        uchCRCHi = auchCRCLo[uIndex];
    }
    return (uchCRCHi << 8 | uchCRCLo);

}

const BYTE CANThread::auchCRCHi[256] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
    0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
    0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81,
    0x40
};

/* 低位字节的CRC 值*/
const BYTE CANThread::auchCRCLo[256] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB,
    0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE,
    0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2,
    0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E,
    0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B,
    0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,
    0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD,
    0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8,
    0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4,
    0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94,
    0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59,
    0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D,
    0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80,
    0x40
};
