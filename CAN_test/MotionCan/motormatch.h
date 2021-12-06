#ifndef MOTORMATCH_H
#define MOTORMATCH_H

#include <QWidget>
#include <canthread.h>
#include<QMessageBox>


namespace Ui {
class MotorMatch;
}

class MotorMatch : public QWidget
{
    Q_OBJECT

public:

    explicit MotorMatch(CANThread* c,QWidget *parent = nullptr);
    ~MotorMatch();
    Ui::MotorMatch *ui;
    bool isMotorEnabled;
    bool isRealShow;
    uint8_t matchStatus;
    QMessageBox *m_box;
    QPushButton *cancel;
    QPushButton *close;

signals:
    void motormatchstart();
    void motormatchcancel();
    void saveMotorParams();

private slots:

    void on_startMatchBtn_clicked();

    void on_nextStepBtn_clicked();

private:

    CANThread *canthread;

    void init();
    UINT8 setMotorPoles();

    uint8_t edit_polePairs;
    uint8_t edit_feekType;
    uint16_t edit_encoderLines;
    float edit_ratedCurrent;
    float edit_stallCurrent;
    uint32_t edit_ratedSpeed;
    uint8_t com_hallOrder;


};

#endif // MOTORMATCH_H
