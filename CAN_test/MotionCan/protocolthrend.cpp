#include "protocolthrend.h"
#include <QThread>
#include <iostream>
#include <windows.h>
using namespace std;

ProtocolThrend::ProtocolThrend()
{

}

void ProtocolThrend::protocolHand(VCI_CAN_OBJ *vci)
{
    qDebug()<<"from thread slot:" <<QThread::currentThreadId();
    qDebug()<<"protocol :"<<vci[0].ID;
}

