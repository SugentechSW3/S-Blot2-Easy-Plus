#ifndef CAMERAOPTIONSETTINGS_H
#define CAMERAOPTIONSETTINGS_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_CameraOptionSettings;
class CameraOptionSettings : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit CameraOptionSettings(QWidget *parent = nullptr);
    virtual ~CameraOptionSettings();

private :
    virtual void initVariables();
    virtual void initSignalSlots();
    virtual void startWidget();
    virtual void stopWidget();

private:
    QScopedPointer<Ui_CameraOptionSettings> ui;
};

#endif // CAMERAOPTIONSETTINGS_H
