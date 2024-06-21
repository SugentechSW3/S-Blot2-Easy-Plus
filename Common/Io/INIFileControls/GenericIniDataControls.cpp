#include "GenericIniDataControls.h"

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"

USING_SUGENTECH


struct GenericIniDataControls::privateStruct
{
    QScopedPointer<CSettingIo> mGenericIO;
    QScopedPointer<CSettingIo> mUserConfigOptionIO;

    const QString mGenericIniFileName = "GENERIC_DATA";
    const QString mUserConfigureOptionFileName = "USER_CONFIG_DATA";
    const QString mDirPath = "GENERIC_DATA";
};

GenericIniDataControls::GenericIniDataControls(QObject *parent) :
                                                                  QObject(parent),
                                                                  d(new privateStruct)
{
    this->initIo();
}

GenericIniDataControls::~GenericIniDataControls()
{

}
void GenericIniDataControls::initIo()
{
    d->mGenericIO.reset(new CSettingIo(d->mGenericIniFileName, d->mDirPath));
    d->mUserConfigOptionIO.reset(new CSettingIo(d->mUserConfigureOptionFileName, d->mDirPath));
}

QString GenericIniDataControls::getCurrentDirPath()
{
    return d->mDirPath;
}

GenericIniDataControls::FIXED_USER_DATA GenericIniDataControls::readFixedUserData()
{
    GenericIniDataControls::FIXED_USER_DATA data;

    data.applicant = d->mGenericIO->getConfigureData("FIXED_USER_DATA", "APPLICANT").toString();
    data.analyst = d->mGenericIO->getConfigureData("FIXED_USER_DATA", "ANALYST").toString();
    data.reviewer = d->mGenericIO->getConfigureData("FIXED_USER_DATA", "REVIEWER").toString();

    return data;
}

void GenericIniDataControls::writeFixedUserData(const GenericIniDataControls::FIXED_USER_DATA& data)
{
    d->mGenericIO->setConfigureData("FIXED_USER_DATA", "APPLICANT", data.applicant);
    d->mGenericIO->setConfigureData("FIXED_USER_DATA", "ANALYST", data.analyst);
    d->mGenericIO->setConfigureData("FIXED_USER_DATA", "REVIEWER", data.reviewer);
}

int GenericIniDataControls::readCurrentLanguage()
{
    return d->mGenericIO->getConfigureData("LANGUAGE", "LANGUAGE_SELECTED").toInt();
}

void GenericIniDataControls::writeCurrentLanguage(GlobalDataStruct::LANGUAGE_LIST language)
{
    d->mGenericIO->setConfigureData("LANGUAGE", "LANGUAGE_SELECTED", language);
}

QString GenericIniDataControls::readServerIPAddress()
{
    return d->mGenericIO->getConfigureData("SERVER_SETTINGS", "IP_ADDRESS").toString();
}

void GenericIniDataControls::writeServerIPAddress(const QString& ipAddress)
{
    d->mGenericIO->setConfigureData("SERVER_SETTINGS", "IP_ADDRESS", ipAddress);
}

QString GenericIniDataControls::readServerPortAddress()
{
    return d->mGenericIO->getConfigureData("SERVER_SETTINGS", "PORT").toString();
}

void GenericIniDataControls::writeServerPortAddress(const QString& port)
{
    d->mGenericIO->setConfigureData("SERVER_SETTINGS", "PORT", port);
}

GlobalDataStruct::HIGH_LIGHT_DATA GenericIniDataControls::readClassHightLightData()
{
    GlobalDataStruct::HIGH_LIGHT_DATA data;

    data.class0 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS0").toBool();
    data.class1 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS1").toBool();
    data.class2 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS2").toBool();
    data.class3 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS3").toBool();
    data.class4 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS4").toBool();
    data.class5 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS5").toBool();
    data.class6 = d->mUserConfigOptionIO->getConfigureData("CLASS_HIGHLIGHT", "CLASS6").toBool();

    return data;
}

void GenericIniDataControls::writeClassHightLightData(const GlobalDataStruct::HIGH_LIGHT_DATA& data)
{
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS0", data.class0);
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS1", data.class1);
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS2", data.class2);
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS3", data.class3);
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS4", data.class4);
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS5", data.class5);
    d->mUserConfigOptionIO->setConfigureData("CLASS_HIGHLIGHT", "CLASS6", data.class6);
}

bool GenericIniDataControls::readDeviceAlramSetupStatusData()
{
    return d->mUserConfigOptionIO->getConfigureData("DEVICE_ALRAM_SETTING", "ALARM_OFF").toBool();
}

void GenericIniDataControls::writeDeviceAlramSetupStatusData(bool isOFF)
{
    d->mUserConfigOptionIO->setConfigureData("DEVICE_ALRAM_SETTING", "ALARM_OFF", isOFF);
}

bool GenericIniDataControls::readUseLowLevelConfigData()
{
    return d->mUserConfigOptionIO->getConfigureData("INTENSITY_TEXT_SETTINGS", "USE_LOW_LEVEL_DATA").toBool();
}

void GenericIniDataControls::writeUseLowLevelConfigData(const bool& isUseLowLevelData)
{
    d->mUserConfigOptionIO->setConfigureData("INTENSITY_TEXT_SETTINGS", "USE_LOW_LEVEL_DATA", isUseLowLevelData);
}

double GenericIniDataControls::readPCCutOff()
{
    return d->mUserConfigOptionIO->getConfigureData("CUT_OFF_SETTINGS", "PC_CUT_OFF").toDouble();
}

void GenericIniDataControls::writePCCutOff(const double& cutOff)
{
    d->mUserConfigOptionIO->setConfigureData("CUT_OFF_SETTINGS", "PC_CUT_OFF", cutOff);
}

double GenericIniDataControls::readTIGECutOff()
{
    return d->mUserConfigOptionIO->getConfigureData("CUT_OFF_SETTINGS", "TIGE_CUT_OFF").toDouble();
}

void GenericIniDataControls::writeTIGECutOff(const double& cutOff)
{
    d->mUserConfigOptionIO->setConfigureData("CUT_OFF_SETTINGS", "TIGE_CUT_OFF", cutOff);
}

double GenericIniDataControls::readtIgEZeroConditionSetting()
{
    auto orgData = d->mUserConfigOptionIO->getConfigureData("TIGE_ZERO_CONDITION_SETTINGS", "TIGE_CONDITION_VALUE").toDouble();
    auto isZero = qFuzzyIsNull(orgData);

    if(isZero)
        return 1.0;

    return orgData;
}

void GenericIniDataControls::writetIgEZeroConditionSetting(const double &value)
{
    d->mUserConfigOptionIO->setConfigureData("TIGE_ZERO_CONDITION_SETTINGS", "TIGE_CONDITION_VALUE", value);
}

double GenericIniDataControls::readsIgEData1()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "VALUE1").toDouble();
}

void GenericIniDataControls::writesIgEData1(const double& data)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "VALUE1", data);
}

double GenericIniDataControls::readsIgEData2()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "VALUE2").toDouble();
}

void GenericIniDataControls::writesIgEData2(const double& data)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "VALUE2", data);
}

double GenericIniDataControls::readsIgEData3()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "VALUE3").toDouble();
}

void GenericIniDataControls::writesIgEData3(const double& data)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "VALUE3", data);
}

double GenericIniDataControls::readsIgEData4()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "VALUE4").toDouble();
}

void GenericIniDataControls::writesIgEData4(const double& data)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "VALUE4", data);
}

double GenericIniDataControls::readsIgEData5()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "VALUE5").toDouble();
}

void GenericIniDataControls::writesIgEData5(const double& data)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "VALUE5", data);
}

double GenericIniDataControls::readsIgEData6()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "VALUE6").toDouble();
}

void GenericIniDataControls::writesIgEData6(const double& data)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "VALUE6", data);
}

int GenericIniDataControls::readsIgESelectedRadioButtonClass()
{
    return d->mUserConfigOptionIO->getConfigureData("S_IGE_USER_DATA", "SELECTED_CLASS").toInt();
}

void GenericIniDataControls::writesIgESelectedRadioButtonClass(const int &classNumber)
{
    d->mUserConfigOptionIO->setConfigureData("S_IGE_USER_DATA", "SELECTED_CLASS", classNumber);
}

int GenericIniDataControls::readBackgroundCutoffData()
{
    return d->mUserConfigOptionIO->getConfigureData("BACK_GROUND_CUT_OFF_DATA", "BG_CUT_OFF_VALUE").toInt();
}

void GenericIniDataControls::writeBackgroundCutoffData(int cutoff)
{
    d->mUserConfigOptionIO->setConfigureData("BACK_GROUND_CUT_OFF_DATA", "BG_CUT_OFF_VALUE", cutoff);
}

bool GenericIniDataControls::readUnmarkIUML()
{
    return d->mUserConfigOptionIO->getConfigureData("UN_MARK_IU_ML_DATA", "UN_MARK").toBool();
}

void GenericIniDataControls::writeUnmarkIUML(bool isUnmark)
{
   d->mUserConfigOptionIO->setConfigureData("UN_MARK_IU_ML_DATA", "UN_MARK", isUnmark);
}

bool GenericIniDataControls::readUseXML()
{
    return d->mUserConfigOptionIO->getConfigureData("LIS", "USE_XML").toBool();
}

void GenericIniDataControls::writeUseXML(const bool &isUseXML)
{
    d->mUserConfigOptionIO->setConfigureData("LIS", "USE_XML", isUseXML);
}

bool GenericIniDataControls::readDisableDatabaseTigeCutoff()
{
    return d->mUserConfigOptionIO->getConfigureData("DATABASE_TIGE_CUT_OFF", "DISABLE_DATABASE_CUT_OFF").toBool();
}

void GenericIniDataControls::writeDisableDatabaseTigeCutoff(bool isDisable)
{
    d->mUserConfigOptionIO->setConfigureData("DATABASE_TIGE_CUT_OFF", "DISABLE_DATABASE_CUT_OFF", isDisable);
}
