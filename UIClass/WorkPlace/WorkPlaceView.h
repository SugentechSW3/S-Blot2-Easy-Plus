#ifndef WORKPLACEVIEW_H
#define WORKPLACEVIEW_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include "Common/Barcode/CQRCode.h"

#include "Analysis/GlobalDataStruct.h"
#include "DataBase/DataBaseCtrl.h"

#define QR_TEST (1)


class QLineEdit;
class Ui_WorkPlaceView;
class WorkPlaceView : public CSerialNetworkWidget
{
private:
    Q_OBJECT

    struct privateStruct;
    struct LED_CONFIGURE_DATA
    {
        int ch1 = 0;
        int ch2 = 0;
        int ch3 = 0;
        int ch4 = 0;
    };

    enum WORKPLACE_STEP
    {
        WORKPLACE_STEP_PATIENT,
        WORKPLACE_STEP_PATIENT2,
        WORKPLACE_STEP_REAGENT,
        WORKPLACE_STEP_PROCESSING
    };

    const int PROBE_RETURN_TIME = 12000;

public:
    explicit WorkPlaceView(QWidget *parent = nullptr);
    virtual ~WorkPlaceView();

    static bool sortStripNumberData(GlobalDataStruct::TABLE_ITEM_ELEMENT& a, GlobalDataStruct::TABLE_ITEM_ELEMENT& b);
    static bool sortPanelNameData(GlobalDataStruct::TABLE_ITEM_ELEMENT& a, GlobalDataStruct::TABLE_ITEM_ELEMENT& b);

    void toRunningStatus(const QVector<GlobalDataStruct::AnalysisReportInfomation>& dbData, bool isRestart = true,
                         const QVector<QByteArray> &sequence = QVector<QByteArray>());

    void setForceStripScan();

    void doorOpen();

signals:
    void onResultShow(QVector<GlobalDataStruct::AnalysisReportInfomation> resultInfomation);
    void onPageReset();

    void onEnableMenuWidget();
    void onDisableMenuWidget();

    void onPatientStarted();
    void onReagentStarted();
    void onRunningStarted();
    void onQRReadCompleted();
    void onMemoryCleared();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();
    void initStyleSheet();

    void allWindowHide();       

    void makeUniqueIDAndDateTime(GlobalDataStruct::AnalysisReportInfomation& element) const;

    bool savePatientData(QVector<GlobalDataStruct::AnalysisReportInfomation>& element) const;
    bool saveResultData(const QVector<GlobalDataStruct::AnalysisReportInfomation> &element) const;
    bool saveFuncFormulaData(const QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT>& list) const;
    bool saveFuncStripRelData(const QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT>& list) const;

    bool updateResultData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result) const;

    bool isNewDBData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result) const;
    void overwriteLastFailureData(QVector<GlobalDataStruct::AnalysisReportInfomation>& result);
    void checkFailDataThenShowMessage(const QVector<GlobalDataStruct::AnalysisReportInfomation> &result);

    bool copyFromUIToAnalysisOrderedData();
    bool copyFromUIToAnalysisData();

    bool updateGeneralPatientInfomationData();
    bool updateStripScanPatientInfomationData();
    QVector<QByteArray> createRequestFullSequenceTimeProtocol();
    void sendPause(bool isPause);

    void moveCameraPosition(int idx);
    void moveTripleQRPosition();
    void moveTripleQrPositionCallBack();
    void readQRCode(bool isStripScan);
    void readStripQRCode();
    void readTripleQRCode();

    bool showManualInputDialog(const QVector<int> &missingStripQRList, QMap<int, QString> &resultData);
    bool isvalidPanelData();
    bool checkQRFuncRelMissingData();
    bool checkQRStripMissingData();
    bool checkContainsQRDataFromDB();
    QVector<int> checkMissingStripQRList();
    void checkQRStripRelMissingDataAndSave(const QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT>& list, const QString& tableName, const QString& columnNameLotNumber) const;

    void parsingExpQRData(const QString& expData);
    bool isExistLotCutoff(const QString& lotNumber);
    bool saveLotOptionalData(const GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT& list);
    QVariantMap makeQROptionalData(const GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT& report) const;

    void qrReadProcessing();
    void qrRescaningProcessing();
    void cleanupQRConfigInformation();
    void cleanupQRModule();
    void blockUIForProbeInit();
    void cameraPosHoming();
    void disConnectQRConnection();
    void commonStartGUIProcessing();
    void saveQRFuncRelDatas();
    void saveLotData();

    void stripScanButtonHide();
    void writeLedOneTimeAmplification();
    void writeLedOneTimeClear();

    void imageSave(const cv::Mat& matrix, QString fileName, int stripNumber, const QString& filePath = QString(""));

    void topLevelWidgetEnable();
    void topLevelWidgetDisable();

    void createAndInitProgressDialog();
    void autoSavePatientData();

    void rescaningForStripScan();
    void generalStripScan();

    void reagentStepUIControl();
    void processingStepUIControl();

    void copyFromAnalysisDataToResultData();

    QVector<GlobalDataStruct::AnalysisReportInfomation> failureAnalysisDataFilter(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList);

    void resultStatisticsAndSaveImage(QVector<GlobalDataStruct::AnalysisReportInfomation>& result);

    void enableStartButton();
    void enablePauseButton();
    void enableStopButton();

    void numberingStripNumber();

    bool isCompletedPatientRowData(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element);
    bool isIncompletedPatientRowData(const GlobalDataStruct::TABLE_ITEM_ELEMENT &element);
    bool isCompletedPatientRowDataForManual(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element);
    bool isIncompletedPatientRowDataForManual(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element);
    bool isCompletedPatientRowDataForFullAuto(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element);
    bool isIncompletedPatientRowDataForFullAuto(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element);

    void buttonControlForStartedStatus();
    void buttonControlForInitStatus();

    void resumeRunningStatus();
    void pauseRunningStatus();

    QString getMissingQuestionMessageText();
    QString getDataEmptyMessageText();
    QString getCrossUseMessageText();

    bool checkAllMissingSample();
    bool isAllMissingSample();

    void showMesageForRescanningAllMissingSample();

    bool isSampleMissingError(const QString& errorReason);
    QString getCombinedErrorString(const GlobalDataStruct::AnalysisReportInfomation& object1, const GlobalDataStruct::AnalysisReportInfomation& object2);

    cv::Mat getTripleQRImage();

    void getContentsCount(int &inhalant, int &etc);
    void initHiddenKey();
    void printLogLotAndPatientData(QVector<GlobalDataStruct::AnalysisReportInfomation>& result) const;
private slots:
    void onClickBackToWork();
    void onClickFrontToWork();

    void onStepPatient();
    void onStepPatient2();
    void onStepReagent();
    bool onStepProcessing();
    void onStepStripScan();

    void onClickStart();
    void onClickPause();
    void onClickStop();
    void onCameraInitFail();

    void onHideButton();

    void onAnalysisFinished(QVector<GlobalDataStruct::AnalysisReportInfomation> result);
    bool onStackSequenceProtocolForFullSequence();
    void onStart();
    void onStartGeneral();
    void onStartStripScan();
    void onStartStripScanWithoutSave();
    void onDisableButtonForShortTime();

    void onRecvData(QByteArray recvData);
    void onQRReadCallBack(const QByteArray& recvData);
    void onQRReadRescaningCallBack(const QByteArray& recvData);

    void onSerialConneted();
    void onNoScanQrRead();   
    void onUnpause();
private:
    QScopedPointer<Ui_WorkPlaceView> ui;
    QScopedPointer<privateStruct> d;
};

#endif // WORKPLACEVIEW_H
