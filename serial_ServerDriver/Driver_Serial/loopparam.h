#ifndef LOOPPARAM_H
#define LOOPPARAM_H

#include <QWidget>
#include <waveformwidget.h>
#include <canthread.h>

//类型定义
typedef	union UN_Param
{
    DWORD param_int;
    FLOAT param_float;
}unParam;

namespace Ui {
class LoopParam;
}

class LoopParam : public QWidget
{
    Q_OBJECT

public:
    explicit LoopParam(CANThread* c,QWidget *parent = nullptr);
    ~LoopParam();
    Ui::LoopParam *ui;
    WaveformWidget *waveformwidget;
    ///波形图显示
    void addCurrentPlotData();
    void addVelocityPlotData();
    void addPositionPlotData();
    ///电机使能
    bool isMotorEnabled;
    ///主界面实时显示
    bool isRealShow;
    ///界面打开方式
    uint8_t loopOpenMode;

    ///输入框电流环P值
    float edit_currentLoopP;
    float edit_currentLoopI;
    float edit_currentLoopD;
    float edit_currentamplitude;

    float edit_velocityLoopP;
    float edit_velocityLoopI;
    float edit_velocityLoopD;
    int edit_speed;

    float edit_positionLoopP;
    float edit_positionLoopI;
    float edit_positionLoopD;
    int edit_acceleration;
    int edit_deceleration;
    float edit_targetAngel;
    int edit_maxVelocity;

    QMessageBox *m_box;
    QPushButton *cancel;
    QPushButton *close;

signals:
    ///电流环测试
    void currentTest();
    void cancelCurrentTest();
    void velocityTest();
    void cancelVelocityTest();
    void positionTest();
    void cancelPositionTest();

private slots:
    ///按键 保存or下一步
    void on_nextStepBtn_clicked();

    ///按键 电流环测试
    void on_currentLoopTestBtn_clicked();

    void on_speedLoopTestBtn_clicked();

    void on_positionLoopTestBtn_clicked();

    void on_clearWaveBtn_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:

    CANThread *canthread;
    void init();
    /// 读取电流环PID和测试值
    void getCurrentLoop();
    /// 读取速度环PID和测试值
    void getVelocityLoop();
    /// 读取位置环PID和测试值
    void getPositionLoop();
    ///写入电流环PID值
    UINT8 setCurrentLoop();
    ///写入速度环PID值
    UINT8 setVelocityLoop();
    ///写入位置环PID值
    UINT8 setPositionLoop();
    ///写入电流环测试值
    UINT8 setCurrentLoopTest();
    ///写入速度环测试值
    UINT8 setVelocityLoopTest();
    ///写入位置环测试值
    UINT8 setPositionLoopTest();
};

#endif // LOOPPARAM_H
