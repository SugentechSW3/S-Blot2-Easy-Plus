#include <QMessageBox>
#include <QUuid>
#include <QVector>
#include <QThread>
#include <QLineEdit>
#include <QAction>
#include <Analysis/Interfaces/IDataClassifier.h>
#include <Analysis/Bases/DataClassifierBase.h>
#include <DataBase/BasicDatabaseCtrl.h>
#include <DataBase/CodeFuncDatabaseCtrl.h>
#include "WorkPlaceView.h"
#include "ui_WorkPlaceView.h"

#include "PatientListWidget.h"
#include "PatientListWidgetStep2.h"
#include "ReagentUserWidget.h"
#include "RunningStatusWidget.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketWriter/WriteProtocolControlWrite.h"
#include "Network/PacketWriter/WriteProtocolControlRead.h"
#include "Network/PacketWriter/WriteProtocolSequenceGenrator.h"
#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"
#include "Network/PacketWriter/WriteProtocolBarcodePage.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"
#include "Network/PacketReader/PacketReader.h"

#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "UIClass/DialogWidget/QRStripCheckDlg.h"
#include "UIClass/DialogWidget/ProgressBarDlg.h"
#include "UIClass/DialogWidget/StatusDialog.h"

#include "Common/CUtil.h"
#include "Common/CLogWriter.h"
#include "Common/DataBase/CDataBase.h"

#include "Analysis/ConfigInformation.h"

#include "CameraControl/CameraInstance.h"
#include "mainwindow.h"

USING_SUGENTECH

struct WorkPlaceView::privateStruct
{
    const int QR_RETRY_COUNT = 5;
    const int QR_FUNC_COUNT = 5;

    QVector<QByteArray> mSequenceQueue;
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> mAnalysisData;
    QVector<GlobalDataStruct::AnalysisReportInfomation> mResultData;

    WorkPlaceView::WORKPLACE_STEP mCurrentStep = WORKPLACE_STEP_PATIENT;

    QScopedPointer<PatientListWidget> mPatientListWidget;
    QScopedPointer<PatientListWidgetStep2> mPatientListWidget2;
    QScopedPointer<ReagentUserWidget> mReagentWidget;
    QScopedPointer<RunningStatusWidget> mRunningStatusWidget;

    QScopedPointer<DataBaseCtrl> mBasicDataBase;
    QScopedPointer<DataBaseCtrl> mCodeFuncDataBase;

    QMetaObject::Connection mQRReadConnection;
    QMetaObject::Connection mCameraConnection;
    QMetaObject::Connection mNetworkStatusConnection;

    CameraInstance* mCameraInstance = nullptr;
    ConfigInformation* mConfigInstance = nullptr;

    QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> mStripRel;
    QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> mNewFormula;
    QVector<QString> mStripLotList;
    QVector<int> mRescaningStripList;

    QScopedPointer<ProgressBarDlg> mStripQRProgressDlg;
    QScopedPointer<StatusDialog> mWaitingQRCodeInitializing;
    QTimer mStartTimer;

    bool mIsStarted = false;
    bool mIsStripScan = false;
    bool mIsQRReading = false;

    int mStripQRReadCount = 0;
    int mRescaningQRCount = 0;
    int mRescaningStripIdx = 0;
};

WorkPlaceView::WorkPlaceView(QWidget *parent) :
                                                CSerialNetworkWidget(parent),
                                                ui(new Ui_WorkPlaceView),
                                                d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    this->initStyleSheet();
    this->initHiddenKey();
}

WorkPlaceView::~WorkPlaceView()
{
    this->stop();
}

void WorkPlaceView::initStyleSheet()
{    
    ui->btnFront->setStyleSheetImage(":/Image/Image/Information/arrow_right_normal.png", ":/Image/Image/Information/arrow_right_over.png",
                                     ":/Image/Image/Information/arrow_right_focus.png", ":/Image/Image/Information/arrow_right_disable.png");

    ui->btnBack->setStyleSheetImage(":/Image/Image/Information/arrow_left_normal.png", ":/Image/Image/Information/arrow_left_over.png",
                                    ":/Image/Image/Information/arrow_left_focus.png", ":/Image/Image/Information/arrow_left_disable.png");

    ui->btnStart->setStyleSheetImage(":/Image/Image/RunningStatus/start_normal.png", ":/Image/Image/RunningStatus/start_over.png",
                                     ":/Image/Image/RunningStatus/start_focus.png", ":/Image/Image/RunningStatus/start_disable.png");

    ui->btnPause->setStyleSheetImage(":/Image/Image/RunningStatus/pause_normal.png", ":/Image/Image/RunningStatus/pause_over.png",
                                     ":/Image/Image/RunningStatus/pause_focus.png", ":/Image/Image/RunningStatus/pause_disable.png");

    ui->btnStop->setStyleSheetImage(":/Image/Image/RunningStatus/stop_normal.png", ":/Image/Image/RunningStatus/stop_over.png",
                                    ":/Image/Image/RunningStatus/stop_focus.png", ":/Image/Image/RunningStatus/stop_disable.png");

    QPalette startPalette = ui->btnStart->palette();
    startPalette.setColor(ui->btnStart->foregroundRole(),Qt::white);
    ui->btnStart->setPalette(startPalette);

    QPalette pausePalette = ui->btnPause->palette();
    pausePalette.setColor(ui->btnPause->foregroundRole(),Qt::white);
    ui->btnPause->setPalette(pausePalette);

    QPalette stopPalette = ui->btnStop->palette();
    stopPalette.setColor(ui->btnStop->foregroundRole(),Qt::white);
    ui->btnStop->setPalette(stopPalette);
}

void WorkPlaceView::initVariables()
{
    d->mBasicDataBase.reset(new DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::BasicDB));
    d->mCodeFuncDataBase.reset(new DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::DBCodeFunc));

    d->mCameraInstance = CameraInstance::getInstance();
    d->mConfigInstance = ConfigInformation::getInstance();
    d->mStartTimer.setSingleShot(true);
    d->mStartTimer.setInterval(PROBE_RETURN_TIME * 2);

    ui->btnStart->saveButtonText();
    ui->btnStop->saveButtonText();
    ui->btnPause->saveButtonText();
}

void WorkPlaceView::initSignalSlots()
{
    connect(ui->btnBack, SIGNAL(clicked()), SLOT(onClickBackToWork()));
    connect(ui->btnFront, SIGNAL(clicked()), SLOT(onClickFrontToWork()));
    connect(ui->btnStart, &QPushButton::clicked, this, &WorkPlaceView::onClickStart);
    connect(ui->btnPause, &QPushButton::clicked, this, &WorkPlaceView::onClickPause);
    connect(ui->btnStop, &QPushButton::clicked, this, &WorkPlaceView::onClickStop);
    connect(this, &WorkPlaceView::onQRReadCompleted, this, &WorkPlaceView::onStart);
    connect(this, &WorkPlaceView::onReadReady, this, &WorkPlaceView::onRecvData);
    connect(&d->mStartTimer, &QTimer::timeout, this, [this]
            {
                this->commonStartGUIProcessing();
            });     
}

void WorkPlaceView::startWidget()
{
    d->mIsStarted = false;
    d->mIsStripScan = false;
    d->mStripQRReadCount = 0;
    d->mRescaningQRCount = 0;
    d->mRescaningStripIdx = 0;
    d->mAnalysisData.clear();
    d->mResultData.clear();
    d->mSequenceQueue.clear();
    d->mStripRel.clear();
    d->mNewFormula.clear();
    d->mStripLotList.clear();
    d->mRescaningStripList.clear();

    d->mCurrentStep = WORKPLACE_STEP::WORKPLACE_STEP_PATIENT;

    d->mPatientListWidget.reset(new PatientListWidget(ui->MainViewWidget));
    ui->mainViewGrid->addWidget(d->mPatientListWidget.data());

    d->mPatientListWidget2.reset(new PatientListWidgetStep2(ui->MainViewWidget));
    ui->mainViewGrid->addWidget(d->mPatientListWidget2.data());

    d->mRunningStatusWidget.reset(new RunningStatusWidget(ui->MainViewWidget));
    ui->mainViewGrid->addWidget(d->mRunningStatusWidget.data());

    d->mReagentWidget.reset(new ReagentUserWidget(ui->MainViewWidget));
    ui->mainViewGrid->addWidget(d->mReagentWidget.data());

    auto topLevelWidget = MainWindow::getInstance();
    d->mWaitingQRCodeInitializing.reset(new StatusDialog(topLevelWidget));
    d->mWaitingQRCodeInitializing->setStatusMessage(tr("Initializing device."));
    d->mWaitingQRCodeInitializing->stopWaiting();

    connect(d->mRunningStatusWidget.data(), &RunningStatusWidget::onCameraInitFail, this, &WorkPlaceView::onCameraInitFail, Qt::DirectConnection);
    connect(d->mRunningStatusWidget.data(), &RunningStatusWidget::onAnalysisReport, this, &WorkPlaceView::onAnalysisFinished, Qt::DirectConnection);
    connect(d->mRunningStatusWidget.data(), &RunningStatusWidget::onHideButtons, this, &WorkPlaceView::onHideButton, Qt::DirectConnection);
    connect(d->mRunningStatusWidget.data(), &RunningStatusWidget::onDryStepCameraInit, this, &WorkPlaceView::onDisableButtonForShortTime, Qt::DirectConnection);
    connect(d->mRunningStatusWidget.data(), &RunningStatusWidget::onUnpause, this, &WorkPlaceView::onUnpause, Qt::DirectConnection);

    this->onStepPatient();
}

void WorkPlaceView::stopWidget()
{
    d->mPatientListWidget.reset();
    d->mPatientListWidget2.reset();
    d->mRunningStatusWidget.reset();
    d->mReagentWidget.reset();
    d->mIsStarted = false;

    QTimer::singleShot(1000, this, [this]
                       { emit onMemoryCleared(); });
}

void WorkPlaceView::onClickBackToWork()
{
    switch(d->mCurrentStep)
    {
    case WORKPLACE_STEP_PATIENT:
        break;

    case WORKPLACE_STEP_PATIENT2:
        this->onStepPatient();
        break;

    case WORKPLACE_STEP_REAGENT:
        this->onStepPatient();
        break;

    case WORKPLACE_STEP_PROCESSING:
        break;

    }
}

void WorkPlaceView::onClickFrontToWork()
{    
    switch(d->mCurrentStep)
    {
    case WORKPLACE_STEP_PATIENT :
        CLogWriter::printLog(QString("[WorkPlaceView::] : ---WORKPLACE_STEP_PATIENT---"));
        if(d->mPatientListWidget->isSelectedStripScan() == true)
        {
            d->mConfigInstance->setSelectSequence("<Strip Scan : Analysis>");
            this->onStepStripScan();
            return;
        }
        d->mConfigInstance->setSelectSequence(d->mPatientListWidget->getStartMode());
        if(this->updateGeneralPatientInfomationData() == false)
            return;

        if(this->onStackSequenceProtocolForFullSequence() == false)
        {
            d->mConfigInstance->setSelectSequence("<Full Sequence>");
            return;
        }

        this->onStepPatient2();
        break;

    case WORKPLACE_STEP_PATIENT2:
        CLogWriter::printLog(QString("[WorkPlaceView::] : ---WORKPLACE_STEP_PATIENT2---"));
        this->onStepReagent();
        break;

    case WORKPLACE_STEP_REAGENT:
        CLogWriter::printLog(QString("[WorkPlaceView::] : ---WORKPLACE_STEP_REAGENT---"));
        break;

    case WORKPLACE_STEP_PROCESSING:
        CLogWriter::printLog(QString("[WorkPlaceView::] : ---WORKPLACE_STEP_PROCESSING---"));
        break;
    }
}

void WorkPlaceView::onStepPatient()
{
    d->mCurrentStep = WORKPLACE_STEP_PATIENT;

    this->allWindowHide();

    d->mPatientListWidget->start();
    d->mPatientListWidget->show();

    if(d->mAnalysisData.isEmpty() == false)
        d->mPatientListWidget->setData(d->mAnalysisData);

    ui->btnFront->show();
    ui->btnBack->show();
    ui->btnFront->setEnabled(true);
    ui->btnBack->setEnabled(false);
    ui->widgetButton->hide();


    emit onPatientStarted();
}

void WorkPlaceView::onStepPatient2()
{
    d->mCurrentStep = WORKPLACE_STEP_PATIENT2;

    this->allWindowHide();

    d->mPatientListWidget2->show();
    d->mPatientListWidget2->setData(d->mAnalysisData);

    ui->btnFront->show();
    ui->btnBack->show();
    ui->btnFront->setEnabled(true);
    ui->btnBack->setEnabled(true);
    ui->widgetButton->hide();
}

void WorkPlaceView::onStepReagent()
{
    d->mCurrentStep = WORKPLACE_STEP_REAGENT;

    this->allWindowHide();
    this->reagentStepUIControl();

#if 0
    auto lastSequence = d->mConfigInstance->getLastSelectedSequence(); //fullAuto Only
#endif
    auto lastSequence = 1;
    this->onQueuedSend(WriteProtocolControlWrite::writeSequenceSelect(lastSequence).toUtf8());

    emit onReagentStarted();
}

bool WorkPlaceView::onStepProcessing()
{
    if(this->savePatientData(d->mResultData) == false)
    {
        CUtil::messageBox(tr("Can not Save PatientData"));
        CLogWriter::printLog("[DataBase] Patient Data Save Fail", CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }
    this->printLogLotAndPatientData(d->mResultData);

    d->mCurrentStep = WORKPLACE_STEP_PROCESSING;

    this->processingStepUIControl();

    QTimer::singleShot(25, this, [this] {d->mWaitingQRCodeInitializing->startWaiting();});

    QTimer::singleShot(PROBE_RETURN_TIME, this, [this]
                       {
                           d->mWaitingQRCodeInitializing->stopWaiting();
                           this->toRunningStatus(d->mResultData, false, d->mSequenceQueue);
                           emit onRunningStarted();
                       });

    return true;
}

void WorkPlaceView::onStepStripScan()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---Strip Scan Start---"));
    auto lastData = ConfigInformation::getInstance()->getLastAnalysisData();
    auto isEmptyLastData = lastData.isEmpty();

    TunnningDataProtocolSendHelper::writeShakeCameraPosMove("WorkPlaceView");
    QTimer::singleShot(1000, this, [this, isEmptyLastData]
    {
        if(isEmptyLastData)
            this->generalStripScan();
        else
            this->rescaningForStripScan();
    });

}

void WorkPlaceView::allWindowHide()
{
    if(d->mPatientListWidget.isNull() == false)
    {
        d->mPatientListWidget->hide();
        d->mPatientListWidget->stop();
    }

    if(d->mPatientListWidget2.isNull() == false)
        d->mPatientListWidget2->hide();

    if(d->mReagentWidget.isNull() == false)
    {
        d->mReagentWidget->hide();
         d->mReagentWidget->stop();
    }

    if(d->mRunningStatusWidget.isNull() == false)
    {
        d->mRunningStatusWidget->hide();
         d->mRunningStatusWidget->stop();
    }
}

void WorkPlaceView::onClickStart()
{    
    SerialCtrl::getInstance()->initPacketFailCount();
    QString seq = d->mConfigInstance->getSelectSequence();
    if(d->mIsStarted == true)
    {
        CLogWriter::printLog(QString("[WorkPlaceView::] : ---Click Start--- Resume"));
        this->sendPause(false);
        this->commonStartGUIProcessing();
    }
    else
    {

        CLogWriter::printLog(QString("[WorkPlaceView::] : ---Click Start--- %1").arg(seq));
        this->readQRCode(false);    
    }
}

void WorkPlaceView::onClickPause()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---Click Pause---"));
    ui->btnPause->setDisabled(true);
    ui->btnStart->setDisabled(true);

    this->pauseRunningStatus();
    this->sendPause(true);
    this->enableStartButton();
}

void WorkPlaceView::onClickStop()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---Click Stop---"));
    auto topLevelWidget = MainWindow::getInstance();
    QuestionDlg dlg(topLevelWidget);
    dlg.setQuestionMessage(tr("Do you want to stop the sequence?"));

    if(dlg.exec() == QDialog::Rejected)
        return;

    d->mIsStarted = false;
    if(d->mRunningStatusWidget.isNull() == false)
    {
        d->mRunningStatusWidget->onTimerStop();
        d->mRunningStatusWidget->onStopAnalysisSequence();
    }
}

void WorkPlaceView::onCameraInitFail()
{
    auto topLevelWidget = MainWindow::getInstance();

    QuestionDlg dlg(topLevelWidget);
    dlg.setQuestionMessage(tr("[Stopped due to camera connection error]: Do you want to stop the full sequence?"));

    if(dlg.exec() == QDialog::Rejected)
    {
        d->mCameraInstance->restart();
        return;
    }

    d->mIsStarted = false;
    CLogWriter::printLog("[WorkPlaceView::] : FullSequence - Camara Init Fail", CLogWriter::LOG_TYPE_RELEASE);

    if(d->mRunningStatusWidget.isNull() == false)
    {
        d->mRunningStatusWidget->onTimerStop();
        d->mRunningStatusWidget->onStopAnalysisSequence();
    }
}

void WorkPlaceView::onHideButton()
{
    ui->widgetButton->hide();
}

bool WorkPlaceView::onStackSequenceProtocolForFullSequence()
{
    d->mSequenceQueue.clear();

    auto currentSequence = d->mConfigInstance->getLastSelectedSequence();

    auto protocolTime = this->createRequestFullSequenceTimeProtocol();
    d->mSequenceQueue.append(protocolTime);

    auto inhalantCount = 0;
    auto dummyCount = 0;
    auto etcCount = 0;

    this->getContentsCount(inhalantCount, etcCount);

    auto totalCount = inhalantCount + etcCount;
    this->onQueuedSend(WriteProtocolControlWrite::writeSampleTotalNumber(inhalantCount, dummyCount, etcCount, totalCount).toUtf8());
    d->mSequenceQueue.append(WriteProtocolControlWrite::writeSequenceStart(currentSequence).toUtf8());

    return true;
}

void WorkPlaceView::onStart()
{    
   if(d->mIsStripScan == false)
        this->onStartGeneral();
   else
        this->onStartStripScan();
}

void WorkPlaceView::onStartGeneral()
{
    if(this->onStepProcessing() == false)
        return;

    this->autoSavePatientData();
    d->mStartTimer.start();
}

void WorkPlaceView::onStartStripScan()
{
    emit onDisableMenuWidget();

    d->mCurrentStep = WORKPLACE_STEP_PROCESSING;

    this->stripScanButtonHide();
    this->allWindowHide();


    if(this->savePatientData(d->mResultData) == false)
    {
        CUtil::messageBox(tr("Can not Save PatientData"));
        CLogWriter::printLog("[WorkPlaceView::] : [DataBase] Patient Data Save Fail", CLogWriter::LOG_TYPE_RELEASE);
        return;
    }
    this->printLogLotAndPatientData(d->mResultData);

    this->autoSavePatientData();

    if(d->mRunningStatusWidget.isNull() == false)
    {
        d->mRunningStatusWidget->show();
        d->mRunningStatusWidget->stripScan(d->mResultData);
        emit onRunningStarted();
    }
}

void WorkPlaceView::onStartStripScanWithoutSave()
{
    auto topLevelWidget = MainWindow::getInstance();

    QuestionDlg dlg(topLevelWidget);
    dlg.setQuestionMessage(tr("Do you want to Start?"));

    if(dlg.exec() == QDialog::Rejected)
        return;

    emit onDisableMenuWidget();

    d->mCurrentStep = WORKPLACE_STEP_PROCESSING;

    this->stripScanButtonHide();
    this->allWindowHide();

    if(d->mRunningStatusWidget.isNull() == false)
    {
        auto lastData = ConfigInformation::getInstance()->getLastAnalysisData();
        auto filteredAnanlysisData = this->failureAnalysisDataFilter(lastData);

        d->mRunningStatusWidget->show();
        d->mRunningStatusWidget->stripScan(filteredAnanlysisData);
        emit onRunningStarted();
    }
}

void WorkPlaceView::onDisableButtonForShortTime()
{
    ui->btnPause->setDisabled(true);
    ui->btnStop->setDisabled(true);

    QTimer::singleShot(10000, this,[this]
                       {
                           this->enablePauseButton();
                           this->enableStopButton();
                       });
}

bool WorkPlaceView::updateGeneralPatientInfomationData()
{
    bool isMissingData = this->copyFromUIToAnalysisOrderedData();
    this->copyFromAnalysisDataToResultData();

    auto topLevelWidget = MainWindow::getInstance();

    if(isMissingData == true)
    {
        QuestionDlg dlg(topLevelWidget);
        dlg.setQuestionMessage(this->getMissingQuestionMessageText());

        if(dlg.exec() == QDialog::Rejected)
            return false;
    }

    if(d->mAnalysisData.isEmpty() == true)
    {
        MessageShowDlg* dlg = new MessageShowDlg(this->getDataEmptyMessageText(), topLevelWidget);
        dlg->exec();
        return false;
    }

    return true;
}

bool WorkPlaceView::updateStripScanPatientInfomationData()
{
    bool isMissingData = this->copyFromUIToAnalysisData();
    this->copyFromAnalysisDataToResultData();

    auto topLevelWidget = MainWindow::getInstance();

    if(isMissingData == true)
    {
        QuestionDlg dlg(topLevelWidget);
        dlg.setQuestionMessage(this->getMissingQuestionMessageText());

        if(dlg.exec() == QDialog::Rejected)
            return false;
    }

    if(d->mAnalysisData.isEmpty() == true)
    {
        MessageShowDlg* dlg = new MessageShowDlg(this->getDataEmptyMessageText(), topLevelWidget);
        dlg->exec();
        return false;
    }

    return true;
}

bool WorkPlaceView::copyFromUIToAnalysisOrderedData()
{
    d->mAnalysisData.clear();
    auto guiData = d->mPatientListWidget->getData();
    auto isMissing = false;

#pragma omp parallel for ordered
    for(int i = 0; i < guiData.count(); i++)
    {
#pragma omp ordered
        {
            auto isCompleted = this->isCompletedPatientRowData(guiData[i]);
            auto isIncompleted = this->isIncompletedPatientRowData(guiData[i]);

            if(isCompleted)
                d->mAnalysisData.push_back(guiData[i]);

            else if (isIncompleted)
                isMissing = true;
        }
    }

    std::sort(d->mAnalysisData.begin(), d->mAnalysisData.end(), WorkPlaceView::sortPanelNameData);

    this->numberingStripNumber();

    return isMissing;
}

bool WorkPlaceView::copyFromUIToAnalysisData()
{
    d->mAnalysisData.clear();
    auto guiData = d->mPatientListWidget->getData();
    auto isMissing = false;

#pragma omp parallel for ordered
    for(int i = 0; i < guiData.count(); i++)
    {
#pragma omp ordered
        {
            auto isCompleted = this->isCompletedPatientRowData(guiData[i]);
            auto isIncompleted = this->isIncompletedPatientRowData(guiData[i]);

            if(isCompleted)
                d->mAnalysisData.push_back(guiData[i]);

            else if (isIncompleted)
                isMissing = true;
        }
    }

    this->numberingStripNumber();

    return isMissing;
}

bool WorkPlaceView::sortStripNumberData(GlobalDataStruct::TABLE_ITEM_ELEMENT& a, GlobalDataStruct::TABLE_ITEM_ELEMENT& b)
{
    return a.stripNumber < b.stripNumber;
}

bool WorkPlaceView::sortPanelNameData(GlobalDataStruct::TABLE_ITEM_ELEMENT& a, GlobalDataStruct::TABLE_ITEM_ELEMENT& b)
{
    if(a.panelName == b.panelName)
        return WorkPlaceView::sortStripNumberData(a,b);

    auto config = ConfigInformation::getInstance();
    auto aPanel = config->getCurrentContentPanelIndex(a.panelName);
    auto bPanel = config->getCurrentContentPanelIndex(b.panelName);

    return (aPanel < bPanel);
}

void WorkPlaceView::toRunningStatus(const QVector<GlobalDataStruct::AnalysisReportInfomation>& dbData, bool isRestart,
                                    const QVector<QByteArray>& sequence)
{
    emit onDisableMenuWidget();

    d->mIsStarted = true;
    d->mCurrentStep = WORKPLACE_STEP_PROCESSING;
    d->mResultData = dbData;

    this->allWindowHide();

    ui->btnBack->hide();
    ui->btnFront->hide();
    ui->widgetButton->show();

    if(isRestart == true)
        ui->btnStart->setDisabled(true);

    if(d->mRunningStatusWidget.isNull() == false)
    {
        d->mRunningStatusWidget->show();
        d->mRunningStatusWidget->start();

        if(isRestart == true)
            d->mRunningStatusWidget->reStartStatus(dbData);
        else
            d->mRunningStatusWidget->startSequence(sequence, dbData);
    }
}

void WorkPlaceView::setForceStripScan()
{
    if(d->mPatientListWidget.isNull() == false)
        d->mPatientListWidget->setForceStripScanRadioButton();
}

void WorkPlaceView::onAnalysisFinished(QVector<GlobalDataStruct::AnalysisReportInfomation> result)
{
    d->mIsStarted = false;

    this->resultStatisticsAndSaveImage(result);

    auto contentsStr = d->mConfigInstance->getCurrentContents();
    auto allergyStr = d->mConfigInstance->getContentsNameFromEnum((int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY);
    auto foodIntoleranceStr = d->mConfigInstance->getContentsNameFromEnum((int)ConfigInformation::STANDARD_CONTETNS_LIST::FOOD_INTOLERANCE);

    auto isALLERGY = (contentsStr == allergyStr);
    auto isFoodIntolerance = (contentsStr == foodIntoleranceStr);

    if(isALLERGY == true)
    {
        QScopedPointer<IDataClassifier> calcClassData(new DataClassifierBase);

        if(calcClassData->calcClassData(result, d->mCodeFuncDataBase.data()) == false)
        {
            CUtil::messageBox(tr("Can not Calc ClassData"));
            CLogWriter::printLog(QString("[WorkPlaceView::] : [DataBase] Calc ClassData Fail"), CLogWriter::LOG_TYPE_RELEASE);
            return;
        }
    }
    else if(isFoodIntolerance)
    {
        QScopedPointer<IDataClassifier> calcClassData(new DataClassifierBase);

        if(calcClassData->calcClassData(result, d->mCodeFuncDataBase.data()) == false)
        {
            CUtil::messageBox(tr("Can not Calc ClassData"));
            CLogWriter::printLog(QString("[DataBase] Calc ClassData Fail"), CLogWriter::LOG_TYPE_RELEASE);
            return;
        }
    }


    if(this->isNewDBData(result) == true)
    {
        if(this->saveResultData(result) == false)
        {
            CUtil::messageBox(tr("Can not Save ResultData"));
            CLogWriter::printLog(QString("[WorkPlaceView::] : [DataBase] Result Data Save Fail"), CLogWriter::LOG_TYPE_RELEASE);
            return;
        }
    }
    else
    {
        if(this->updateResultData(result) == false)
        {
            CUtil::messageBox(tr("Can not Update ResultData"));
            CLogWriter::printLog(QString("[WorkPlaceView::] : [DataBase] Result Data Update Fail"), CLogWriter::LOG_TYPE_RELEASE);
            return;
        }

        this->overwriteLastFailureData(result);
    }

    this->checkFailDataThenShowMessage(result);

    emit onResultShow(result);
    emit onEnableMenuWidget();
}

void WorkPlaceView::makeUniqueIDAndDateTime(GlobalDataStruct::AnalysisReportInfomation& element) const
{
    if(element.uniqueID.isEmpty() == true)
        element.uniqueID = QUuid::createUuid().toString();

    const auto dateTime = QDateTime::currentDateTime();
    element.dateTime = dateTime;
}

bool WorkPlaceView::savePatientData(QVector<GlobalDataStruct::AnalysisReportInfomation>& element) const
{
    QVector<QVariantMap> queryData;

    for(int i =0; i< element.count() ; i++)
    {
        this->makeUniqueIDAndDateTime(element[i]);
        auto data = BasicDatabaseCtrl::makePatientData(element[i]);
        queryData.push_back(data);
    }

    return d->mBasicDataBase->insertData("PATIENT_TABLE", queryData);
}

bool WorkPlaceView::saveResultData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& element) const
{
    if(element.isEmpty() == true)
        return false;

    QVector<QVariantMap> queryData;

    for(int i =0; i< element.count() ; i++)
    {
        auto data = BasicDatabaseCtrl::makeResultData(element[i]);
        queryData.push_back(data);        
    }

    return d->mBasicDataBase->insertData("RESULT_TABLE", queryData);
}

bool WorkPlaceView::saveFuncFormulaData(const QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT>& list) const
{
    QString tableName("FUNC_TABLE");
    QString columnNameFunc = d->mCodeFuncDataBase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FUNC, tableName);

    QVector<QString> whereColumn;
    QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> insertData;
    QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> updateData;

    for(auto& itr : list )
    {
        QString query = QString("SELECT * FROM %1 WHERE %2 = '%3'").arg(tableName).arg(columnNameFunc).arg(itr.newFunctionNumber);

        if(d->mCodeFuncDataBase->select(query).isEmpty() == true)
            insertData.push_back(itr);
        else
            updateData.push_back(itr);
    }

    auto insertVariantMap = CodeFuncDatabaseCtrl::makeFuncData(insertData);
    auto updateVariantMap = CodeFuncDatabaseCtrl::makeFuncData(updateData);

    whereColumn << columnNameFunc;

    if(d->mCodeFuncDataBase->insertData(tableName, insertVariantMap) == false)
        return false;

    if(d->mCodeFuncDataBase->updateData(tableName, updateVariantMap, whereColumn) == false)
        return false;

    return true;
}

bool WorkPlaceView::saveFuncStripRelData(const QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT>& list) const
{
    const QString tableName("STRIP_REL_TABLE");
    const QString columnNameLotNumber = d->mCodeFuncDataBase->convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::LOT_NUMBER, tableName);

    this->checkQRStripRelMissingDataAndSave(list, tableName, columnNameLotNumber);

    return true;
}

bool WorkPlaceView::updateResultData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result) const
{
    QVector<QVariantMap> updateVariantMap;
    QVector<QString> whereColumn;
    QString tableName("RESULT_TABLE");
    QString resultColumnUniqueIDStr = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), tableName);

    for(auto&itr : result)
        updateVariantMap << BasicDatabaseCtrl::makeResultData(itr);

    whereColumn << resultColumnUniqueIDStr;

    if(d->mBasicDataBase->updateData(tableName, updateVariantMap, whereColumn) == false)
        return false;

    return true;
}

bool WorkPlaceView::isNewDBData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result) const
{
    QString tableName("RESULT_TABLE");
    QString resultColumnUniqueIDStr = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), tableName);

    for(auto& itr : result)
    {
        QString query = QString("SELECT * FROM %1 WHERE %2 = '%3'").arg(tableName).arg(resultColumnUniqueIDStr).arg(itr.uniqueID);

        if(d->mBasicDataBase->select(query).isEmpty() == false)
            return false;
    }

    return true;
}

void WorkPlaceView::overwriteLastFailureData(QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    auto lastAnalysisData = ConfigInformation::getInstance()->getLastAnalysisData();

    for(auto& newDataItr: result)
    {
        for(auto& originaDatalItr: lastAnalysisData)
        {
            if(originaDatalItr.uniqueID == newDataItr.uniqueID)
            {
                originaDatalItr = newDataItr;
                break;
            }
        }
    }

    result = lastAnalysisData;
}

void WorkPlaceView::checkFailDataThenShowMessage(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    QString failureList;

    for(auto& itr : result)
    {
        if(itr.result.isSuccess == false)
        {
            if(failureList.isEmpty() == false)
                failureList.append(", ");

            failureList.append(QString("%1").arg(itr.element.stripNumber));
        }
    }

    if(failureList.isEmpty())
        return;

    auto topLevelWidget = MainWindow::getInstance();
    auto dlg = new MessageShowDlg(tr("The analysis of the following strip has finally failed. \n [Strip] : %1").arg(failureList), topLevelWidget);
    dlg->open();
}

void WorkPlaceView::sendPause(bool isPause)
{
    auto data = WriteProtocolControlWrite::writePause(isPause).toUtf8();
    this->onQueuedSend(data);
}

QVector<QByteArray> WorkPlaceView::createRequestFullSequenceTimeProtocol()
{
    QVector<QByteArray> protocols;
    auto currentContents = d->mConfigInstance->getCurrentContents();
    int count = d->mConfigInstance->getSequenceList(currentContents).count();

    for(int i = 0; i < count; i++)
    {
        auto data = WriteProtocolStatusRead::writeStepTimeRead(i+1).toUtf8();
        protocols.push_back(data);
    }

    return protocols;
}

void WorkPlaceView::moveCameraPosition(int idx)
{
    if(idx <= 0)
        return ;

    auto protocol =  WriteProtocolCameraLedSetting::writePostCameraPosMoveFinishedReq(idx).toUtf8();
    this->onQueuedSend(protocol);
    d->mConfigInstance->setCurrentStripPoint(idx);
}

void WorkPlaceView::moveTripleQRPosition()
{    
    int cameraQrTrayMoveDelay = 2000;

    auto positionHome = WriteProtocolControlRead::writeXAxisHome().toUtf8();
    this->onQueuedSend(positionHome);

    QTimer::singleShot(cameraQrTrayMoveDelay, this, [this]
    {
        auto positionProtocol = WriteProtocolBarcodePage::writeQRTrayMove().toUtf8();
        this->onQueuedSend(positionProtocol);
    });
}

void WorkPlaceView::moveTripleQrPositionCallBack()
{
    QTimer::singleShot(PROBE_RETURN_TIME, this, [this]
                       {
                           this->readTripleQRCode();
                           this->saveQRFuncRelDatas();
                           this->topLevelWidgetEnable();
                           d->mStripQRProgressDlg->hide();

                           if(this->checkQRStripMissingData() == false)
                               return;

                           if(this->checkQRFuncRelMissingData() == false)
                               return;

                           if(this->isvalidPanelData() == false)
                               return;

                           this->saveLotData();
                           this->cleanupQRModule();
                           emit onQRReadCompleted();
                       });
}

void WorkPlaceView::readQRCode(bool isStripScan)
{
    d->mIsStripScan = isStripScan;
    d->mIsQRReading = true;

    this->cleanupQRConfigInformation();
    this->cleanupQRModule();
    this->topLevelWidgetDisable();
    this->qrReadProcessing();
}

void WorkPlaceView::readStripQRCode()
{
    cv::Rect qrRect;
    auto fixedQRRect = d->mConfigInstance->getQRStripFixedROI();
    qrRect.x = fixedQRRect.x();
    qrRect.y = fixedQRRect.y();
    qrRect.width = fixedQRRect.width();
    qrRect.height = fixedQRRect.height();

    QVector<QString> decodedStringList;
    d->mCameraInstance->flushBuffer();

    for(int i =0 ; i < d->QR_RETRY_COUNT ; i++)
    {
        auto matrix = d->mCameraInstance->getFullConfigureImage(qrRect);

        if(matrix.data == nullptr)
            continue;

        auto currentFlip = d->mConfigInstance->getFlip();

        if(currentFlip != 0)
            CameraInstance::flip(matrix, currentFlip);

        auto stripPos = ConfigInformation::getInstance()->getCurrentStripPoint();
        this->imageSave(matrix, "QR-Debug-Strip", stripPos);

        decodedStringList = CQRCode::deCodeImage(matrix);

        if(decodedStringList.isEmpty() == false)
            break;;

        this->thread()->msleep(1000);
    }

    if(d->mRescaningStripIdx == 0)
    {
        if(decodedStringList.isEmpty() == false)
            d->mStripLotList << decodedStringList;
        else
            d->mStripLotList << QString("");

        qDebug() << "[WorkPlaceView::readStripQRCode]" << "idx:" << d->mStripLotList.count() << "qr:"<< decodedStringList;
    }
    else
    {
        if(decodedStringList.isEmpty() == false)
            d->mStripLotList[d->mRescaningStripIdx-1] = decodedStringList.first();
        else
            d->mStripLotList[d->mRescaningStripIdx-1] = QString("");

        qDebug() << "[WorkPlaceView::readStripQRCode]" << "idx:" << d->mRescaningStripIdx << "qr:"<< decodedStringList;
    }
}

void WorkPlaceView::readTripleQRCode()
{
    QVector<QString> decodedStringList;

    d->mCameraInstance->flushBuffer();

    for(int i =0; i < d->QR_RETRY_COUNT ; i++)
    {
        auto matrix = this->getTripleQRImage();

        if(matrix.data == nullptr)
            continue;

        decodedStringList = CQRCode::deCodeImage(matrix);

        if(decodedStringList.count() != d->QR_FUNC_COUNT)
        {
            this->thread()->msleep(1000);
            continue;
        }

        if(decodedStringList.isEmpty() == false)
            break;

        this->thread()->msleep(1000);
    }

    d->mIsQRReading = false;

    QVector<QString> qrRelParts;
    QVector<QString> newFunctionList;

    for(auto& itr : decodedStringList)
    {
        auto typeFlag = CQRCode::qrFlagParser(itr);

        if(typeFlag == "A")
        {
            if(CQRCode::isExpQRData(itr))
            {
                this->parsingExpQRData(itr);
                continue;
            }
            else
                qrRelParts.push_back(itr);
        }

        else if(typeFlag == "B")
            newFunctionList.push_back(itr);
    }

    qDebug() << "[WorkPlaceView::readTripleQRCode] : " << decodedStringList;


    auto isStripRelSuccess = false;
    auto isNewFormualSuccess = false;

    d->mStripRel =  CQRCode::qrAssemblyOfPartsAndParsing(qrRelParts, isStripRelSuccess);
    d->mNewFormula = CQRCode::qrFunctionNewFunctionParser(newFunctionList, isNewFormualSuccess);

    auto isQRFailRel = (isStripRelSuccess == false);
    auto isQRFailNew = (isNewFormualSuccess == false);

    if(isQRFailRel || isQRFailNew)
    {
        auto topLevelWidget = MainWindow::getInstance();
        auto msg = new MessageShowDlg(tr("Error in QR format or previous version. Please remove the QR."), topLevelWidget);
        msg->exec();
        return;
    }

    qDebug() << "[TEST]" << decodedStringList;
}

bool WorkPlaceView::showManualInputDialog(const QVector<int> &missingStripQRList, QMap<int , QString>& resultData)
{
    auto topLevelWidget = MainWindow::getInstance();

    QRStripCheckDlg dlg(topLevelWidget);
    dlg.setAnalysisData(d->mAnalysisData);
    dlg.setMissingQRList(missingStripQRList);

    if(dlg.exec() == QDialog::Rejected)
        return false;

    resultData = dlg.getInputedLotData();
    return true;
}

bool WorkPlaceView::isvalidPanelData()
{
    QVector<int> invalidStripList;

    for(int i = 0 ; i < d->mStripLotList.count() ; i++)
    {
        auto fromQRPanelName = CQRCode::getPanelName(d->mStripLotList[i]);

        for(int idx = 0; idx < d->mResultData.count() ; idx++)
        {
            auto currentElement = d->mResultData[idx].element;
            auto stripNumber = i + 1;

            if(currentElement.stripNumber == stripNumber)
            {
                qDebug() <<  "currentElement.stripNumber" <<currentElement.stripNumber <<"stripNumber" << stripNumber << "current" << currentElement.panelName << "from" << fromQRPanelName;

                if(currentElement.panelName != fromQRPanelName)
                {
                    d->mStripLotList[i] = QString();
                    invalidStripList << stripNumber;
                }
                break;
            }
        }
    }

    if(invalidStripList.isEmpty() == false)
    {
        QString invalidStripListText;

        for(auto i = 0; i < invalidStripList.count(); i++)
        {
            auto itr = invalidStripList[i];

            if(invalidStripListText.isEmpty() == false)
            {
                invalidStripListText.append(", ");

                if(i%30 == 0)
                    invalidStripListText.append("\n");
            }


            invalidStripListText.append(QString::number(itr));
        }

        QString message;
        message.append(tr("The QR Panel data does not match the patient information data. \n Please check the strips. : %1 ").arg(invalidStripListText));

        auto topLevelWidget = MainWindow::getInstance();
        QuestionDlg dlg(topLevelWidget);
        dlg.setQuestionMessage(message);

        if(dlg.exec() == QDialog::Accepted)
        {
            this->cameraPosHoming();
            QTimer::singleShot(25, this, [this] { d->mWaitingQRCodeInitializing->startWaiting();});
            QTimer::singleShot(PROBE_RETURN_TIME, this, [this]
                               {
                                   d->mWaitingQRCodeInitializing->stopWaiting();
                                   this->qrRescaningProcessing();
                               });
            return false;
        }

        this->cleanupQRModule();
        this->blockUIForProbeInit();
        return false;
    }

    return true;
}

bool WorkPlaceView::checkQRFuncRelMissingData()
{
    if(this->checkContainsQRDataFromDB() == false)
    {
        auto topLevelWidget = MainWindow::getInstance();
        MessageShowDlg* dlg = new MessageShowDlg(tr("Required QR code does not exist.\n Please contact the manufacturer."), topLevelWidget);
        dlg->exec();
        this->cleanupQRModule();
        this->blockUIForProbeInit();
        return false;
    }

    return true;
}

bool WorkPlaceView::checkQRStripMissingData()
{
    QVector<int> missingStripQRList = this->checkMissingStripQRList();
    auto topLevelWidget = MainWindow::getInstance();

    QMap<int , QString> editList;
    this->topLevelWidgetEnable();

    if(this->showManualInputDialog(missingStripQRList, editList) == false)
    {
        MessageShowDlg* dlg = new MessageShowDlg(tr("Required data input has been canceled."), topLevelWidget);
        dlg->exec();
        this->cleanupQRModule();
        this->blockUIForProbeInit();
        return false;
    }

    QMapIterator<int, QString> itr(editList) ;

    while(itr.hasNext())
    {
        itr.next();
        auto currentIdx = itr.key() - 1;

        if((currentIdx < 0) || (d->mStripLotList.count() <=  currentIdx))
            break;

        auto editBoxText = itr.value();
        d->mStripLotList[currentIdx] = editBoxText;
    }

    return true;
}

bool WorkPlaceView::checkContainsQRDataFromDB()
{
    QString tableName("STRIP_REL_TABLE");
    QString columnNameLotNumber = d->mCodeFuncDataBase->convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::LOT_NUMBER, tableName);
    QString selectQuery = QString("SELECT %1 FROM %2 ").arg(columnNameLotNumber).arg(tableName);

    auto queryResult = d->mCodeFuncDataBase->select(selectQuery);


    for(auto& itr : d->mStripLotList)
    {
        bool isContains = false;

        for(auto& rowitr : queryResult)
        {
            if(rowitr[columnNameLotNumber].toString() == itr)
            {
                isContains = true;
                break;
            }
        }

        if(isContains == false)
            return false;
    }

    return true;
}

QVector<int> WorkPlaceView::checkMissingStripQRList()
{
    QVector<int> missingStripQRList;

    for(int i = 0; i < d->mStripLotList.count(); i++)
    {
        if(d->mStripLotList[i].isEmpty() == true)
            missingStripQRList << i + 1;
    }

    return missingStripQRList;
}

void WorkPlaceView::checkQRStripRelMissingDataAndSave(const QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> &list, const QString& tableName, const QString& columnNameLotNumber) const
{
    QVector<QVariantMap> insertList;

    for(auto& itr : list)
    {
        QString where = QString("%1 = '%2'").arg(columnNameLotNumber).arg(itr.lotNumber);
        QString selectQuery = QString("SELECT %1 FROM %2 WHERE %3").arg(columnNameLotNumber).arg(tableName).arg(where);
        auto queryResult = d->mCodeFuncDataBase->select(selectQuery);

        if(queryResult.isEmpty() == true)
        {
            auto dbData = CodeFuncDatabaseCtrl::makeStripRelData(itr);
            d->mCodeFuncDataBase->insertData(tableName, dbData);
        }
    }
}

void WorkPlaceView::parsingExpQRData(const QString &expData)
{
    GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT parsingData = CQRCode::getLotExpData(expData);

    if(parsingData.lotNumber.isEmpty())
        return;

    if(isExistLotCutoff(parsingData.lotNumber))
        return;

    this->saveLotOptionalData(parsingData);
}

bool WorkPlaceView::isExistLotCutoff(const QString &lotNumber)
{
    QString tableName("LOT_OPTIONAL_TABLE");
    QString colunmName("LOT_NUMBER");

    QString selectQuery = QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
            .arg(colunmName)
            .arg(tableName)
            .arg(colunmName)
            .arg(lotNumber);

    auto result  = d->mCodeFuncDataBase->select(selectQuery);

    if(result.count() > 0)
        return true;

    return false;
}

bool WorkPlaceView::saveLotOptionalData(const GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT &list)
{
    QString tableName("LOT_OPTIONAL_TABLE");

    auto query  = this->makeQROptionalData(list);
    return d->mCodeFuncDataBase->insertData(tableName,query);
}

QVariantMap WorkPlaceView::makeQROptionalData(const GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT &report) const
{
    QVariantMap query;

    QString tableName("LOT_OPTIONAL_TABLE");
    QString lotName = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::LOT_OPTIONAL_TABLE::LOT_NUMBER), tableName);
    QString cutoff = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::LOT_OPTIONAL_TABLE::CUT_OFF), tableName);
    QString expData = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::LOT_OPTIONAL_TABLE::EXP_DATE), tableName);

    query[lotName] = report.lotNumber;
    query[cutoff] = report.cutoff;
    query[expData] = report.expDate;

    return query;
}


void WorkPlaceView::cleanupQRConfigInformation()
{
    d->mStripLotList.clear();
    d->mStripRel.clear();
    d->mNewFormula.clear();
}

void WorkPlaceView::cleanupQRModule()
{
    this->writeLedOneTimeClear();
    d->mCameraInstance->analysisLedOFF();
    d->mCameraInstance->instanceStop();
    this->cameraPosHoming();
    this->disConnectQRConnection();

    d->mStripQRProgressDlg.reset();
    d->mRescaningStripIdx = 0;
}

void WorkPlaceView::blockUIForProbeInit()
{
    QTimer::singleShot(50, this, [this]
                       {
                           this->topLevelWidgetDisable();
                           d->mWaitingQRCodeInitializing->startWaiting();
                       });

    QTimer::singleShot(PROBE_RETURN_TIME, this, [this]
                       {
                           d->mWaitingQRCodeInitializing->stopWaiting();
                           this->topLevelWidgetEnable();
                       });
}

void WorkPlaceView::cameraPosHoming()
{

    QString protocolStr = WriteProtocolControlRead::writeXAxisHome();
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);

    d->mConfigInstance->setCurrentStripPoint(1);
}

void WorkPlaceView::qrReadProcessing()
{
    this->disConnectQRConnection();
    d->mCameraConnection = connect(d->mCameraInstance, &CameraInstance::onFinishedInitalizing, this, [this]
                                   {
                                       this->writeLedOneTimeAmplification();


                                       d->mQRReadConnection = connect(this, &WorkPlaceView::onReadReady, this, [this]
                                                                      (QByteArray recvData)
                                                                      {
                                                                          this->onQRReadCallBack(recvData);
                                                                      });

                                       QTimer::singleShot(1000, this, [this]
                                                          {
                                                               d->mStripQRReadCount = 1;
                                                              this->createAndInitProgressDialog();
                                                              this->topLevelWidgetDisable();

                                                              for(auto i : d->mAnalysisData)
                                                                  d->mStripLotList << "";

                                                              this->moveTripleQRPosition();
                                                          });
                                   });

    d->mNetworkStatusConnection = connect(this, &WorkPlaceView::onConnectedNetwork, this, &WorkPlaceView::onSerialConneted);

    d->mCameraInstance->instanceStart();
}

void WorkPlaceView::qrRescaningProcessing()
{
    d->mRescaningStripList = this->checkMissingStripQRList();
    d->mRescaningQRCount = d->mRescaningStripList.count();
    d->mRescaningStripIdx = 0;
    this->disConnectQRConnection();

    if(d->mRescaningStripList.isEmpty() == true)
        return;

    d->mQRReadConnection = connect(this, &WorkPlaceView::onReadReady, this, [this]
                                   (QByteArray recvData)
                                   {
                                       this->onQRReadRescaningCallBack(recvData);
                                   });


    QTimer::singleShot(1000, this, [this]
                       {
                           d->mRescaningStripIdx = d->mRescaningStripList.takeFirst();
                           this->topLevelWidgetDisable();

                           for(auto i : d->mAnalysisData)
                               d->mStripLotList << "";

                           this->moveTripleQRPosition();
                       });
}

void WorkPlaceView::disConnectQRConnection()
{
    disconnect(d->mQRReadConnection);
    disconnect(d->mCameraConnection);
    disconnect(d->mNetworkStatusConnection);

    d->mQRReadConnection = QMetaObject::Connection();
    d->mCameraConnection = QMetaObject::Connection();
    d->mNetworkStatusConnection = QMetaObject::Connection();
}

void WorkPlaceView::commonStartGUIProcessing()
{
    d->mIsStarted = true;

    this->enablePauseButton();
    this->enableStopButton();
    this->resumeRunningStatus();
}

void WorkPlaceView::saveQRFuncRelDatas()
{
    this->saveFuncStripRelData(d->mStripRel);
    this->saveFuncFormulaData(d->mNewFormula);
}

void WorkPlaceView::saveLotData()
{
    for(auto& itr : d->mResultData)
    {
        for(int idx = 0 ; idx < d->mStripLotList.count(); idx++)
        {
            if(itr.element.stripNumber == (idx+1))
            {
                itr.lotNumber = d->mStripLotList[idx];
                break;
            }
        }
    }
}

void WorkPlaceView::stripScanButtonHide()
{
    ui->btnFront->hide();
    ui->btnBack->hide();
    ui->widgetButton->hide();
    ui->btnStart->hide();
    ui->btnPause->hide();
    ui->btnStop->hide();
}

void WorkPlaceView::imageSave(const cv::Mat& matrix, QString fileName, int stripNumber, const QString& filePath)
{
    QString fullPath;

    if(filePath.isEmpty() == true)
        fullPath = "AnalysisImage";
    else
        fullPath = filePath;

    fileName.prepend(QString("[Strip%1]-").arg(stripNumber));
    fileName.append(".png");

    fullPath.append("/");
    fullPath.append(fileName);

    cv::imwrite(fullPath.toStdString().data(), matrix);
}

void WorkPlaceView::onRecvData(QByteArray recvData)
{
    if(PacketReader::modeReader(recvData) != MD_MODE::MD_MODE_GENERAL_SETTING)
        return;

    switch(PacketReader::commandReader(recvData))
    {

    }
}

void WorkPlaceView::onQRReadCallBack(const QByteArray& recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_CONTROL_WRITE:
    {
        switch(PacketReader::commandReader(recvData))
        {
        case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_REQ:
        {
            int cameraMoveDelay = 2000;

            if(d->mStripQRReadCount == 1)
                cameraMoveDelay = 30000;

            QTimer::singleShot(cameraMoveDelay, this,[this]
                               {
                                   this->readStripQRCode();
                                   auto oneStep = 100.0/(d->mAnalysisData.count() + 1);
                                   d->mStripQRProgressDlg->setValue((oneStep * d->mStripQRReadCount));

                                   if(d->mAnalysisData.count() <= d->mStripQRReadCount )
                                   {
                                       this->moveTripleQRPosition();
                                       return;
                                   }

                                   this->moveCameraPosition(++d->mStripQRReadCount);
                               });
        }break;



        case CMD_QR_SETTING_MODE::CMD_SETTING_MODE_QR_TRAY_POS_MOVE:
            this->moveTripleQrPositionCallBack();
            break;

        }


    }break;
    }
}

void WorkPlaceView::onQRReadRescaningCallBack(const QByteArray& recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_CONTROL_WRITE:
    {
        switch(PacketReader::commandReader(recvData))
        {
        case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_REQ:
        {
            int nextPositon = 0;
            int calcMoveDelay = 0;

            if(d->mRescaningStripList.isEmpty() == false)
                nextPositon = d->mRescaningStripList.first();

            if(nextPositon != 0)
                calcMoveDelay = abs(nextPositon - d->mConfigInstance->getCurrentStripPoint())+ 1;

            int cameraMoveDelay = 2000 + (500 * calcMoveDelay);

            QTimer::singleShot(cameraMoveDelay, this,[this]
                               {
                                   this->readStripQRCode();
                                   auto oneStep = 100.0/(d->mRescaningQRCount + 1);
                                   auto etcCount = d->mRescaningQRCount - d->mRescaningStripList.count();

                                   d->mStripQRProgressDlg->setValue((oneStep * etcCount));

                                   if(d->mRescaningStripList.isEmpty())
                                   {
                                       this->moveTripleQRPosition();
                                       return;
                                   }

                                   d->mRescaningStripIdx = d->mRescaningStripList.takeFirst();
                                   this->moveCameraPosition(d->mRescaningStripIdx);
                               });
        }break;


        case CMD_QR_SETTING_MODE::CMD_SETTING_MODE_QR_TRAY_POS_MOVE:
            this->moveTripleQrPositionCallBack();
            break;
        }

    }break;
    }
}

void WorkPlaceView::onSerialConneted()
{
    if(d->mIsQRReading)
    {
        QTimer::singleShot(500, this, []
                           {
                               auto topLevelWidget = MainWindow::getInstance();
                               topLevelWidget->setDisableTopLevelWidget(true);
                           });


        this->readQRCode(d->mIsStripScan);
    }
}

void WorkPlaceView::writeLedOneTimeAmplification()
{
    auto instance = CameraInstance::getInstance();
    auto parameterList = instance->getCameraPropertyList();

    if(parameterList.isEmpty())
    {
        CLogWriter::printLog("<CameraLightSetting::> ParameterRead: Can not Read");
        return;
    }

    auto exposure = parameterList[cv::CAP_PROP_EXPOSURE];
    instance->setCameraProperty(cv::CAP_PROP_EXPOSURE, exposure.defaultValue+1);
    this->thread()->sleep(1);
}

void WorkPlaceView::writeLedOneTimeClear()
{
}

void WorkPlaceView::topLevelWidgetEnable()
{
    auto mainViewInstance = MainWindow::getInstance();

    if(mainViewInstance != nullptr)
        mainViewInstance->setDisableTopLevelWidget(false);
}

void WorkPlaceView::topLevelWidgetDisable()
{
    auto mainViewInstance = MainWindow::getInstance();

    if(mainViewInstance != nullptr)
        mainViewInstance->setDisableTopLevelWidget(true);
}

void WorkPlaceView::createAndInitProgressDialog()
{
    auto topLevelWidget = MainWindow::getInstance();
    d->mStripQRProgressDlg.reset(new ProgressBarDlg(topLevelWidget));
    d->mStripQRProgressDlg->open();
}

void WorkPlaceView::autoSavePatientData()
{
    if(d->mPatientListWidget.isNull() == false)
    {
        d->mPatientListWidget->setData(d->mAnalysisData);
        d->mPatientListWidget->savePatientListData();
    }
}

void WorkPlaceView::rescaningForStripScan()
{
    if(this->checkAllMissingSample())
        return;

    this->onStartStripScanWithoutSave();
}

void WorkPlaceView::generalStripScan()
{
    if(this->updateStripScanPatientInfomationData() == false)
        return;

    this->readQRCode(true);
}

void WorkPlaceView::reagentStepUIControl()
{
    d->mReagentWidget->start();
    d->mReagentWidget->updateWidget(d->mAnalysisData, d->mConfigInstance->getLastSelectedSequence());
    d->mReagentWidget->show();

    ui->btnBack->setEnabled(true);
    ui->btnFront->setEnabled(false);
    ui->btnStart->setEnabled(true);
    ui->widgetButton->show();
    ui->btnStart->show();
    ui->btnPause->hide();
    ui->btnStop->hide();
}

void WorkPlaceView::processingStepUIControl()
{
    ui->btnStart->setDisabled(true);
    ui->btnPause->setDisabled(true);
    ui->btnStop->setDisabled(true);

    ui->btnFront->hide();
    ui->btnBack->hide();
    ui->widgetButton->show();
    ui->btnStart->show();
    ui->btnPause->show();
    ui->btnStop->show();
}

void WorkPlaceView::copyFromAnalysisDataToResultData()
{
    d->mResultData.clear();
    d->mResultData.resize(d->mAnalysisData.count());

    for(int i = 0; i < d->mAnalysisData.count(); i++)
    {
        d->mResultData[i].element = d->mAnalysisData[i];
        d->mResultData[i].element.contentsName = d->mConfigInstance->getCurrentContents();
    }
}

QVector<GlobalDataStruct::AnalysisReportInfomation> WorkPlaceView::failureAnalysisDataFilter(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList)
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> filteredData;

    if(analysisList.isEmpty() == true)
        return filteredData;

    for(auto& itr : analysisList)
    {
        if(itr.result.isSuccess == false)
            filteredData.push_back(itr);
    }

    return filteredData;
}

void WorkPlaceView::resultStatisticsAndSaveImage(QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    int success = 0;
    int fail = 0;

    for(auto& itr : result)
    {
        if(itr.result.isSuccess == true)
            success++;
        else
        {
            fail++;
            CLogWriter::printLog(QString("Fail Reason [%1]: %2 ").arg(QString::number(itr.element.stripNumber)).arg(itr.result.errorReason), CLogWriter::LOG_TYPE_RELEASE);
        }

        QString fileName = QString("%1_%2")
                               .arg(itr.dateTime.toString("yyyy-MM-dd-hh-mm-ss"))
                               .arg(itr.element.stripNumber, 2, 10, QChar('0'));

        auto fullPath =  BasicSupremeIAnalysis::resultImageSave(itr.result.image, fileName);
        itr.result.resultImagePath = fullPath;

        CLogWriter::printLog(QString("Idx [%1]: %2 ").arg(QString::number(itr.element.stripNumber)).arg(fullPath));
    }

    CLogWriter::printLog(QString("Success : %1 / Fail: %2 ").arg(QString::number(success)).arg(QString::number(fail)));
}

void WorkPlaceView::enableStartButton()
{
    ui->btnPause->setDisabled(true);
    ui->btnStart->setDisabled(true);
    ui->btnStart->setEnabledChange();
}

void WorkPlaceView::enablePauseButton()
{
    ui->btnPause->setDisabled(true);
    ui->btnStart->setDisabled(true);
    ui->btnPause->setEnabledChange();
}

void WorkPlaceView::enableStopButton()
{
    ui->btnStop->setDisabled(true);
    ui->btnStop->setEnabledChange();
}

void WorkPlaceView::numberingStripNumber()
{
#pragma omp parallel for ordered
    for(int i = 0 ; i < d->mAnalysisData.count(); i++)
#pragma omp ordered
        d->mAnalysisData[i].stripNumber = i+1;
}

bool WorkPlaceView::isCompletedPatientRowData(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element)
{
    auto isSelectedStripScan = d->mPatientListWidget->isSelectedStripScan();

    if(isSelectedStripScan)
        return this->isCompletedPatientRowDataForManual(element);
    else
        return this->isCompletedPatientRowDataForFullAuto(element);
}

bool WorkPlaceView::isIncompletedPatientRowData(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element)
{
    auto isSelectedStripScan = d->mPatientListWidget->isSelectedStripScan();

    if(isSelectedStripScan)
        return this->isIncompletedPatientRowDataForManual(element);
    else
        return this->isIncompletedPatientRowDataForFullAuto(element);
}

bool WorkPlaceView::isCompletedPatientRowDataForManual(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element)
{
    auto isvalidPatientID = (element.patientID.isEmpty() == false);
    auto isvalidPanel = (element.panelName.isEmpty() == false && element.panelName != "PANEL");

    if (isvalidPatientID && isvalidPanel )
        return true;
    else
        return false;
}

bool WorkPlaceView::isIncompletedPatientRowDataForManual(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element)
{
    auto isInvalidPatientID = element.patientID.isEmpty();
    auto isInvalidPanel = element.panelName.isEmpty() || (element.panelName == "PANEL");

    if(isInvalidPatientID && isInvalidPanel)
        return false;
    else
        return true;
}

bool WorkPlaceView::isCompletedPatientRowDataForFullAuto(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element)
{
    auto isvalidPatientID = (element.patientID.isEmpty() == false);
    auto isvalidPanel = (element.panelName.isEmpty() == false && element.panelName != "PANEL");

    if (isvalidPatientID && isvalidPanel )
        return true;
    else
        return false;
}

bool WorkPlaceView::isIncompletedPatientRowDataForFullAuto(const GlobalDataStruct::TABLE_ITEM_ELEMENT& element)
{
    auto isInvalidPatientID = element.patientID.isEmpty();
    auto isInvalidPanel = element.panelName.isEmpty() || (element.panelName == "PANEL");

    if(isInvalidPatientID && isInvalidPanel)
        return false;
    else
        return true;
}

void WorkPlaceView::buttonControlForStartedStatus()
{
    this->enableStartButton();
}

void WorkPlaceView::buttonControlForInitStatus()
{
    d->mStartTimer.stop();
    d->mIsStarted = true;

   this->enableStartButton();
}

void WorkPlaceView::resumeRunningStatus()
{
    if(d->mRunningStatusWidget.isNull() == false)
        d->mRunningStatusWidget->onPauseUIControl(false);
}

void WorkPlaceView::pauseRunningStatus()
{
    if(d->mRunningStatusWidget.isNull() == false)
        d->mRunningStatusWidget->onPauseUIControl(true);
}

void WorkPlaceView::doorOpen()
{
    if(d->mIsStarted)
        this->buttonControlForStartedStatus();
    else
        this->buttonControlForInitStatus();

    this->pauseRunningStatus();
}

QString WorkPlaceView::getMissingQuestionMessageText()
{
    QString messageText;
    messageText.append(tr("Data that is entered without essential information is not sent.(ID, PANEL, SAMPLE NO.) \n"));
    messageText.append(tr("Would you like to proceed?"));

    return messageText;
}

QString WorkPlaceView::getDataEmptyMessageText()
{
    return tr("The entered data does not exist.");
}

QString WorkPlaceView::getCrossUseMessageText()
{
    return tr("Combined is not allowed to cross-use.");
}

bool WorkPlaceView::checkAllMissingSample()
{
    auto isMissing = this->isAllMissingSample();

    if(isMissing)
        this->showMesageForRescanningAllMissingSample();

    return isMissing;
}

bool WorkPlaceView::isAllMissingSample()
{
    auto lastData = d->mConfigInstance->getLastAnalysisData();
    auto isAllMissing = true;

    for(auto&itr : lastData)
    {
        auto errorReason = itr.result.errorReason;

        if(errorReason.isEmpty())
            continue;

        auto isSampleMissing = this->isSampleMissingError(errorReason);

        if(isSampleMissing == false)
            isAllMissing = false;
    }

    return isAllMissing;
}

void WorkPlaceView::showMesageForRescanningAllMissingSample()
{
    auto topLevelWidget = MainWindow::getInstance();
    auto dlg = new MessageShowDlg(tr("No samples were injected into all strips."), topLevelWidget);
    dlg->exec();
}

bool WorkPlaceView::isSampleMissingError(const QString& errorReason)
{
    if(errorReason.isEmpty())
        return false;

    auto errorToken = errorReason.split(":");
    auto errorHeader = errorToken.first();

    return errorHeader.contains("[ETC-S]");
}

QString WorkPlaceView::getCombinedErrorString(const GlobalDataStruct::AnalysisReportInfomation& object1, const GlobalDataStruct::AnalysisReportInfomation& object2)
{
    QString errorReason = tr("[Strip Scan Error] : Either or both failed strip analysis.");

    auto isHasETCError1 = this->isSampleMissingError(object1.result.errorReason);
    auto isHasETCError2 = this->isSampleMissingError(object2.result.errorReason);

    if(isHasETCError1 || isHasETCError2)
        errorReason = tr("[ETC-S] : Either or both samples are not injected into the strip.");

    return errorReason;
}

cv::Mat WorkPlaceView::getTripleQRImage()
{
    auto matrix = d->mCameraInstance->getFullConfigureImage();

    if(matrix.data == nullptr)
    {
        this->thread()->msleep(1500);
        return cv::Mat();
    }

    auto currentFlip = d->mConfigInstance->getFlip();

    if(currentFlip != 0)
        CameraInstance::flip(matrix, currentFlip);

    this->imageSave(matrix, "QR-Debug-TripleCode", 1);

    return matrix;
}

void WorkPlaceView::getContentsCount(int& plusAllergy, int& etc)
{
    plusAllergy = 0;
    etc = 0;

    QString strPlusAllergy = ConfigInformation::convertQEnumToQString(ConfigInformation::ALLERGY_PANEL_LISTS::ALLERGY_PANEL,"ALLERGY_PANEL_LISTS");

    for(auto& itr : d->mAnalysisData)
    {
        bool isPlusAllergy = (itr.panelName == strPlusAllergy);

        if (isPlusAllergy)
            plusAllergy++;
        else
            etc++;
    }
}
void WorkPlaceView::initHiddenKey()
{
    QAction* noScanQrSignal = new QAction(this);
    noScanQrSignal->setShortcut(Qt::CTRL | Qt::Key_R);
    connect(noScanQrSignal,&QAction::triggered,this,&WorkPlaceView::onNoScanQrRead);
    this->addAction(noScanQrSignal);
}

void WorkPlaceView::onNoScanQrRead()
{
    if(d->mCurrentStep==WORKPLACE_STEP_REAGENT)
    {
        d->mIsStripScan =false;
        for(int i = 0; i<d->mResultData.count(); i++)
        {
            d->mResultData[i].lotNumber =QString("skip");
            d->mResultData[i].isQRSkipRead = true;
        }
        this->cleanupQRModule();
        emit onQRReadCompleted();

    }
}

void WorkPlaceView::onUnpause()
{
    ui->btnStart->setDisabled(true);
    this->commonStartGUIProcessing();
}

void WorkPlaceView::printLogLotAndPatientData(QVector<GlobalDataStruct::AnalysisReportInfomation>& result) const
{
    QString lotList;
    for(int i =0; i< result.count() ; i++)
    {
        lotList += "\r\n";
        lotList += QString("%1, %2, %3").arg(result[i].element.stripNumber).arg(result[i].lotNumber).arg(result[i].element.patientID);
    }
    CLogWriter::printLotLog(lotList);
}
