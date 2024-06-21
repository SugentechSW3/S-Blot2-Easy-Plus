#ifndef PROTOCOL_PROBE_SETTING_MODE_H
#define PROTOCOL_PROBE_SETTING_MODE_H

#include "Network/Protocols/SerialProtocol.h"

enum CMD_PROBE_SETTING_MODE
{
    CMD_PROBE_SETTING_MODE_PAGE =0x00,
    CMD_PROBE_SETTING_MODE_BATH_Y_SET,
    CMD_PROBE_SETTING_MODE_BATH_Y_READ,
    CMD_PROBE_SETTING_MODE_BATH_Z_SET,
    CMD_PROBE_SETTING_MODE_BATH_Z_READ,
    CMD_PROBE_SETTING_MODE_SAMPLE_X_SET,
    CMD_PROBE_SETTING_MODE_SAMPLE_X_READ,
    CMD_PROBE_SETTING_MODE_SAMPLE_X_END_SET,
    CMD_PROBE_SETTING_MODE_SAMPLE_X_END_READ,
    CMD_PROBE_SETTING_MODE_SAMPLE_Y1_SET,
    CMD_PROBE_SETTING_MODE_SAMPLE_Y1_READ,
    CMD_PROBE_SETTING_MODE_SAMPLE_Y2_SET,
    CMD_PROBE_SETTING_MODE_SAMPLE_Y2_READ,
    CMD_PROBE_SETTING_MODE_SAMPLE_Z_SET,
    CMD_PROBE_SETTING_MODE_SAMPLE_Z_READ,
    CMD_PROBE_SETTING_MODE_TRAY_X_SET,
    CMD_PROBE_SETTING_MODE_TRAY_X_READ,
    CMD_PROBE_SETTING_MODE_TRAY_X_END_SET,
    CMD_PROBE_SETTING_MODE_TRAY_X_END_READ,
    CMD_PROBE_SETTING_MODE_TRAY_Y_SET,
    CMD_PROBE_SETTING_MODE_TRAY_Y_READ,
    CMD_PROBE_SETTING_MODE_TRAY_Z_SET,
    CMD_PROBE_SETTING_MODE_TRAY_Z_READ,
    CMD_PROBE_SETTING_MODE_PROBE_BATH_INIT_Y_SET,
    CMD_PROBE_SETTING_MODE_PROBE_BATH_INIT_Y_READ

};

#endif // PROTOCOL_PROBE_SETTING_MODE_H
