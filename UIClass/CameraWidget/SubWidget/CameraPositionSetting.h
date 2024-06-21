#ifndef CAMERAPOSITIONSETTING_H
#define CAMERAPOSITIONSETTING_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_CameraPositionSetting;
class CameraPositionSetting : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit CameraPositionSetting(QWidget *parent = nullptr);
    virtual ~CameraPositionSetting();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

private slots:
    void onClickedXPosRead();
    void onClickedXPosWrite();

    void onClickedCameraWidthPosRead();
    void onClickedCameraWidthPosWrite();

    void onRecvData(QByteArray recvData);

private:
    QScopedPointer<Ui_CameraPositionSetting> ui;
};

#endif // CAMERAPOSITIONSETTING_H
