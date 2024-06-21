#ifndef SEMIAUTOCALIBRATION_H
#define SEMIAUTOCALIBRATION_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_SemiAutoCalibrationWidget;
class SemiAutoCalibrationWidget : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit SemiAutoCalibrationWidget(QWidget *parent = nullptr);
    virtual ~SemiAutoCalibrationWidget();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();
    void xAxisSettingSignalSlots();
    void pumpSettingSignalSlots();
    void shakeSettingSignalSlots();
    void aspPinSettingSignalSlots();
private slots:
    void onClickReadAllParam(bool);
    void onClickWasteBottleSensorON(bool status);

    void onClickPumpOffsetSet(bool);
    void onClickPumpTimeSet(bool);

    void onClickXAxisHome(bool);
    void onClickXAxisBathMove(bool);
    void onClickXAxisASPMove(bool);
    void onClickXAxisDSPMove(bool);

    void onClickShakeHome(bool);
    void onClickShakeAspMove(bool);
    void onClickShakeDspMove(bool);
    void onClickShakeDryMove(bool);
    void onClickShakeAnalysisMove(bool);
    void onClickShakeSampleDspMove(bool);
    void onClickShakeHomeMove(bool);

    void onClickAspHome(bool);
    void onClickAspPinMove(bool);
    void onClickAspBathMove(bool);
    void onClickAspTrayMove(bool);

    void onClickShakeSpeedMove(bool);
    void onClickShakePeriodMove(bool);

    void onRecvData(QByteArray recvData);

private:
    QScopedPointer<Ui_SemiAutoCalibrationWidget> ui;
    QScopedPointer<privateStruct> d;

};

#endif // SEMIAUTOCALIBRATION_H
