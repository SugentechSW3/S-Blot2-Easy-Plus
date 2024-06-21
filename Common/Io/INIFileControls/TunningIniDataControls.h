#ifndef TUNNINGINIDATACONTROLS_H
#define TUNNINGINIDATACONTROLS_H

#include <QObject>

class TunningIniDataControls : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit TunningIniDataControls(QObject *parent = nullptr);
    ~TunningIniDataControls();

    QString getCurrentDirPath();

    QString readSelectedContents();
    void writeSelectedContents(const QString &selectedContetns);

    //#####Pump#####
    void writePumpOffsetData(uchar pumpChannel, int data);
    void writePumpUseTimeData(uchar pumpChannel, int data);

    //#####Moter#####
    void writeXAxisBathPosData(int data);
    void writeXAxisAspPosData(int data);
    void writeXAxisDspPosData(int data);
    void writeShakeAspPosData(int data);
    void writeShakeDspPosData(int data);
    void writeShakeDryPosData(int data);
    void writeShakeCameraPosData(int data);
    int readShakeCameraPosData();
    void writeShakeSampleDspData(int data);
    void writeShakeHomeData(int data);
    void writeAspPinBathData(int data);
    void writeAspPinTrayData(int data);
    void writeShakeSpeedSpeedData(int data);
    void writeShakeSpeedPeriodData(int data);

    //#####Another#####
    void writeLEDParamData(uchar ch1, uchar ch2, uchar ch3, uchar ch4);

    void writeCameraPositionStartData(int data);
    void writeCameraPositionEndData(int data);
    void writeCameraPositionHomeData(int data);
    void writeCameraPositionWidthData(int data);

    void writeQRTrayPositionData(int data);

    //#####AddtionalProtocols#####
    void writeHeatingPadTempCh1Data(int value);
    void writeHeatingPadTempCh2Data(int value);

private:
    void initIo();

private:
    QScopedPointer<privateStruct> d;
};

#endif // TUNNINGINIDATACONTROLS_H
