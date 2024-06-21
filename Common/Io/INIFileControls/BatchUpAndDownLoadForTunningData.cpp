#include "BatchUpAndDownLoadForTunningData.h"

#include "Analysis/ConfigInformation.h"

#include "AllergyIniDataControls.h"
#include "TunningIniDataControls.h"

#include "Network/SerialCtrl.h"
#include "Network/Protocols/Legacy/ProtocolPumpSettingMode.h"
#include "Network/Protocols/Legacy/ProtocolMotorSettingMode.h"
#include "Network/Protocols/CameraLED/ProtocolCameraLedSetting.h"
#include "Network/Protocols/Default/ProtocolGeneralSettingMode.h"
#include "Network/Protocols/Position/ProtocolBarcodeSettingMode.h"
#include "Network/Protocols/Legacy/ProtocolQCMode.h"
#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"

#include "Common/CLogWriter.h"

USING_SUGENTECH

struct BatchUpAndDownLoadForTunningData::privateStruct
{
    ConfigInformation* mConfigure = nullptr;
    SerialCtrl* mSerial = nullptr;
    int mChannelCount = 0;
};

BatchUpAndDownLoadForTunningData::BatchUpAndDownLoadForTunningData(QObject *parent) :
                                                                                      QObject(parent),
                                                                                      d(new privateStruct)
{
    this->initVariables();
    this->initSignalSlots();
}

BatchUpAndDownLoadForTunningData::~BatchUpAndDownLoadForTunningData()
{

}

void BatchUpAndDownLoadForTunningData::initVariables()
{
    d->mConfigure = ConfigInformation::getInstance();
    d->mSerial = SerialCtrl::getInstance();
}

void BatchUpAndDownLoadForTunningData::initSignalSlots()
{
    connect(d->mConfigure, &ConfigInformation::endConfigProtocol, this, [this]
            (bool)
            {
                d->mChannelCount = 0;
                emit onExchangeSuccess();
            });

    connect(d->mSerial, &SerialCtrl::onRecvDataForSerial, this, &BatchUpAndDownLoadForTunningData::onRecvData);
}

QStringList BatchUpAndDownLoadForTunningData::getSupportedDirList()
{
    TunningIniDataControls tunning;

    auto config = ConfigInformation::getInstance();
    auto list = config->getContentsList();

    list.push_back(tunning.getCurrentDirPath());

    return list;
}

void BatchUpAndDownLoadForTunningData::downLoadAlldata()
{

    auto config = ConfigInformation::getInstance();
    auto currentContents = config->getCurrentContents();
    auto contentsNameAllergy = ConfigInformation::getContentsNameFromEnum((int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY);
    QString contentsNameANA = "ana";

    if(contentsNameAllergy == currentContents)
        this->readAllergyDatas();

    else if(contentsNameANA == currentContents)
        this->readAllANADatas();

    this->readAllTunningDatas();

    d->mConfigure->sendDataEnd();
}

void BatchUpAndDownLoadForTunningData::readAllergyDatas()
{
    auto config = ConfigInformation::getInstance();
    auto contentsName = ConfigInformation::getContentsNameFromEnum((int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY);

    config->saveContentsTunningINIData(contentsName);
}

void BatchUpAndDownLoadForTunningData::writeAllergyDatas()
{

}

void BatchUpAndDownLoadForTunningData::readAllANADatas()
{

}

void BatchUpAndDownLoadForTunningData::writeAllANADatas()
{

}

void BatchUpAndDownLoadForTunningData::readAllTunningDatas()
{
    for(uchar i = 0; i < 9 ; i++)
    {
        TunnningDataProtocolSendHelper::writePumpOffsetRead(i);
        TunnningDataProtocolSendHelper::writePumpUseTimeRead(i);
    }

    TunnningDataProtocolSendHelper::writeXAxisBathPosRead();
    TunnningDataProtocolSendHelper::writeXAxisAspPosRead();
    TunnningDataProtocolSendHelper::writeXAxisDspPosRead();
    TunnningDataProtocolSendHelper::writeShakeAspPosRead();
    TunnningDataProtocolSendHelper::writeShakeDspPosRead();
    TunnningDataProtocolSendHelper::writeShakeDryPosRead();
    TunnningDataProtocolSendHelper::writeShakeCameraPosRead();
    TunnningDataProtocolSendHelper::writeShakeSampleDspRead();
    TunnningDataProtocolSendHelper::writeShakeHomeRead();
    TunnningDataProtocolSendHelper::writeAspPinBathRead();
    TunnningDataProtocolSendHelper::writeAspPinTrayRead();
    TunnningDataProtocolSendHelper::writeShakeSpeedSpeedRead();
    TunnningDataProtocolSendHelper::writeShakeSpeedPeriodRead();

    TunnningDataProtocolSendHelper::writeLEDRead();
    TunnningDataProtocolSendHelper::writeXPosRead();
    TunnningDataProtocolSendHelper::writeXEndPosRead();
    TunnningDataProtocolSendHelper::writeCameraHomePosRead();
    TunnningDataProtocolSendHelper::writeCameraWidthPosRead();
    TunnningDataProtocolSendHelper::writeQRTrayPosRead();
    TunnningDataProtocolSendHelper::writeHeatingPadTempCh1Read();
    TunnningDataProtocolSendHelper::writeHeatingPadTempCh2Read();
}

void BatchUpAndDownLoadForTunningData::onRecvData(QByteArray recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_GENERAL_SETTING:
        this->onRecvGeneralSettingData(recvData);
        break;

    case MD_MODE::MD_MODE_MOTOR_SETTING_MODE:
        this->onRecvMotorData(recvData);
        this->onRecvQRSettingMode(recvData);
        this->onRecvCameraPositionSetting(recvData);
        break;

    case MD_MODE::MD_MODE_PUMP_SETTING_MODE:
        this->onRecvPumpData(recvData);
        break;

    case MD_MODE::MD_MODE_QC_MODE:
        this->onRecvQCEtc(recvData);
        break;
    }
}

void BatchUpAndDownLoadForTunningData::onRecvPumpData(QByteArray& recvData)
{
    TunningIniDataControls iniControls;
    auto value = PacketReader::dataReader(recvData).right(4);
    int data = value.toUInt(nullptr, 16);

    switch(PacketReader::commandReader(recvData))
    {
    case CMD_PROTOCOL_PUMP_SETTING_MODE::CMD_PROTOCOL_PUMP_SETTING_MODE_OFFSET_SET :
        iniControls.writePumpOffsetData(d->mChannelCount, data);
        break;

    case CMD_PROTOCOL_PUMP_SETTING_MODE::CMD_PROTOCOL_PUMP_SETTING_MODE_USE_TIME_READ :
        iniControls.writePumpUseTimeData(d->mChannelCount, data);
        d->mChannelCount++;
        break;

    default:
        CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : UNKNOWN"), CLogWriter::LOG_TYPE_RELEASE);
        break;
    }

    CLogWriter::printLog(QString("------------------ NEXT ------------------"));
}

void BatchUpAndDownLoadForTunningData::onRecvMotorData(QByteArray& recvData)
{
    TunningIniDataControls iniControls;
    auto value = PacketReader::dataReader(recvData);
    int data = value.toUInt(nullptr, 16);

    switch(PacketReader::commandReader(recvData))
    {

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_BATH_X_POS_READ:
        iniControls.writeXAxisBathPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_ASP_X_START_READ:
        iniControls.writeXAxisAspPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_DISP_X_START_READ:
        iniControls.writeXAxisDspPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_ASP_POS_READ:
        iniControls.writeShakeAspPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_DSP_POS_READ:
        iniControls.writeShakeDspPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_DRY_POS_READ:
        iniControls.writeShakeDryPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_CAMERA_POS_READ:
        iniControls.writeShakeCameraPosData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_SAMPLE_DSP_POS_READ:
        iniControls.writeShakeSampleDspData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_HOME_POS_READ:
        iniControls.writeShakeHomeData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_BATH_ASP_PIN_HEIGHT_READ:
        iniControls.writeAspPinBathData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_ASP_PIN_HEIGHT_READ:
        iniControls.writeAspPinTrayData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_READ:
        iniControls.writeShakeSpeedSpeedData(data);
        break;

    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_PERIOD_READ:
        iniControls.writeShakeSpeedPeriodData(data);
        break;

    default:
        CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : UNKNOWN"), CLogWriter::LOG_TYPE_RELEASE);
        break;
    }

    CLogWriter::printLog(QString("------------------ NEXT ------------------"));
}

void BatchUpAndDownLoadForTunningData::onRecvGeneralSettingData(QByteArray& recvData)
{
    TunningIniDataControls iniControls;
    auto value = PacketReader::dataReader(recvData);
    int data = value.toUInt(nullptr, 16);

    switch(PacketReader::commandReader(recvData))
    {
    case CMD_GENERAL_SETTING_MODE::CMD_GENERAL_SETTING_MODE_LED_LIGHT_POWER_READ:
    {
        auto led1 = value.left(2).toUInt(nullptr, 16);
        auto led2 = value.mid(2,2).toUInt(nullptr, 16);
        auto led3 = value.mid(4,2).toUInt(nullptr, 16);
        auto led4 = value.right(2).toUInt(nullptr, 16);

        iniControls.writeLEDParamData(led1, led2, led3, led4);
        CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : CMD_CAMERA_LED_SETTING_LED_PARAM_READ_RESP"));
    }break;

    default:
        CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : UNKNOWN"), CLogWriter::LOG_TYPE_RELEASE);
        break;
    }

    CLogWriter::printLog(QString("------------------ NEXT ------------------"));
}

void BatchUpAndDownLoadForTunningData::onRecvQRSettingMode(QByteArray& recvData)
{
    TunningIniDataControls iniControls;
    auto value = PacketReader::dataReader(recvData);
    int data = value.toUInt(nullptr, 16);

    switch(PacketReader::commandReader(recvData))
    {
        case CMD_QR_SETTING_MODE::CMD_SETTING_MODE_QR_TRAY_POS_READ:
            iniControls.writeQRTrayPositionData(data);
            CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : CMD_SETTING_MODE_QR_TRAY_POS_READ"));
            break;

        default:
            CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : UNKNOWN"), CLogWriter::LOG_TYPE_RELEASE);
            break;
    }

    CLogWriter::printLog(QString("------------------ NEXT ------------------"));
}

void BatchUpAndDownLoadForTunningData::onRecvCameraPositionSetting(QByteArray& recvData)
{
    TunningIniDataControls iniControls;
    auto value = PacketReader::dataReader(recvData);
    int data = value.toUInt(nullptr, 16);

    switch(PacketReader::commandReader(recvData))
    {
        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_READ:
            iniControls.writeCameraPositionStartData(data);
            CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_READ"));
            break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_CAMERA_END_XPOS_READ:
            iniControls.writeCameraPositionEndData(data);
            CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : CMD_MOTOR_SETTING_MODE_CAMERA_END_XPOS_READ"));
            break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_CAM_HOME_POSITION_READ:
            iniControls.writeCameraPositionHomeData(data);
            CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : CMD_MOTOR_SETTING_MODE_CAM_HOME_POSITION_READ"));
            break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_READ:
            iniControls.writeCameraPositionWidthData(data);
            CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::) : CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_READ"));
            break;
    }

    CLogWriter::printLog(QString("------------------ NEXT ------------------"));
}

void BatchUpAndDownLoadForTunningData::onRecvQCEtc(QByteArray& recvData)
{
    TunningIniDataControls iniControls;
    auto value = PacketReader::dataReader(recvData);
    int data = value.toUInt(nullptr, 16);

    switch(PacketReader::commandReader(recvData))
    {

    case CMD_PROTOCOL_QC_MODE::CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH1_READ:
    {
        iniControls.writeHeatingPadTempCh1Data(data);
        CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::)Recv : CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH1_READ"));
    }break;

    case CMD_PROTOCOL_QC_MODE::CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH2_READ:
    {
        iniControls.writeHeatingPadTempCh2Data(data);
        CLogWriter::printLog(QString("(BatchUpAndDownLoadForTunningData::)Recv : CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH2_READ"));
    }break;

    }

    CLogWriter::printLog(QString("------------------ NEXT ------------------"));
}
