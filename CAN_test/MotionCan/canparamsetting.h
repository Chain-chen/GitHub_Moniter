#ifndef CANPARAMSETTING_H
#define CANPARAMSETTING_H

#include <QDialog>

namespace Ui {
class CANParamSetting;
}

class CANParamSetting : public QDialog
{
    Q_OBJECT

public:
    explicit CANParamSetting(QWidget *parent = 0);
    ~CANParamSetting();
    int CANtype;//CAN卡类型
    int index;//索引号
    int baundRate;//波特率
    int devicCOM;//端口号

private slots:
    //保存CAN参数设置
    void saveCanParam();

    void on_quitBtn_clicked();

private:
    Ui::CANParamSetting *ui;
};

#endif // CANPARAMSETTING_H
