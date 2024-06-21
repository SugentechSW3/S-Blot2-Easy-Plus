#include "TunningIniDataControls.h"

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"

USING_SUGENTECH

struct TunningIniDataControls::privateStruct
{
    QScopedPointer<CSettingIo> mTunningGeneralDataIO;
    QScopedPointer<CSettingIo> mTunningPumpDataIO;
    QScopedPointer<CSettingIo> mTunningMotorDataIO;
    QScopedPointer<CSettingIo> mTunningAnotherDataIO;

    const QString mTunningGeneralIniFileName = "TUNNING_GENERAL_DATA";
    const QString mTunningPumpIniFileName = "TUNNING_PUMP_DATA";
    const QString mTunningMotorIniFileName = "TUNNING_MOTOR_DATA";
    const QString mTunningAnotherIniFileName = "TUNNING_ANOTHER_DATA";

    const QString mDirPath = "TUNNING_DATA";
};

TunningIniDataControls::TunningIniDataControls(QObject *parent) :
                                                                  QObject(parent),
                                                                  d(new privateStruct)
{
    this->initIo();
}

TunningIniDataControls::~TunningIniDataControls()
{

}

void TunningIniDataControls::initIo()
{
    d->mTunningGeneralDataIO.reset(new CSettingIo(d->mTunningGeneralIniFileName, d->mDirPath));
    d->mTunningPumpDataIO.reset(new CSettingIo(d->mTunningPumpIniFileName, d->mDirPath));
    d->mTunningMotorDataIO.reset(new CSettingIo(d->mTunningMotorIniFileName, d->mDirPath));
    d->mTunningAnotherDataIO.reset(new CSettingIo(d->mTunningAnotherIniFileName, d->mDirPath));
}

QString TunningIniDataControls::getCurrentDirPath()
{
    return d->mDirPath;
}

QString TunningIniDataControls::readSelectedContents()
{
    return d->mTunningGeneralDataIO->getConfigureData("CONTENTS", "00_SELECTED_CONTENTS").toString();
}

void TunningIniDataControls::writeSelectedContents(const QString& selectedContetns)
{
    d->mTunningGeneralDataIO->setConfigureData("CONTENTS", "00_SELECTED_CONTENTS", selectedContetns);
}

//#####Pump#####
void TunningIniDataControls::writePumpOffsetData(uchar pumpChannel, int data)
{
    QString channelName = "PUMP_OFFSET_CHANNEL";

    if(pumpChannel == 0)
        channelName.append("_ALL");
    else
        channelName.append(QString("_%1").arg(pumpChannel));

    d->mTunningPumpDataIO->setConfigureData("PUMP_OFFSET_DATA", channelName, data);
}

void TunningIniDataControls::writePumpUseTimeData(uchar pumpChannel, int data)
{
    QString channelName = "PUMP_USE_TIME_CHANNEL";

    if(pumpChannel == 0)
        channelName.append("_ALL");
    else
        channelName.append(QString("_%1").arg(pumpChannel));

    d->mTunningPumpDataIO->setConfigureData("PUMP_USE_TIME_DATA", channelName, data);
}

//#####Moter#####
void TunningIniDataControls::writeXAxisBathPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_X_AXIS_POSITION", "01_X_AXIS_BATH_POSITION", data);
}

void TunningIniDataControls::writeXAxisAspPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_X_AXIS_POSITION", "02_X_AXIS_ASP_POSITION", data);
}

void TunningIniDataControls::writeXAxisDspPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_X_AXIS_POSITION", "03_X_AXIS_DISP_POSITION", data);
}

void TunningIniDataControls::writeShakeAspPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_POSITION", "04_SHAKE_ASP_POSITION", data);
}

void TunningIniDataControls::writeShakeDspPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_POSITION", "05_SHAKE_DISP_POSITION", data);
}

void TunningIniDataControls::writeShakeDryPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_POSITION", "06_SHAKE_DRY_POSITION", data);
}

void TunningIniDataControls::writeShakeCameraPosData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_POSITION", "07_SHAKE_CAMERA_POSITION", data);
}

int TunningIniDataControls::readShakeCameraPosData()
{
    return d->mTunningMotorDataIO->getConfigureData("MOTOR_SHAKE_POSITION", "07_SHAKE_CAMERA_POSITION").toInt();
}

void TunningIniDataControls::writeShakeSampleDspData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_POSITION", "08_SHAKE_SAMPLE_DISP_POSITION", data);
}

void TunningIniDataControls::writeShakeHomeData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_POSITION", "09_SHAKE_HOME_POSITION", data);
}

void TunningIniDataControls::writeAspPinBathData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_ASP_PIN", "10_ASP_PIN_BATH", data);
}

void TunningIniDataControls::writeAspPinTrayData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_ASP_PIN", "11_ASP_PIN_TRAY", data);
}

void TunningIniDataControls::writeShakeSpeedSpeedData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_SPEED", "12_SHAKE_SPEED", data);
}

void TunningIniDataControls::writeShakeSpeedPeriodData(int data)
{
    d->mTunningMotorDataIO->setConfigureData("MOTOR_SHAKE_SPEED", "13_SHAKE_PERIOD", data);
}

//#####General Setting Camera#####
void TunningIniDataControls::writeLEDParamData(uchar ch1, uchar ch2, uchar ch3, uchar ch4)
{
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_LED", "14_LED_CHANNEL_1", ch1);
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_LED", "15_LED_CHANNEL_2", ch2);
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_LED", "16_LED_CHANNEL_3", ch3);
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_LED", "17_LED_CHANNEL_4", ch4);
}

void TunningIniDataControls::writeCameraPositionStartData(int data)
{
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_POSITION", "18_CAMERA_POSITION_START", data);
}

void TunningIniDataControls::writeCameraPositionEndData(int data)
{
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_POSITION", "19_CAMERA_POSITION_END", data);
}

void TunningIniDataControls::writeCameraPositionHomeData(int data)
{
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_POSITION", "20_CAMERA_POSITION_HOME", data);
}

void TunningIniDataControls::writeCameraPositionWidthData(int data)
{
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_POSITION", "21_CAMERA_POSITION_WIDTH", data);
}

void TunningIniDataControls::writeQRTrayPositionData(int data)
{
    d->mTunningAnotherDataIO->setConfigureData("ANOTHER_CAMERA_SETTINGS_QR", "22_QR_TRAY_POSITION", data);
}

//#####AddtionalProtocols#####
void TunningIniDataControls::writeHeatingPadTempCh1Data(int value)
{
    d->mTunningAnotherDataIO->setConfigureData("HEATING_PAD_TEMP", "23_HEATINGPAD_TEMP_CH1", value);
}

void TunningIniDataControls::writeHeatingPadTempCh2Data(int value)
{
    d->mTunningAnotherDataIO->setConfigureData("HEATING_PAD_TEMP", "24_HEATINGPAD_TEMP_CH2", value);
}
