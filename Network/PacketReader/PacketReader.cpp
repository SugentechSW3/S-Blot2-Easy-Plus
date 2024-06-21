#include "PacketReader.h"

PacketReader::PacketReader()
{

}

uchar PacketReader::modeReader(const QByteArray rawData)
{
    QByteArray data(rawData);

    if(data.size() < SIZE_MD_SIZE)
        return -1;

    return data.mid(0, SIZE_MD_SIZE).toInt(nullptr, 16);
}

uchar PacketReader::commandReader(const QByteArray rawData)
{
    QByteArray data(rawData);

    if(data.size() < SIZE_COMMAND)
        return -1;

    return data.mid(SIZE_MD_SIZE, SIZE_CMD_SIZE).toInt(nullptr, 16);
}

QByteArray PacketReader::dataReader(const QByteArray rawData)
{
    QByteArray data(rawData);

    if(data.size() < SIZE_COMMAND + SIZE_LENGTH + SIZE_DATA)
        return QByteArray();

    return data.mid(SIZE_MD_SIZE + SIZE_CMD_SIZE + SIZE_LENGTH, SIZE_DATA);
}

