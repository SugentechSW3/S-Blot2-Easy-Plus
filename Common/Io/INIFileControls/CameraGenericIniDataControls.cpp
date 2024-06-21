#include "CameraGenericIniDataControls.h"

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"
#include "Common/Io/INIFileControls/GenericIniDataControls.h"

USING_SUGENTECH

struct CameraGenericIniDataControls::privateStruct
{
    QScopedPointer<CSettingIo> mCameraGenericSettingIo;

    const QString mGenericCameraSettingFileName = "GENERIC_CAMERA_DATA";
};

CameraGenericIniDataControls::CameraGenericIniDataControls(QObject *parent) :
                                                                              QObject(parent),
                                                                              d(new privateStruct)
{
    this->initIo();
}

CameraGenericIniDataControls::~CameraGenericIniDataControls()
{

}

void CameraGenericIniDataControls::initIo()
{
    GenericIniDataControls io;
    auto currentPath = io.getCurrentDirPath();

    d->mCameraGenericSettingIo.reset(new CSettingIo(d->mGenericCameraSettingFileName, currentPath));
}

GlobalDataStruct::CAMERA_LIGHT_SETTING CameraGenericIniDataControls::readCameraLightSettings()
{
    GlobalDataStruct::CAMERA_LIGHT_SETTING lightSetting;

    lightSetting.hue = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "HUE").toInt();
    lightSetting.gain = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "GAIN").toInt();
    lightSetting.focus = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "FOCUS").toInt();
    lightSetting.gamma = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "GAMMA").toInt();
    lightSetting.contrast = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "CONTRAST").toInt();
    lightSetting.expousre = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "EXPOSURE").toInt();
    lightSetting.sharpness = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "SHARPNESS").toInt();
    lightSetting.brightness = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "BRIGHTNESS").toInt();
    lightSetting.saturation = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "SATURATION").toInt();
    lightSetting.whitebalance = d->mCameraGenericSettingIo->getConfigureData("CAMERA_LIGHT_SETTINGS", "WHITE_BALANCE").toInt();

    return lightSetting;
}


void CameraGenericIniDataControls::writeCameraLightSettings(const GlobalDataStruct::CAMERA_LIGHT_SETTING& lightSetting)
{
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "HUE", lightSetting.hue);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "GAIN", lightSetting.gain);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "FOCUS", lightSetting.focus);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "GAMMA", lightSetting.gamma);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "CONTRAST", lightSetting.contrast);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "EXPOSURE", lightSetting.expousre);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "SHARPNESS", lightSetting.sharpness);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "BRIGHTNESS", lightSetting.brightness);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "SATURATION", lightSetting.saturation);
    d->mCameraGenericSettingIo->setConfigureData("CAMERA_LIGHT_SETTINGS", "WHITE_BALANCE", lightSetting.whitebalance);
}

GlobalDataStruct::GUIDE_LINE CameraGenericIniDataControls::readGuideLine()
{
    GlobalDataStruct::GUIDE_LINE guideLine;

    guideLine.x1 = d->mCameraGenericSettingIo->getConfigureData("GUIDE_LINE", "X1").toInt();
    guideLine.x2 = d->mCameraGenericSettingIo->getConfigureData("GUIDE_LINE", "X2").toInt();
    guideLine.y1 = d->mCameraGenericSettingIo->getConfigureData("GUIDE_LINE", "Y1").toInt();
    guideLine.y2 = d->mCameraGenericSettingIo->getConfigureData("GUIDE_LINE", "Y2").toInt();

    return guideLine;
}

void CameraGenericIniDataControls::writeGuideLine(const GlobalDataStruct::GUIDE_LINE& guideLine)
{
    d->mCameraGenericSettingIo->setConfigureData("GUIDE_LINE", "X1", guideLine.x1);
    d->mCameraGenericSettingIo->setConfigureData("GUIDE_LINE", "X2", guideLine.x2);
    d->mCameraGenericSettingIo->setConfigureData("GUIDE_LINE", "Y1", guideLine.y1);
    d->mCameraGenericSettingIo->setConfigureData("GUIDE_LINE", "Y2", guideLine.y2);
}

int CameraGenericIniDataControls::readFilpData()
{
    return d->mCameraGenericSettingIo->getConfigureData("FLIP", "FLIP_VALUE").toInt();
}

void CameraGenericIniDataControls::writeFilpData(int flip)
{
    d->mCameraGenericSettingIo->setConfigureData("FLIP", "FLIP_VALUE", flip);
}
