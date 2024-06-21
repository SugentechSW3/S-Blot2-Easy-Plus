#ifndef RUNNINGSTATUSWIDGET_H
#define RUNNINGSTATUSWIDGET_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include <Analysis/BasicSupremeIAnalysis.h>
#include <Analysis/ConfigInformation.h>

class Ui_RunningStatusWidget;
class RunningStatusWidget : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit RunningStatusWidget(QWidget *parent = nullptr);
    virtual ~RunningStatusWidget();

    void startSequence(QVector<QByteArray> sequence, const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList);
    void reStartStatus(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList);
    void stripScan(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList);

signals:
    void onAnalysisReport(QVector<GlobalDataStruct::AnalysisReportInfomation> result);
    void onCameraInitFail();
    void onHideButtons();
    void onDryStepCameraInit();
    void onUnpause();

public slots:
    void onResumeItem();
    void onPauseItem();
    void onPauseUIControl(bool isPause);
    void onTimerStart(int mSec = 0);
    void onTimerStop();
    void onTimerRestart();

    void onStopAnalysisSequence();
    bool onCheckReTry(const QVector<GlobalDataStruct::AnalysisReportInfomation> &result);

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void startNetworkConnectionControl();
    void stopNetworkConnectionControl();

    void scannerNextStep();

    void calcETATimeAndSetupUI();
    void clacTimeTotalTimeAndSpendTimeForSetupUI(const QTime &currentTime);
    void DisplayingDialogForSemiAuto();

    void analysisMemoryClear();
    void cameraInstanceClear();
    void analysisCameraON();

    void updateErrorResonForUnchecked(const QVector<int>& uncheckedList);

    void sendPause(bool isPause);

private slots:
    void onCurrentStatusUpdate();
    void onReadSequenceTime();
    void showDialogForCheckList(const QVector<GlobalDataStruct::AnalysisReportInfomation> &list);
    void onAnalysisProcessing(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisIdxList);
    void onFinished();

    void onSendXHome();
    void onSendASPHome();

    void onSendLedOnOff(bool isOn);

    void onSendCurrentStep();
    void onSendCurrentTime();

    void onUpdateStatus(const int& currentTime);
    void onUpdateLastIdx(int currentIdx);
    void onUpdateCurrentStep(int currentStepNumber);
    void onUpdateDispensePause(const int& stepNumber, const int& processNumber);
    void onRecvDryStep(const int& stepNumber);

    void onRecvStepAnalysis();
    void onRecvRunningStateData(const QByteArray& recvData);
    void onRecvAnalysisData(const QByteArray& recvData);

    void onRecvData(QByteArray recvData);

    void onPauseControlDefaultTimer(bool isPause);

private:
    QScopedPointer<Ui_RunningStatusWidget> ui;
    QScopedPointer<privateStruct> d;

    const int TIME_STATUS_TICK = 30000;
    QString mCurrentStep = "";
};

#endif // RUNNINGSTATUSWIDGET_H
