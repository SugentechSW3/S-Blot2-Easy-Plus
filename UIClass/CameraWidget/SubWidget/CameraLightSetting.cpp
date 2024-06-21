#include <QThread>
#include "CameraLightSetting.h"
#include "ui_CameraLightSetting.h"

#include "Analysis/ConfigInformation.h"
#include "Analysis/GlobalDataStruct.h"

#include "CameraControl/CameraInstance.h"
#include <Common/CLogWriter.h>

USING_SUGENTECH

struct CameraLightSetting::privateStruct
{
    CameraInstance* mInstance = nullptr;
    bool mIsInit = false;
};

CameraLightSetting::CameraLightSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_CameraLightSetting),
    d(new privateStruct)
{
    ui->setupUi(this);
    d->mInstance = CameraInstance::getInstance();
    this->initSignalSlots();
}

CameraLightSetting::~CameraLightSetting()
{
}

void CameraLightSetting::start()
{
    if(d->mIsInit == false)
        return;

    this->initParameterLimit();
}

void CameraLightSetting::initSignalSlots()
{
    connect(d->mInstance, &CameraInstance::onFinishedInitalizing, this, [this]
            {
                if(d->mIsInit == true)
                    return;

                connect(ui->widgetExposure, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueExposureChanged);
                connect(ui->widgetWhitebalance, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueWhiteBlanceChanged);
                connect(ui->widgetGain, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueGainChanged);
                connect(ui->widgetBrightness, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueBrightnessChanged);
                connect(ui->widgetContrast, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueContrastChanged);
                connect(ui->widgetSaturation, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueSaturationChanged);
                connect(ui->widgetSharpness, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueSharpnessChanged);
                connect(ui->widgetHue, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueHueChanged);
                connect(ui->widgetGamma, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueGammaChanged);
                connect(ui->widgetFocus, &SpinSliderWidget::valueChanged, this, &CameraLightSetting::onValueFocusChanged);

                 d->mIsInit = true;

                this->initParameterLimit();
            });

    connect(ui->btnDefault,  &QPushButton::clicked, this, &CameraLightSetting::onClickedDefault);
    connect(ui->btnReadConfig, &QPushButton::clicked, this, &CameraLightSetting::onClickedRead);
    connect(ui->btnWriteConfig, &QPushButton::clicked, this, &CameraLightSetting::onClickedWrite);
}

void CameraLightSetting::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getCameraLightSetting();

    if(configData.expousre != 0)
        ui->widgetExposure->setValue(configData.expousre);

    if(configData.gain != 0)
        ui->widgetGain->setValue(configData.gain);

    if(configData.brightness != 0)
        ui->widgetBrightness->setValue(configData.brightness);

    if(configData.contrast != 0)
        ui->widgetContrast->setValue(configData.contrast);

    if(configData.saturation != 0)
        ui->widgetSaturation->setValue(configData.saturation);

    if(configData.sharpness != 0)
        ui->widgetSharpness->setValue(configData.sharpness);

    if(configData.whitebalance != 0)
        ui->widgetWhitebalance->setValue(configData.whitebalance);

    if(configData.hue != 0)
        ui->widgetHue->setValue(configData.hue);

    if(configData.gamma != 0)
        ui->widgetGamma->setValue(configData.gamma);

    if(configData.focus != 0)
        ui->widgetFocus->setValue(configData.focus);
}

void CameraLightSetting::writeConfigData()
{
    auto instance = this->getConfigInstance();

    GlobalDataStruct::CAMERA_LIGHT_SETTING configData;
    configData.expousre = ui->widgetExposure->getValue();
    configData.gain = ui->widgetGain->getValue();
    configData.brightness = ui->widgetBrightness->getValue();
    configData.contrast = ui->widgetContrast->getValue();
    configData.saturation = ui->widgetSaturation->getValue();
    configData.sharpness = ui->widgetSharpness->getValue();
    configData.whitebalance = ui->widgetWhitebalance->getValue();
    configData.hue = ui->widgetHue->getValue();
    configData.gamma = ui->widgetGamma->getValue();
    configData.focus = ui->widgetFocus->getValue();

    instance->setCameraLightSetting(configData);
}

void CameraLightSetting::initParameterLimit()
{
    auto instance =  CameraInstance::getInstance();
    auto parameterList = instance->getCameraPropertyList();

    if(parameterList.isEmpty() == true)
    {
        CLogWriter::printLog("<CameraLightSetting::> ParameterRead: Can not Read");
        return;
    }

    auto exposure = parameterList[cv::CAP_PROP_EXPOSURE];
    auto gain = parameterList[cv::CAP_PROP_GAIN];
    auto brightness = parameterList[cv::CAP_PROP_BRIGHTNESS];
    auto contrast = parameterList[cv::CAP_PROP_CONTRAST];
    auto saturation = parameterList[cv::CAP_PROP_SATURATION];
    auto sharpness = parameterList[cv::CAP_PROP_SHARPNESS];
    auto whitebalance = parameterList[cv::CAP_PROP_WHITE_BALANCE_RED_V];
    auto hue = parameterList[cv::CAP_PROP_HUE];
    auto gamma = parameterList[cv::CAP_PROP_GAMMA];
    auto focus = parameterList[cv::CAP_PROP_FOCUS];

    //Range설정
    ui->widgetExposure->setMinimumValue(exposure.min);
    ui->widgetExposure->setMaximumValue(exposure.max);
    ui->widgetExposure->setStep(exposure.step);

    ui->widgetGain->setMinimumValue(gain.min);
    ui->widgetGain->setMaximumValue(gain.max);
    ui->widgetGain->setStep(gain.step);

    ui->widgetBrightness->setMinimumValue(brightness.min);
    ui->widgetBrightness->setMaximumValue(brightness.max);
    ui->widgetBrightness->setStep(brightness.step);

    ui->widgetContrast->setMinimumValue(contrast.min);
    ui->widgetContrast->setMaximumValue(contrast.max);
    ui->widgetContrast->setStep(contrast.step);

    ui->widgetSaturation->setMinimumValue(saturation.min);
    ui->widgetSaturation->setMaximumValue(saturation.max);
    ui->widgetSaturation->setStep(saturation.step);

    ui->widgetSharpness->setMinimumValue(sharpness.min);
    ui->widgetSharpness->setMaximumValue(sharpness.max);
    ui->widgetSharpness->setStep(sharpness.step);

    ui->widgetWhitebalance->setMinimumValue(whitebalance.min);
    ui->widgetWhitebalance->setMaximumValue(whitebalance.max);
    ui->widgetWhitebalance->setStep(whitebalance.step);

    ui->widgetHue->setMinimumValue(hue.min);
    ui->widgetHue->setMaximumValue(hue.max);
    ui->widgetHue->setStep(hue.step);

    ui->widgetGamma->setMinimumValue(gamma.min);
    ui->widgetGamma->setMaximumValue(gamma.max);
    ui->widgetGamma->setStep(gamma.step);

    ui->widgetFocus->setMinimumValue(focus.min);
    ui->widgetFocus->setMaximumValue(focus.max);
    ui->widgetFocus->setStep(focus.step);

    this->getCurrentValue();
    this->readConfigData();
}

void CameraLightSetting::getCurrentValue()
{
    auto instance =  CameraInstance::getInstance();

    //기본값으로 초기화(기본값으로 초기화한뒤 설정파일에서 읽어들여서 후처리 해야함)
    ui->widgetExposure->setValue(instance->getCameraProperty(cv::CAP_PROP_EXPOSURE));
    ui->widgetGain->setValue(instance->getCameraProperty(cv::CAP_PROP_GAIN));
    ui->widgetBrightness->setValue(instance->getCameraProperty(cv::CAP_PROP_BRIGHTNESS));
    ui->widgetContrast->setValue(instance->getCameraProperty(cv::CAP_PROP_CONTRAST));
    ui->widgetSaturation->setValue(instance->getCameraProperty(cv::CAP_PROP_SATURATION));
    ui->widgetSharpness->setValue(instance->getCameraProperty(cv::CAP_PROP_SHARPNESS));
    ui->widgetWhitebalance->setValue(instance->getCameraProperty(cv::CAP_PROP_WHITE_BALANCE_RED_V));
    ui->widgetHue->setValue(instance->getCameraProperty(cv::CAP_PROP_HUE));
    ui->widgetGamma->setValue(instance->getCameraProperty(cv::CAP_PROP_GAMMA));
    ui->widgetFocus->setValue(instance->getCameraProperty(cv::CAP_PROP_FOCUS));
}

void CameraLightSetting::onValueExposureChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_EXPOSURE, value);
}

void CameraLightSetting::onValueGainChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_GAIN, value);
}

void CameraLightSetting::onValueBrightnessChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_BRIGHTNESS, value);
}

void CameraLightSetting::onValueContrastChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_CONTRAST, value);
}

void CameraLightSetting::onValueSaturationChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_SATURATION, value);
}

void CameraLightSetting::onValueSharpnessChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_SHARPNESS, value);
}

void CameraLightSetting::onValueWhiteBlanceChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_WHITE_BALANCE_RED_V, value);
}

void CameraLightSetting::onValueHueChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_HUE, value);
}

void CameraLightSetting::onValueGammaChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_GAMMA, value);
}

void CameraLightSetting::onValueFocusChanged(int value)
{
    auto instance = CameraInstance::getInstance();
    instance->setCameraProperty(cv::CAP_PROP_FOCUS, value);
}

void CameraLightSetting::onClickedDefault(bool)
{
    auto instance =  CameraInstance::getInstance();
    auto parameterList = instance->getCameraPropertyList();

    if(parameterList.isEmpty() == true)
    {
        CLogWriter::printLog("<CameraLightSetting::> ParameterRead: Can not Read");
        return;
    }

    auto exposure = parameterList[cv::CAP_PROP_EXPOSURE];
    auto gain = parameterList[cv::CAP_PROP_GAIN];
    auto brightness = parameterList[cv::CAP_PROP_BRIGHTNESS];
    auto contrast = parameterList[cv::CAP_PROP_CONTRAST];
    auto saturation = parameterList[cv::CAP_PROP_SATURATION];
    auto sharpness = parameterList[cv::CAP_PROP_SHARPNESS];
    auto whitebalance = parameterList[cv::CAP_PROP_WHITE_BALANCE_RED_V];
    auto hue = parameterList[cv::CAP_PROP_HUE];
    auto gamma = parameterList[cv::CAP_PROP_GAMMA];
    auto focus = parameterList[cv::CAP_PROP_FOCUS];

    instance->setCameraProperty(cv::CAP_PROP_EXPOSURE, exposure.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_GAIN, gain.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_BRIGHTNESS, brightness.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_CONTRAST, contrast.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_SATURATION, saturation.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_SHARPNESS, sharpness.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_WHITE_BALANCE_RED_V, whitebalance.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_HUE, hue.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_GAMMA, gamma.defaultValue);
    instance->setCameraProperty(cv::CAP_PROP_FOCUS, focus.defaultValue);

    ui->widgetExposure->setValue(exposure.defaultValue);
    ui->widgetGain->setValue(gain.defaultValue);
    ui->widgetBrightness->setValue(brightness.defaultValue);
    ui->widgetContrast->setValue(contrast.defaultValue);
    ui->widgetSaturation->setValue(saturation.defaultValue);
    ui->widgetSharpness->setValue(sharpness.defaultValue);
    ui->widgetWhitebalance->setValue(whitebalance.defaultValue);
    ui->widgetHue->setValue(hue.defaultValue);
    ui->widgetGamma->setValue(gamma.defaultValue);
    ui->widgetFocus->setValue(focus.defaultValue);
}

void CameraLightSetting::onClickedRead(bool)
{
    this->readConfigData();
}

void CameraLightSetting::onClickedWrite(bool)
{
    this->writeConfigData();
}
