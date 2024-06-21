#include <QTimer>
#include <QBitArray>
#include <QtEndian>

#include "RunningStatusWidget.h"
#include "ui_RunningStatusWidget.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h"
#include "Network/PacketWriter/WriteProtocolControlRead.h"
#include "Network/PacketWriter/WriteProtocolControlWrite.h"
#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"

#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "UIClass/DialogWidget/StatusDialog.h"
#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/CheckListDlg.h"

#include "CameraControl/CameraInstance.h"

#include "Analysis/ErrorResonIO.h"

#include "mainwindow.h"

USING_SUGENTECH

struct RunningStatusWidget::privateStruct
{
    QTimer mTimeUpdateTimer;
    QTimer mCurrentStatusTimer;

    QVector<GlobalDataStruct::AnalysisReportInfomation> mItemList;
    QVector<GlobalDataStruct::AnalysisReportInfomation> mResultList;

    QScopedPointer<BasicSupremeIAnalysis> mAnalysis;

    QMetaObject::Connection mNetworkConnect;
    QMetaObject::Connection mNetworkDisconnect;

    CameraInstance* mCameraInstance = nullptr;
    StatusDialog mWaitingDlg;

    int mCurrentStep = ConfigInformation::STEP_LIST_NONE;

    int mTotalStepCount = 0;
    int mLastIndex = 0;

    bool mIsRetry = false;
    bool mIsInitTime = false;
};

RunningStatusWidget::RunningStatusWidget(QWidget *parent) :
                                                            CSerialNetworkWidget(parent),
                                                            ui(new Ui_RunningStatusWidget),
                                                            d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
}

RunningStatusWidget::~RunningStatusWidget()
{
    this->stop();
    this->analysisMemoryClear();
    this->cameraInstanceClear();
}

void RunningStatusWidget::initVariables()
{
    d->mCameraInstance = CameraInstance::getInstance();
    auto topLevelWidget = MainWindow::getInstance();

    d->mWaitingDlg.setParent(topLevelWidget);
    d->mWaitingDlg.setStatusMessage(tr("The light source is stabilizing."));
    d->mWaitingDlg.stopWaiting();

    d->mCurrentStatusTimer.setInterval(2000);
    d->mCurrentStatusTimer.setSingleShot(true);
}

void RunningStatusWidget::initSignalSlots()
{
    connect(this, &RunningStatusWidget::onReadReady, this, &RunningStatusWidget::onRecvData);
    connect(&d->mTimeUpdateTimer, &QTimer::timeout, this, &RunningStatusWidget::onCurrentStatusUpdate);
    connect(&d->mCurrentStatusTimer, &QTimer::timeout, this, &RunningStatusWidget::onSendCurrentTime);
}

void RunningStatusWidget::startWidget()
{
    this->onTimerStop();
    this->startNetworkConnectionControl();

    auto config = ConfigInformation::getInstance();
    auto currentContents = config->getCurrentContents();
    auto currentSequence =  ConfigInformation::getInstance()->getSequenceList(currentContents);

    ui->widgetProcessingBar->clearItem();
    d->mItemList.clear();
    d->mResultList.clear();

    for(int i = 0; i < currentSequence.count(); i++)
        ui->widgetProcessingBar->addItem(currentSequence[i].stepNumber, QTime(0,0,0));

    d->mTotalStepCount =  currentSequence.count();
    d->mIsRetry = false;
}

void RunningStatusWidget::stopWidget()
{
    this->onTimerStop();
    this->stopNetworkConnectionControl();
}

void RunningStatusWidget::startNetworkConnectionControl()
{
    d->mNetworkConnect = connect(this, &RunningStatusWidget::onConnectedNetwork, this, [this]
                                 {
                                     this->onPauseUIControl(false);
                                     this->onPauseControlDefaultTimer(false);
                                 });

    d->mNetworkDisconnect = connect(this, &RunningStatusWidget::onDisconnectNetwork, this, [this]
                                 {
                                     this->onPauseUIControl(true);
                                     this->onPauseControlDefaultTimer(true);
                                     d->mCameraInstance->instanceStop();
                                 });
}

void RunningStatusWidget::stopNetworkConnectionControl()
{
    disconnect(d->mNetworkConnect);
    disconnect(d->mNetworkDisconnect);
}

void RunningStatusWidget::startSequence(QVector<QByteArray> sequence, const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList)
{
    mCurrentStep = "";
    ConfigInformation::getInstance()->saveLastConfigureData();
    d->mItemList = analysisList;

    QTimer::singleShot(1000, this, [=]
                       {

                           for(auto itr : sequence)
                               this->onQueuedSend(itr);

                           this->onTimerStart();

                           QTimer::singleShot(4000, this, [this]
                                              {
                                                  this->onCurrentStatusUpdate();
                                              });
                       });
}

void RunningStatusWidget::reStartStatus(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList)
{
    d->mItemList.clear();
    d->mItemList = analysisList;
    this->onReadSequenceTime();
    this->onTimerStart();

    QTimer::singleShot(4000, this, [this]
                       {
                           this->onCurrentStatusUpdate();
                       });
}

void RunningStatusWidget::stripScan(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisList)
{
    d->mItemList.clear();
    d->mResultList.clear();

    d->mItemList = analysisList;

    ui->widgetProcessingBar->addItem(ConfigInformation::STEP_LIST_ANALYSIS, QTime(0,0,0));
    ui->widgetProcessingBar->startLastItem();

    if(ui->widgetProcessingBar->isHaveAnalysisSequence() == true)
    {
        auto analysisTime = ui->widgetProcessingBar->calcAnalysisTime(d->mItemList.count());
        QTime time(0,0,0);
        time = time.addSecs(analysisTime);
        ui->widgetProcessingBar->setTotalTime(time, ui->widgetProcessingBar->getCount()-1);
    }

    ui->labelCurrentSequence->setText(ui->widgetProcessingBar->getCurrentLabelName());

    connect(d->mCameraInstance, &CameraInstance::onFinishedInitalizing, this, [this]
            {
                d->mWaitingDlg.startWaiting();
                QTimer::singleShot(1000*180, this, [this]
                                   {
                                       d->mWaitingDlg.stopWaiting();
                                       this->onAnalysisProcessing(d->mItemList);
                                   });
            });

    QTimer::singleShot(1, d->mCameraInstance, &CameraInstance::instanceStart);
}


void RunningStatusWidget::DisplayingDialogForSemiAuto()
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> manualStripData;
    manualStripData = d->mItemList;

    if(manualStripData.isEmpty())
        return;

    this->showDialogForCheckList(manualStripData);
}

void RunningStatusWidget::calcETATimeAndSetupUI()
{
    QTime etaTime = QTime::currentTime();
    auto totTime = QTime(0,0,0).secsTo(ui->widgetProcessingBar->getTotalNeedTime());
    auto spendTime = QTime(0,0,0).secsTo(ui->widgetProcessingBar->getTotalUsedSpendTime());

    auto totNeedTimeSec = totTime - spendTime;
    etaTime = etaTime.addSecs(totNeedTimeSec);
    ui->widgetCircle->setETATime(etaTime.toString("HH:mm"));
}

void RunningStatusWidget::clacTimeTotalTimeAndSpendTimeForSetupUI(const QTime& currentTime)
{
    ui->widgetProcessingBar->setCurrentSpendTime(currentTime);
    ui->widgetCircle->setValue(ui->widgetProcessingBar->getTotalRunningPercent());
    QString stepName = ui->widgetProcessingBar->getCurrentLabelName();
    ui->labelCurrentSequence->setText(stepName);
    ui->labelCurrentSequenceTime->setText(ui->widgetProcessingBar->getCurrentSequenceRunningTimeString());

    auto totalUsedTimeString = ui->widgetProcessingBar->getTotalUsedSpendTime().toString("HH:mm");
    auto totalNeedTimeString = ui->widgetProcessingBar->getTotalNeedTime().toString("HH:mm");

    ui->labelProcessingTimeStatus->setText(tr("Total progress time %1 / %2").arg(totalUsedTimeString).arg(totalNeedTimeString));

    if(mCurrentStep != stepName)
    {
        auto currentContents = ConfigInformation::getInstance()->getCurrentContents();
        CLogWriter::printLog("============================");
        CLogWriter::printLog(QString("Contents : %1").arg(currentContents));
        CLogWriter::printLog(QString("Step : %1").arg(stepName));
        CLogWriter::printLog("============================");
    }
    mCurrentStep = stepName;
}

void RunningStatusWidget::onCurrentStatusUpdate()
{
    this->onSendCurrentStep();

    if(d->mCurrentStep == ConfigInformation::STEP_LIST_ANALYSIS)
        return;

    d->mCurrentStatusTimer.start();
}

void RunningStatusWidget::onReadSequenceTime()
{
    for(int i = 0; i < d->mTotalStepCount; i++)
    {
        auto data = WriteProtocolStatusRead::writeStepTimeRead(i+1).toUtf8();
        this->onQueuedSend(data);
    }
}

void RunningStatusWidget::showDialogForCheckList(const QVector<GlobalDataStruct::AnalysisReportInfomation> &list)
{
    QString message;
    message.append(tr("Add samples & Click the “CONTINUE” button. \n"));
    message.append(tr("Please check and continue pressing the button."));
    CheckListDlg checkDlg(this);
    checkDlg.setText(message);
    checkDlg.setFontSize(20);
    checkDlg.setButtonHeight(50);
    checkDlg.setCheckListSingleDataMode(list);

    if(checkDlg.exec() == QDialog::Accepted)
        return;

    MessageShowDlg* msgDlg = new MessageShowDlg(tr("Sampling waiting timed out."), this);
    msgDlg->open();

    auto unCheckedStripNumberList = checkDlg.getUncheckedStripNumberList();
    this->updateErrorResonForUnchecked(unCheckedStripNumberList);
}

void RunningStatusWidget::onAnalysisProcessing(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisIdxList)
{
    BasicSupremeIAnalysis* analysis = new BasicSupremeIAnalysis(nullptr);
    d->mAnalysis.reset(analysis);

    connect(analysis, &BasicSupremeIAnalysis::onCurrentProcessing, this, [this] (int count)
            {
                this->onUpdateStatus((ui->widgetProcessingBar->ANALYSIS_TIME * count)/60.0);
            });

    connect(analysis, &BasicSupremeIAnalysis::onAnalysisFinished, this, [this]
            (QVector<GlobalDataStruct::AnalysisReportInfomation> resultList)
            {
                if(d->mResultList.isEmpty() == true)
                    d->mResultList = resultList;

                if(this->onCheckReTry(resultList) == true)
                    return;

                this->onFinished();
                this->analysisMemoryClear();
                emit onAnalysisReport(d->mResultList);
            });

    connect(analysis, &BasicSupremeIAnalysis::onAnalysisStart, this, [this]
            (bool isSuccess)
            {
                if(isSuccess == true)
                    return ;

                emit onCameraInitFail();
            });

    QTimer::singleShot(1500, this, [=]
                       {
                           d->mAnalysis->analysisStart(analysisIdxList);
                       });

}

void RunningStatusWidget::onFinished()
{
    CLogWriter::printLog("[RunningStatusWidget::] : ---onFinished---");
    this->onTimerStop();
    ui->widgetProcessingBar->stopCurrentItem();
    auto lastTotalTime = ui->widgetProcessingBar->getCurrentTotalTime();
    this->onUpdateStatus(lastTotalTime.minute());
}

void RunningStatusWidget::onStopAnalysisSequence()
{
    CLogWriter::printLog("[RunningStatusWidget::] : onStopAnalysisSequence");
    this->onTimerStop();
    ui->widgetProcessingBar->stopCurrentItem();

    if(d->mAnalysis.isNull() == false)
    {
        d->mAnalysis->onStop();
        d->mAnalysis.reset();
    }
    else
        CameraInstance::getInstance()->instanceStop();

    auto data = WriteProtocolControlWrite::writeSequenceStop().toUtf8();
    this->onQueuedSend(data);
}

bool RunningStatusWidget::onCheckReTry(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    CLogWriter::printLog("[RunningStatusWidget::] : onCheckReTry");
    QString failureList;
    QVector<GlobalDataStruct::AnalysisReportInfomation> failList;

    for(auto itr : d->mResultList)
    {
        if(itr.result.isSuccess == false)
        {
            failList.push_back(itr);

            if(failureList.isEmpty() == false)
                failureList.append(", ");

            failureList.append(QString("%1").arg(itr.element.stripNumber));
        }
    }

    if (d->mIsRetry == false && failureList.isEmpty() == false)
    {
        QuestionDlg dlg;
        dlg.setQuestionMessage(tr("Analysis Failure Strip Detected \n %1 \n Do you ReScanning?").arg(failureList));

        if(dlg.exec() == QDialog::Accepted)
        {
            d->mIsRetry = true;
            this->onAnalysisProcessing(failList);
            return true;
        }
        else
            return false;
    }

    else if(d->mIsRetry == false && failureList.isEmpty() == true)
        return false;


    for(int i = 0; i < d->mResultList.count(); i++)
    {
        for(int idx = 0; idx < result.count(); idx++)
        {
            if(d->mResultList[i].uniqueID == result[idx].uniqueID)
            {
                d->mResultList[i] = result[idx];
                break;
            }
        }
    }

    return false;
}

void RunningStatusWidget::onPauseUIControl(bool isPause)
{
    if(isPause)
        this->onPauseItem();
    else
        this->onResumeItem();
}

void RunningStatusWidget::onPauseControlDefaultTimer(bool isPause)
{
    if(isPause)
    {
        this->onTimerStop();
        return;
    }

    if(d->mCurrentStep == ConfigInformation::STEP_LIST_ANALYSIS)
        return;

    this->onTimerStart();
}

void RunningStatusWidget::onSendXHome()
{
    CLogWriter::printLog("[RunningStatusWidget::] : onSendXHome");
    QString protocolStrX = WriteProtocolControlRead::writeXAxisHome();
    QByteArray sendDataX = protocolStrX.toUtf8();
    this->onQueuedSend(sendDataX);
}

void RunningStatusWidget::onSendASPHome()
{
    CLogWriter::printLog("[RunningStatusWidget::] : onSendASPHome");
    QString protocolStrASP = WriteProtocolControlRead::writeASPHome();
    QByteArray sendDataASP = protocolStrASP.toUtf8();
    this->onQueuedSend(sendDataASP);
}

void RunningStatusWidget::onSendLedOnOff(bool isOn)
{
    //CLogWriter::printLog(QString("[RunningStatusWidget::] : onSendLedOnOff [ %1 ]").arg(isOn ? "ON" : "OFF"));
    WriteProtocolCameraLedSetting::PROTOCOL_LED ledStatus = WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_OFF;

    if(isOn == true)
        ledStatus = WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_ON;

    auto ledOn =  WriteProtocolCameraLedSetting::writeLEDOnOff(ledStatus).toUtf8();
    this->onQueuedSend(ledOn);
}

void RunningStatusWidget::onSendCurrentStep()
{
    auto currentStepData = WriteProtocolStatusRead::writeCurrentStatusRead().toUtf8();
    this->onQueuedSend(currentStepData);
}

void RunningStatusWidget::onSendCurrentTime()
{
    auto stepTimeData = WriteProtocolStatusRead::writeCurrentStepTime().toUtf8();
    this->onQueuedSend(stepTimeData);
}

void RunningStatusWidget::onRecvData(QByteArray recvData)
{
    if(ui->widgetProcessingBar->getCount() == 0)
        return;

    if(d->mItemList.count() == 0)
        return;

    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_STATUS_READ:
        this->onRecvRunningStateData(recvData);
        break;

    case ConfigInformation::STEP_LIST_ANALYSIS:
        this->onRecvAnalysisData(recvData);
        break;
    }
}

void RunningStatusWidget::onUpdateStatus(const int& currentTime)
{
    if(d->mIsRetry == true)
        return;

    QTime time(0,0,0);
    time = time.addSecs(currentTime*60);

    qDebug() <<"[CurrentTime] : " << time;

    this->clacTimeTotalTimeAndSpendTimeForSetupUI(time);
    this->calcETATimeAndSetupUI();
}

void RunningStatusWidget::onUpdateLastIdx(int currentIdx)
{
    if(currentIdx < 0)
        return;

    ui->widgetProcessingBar->startItem(currentIdx);

    if(d->mLastIndex != currentIdx)
        QTimer::singleShot(2000, this, [this]{this->onUpdateStatus(0);});

    d->mLastIndex = currentIdx;
}

void RunningStatusWidget::onUpdateCurrentStep(int currentStepNumber)
{
    d->mCurrentStep = currentStepNumber;
}

void RunningStatusWidget::onUpdateDispensePause(const int& stepNumber, const int& processNumber)
{
    if(stepNumber != ConfigInformation::STEP_LIST_DISPENSE)
        return;

    if(processNumber != ConfigInformation::PROCESS_LIST_PAUSE)
        return;

    if(d->mTimeUpdateTimer.isActive() == true)
    {
        this->onTimerStop();
        this->onPauseUIControl(true);
        this->DisplayingDialogForSemiAuto();
        this->sendPause(false);
        emit this->onUnpause();
        this->onTimerStart();
        this->onPauseUIControl(false);
    }
}

void RunningStatusWidget::onRecvDryStep(const int& stepNumber)
{
    if(stepNumber != ConfigInformation::STEP_LIST_DRY)
        return;

    this->analysisCameraON();
}

void RunningStatusWidget::onResumeItem()
{
    ui->widgetProcessingBar->resumeCurrentItem();
}

void RunningStatusWidget::onPauseItem()
{
    ui->widgetProcessingBar->pauseCurrentItem();
}

void RunningStatusWidget::onTimerStart(int mSec)
{
    if(d->mTimeUpdateTimer.isActive())
        return;

    if(mSec == 0)
        d->mTimeUpdateTimer.start(TIME_STATUS_TICK);
    else
        d->mTimeUpdateTimer.start(mSec);

    CLogWriter::printLog("[RunningStatusWidget::] Start Timer");
}

void RunningStatusWidget::onTimerRestart()
{
    this->onTimerStop();
    this->onTimerStart();

    if(d->mTimeUpdateTimer.isActive() == true)
        CLogWriter::printLog("[RunningStatusWidget::] Restart Timer");
}

void RunningStatusWidget::onTimerStop()
{
    if(d->mTimeUpdateTimer.isActive() == false)
        return;

    d->mTimeUpdateTimer.stop();
    d->mCurrentStatusTimer.stop();
    CLogWriter::printLog("[RunningStatusWidget::] Stop Timer");
}

void RunningStatusWidget::analysisMemoryClear()
{
    CLogWriter::printLog("[RunningStatusWidget::] analysisMemoryClear");
    if(d->mAnalysis.isNull() == false)
    {
        d->mAnalysis->onStop();
        d->mAnalysis.reset();
    }
}

void RunningStatusWidget::cameraInstanceClear()
{
    CLogWriter::printLog("[RunningStatusWidget::] cameraInstanceClear");
    if(d->mCameraInstance != nullptr)
    {
        d->mCameraInstance->instanceStop();
        d->mCameraInstance = nullptr;
    }
}

void RunningStatusWidget::analysisCameraON()
{
    CLogWriter::printLog("[RunningStatusWidget::] analysisCameraON");
    if(d->mCameraInstance->isOpen() == false)
    {
        emit this->onDryStepCameraInit();

        d->mCameraInstance->setEnableStartInitSignal(false);
        d->mCameraInstance->instanceStart();
    }
}

void RunningStatusWidget::onRecvStepAnalysis()
{
    if(d->mTimeUpdateTimer.isActive())
    {
        this->onTimerStop();
        emit onHideButtons();
        ui->widgetProcessingBar->startLastItem();
        this->onUpdateStatus(0);
        this->onAnalysisProcessing(d->mItemList);
    }
}

void RunningStatusWidget::onRecvRunningStateData(const QByteArray& recvData)
{
    switch(PacketReader::commandReader(recvData))
    {

    case CMD_STATUS_READ::CMD_STATUS_READ_CURRENT_STEP_TIME:
    {
        auto value = PacketReader::dataReader(recvData);
        int currentIndex = value.left(2).toUInt(nullptr, 16) - 1;
        int currentTime = value.right(2).toUInt(nullptr, 16);

        this->onUpdateLastIdx(currentIndex);
        this->onUpdateStatus(currentTime);
    }break;

    case CMD_STATUS_READ::CMD_STATUS_READ_STEP_TIME:
    {
        auto value = PacketReader::dataReader(recvData);
        int idx = value.left(4).toInt(nullptr, 16) - 1;
        int timeMin = value.right(4).toInt(nullptr, 16);

        QTime time(0,0,0);
        time = time.addSecs(timeMin*60);

        if(ui->widgetProcessingBar->isAnalysisSequence(idx) == true)
        {
            auto analysisTime = ui->widgetProcessingBar->calcAnalysisTime(d->mItemList.count());
            QTime aTime(0,0,0);
            aTime = aTime.addSecs(analysisTime);
            ui->widgetProcessingBar->setTotalTime(aTime, idx);
            return;
        }

        ui->widgetProcessingBar->setTotalTime(time, idx);
        qDebug() << "[TEST]" << time << idx;
    }break;

    case CMD_STATUS_READ::CMD_STATUS_READ_CURRENT_STATUS_READ:
    {
        auto value = PacketReader::dataReader(recvData);

        int stepIndex = value.mid(2,2).toInt(nullptr, 16)-1;
        int processNumber = value.mid(6,2).toInt(nullptr, 16);

        if(stepIndex < 0)
            return;

        auto currentContents = ConfigInformation::getInstance()->getCurrentContents();
        auto currentSequence =  ConfigInformation::getInstance()->getSequenceList(currentContents);

        if(currentSequence.count() <= stepIndex)
            return;

        auto stepNumber = currentSequence[stepIndex].stepNumber;

        this->onUpdateCurrentStep(stepNumber);
        this->onUpdateDispensePause(stepNumber, processNumber);
        this->onRecvDryStep(stepNumber);
    }break;
    }
}

void RunningStatusWidget::onRecvAnalysisData(const QByteArray& recvData)
{
    switch(PacketReader::commandReader(recvData))
    {
    case ConfigInformation::STEP_LIST_ANALYSIS:
        this->onRecvStepAnalysis();
        break;
    }
}

void RunningStatusWidget::updateErrorResonForUnchecked(const QVector<int>& uncheckedList)
{
    for(auto& uncheckedItr : uncheckedList)
    {
        for(auto& itemItr : d->mItemList)
        {
            if(itemItr.element.stripNumber == uncheckedItr)
                itemItr.result.errorReason = tr("[ETC-S] : No samples were injected.");
        }
    }

    if(uncheckedList.isEmpty())
        return;

    ErrorResonIO::saveErrorReason(d->mItemList);
}

void RunningStatusWidget::sendPause(bool isPause)
{
    auto data = WriteProtocolControlWrite::writePause(isPause).toUtf8();
    this->onQueuedSend(data);
}
