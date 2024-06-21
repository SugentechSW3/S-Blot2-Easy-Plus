#ifndef WRITEPROTOCOLGENERALSETTING_H
#define WRITEPROTOCOLGENERALSETTING_H

#include <QString>
#include "Network/Protocols/Default/ProtocolPageRead.h"

class WriteProtocolPageRead
{
public:
    WriteProtocolPageRead();

    static QString writePositionPage();
    static QString writeLLDPage();
    static QString writeDispensePage();
    static QString writeCameraPage();
    static QString writeETCPage();

};

#endif // WRITEPROTOCOLGENERALSETTING_H
