#ifndef CAMERALIGHTSETTING_H
#define CAMERALIGHTSETTING_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_CameraLightSetting;
class CameraLightSetting : public QWidget, public CConfig
{
    Q_OBJECT

    struct privateStruct;
public:
    explicit CameraLightSetting(QWidget *parent = nullptr);
    virtual ~CameraLightSetting();

    void start();
    void readConfigData();
    void writeConfigData();

    void initParameterLimit();

private:
    void initSignalSlots();

private slots:
    void onValueExposureChanged(int value);
    void onValueGainChanged(int value);
    void onValueBrightnessChanged(int value);
    void onValueContrastChanged(int value);
    void onValueSaturationChanged(int value);
    void onValueSharpnessChanged(int value);
    void onValueWhiteBlanceChanged(int value);
    void onValueHueChanged(int value);
    void onValueGammaChanged(int value);
    void onValueFocusChanged(int value);

    void onClickedDefault(bool);
    void onClickedRead(bool);
    void onClickedWrite(bool);

    void getCurrentValue();
private:
    QScopedPointer<Ui_CameraLightSetting> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CAMERALIGHTSETTING_H
