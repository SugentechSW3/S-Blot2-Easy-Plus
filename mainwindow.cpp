#include <QMessageBox>
#include <QPropertyAnimation>
#include <QTimer>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolControlWrite.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h"

#include "Common/CUtil.h"
#include "Common/CLogWriter.h"
#include "Common/Io/CFileio.h"

#include "UIClass/DialogWidget/StatusDialog.h"
#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"

#include "Report/LagacyReportData.h"

#include "CameraControl/CameraInstance.h"

#include "Analysis/ConfigInformation.h"


#ifdef __WIN32
#include <windows.h>
#include <qt_windows.h>
#include <dbt.h>
#endif

USING_SUGENTECH

MainWindow* MainWindow::instance = nullptr;

struct MainWindow::privateStruct
{
    StatusDialog mWaitingConnectionDlg;
    StatusDialog mWaitingResetDlg;
    StatusDialog mWaitingCameraConfigureDlg;
    StatusDialog mWaitingAutoCleanDlg;

    SerialCtrl* mSerialInstance = nullptr;
    CameraInstance* mCameraInstance = nullptr;

    QTimer mAutoCleanTimer;

    bool mIsWaiting = false;
    bool mIsResultSkip = false;
    bool mIsClosedMainWindow = false;
};

// assert(instance != nullptr);
MainWindow* MainWindow::getInstance()
{
    return instance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui_MainWindow),
    d(new privateStruct)
{
    d->mSerialInstance = SerialCtrl::getInstance();
    d->mCameraInstance = CameraInstance::getInstance();

    ui->setupUi(this);
    instance = this;
    CLogWriter::printLog(QString("[MainWindow::] : ---Start the Program Boot---"));
    this->initShortCut();
    this->initDlg();
    this->initSignalSlots();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->showMaximized();

#ifdef __WIN32
    ::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_AWAYMODE_REQUIRED); //절전모드 방지
#endif

    this->writeLogForCompanyName();
    this->setDisableTopLevelWidget(false);
    this->setWindowTitle("S-Blot 2 Easy PLUS");

    QTimer::singleShot(1000, this, [this]
                       {
                           ui->mainWidget->checkReSequenceProcess();
                           this->stopDlg();
                       });

    CFileIO::removeAllOldData();
    ui->mainWidget->start();
}

MainWindow::~MainWindow()
{
    this->stopDlg();
    CLogWriter::printLog(QString("[MainWindow::] : ---End the program---"));
}

void MainWindow::initShortCut()
{
    QAction* exitSignal = new QAction(this);
    exitSignal->setShortcut(Qt::CTRL | Qt::Key_Q);

    connect(exitSignal, &QAction::triggered, this, &MainWindow::onProgramQuit);
    this->addAction(exitSignal);
}

void MainWindow::initSignalSlots()
{
    connect(d->mSerialInstance, &SerialCtrl::onDeviceIsConnected, this, &MainWindow::onSerialConnected);
    connect(d->mSerialInstance, &SerialCtrl::onDeviceIsDisConnected, this, &MainWindow::onSerialDisConnected);
    connect(d->mSerialInstance, &SerialCtrl::onResponseResetCommand, this, &MainWindow::onResetWait);
    connect(d->mSerialInstance, &SerialCtrl::onResponseResetEndCommand, this, &MainWindow::onResetEnd);
    connect(d->mSerialInstance, &SerialCtrl::onRecvDataForSerial, this, &MainWindow::onRecvData);

    connect(this, &MainWindow::onDetectedAddDevice, d->mSerialInstance, &SerialCtrl::onConnectCallBack);
    connect(this, &MainWindow::onDetectedRemoveDevice, d->mSerialInstance, &SerialCtrl::onDisConnectCallBack);
    connect(this, &MainWindow::onDetectedAddDevice, d->mCameraInstance, &CameraInstance::onCallBackReconnection);

    connect(ui->mainWidget, &MainView::onResultReset, this, &MainWindow::onResultResetProcess);

    connect(&d->mAutoCleanTimer, &QTimer::timeout, this, &MainWindow::sendAutoCleanStatusProtocol);

    connect(d->mCameraInstance, &CameraInstance::onStartInitalizing, this, [this]
            {
                d->mWaitingCameraConfigureDlg.startWaiting();
                this->setDisabled(true);
            });

    connect(d->mCameraInstance, &CameraInstance::onFinishedInitalizing, this, [this]
            {
                this->setEnabled(true);
                d->mWaitingCameraConfigureDlg.stopWaiting();
            });

    connect(d->mCameraInstance, &CameraInstance::onInitializingFail, this, [this]
            {
                MessageShowDlg* dlg = new MessageShowDlg(tr("[Camera Open Error] : Please check the connection line of the camera."), this);
                dlg->exec();
            });
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);

    ui->mainWidget->resize(event->size());
    ui->widgetAlpha->resize(event->size());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();

    QuestionDlg dlg(this);
    dlg.setQuestionMessage(tr("Will you close the application?"));

    if(dlg.exec() == QDialog::Accepted)
    {
        auto config = ConfigInformation::getInstance();

        if(config->isOffLine())
            this->onProgramQuit();
        else
            this->programCloseProcess();
    }
    else
        d->mWaitingConnectionDlg.raise();
}

void MainWindow::initDlg()
{
    d->mWaitingConnectionDlg.setStatusMessage(tr("Device Is DisConnected Please Watch Your Cable Connection"));
    d->mWaitingResetDlg.setStatusMessage(tr("The device is being restarted"));
    d->mWaitingCameraConfigureDlg.setStatusMessage(tr("Initializing camera settings."));

    d->mWaitingAutoCleanDlg.setParent(this);
    d->mWaitingAutoCleanDlg.setStatusMessage(tr("AutoCleaning In Progress Please Wait"));
    d->mWaitingAutoCleanDlg.stopWaiting();

    d->mWaitingConnectionDlg.setParent(this);
    d->mWaitingResetDlg.setParent(this);
    d->mWaitingCameraConfigureDlg.setParent(this);
}

void MainWindow::initCamera()
{
    CLogWriter::printLog("[MainWindow::] : initCamera");
    this->setDisabled(true);
    QTimer::singleShot(1, d->mCameraInstance, &CameraInstance::restart);
}

void MainWindow::onSerialConnected()
{
    if(d->mIsWaiting == true)
        return;

    this->setEnabled(true);
    this->stopDisconnectProcess();
}

void MainWindow::onSerialDisConnected()
{
    if(d->mIsWaiting == true)
        return;

    this->setEnabled(false);
    this->startDisconnectProcess();
}

void MainWindow::onSendExitSignal()
{
    CLogWriter::printLog("[MainWindow::] : onSendExitSignal");
    this->stopDisconnectProcess();
    d->mSerialInstance->closeApplication();
    d->mWaitingConnectionDlg.close();
}

void MainWindow::onResetWait()
{
    CLogWriter::printLog("[MainWindow::] : onResetWait");
    d->mIsWaiting = true;

    if(d->mIsResultSkip == true)
        return;

    this->setEnabled(false);

    d->mWaitingResetDlg.startWaiting();
    this->stopDisconnectProcess();
}

void MainWindow::onResetEnd()
{
    CLogWriter::printLog("[MainWindow::] : onResetEnd");
    d->mWaitingResetDlg.stopWaiting();
    this->stopDisconnectProcess();

    if(d->mIsResultSkip == false)
        ui->mainWidget->onResetGUI();

    d->mIsWaiting = false;
    d->mIsResultSkip = false;
    ui->mainWidget->setResetStatus(false);

    this->setEnabled(true);
}

void MainWindow::stopDlg()
{
    CLogWriter::printLog("[MainWindow::] : stopDlg");
    this->stopDisconnectProcess();
    d->mWaitingResetDlg.stopWaiting();
    d->mWaitingCameraConfigureDlg.stopWaiting();
}

void MainWindow::programCloseProcess()
{
    CLogWriter::printLog("[MainWindow::] : programCloseProcess");
#ifdef IS_DEBUG_PC
    this->onProgramQuit();
    return;
#endif

    connect(this, &MainWindow::onFinishedAutoClean, this, [this]
            {                
                this->onProgramQuit();
            });

    this->sendAutoCleanProtocolAndShowMessage();
}

void MainWindow::startDisconnectProcess()
{
    CLogWriter::printLog("[MainWindow::] : startDisconnectProcess");
    d->mWaitingConnectionDlg.startWaiting();
}

void MainWindow::stopDisconnectProcess()
{
    CLogWriter::printLog("[MainWindow::] : stopDisconnectProcess");
    d->mWaitingConnectionDlg.stopWaiting();
}

void MainWindow::writeLogForCompanyName()
{
    auto config = ConfigInformation::getInstance();
    auto institution = config->getCompanyName();
    CLogWriter::printLog(QString("[Institution Name] : %1").arg(institution));
}

void MainWindow::sendAutoCleanProtocolAndShowMessage()
{
    this->showAutoCleanMessage();
    this->sendAutoCleanProtocol();

    QTimer::singleShot(25, this, [this]
                       {
                           //d->mAutoCleanTimer.start(2000);//펌웨어에서 알아서 완료되면 보내주므로 주석처리 2024.06.03
                           d->mWaitingAutoCleanDlg.startWaiting();
                       });
}

void MainWindow::showAutoCleanMessage()
{
    CLogWriter::printLog("[MainWindow::] : showAutoCleanMessage");
    QString message;
    message.append(tr("1. Prepare The Auto-Cleaning Bath with Distilled Water. \n"));
    message.append(tr("2. Connect The Tubing to the Auto-Cleaning Bath, after checking"));

    MessageShowDlg* dlg = new MessageShowDlg(message, this);
    dlg->exec();
}


void MainWindow::sendAutoCleanProtocol()
{
    QString protocolStr = WriteProtocolControlWrite::writeAutoClean();
    QByteArray sendData = protocolStr.toUtf8();
    d->mSerialInstance->writeQueuedData(sendData);
}

void MainWindow::sendAutoCleanStatusProtocol()
{
    auto currentStepData = WriteProtocolStatusRead::writeCurrentStatusRead().toUtf8();
    d->mSerialInstance->writeQueuedData(currentStepData);
}

void MainWindow::sendResumeProtocol()
{
    uchar state = 0;

    auto data = WriteProtocolControlWrite::writePause(state).toUtf8();
    d->mSerialInstance->writeQueuedData(data);
}

void MainWindow::onProgramQuit()
{
    CLogWriter::printLog(QString("[MainWindow::] : ---onProgramQuit---"));
    d->mIsClosedMainWindow = true;
    this->onSendExitSignal();

    QTimer::singleShot(500, this,[]
                       {
                           QApplication::quit();
                       });
}

void MainWindow::onResultResetProcess()
{
    CLogWriter::printLog("[MainWindow::] : onResultResetProcess");
    d->mIsResultSkip = true;
    ui->mainWidget->setResetStatus(true);

    auto data = WriteProtocolControlWrite::writeSequenceStop().toUtf8();
    d->mSerialInstance->writeQueuedData(data);
}

void MainWindow::onAutocleanProcess(const QByteArray& recvData)
{
    //CLogWriter::printLog("[MainWindow::] : onAutocleanProcess");
    auto value = PacketReader::dataReader(recvData);
    int stepIndex = value.mid(2,2).toInt(nullptr, 16) - 1;
    int processNumber = value.mid(6,2).toInt(nullptr, 16);

    if (stepIndex < 0)
        return;

    auto currentContents = ConfigInformation::getInstance()->getCurrentContents();
    auto currentSequence =  ConfigInformation::getInstance()->getSequenceList(currentContents);

    auto isWetting =  false;
    auto isAutoClean = false;

    if(stepIndex < currentSequence.count())
        isWetting = (ConfigInformation::STEP_LIST_WETTING == currentSequence[stepIndex].stepNumber);

    isAutoClean = (ConfigInformation::PROCESS_LIST_AUTO_CLEAN == processNumber);

    auto isAutoCleanFinishedFirst = ((stepIndex+1) == 0xFF);
    auto isAutoCleanFinishedSecond = (processNumber == 0xAA);

    if(isWetting == true && isAutoClean == true)
    {
        QTimer::singleShot(2000, this, [this]
                           {
                               this->sendResumeProtocol();
                           });
        return;
    }

    if(isAutoCleanFinishedFirst == true && isAutoCleanFinishedSecond == true)
    {
        d->mAutoCleanTimer.stop();
        d->mWaitingAutoCleanDlg.stopWaiting();

        emit onFinishedAutoClean();
        CLogWriter::printLog(QString("[MainWindow::] : ---Finished Auto Clean---"));
    }
}

void MainWindow::onRecvData(QByteArray recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_STATUS_READ:
        this->onRecvStatusRead(recvData);
        break;
    case MD_MODE::MD_MODE_ERROR:
        this->onRecvErrorRead(recvData);
        break;
    }
}

void MainWindow::onRecvStatusRead(const QByteArray& recvData)
{
    switch(PacketReader::commandReader(recvData))
    {

    case CMD_STATUS_READ::CMD_STATUS_READ_CURRENT_STATUS_READ:
        this->onAutocleanProcess(recvData);
        break;
    }
}

void MainWindow::onRecvErrorRead(const QByteArray& recvData)
{
    switch(PacketReader::commandReader(recvData))
    {

    case CMD_STATUS_READ::CMD_STATUS_READ_DEVICE_CHECK_LEGACY:
    {
        auto value = PacketReader::dataReader(recvData);
        int errorCode = value.left(2).toInt(nullptr, 16);
        CLogWriter::printLog(QString("[MainWindow::] onRecvRunningStateData - Error Code : %1").arg(errorCode));

        stopDisconnectProcess();

        MessageShowDlg dlg(tr("There may be a leak in the device or a problem with the pump.\n"
                              "Turn off the equipment and contact the manufacturer.\n"
                              "When you click the OK button, the software will close."));
        dlg.setButtonText(tr("OK"));
        dlg.exec();

        this->onProgramQuit();
    }
    break;
    }
}

void MainWindow::setDisableTopLevelWidget(bool isDisable)
{
    if(ui->widgetAlpha == nullptr)
        return;

    if(d->mIsClosedMainWindow)
        return;

    ui->widgetAlpha->raise();

#ifdef IS_DEBUG_PC
    ui->widgetAlpha->hide();
    return;
#endif

    if(isDisable == true)
        ui->widgetAlpha->show();
    else
        ui->widgetAlpha->hide();
}

void MainWindow::startAutoCleanProcess()
{    
    QuestionDlg dlg(this);
    dlg.setQuestionMessage(tr("Do you want to run auto clean?"));

    if(dlg.exec() == QDialog::Rejected)
        return;

    CLogWriter::printLog("[MainWindow::] : startAutoCleanProcess");
    this->sendAutoCleanProtocolAndShowMessage();
}

bool MainWindow::nativeEvent(const QByteArray & /*eventType*/, void *msg, long * /*result*/)
{
    auto message = static_cast<MSG*>(msg);

    if(message == nullptr)
        return false;

    switch(message->message)
    {
    case WM_DEVICECHANGE:
    {
        DEV_BROADCAST_HDR* deviceType = (DEV_BROADCAST_HDR*) message->lParam;

        if(deviceType == nullptr)
            return false;

        if(deviceType->dbch_devicetype != DBT_DEVTYP_PORT) //USB Port
            return false;        

        switch(message->wParam)
        {
        case DBT_DEVICEARRIVAL:
            CLogWriter::printLog("[Detected] : Physical USB Port Installed");
            emit onDetectedAddDevice();
            break;

        case DBT_DEVICEREMOVECOMPLETE:
            CLogWriter::printLog("[Detected] : Physical USB Port Removed");
            emit onDetectedRemoveDevice();
            break;
        }

    }   break;

    }

    return false;
}
