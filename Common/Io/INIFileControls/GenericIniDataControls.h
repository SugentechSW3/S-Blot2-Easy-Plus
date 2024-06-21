#ifndef GENERICINIDATACONTROLS_H
#define GENERICINIDATACONTROLS_H

#include <QObject>

#include "Analysis/GlobalDataStruct.h"

class GenericIniDataControls : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    struct FIXED_USER_DATA
    {
        QString applicant;
        QString analyst;
        QString reviewer;
    };

    explicit GenericIniDataControls(QObject *parent = nullptr);
    ~GenericIniDataControls();

    QString getCurrentDirPath();

    GenericIniDataControls::FIXED_USER_DATA readFixedUserData();
    void writeFixedUserData(const GenericIniDataControls::FIXED_USER_DATA& data);

    int readCurrentLanguage();
    void writeCurrentLanguage(GlobalDataStruct::LANGUAGE_LIST language);

    QString readServerIPAddress();
    void writeServerIPAddress(const QString& ipAddress);

    QString readServerPortAddress();
    void writeServerPortAddress(const QString& port);

    GlobalDataStruct::HIGH_LIGHT_DATA readClassHightLightData();
    void writeClassHightLightData(const GlobalDataStruct::HIGH_LIGHT_DATA &data);

    bool readDeviceAlramSetupStatusData();
    void writeDeviceAlramSetupStatusData(bool isOFF);

    bool readUseLowLevelConfigData();
    void writeUseLowLevelConfigData(const bool& isUseLowLevelData);

    double readPCCutOff();
    void writePCCutOff(const double &cutOff);

    double readTIGECutOff();
    void writeTIGECutOff(const double &cutOff);

    double readtIgEZeroConditionSetting();
    void writetIgEZeroConditionSetting(const double& value);

    double readsIgEData1();
    void writesIgEData1(const double& data);

    double readsIgEData2();
    void writesIgEData2(const double& data);

    double readsIgEData3();
    void writesIgEData3(const double& data);

    double readsIgEData4();
    void writesIgEData4(const double& data);

    double readsIgEData5();
    void writesIgEData5(const double& data);

    double readsIgEData6();
    void writesIgEData6(const double& data);

    int readsIgESelectedRadioButtonClass();
    void writesIgESelectedRadioButtonClass(const int& classNumber);

    int readBackgroundCutoffData();
    void writeBackgroundCutoffData(int cutoff);

    bool readUnmarkIUML();
    void writeUnmarkIUML(bool isUnmark);

    bool readUseXML();
    void writeUseXML(const bool& isUseXML);

    bool readDisableDatabaseTigeCutoff();
    void writeDisableDatabaseTigeCutoff(bool isDisable);

private:
    void initIo();

private:
    QScopedPointer<privateStruct> d;
};

#endif // GENERICINIDATACONTROLS_H
