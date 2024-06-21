#ifndef TUNNNINGDATAPROTOCOLSENDHELPER_H
#define TUNNNINGDATAPROTOCOLSENDHELPER_H

#include <QObject>

class TunnningDataProtocolSendHelper : public QObject
{
private:
    Q_OBJECT

public:
    explicit TunnningDataProtocolSendHelper(QObject *parent = nullptr);
    ~TunnningDataProtocolSendHelper();

    static void writePumpOffsetRead(uchar pumpChannel);
    static void writePumpOffsetWrite(uchar pumpChannel, ushort value);
    static void writePumpUseTimeRead(uchar pumpChannel);
    static void writePumpUseTimeWrite(uchar pumpChannel, ushort value);

    static void writeXAxisBathPosRead();
    static void writeXAxisBathPosWrite(int value);
    static void writeXAxisAspPosRead();
    static void writeXAxisAspPosWrite(int value);
    static void writeXAxisDspPosRead();
    static void writeXAxisDspPosWrite(int value);
    static void writeShakeAspPosRead();
    static void writeShakeAspPosWrite(int value);
    static void writeShakeDspPosRead();
    static void writeShakeDspPosWrite(int value);
    static void writeShakeDryPosRead();
    static void writeShakeDryPosWrite(int value);

    static void writeShakeCameraPosRead();
    static void writeShakeCameraPosWrite(int value);
    static void writeShakeCameraPosMove(int value);
    static void writeShakeCameraPosMove(QString class_name);

    static void writeShakeSampleDspRead();
    static void writeShakeSampleDspWrite(int value);
    static void writeShakeHomeRead();
    static void writeShakeHomeWrite(int value);
    static void writeAspPinBathRead();
    static void writeAspPinBathWrite(int value);
    static void writeAspPinTrayRead();
    static void writeAspPinTrayWrite(int value);
    static void writeShakeSpeedSpeedRead();
    static void writeShakeSpeedSpeedWrite(int value);
    static void writeShakeSpeedPeriodRead();
    static void writeShakeSpeedPeriodWrite(int value);

    static void writeLEDRead();
    static void writeLEDWrite(uchar ch1, uchar ch2, uchar ch3, uchar ch4);
    static void writeXPosRead();
    static void writeXPosWrite(int value);
    static void writeXEndPosRead();
    static void writeXEndPosWrite(int value);
    static void writeCameraHomePosRead();
    static void writeCameraHomePosWrite(int value);
    static void writeCameraWidthPosRead();
    static void writeCameraWidthPosWrite(int value);
    static void writeQRTrayPosRead();
    static void writeQRTrayPosWrite(int value);

    static void writeLEDOn();
    static void writeLEDOff();

    //##Additional Protocol
    static void writeHeatingPadTempCh1Read();
    static void writeHeatingPadTempCh1Write(int value);

    static void writeHeatingPadTempCh2Read();
    static void writeHeatingPadTempCh2Write(int value);

public slots:
    static void onQueuedSend(const QByteArray &sendData);
};

#endif // TUNNNINGDATAPROTOCOLSENDHELPER_H
