#include "TunnningDataProtocolSendHelper.h"

#include "Common/Io/INIFileControls/TunningIniDataControls.h"
#include "Network/PacketWriter/WriteProtocolDispAspPage.h"
#include "Network/PacketWriter/WriteProtocolDIWaterPage.h"
#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/WriteProtocolBarcodePage.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"

#include "Network/SerialCtrl.h"
#include <Analysis/ConfigInformation.h>
#include "Common/CLogWriter.h"

USING_SUGENTECH

TunnningDataProtocolSendHelper::TunnningDataProtocolSendHelper(QObject *parent) :
                                                                                  QObject(parent)
{

}

TunnningDataProtocolSendHelper::~TunnningDataProtocolSendHelper()
{

}
/*
QString protocolStr = WriteProtocolTraySetting::writeTrayHommingSet(position);
QByteArray sendData = protocolStr.toUtf8();
TunnningDataProtocolSendHelper::onQueuedSend(sendData);

TunningIniDataControls controls;
controls.writeTrayHommingData(position);
*/
    void TunnningDataProtocolSendHelper::writePumpOffsetRead(uchar pumpChannel)
{
    auto protocolStr = WriteProtocolDIWaterPage::writePumpOffsetRead(pumpChannel);
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writePumpOffsetWrite(uchar pumpChannel, ushort value)
{
    QString protocolStr = WriteProtocolDIWaterPage::writePumpOffsetSet(pumpChannel, value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writePumpOffsetData(pumpChannel, value);
}

void TunnningDataProtocolSendHelper::writePumpUseTimeRead(uchar pumpChannel)
{
    auto protocolStr = WriteProtocolDIWaterPage::writePumpUseTimeRead(pumpChannel);
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writePumpUseTimeWrite(uchar pumpChannel, ushort value)
{
    QString protocolStr = WriteProtocolDIWaterPage::writePumpUseTimeSet(pumpChannel, value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writePumpUseTimeData(pumpChannel, value);
}

void TunnningDataProtocolSendHelper::writeXAxisBathPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeXAxisBathPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeXAxisBathPosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeXAxisBathPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeXAxisBathPosData(value);
}

void TunnningDataProtocolSendHelper::writeXAxisAspPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeXAxisAspPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeXAxisAspPosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeXAxisAspPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeXAxisAspPosData(value);
}

void TunnningDataProtocolSendHelper::writeXAxisDspPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeXAxisDspPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeXAxisDspPosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeXAxisDspPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeXAxisDspPosData(value);
}

void TunnningDataProtocolSendHelper::writeShakeAspPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeAspPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeAspPosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeAspPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeAspPosData(value);
}

void TunnningDataProtocolSendHelper::writeShakeDspPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeDspPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeDspPosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeDspPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeDspPosData(value);
}

void TunnningDataProtocolSendHelper::writeShakeDryPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeDryPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeDryPosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeDryPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeDryPosData(value);
}

void TunnningDataProtocolSendHelper::writeShakeCameraPosRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeCameraPosRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeCameraPosWrite(int value)
{
    writeShakeCameraPosMove(value);

    TunningIniDataControls controls;
    controls.writeShakeCameraPosData(value);
}

void TunnningDataProtocolSendHelper::writeShakeCameraPosMove(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeCameraPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeCameraPosMove(QString class_name)
{
    auto contents = ConfigInformation::getInstance()->getCurrentContents();
    TunningIniDataControls controls;
    auto value = controls.readShakeCameraPosData();
    CLogWriter::printLog(QString("[%1::] : writeShakeCameraPosMove(value = %2)").arg(class_name).arg(value));
    writeShakeCameraPosMove(value);
}

void TunnningDataProtocolSendHelper::writeShakeSampleDspRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeSampleDspRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeSampleDspWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeSampleDspSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeSampleDspData(value);
}

void TunnningDataProtocolSendHelper::writeShakeHomeRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeHomeRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeHomeWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeHomeSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeHomeData(value);
}

void TunnningDataProtocolSendHelper::writeAspPinBathRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeAspPinBathRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeAspPinBathWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeAspPinBathSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeAspPinBathData(value);
}

void TunnningDataProtocolSendHelper::writeAspPinTrayRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeAspPinTrayRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeAspPinTrayWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeAspPinTraySet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeAspPinTrayData(value);
}

void TunnningDataProtocolSendHelper::writeShakeSpeedSpeedRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeSpeedSpeedRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeSpeedSpeedWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeSpeedSpeedSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeSpeedSpeedData(value);
}

void TunnningDataProtocolSendHelper::writeShakeSpeedPeriodRead()
{
    auto protocolStr = WriteProtocolDispAspPage::writeShakeSpeedPeriodRead();
    auto sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeShakeSpeedPeriodWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeShakeSpeedPeriodSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeShakeSpeedPeriodData(value);
}

void TunnningDataProtocolSendHelper::writeLEDRead()
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeLEDParamReadReq();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeLEDWrite(uchar ch1, uchar ch2, uchar ch3, uchar ch4)
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeLEDParamWriteReq(ch1, ch2, ch3, ch4);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeLEDParamData(ch1, ch2, ch3, ch4);
}

void TunnningDataProtocolSendHelper::writeXPosRead()
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeAnlyXPosReadReq();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeXPosWrite(int value)
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeAnlyXPosSetReq(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeCameraPositionStartData(value);
}

void TunnningDataProtocolSendHelper::writeXEndPosRead()
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeAnlyXEndPosReadReq();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeXEndPosWrite(int value)
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeAnlyXEndPosSetReq(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeCameraPositionEndData(value);
}

void TunnningDataProtocolSendHelper::writeCameraHomePosRead()
{
    QString protocolStr = WriteProtocolDispAspPage::writeXAxisCameraHomePosRead();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeCameraHomePosWrite(int value)
{
    QString protocolStr = WriteProtocolDispAspPage::writeXAxisCameraHomePosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeCameraPositionHomeData(value);
}

void TunnningDataProtocolSendHelper::writeCameraWidthPosRead()
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeStripWidthPosRead();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeCameraWidthPosWrite(int value)
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeStripWidthPosWrite(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeCameraPositionWidthData(value);
}

void TunnningDataProtocolSendHelper::writeQRTrayPosRead()
{
    QString protocolStr = WriteProtocolBarcodePage::writeBarcodeQRTrayPosRead();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeQRTrayPosWrite(int value)
{
    QString protocolStr = WriteProtocolBarcodePage::writeBarcodeQRTrayPosSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeQRTrayPositionData(value);
}

void TunnningDataProtocolSendHelper::writeHeatingPadTempCh1Read()
{
    QString protocolStr = WriteProtocolETCPage::writeHeatingPadTempCh1Read();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeHeatingPadTempCh1Write(int value)
{
    QString protocolStr = WriteProtocolETCPage::writeHeatingPadTempCh1Set(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeHeatingPadTempCh1Data(value);
}

void TunnningDataProtocolSendHelper::writeHeatingPadTempCh2Read()
{
    QString protocolStr = WriteProtocolETCPage::writeHeatingPadTempCh2Read();
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);
}

void TunnningDataProtocolSendHelper::writeHeatingPadTempCh2Write(int value)
{
    QString protocolStr = WriteProtocolETCPage::writeHeatingPadTempCh2Set(value);
    QByteArray sendData = protocolStr.toUtf8();
    TunnningDataProtocolSendHelper::onQueuedSend(sendData);

    TunningIniDataControls controls;
    controls.writeHeatingPadTempCh2Data(value);
}

void TunnningDataProtocolSendHelper::onQueuedSend(const QByteArray& sendData)
{
    auto serial = SerialCtrl::getInstance();

    if(!sendData.isEmpty())
        serial->writeQueuedData(sendData);
}
