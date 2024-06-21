#ifndef PACKETREADER_H
#define PACKETREADER_H

#include <QByteArray>
#include "Network/Protocols/SerialProtocol.h"

class PacketReader
{
public:
    PacketReader();

    static uchar modeReader(const QByteArray rawData);
    static uchar commandReader(const QByteArray rawData);
    static QByteArray dataReader(const QByteArray rawData);
};

#endif // PACKETREADER_H
