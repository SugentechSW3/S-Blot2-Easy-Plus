#ifndef BATCHUPANDDOWNLOADFORTUNNINGDATA_H
#define BATCHUPANDDOWNLOADFORTUNNINGDATA_H

#include <QObject>

class BatchUpAndDownLoadForTunningData : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit BatchUpAndDownLoadForTunningData(QObject *parent = nullptr);
    ~BatchUpAndDownLoadForTunningData();

    static QStringList getSupportedDirList();

    void downLoadAlldata();

signals:
    void onExchangeSuccess();

private:
    void initVariables();
    void initSignalSlots();

    void readAllergyDatas();
    void writeAllergyDatas();

    void readAllANADatas();
    void writeAllANADatas();

    void readAllTunningDatas();

private slots:
    void onRecvData(QByteArray recvData);
    void onRecvPumpData(QByteArray& recvData);
    void onRecvMotorData(QByteArray& recvData);
    void onRecvGeneralSettingData(QByteArray& recvData);
    void onRecvQRSettingMode(QByteArray& recvData);
    void onRecvCameraPositionSetting(QByteArray& recvData);
    void onRecvQCEtc(QByteArray& recvData);

private:
    QScopedPointer<privateStruct> d;

};

#endif // BATCHUPANDDOWNLOADFORTUNNINGDATA_H
