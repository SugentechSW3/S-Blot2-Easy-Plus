#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>
#include <QScopedPointer>

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

#include <opencv2/core/mat.hpp>

#include "Analysis/GlobalDataStruct.h"

class Ui_CameraView;
class CameraView : public CSerialNetworkWidget
{
private:
    Q_OBJECT

    struct privateStruct;
public:
    explicit CameraView(QWidget *parent = nullptr);
    virtual ~CameraView();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

private slots:
    void onUpdatedConfigure(GlobalDataStruct::HOUSING_ROI configData);
    void onDrawGraphRequest(QPoint pos, cv::Mat image);

    void onEnableWidget();
    void onDisableWidget();

    void onFindPC();
    void onAnalysisImage();
    void onReadQRTest();
    void onAngleReset();

    void onUpdateFindPcPosX(int idx, int value);
    void onUpdateFindPcPosX(QVector<int> value);

    void onLedOFF();
    void onSendXHome();

protected:
    void showEvent(QShowEvent *event) override;

private:
    QScopedPointer<Ui_CameraView> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CAMERAVIEW_H
