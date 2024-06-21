#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include "Analysis/GlobalDataStruct.h"

class Ui_MainWidget;
class MainView : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

    struct SW_VERSION
    {
        int major = 1;
        int minor = 0;
        int modified = 0;
        int bugfix = 0;
    };

    struct HW_VERSION
    {
        QString major;
        QString minor;
        QString modified;
        QString bugfix;
    };

    struct FW_VERSION
    {
        QString major;
        QString minor;
        QString modified;
        QString bugfix;
    };
//test
public:
    explicit MainView(QWidget *parent = nullptr);
    virtual ~MainView();

    void checkReSequenceProcess();
    void setResetStatus(bool status);


signals:
    void onResultReset();

public slots:
    void onResetGUI();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void initDownloadData();
    void initLoginProcess();
    void initGUI();
    void initDeviceConfig();
    void initWaitDlg();
    void initAdmin();
    void initUser();
    void initStyleSheetButton();

    void hideAllViews();

    QVector<GlobalDataStruct::AnalysisReportInfomation> readLastestDataBaseData();
    void moveToRunningStatus(const QVector<GlobalDataStruct::AnalysisReportInfomation>& dbData);
    void offLineGUIProcessing();

    void showResultData();
    void disconnectMemoryConnection();

    bool isRunningMainViewFunction();

private slots:
    void onClickMainBtn();
    void onClickCameraBtn();
    void onClickETCBtn();
    void onClickSequenceBtn();
    void onClickSemiautoBtn();
    void onClickWorkPlaceBtn();
    void onClickResultViewBtn();
    void onClickResetBtn();
    void onClickAutoCleanBtn();
    void onClickLanguageSelect();
    void onClickIPSetting();
    void onClickDataExchange();
    void onClickUserModeConfigureBtn();
    void onClickAdminModeConfigureBtn();
    void onClickContentsChangeBtn();
    void onClickStripScan();
    void onClickAspration();
    void onClickHiddenBand();

    void onResult(QVector<GlobalDataStruct::AnalysisReportInfomation> resultInfomation);

    void onEnableMenuWidget();
    void onDisableMenuWidget();

    void onLoadDeviceVersion();
    void onLoadQRTriplePosition();

    void onRecvStatusRead(const QByteArray& recvData);
    void onRecvQRDataSetting(const QByteArray& recvData);

    void onRecvData(QByteArray recvData);

private:
    QScopedPointer<Ui_MainWidget> ui;
    QScopedPointer<privateStruct> d;
    bool mLoading = false;
};

#endif // MAINWIDGET_H
