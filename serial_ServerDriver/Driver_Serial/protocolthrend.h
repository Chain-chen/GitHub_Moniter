#ifndef PROTOCOLTHREND_H
#define PROTOCOLTHREND_H


#include <QObject>
#include "ControlCAN.h"
#include <QDebug>
#include <windows.h>


class ProtocolThrend:public QObject
{
    Q_OBJECT
public:
    ProtocolThrend();

public slots:
    void protocolHand(VCI_CAN_OBJ *vci);
};

#endif // PROTOCOLTHREND_H
