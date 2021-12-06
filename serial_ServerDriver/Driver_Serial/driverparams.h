#ifndef DRIVERPARAMS_H
#define DRIVERPARAMS_H

#include <QWidget>
#include <canthread.h>

namespace Ui {
class DriverParams;
}

class DriverParams : public QWidget
{
    Q_OBJECT

public:
    explicit DriverParams(CANThread* c,QWidget *parent = nullptr);
    ~DriverParams();
    Ui::DriverParams *ui;
    ///电机使能
    bool isMotorEnabled;
    ///主界面实时显示
    bool isRealShow;
    ///界面打开方式
    uint8_t loopOpenMode;

private slots:
    void on_nextBtn_clicked();
signals:
    void setDriverParam();
private:

    CANThread *canthread;
    ///初始化函数
    void init();

    UINT8 setApplication();

    float edit_appRatedCurrent;
    float edit_appStallCurrent;
    int edit_appMaxSpeed;
    int edit_acceleration;
    int edit_deceleration;
    int edit_stopDeceleration;
    float edit_maxPosition;
    float edit_minPosition;
    int edit_initialPostion;

    float edit_driverRatedCurrent;
    float edit_driverStallCurrent;
    int edit_driverMaxSpeed;
    int edit_driverMinSpeed;
    float edit_driverMaxVoltage;
    float edit_driverMinVoltage;

};

#endif // DRIVERPARAMS_H
