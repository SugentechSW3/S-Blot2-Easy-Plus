#ifndef PROTOCOLSTATUSREAD_H
#define PROTOCOLSTATUSREAD_H
#include "Network/Protocols/SerialProtocol.h"


enum CMD_STATUS_READ
{
    CMD_STATUS_READ_DEVICE_CHECK_LEGACY = 0x00,
    CMD_STATUS_READ_EQUIPMENT_SERIAL_NUMBER,
    CMD_STATUS_READ_FIRMWARE_VERSION,
    CMD_STATUS_READ_RESERVED_0X03,
    CMD_STATUS_READ_HARDWARE_VERSION,
    CMD_STATUS_READ_STEP_ID,                    // 0005
    CMD_STATUS_READ_TOTAL_SQ_NUMBER, //deprecated
    CMD_STATUS_READ_SQ_ID, //deprecated
    CMD_STATUS_READ_SEQUENCE_SELECT_R, //deprecated
    CMD_STATUS_READ_TOTAL_STEP_NUMBER,
    CMD_STATUS_READ_CURRENT_TOTAL_TIME,//deprecated
    CMD_STATUS_READ_CURRENT_STEP_TIME,              // 000B
    CMD_STATUS_READ_CURRENT_DEVICE_STATE,
    CMD_STATUS_READ_STEP_TIME,                      // 000D
    CMD_STATUS_READ_CURRENT_STATUS_READ,            // 000E
    CMD_STATUS_END = 0xFF
};


#endif // PROTOCOLSTATUSREAD_H