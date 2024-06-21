#ifndef CAMERAGENERICINIDATACONTROLS_H
#define CAMERAGENERICINIDATACONTROLS_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"

class CameraGenericIniDataControls : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    CameraGenericIniDataControls(QObject *parent = nullptr);
    ~CameraGenericIniDataControls();

    GlobalDataStruct::CAMERA_LIGHT_SETTING readCameraLightSettings();
    void writeCameraLightSettings(const GlobalDataStruct::CAMERA_LIGHT_SETTING& lightSetting);

    GlobalDataStruct::GUIDE_LINE readGuideLine();
    void writeGuideLine(const GlobalDataStruct::GUIDE_LINE &guideLine);

    int readFilpData();
    void writeFilpData(int flip);
private:
    void initIo();


private:
    QScopedPointer<privateStruct> d;
};

#endif // CAMERAGENERICINIDATACONTROLS_H
