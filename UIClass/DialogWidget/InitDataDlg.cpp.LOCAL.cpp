#ifdef __WIN32
#include <windows.h>
#include <qt_windows.h>
#include <dbt.h>
#endif

#include <QTimer>

#include "InitDataDlg.h"
#include "ui_InitDataDlg.h"

#include "UIClass/DialogWidget/MessageShowDlg.h"

#include "Common/CUtil.h"
#include "Analysis/ConfigInformation.h"

#include "CameraControl/CameraInstance.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h" 

USING_SUGENTECH

struct InitDataDlg::privateStruct
{
    SerialCtrl* mSerialInstance = nullptr;
    ConfigInformation* mConfig = nullptr;
    int mTotalCount = 0;
    float mPrograssSI = 0;

    bool mIsInitContentsData = true;
    bool mIsSequencing = false;
    bool mIsDisconnectedProcess = false;

    QStringList mContentsErrorList;
    QTimer mTimer;
    QTimer mHeartBeat;
    QTimer mFullsequenceTimer;
};

InitDataDlg::InitDataDlg(QWidget *parent) :
                                            CDialogParentWidget(parent),
                                            ui(new Ui_InitDataDlg),
                                            d(new privateStruct)
{
    ui->setupUi(this);

    d->mConfig = ConfigInformation::getInstance();
    d->mSerialInstance = SerialCtrl::getInstance();

    d->mTotalCount = d->mConfig->getContentsList().count();
    d->mPrograssSI = 100 / 3;

    this->initStyleSheet();
    this->initTimerSettings();
    this->initSignalSlots();

    if(d->mSerialInstance->openSerial() == true)
        this->sendHeartBeat();
    else
        d->mTimer.start();

    d->mFullsequenceTimer.start();
}

InitDataDlg::~InitDataDlg()
{

}

void InitDataDlg::initTimerSettings()
{
    d->mTimer.setSingleShot(true);
    d->mTimer.setInterval(1000);

    d->mHeartBeat.setInterval(2000);

    d->mFullsequenceTimer.setSingleShot(true);
    d->mFullsequenceTimer.setInterval(100*1000);
}

void InitDataDlg::initStyleSheet()
{
    ui->labelTitle->setText(QCoreApplication::applicationName());
}

void InitDataDlg::initSignalSlots()
{
    ui->listWidget->addItem(tr("SW and device are communicating. This may take up to two minutes."));
    ui->listWidget->addItem(tr("Do not shut down the device while waiting."));

    connect(d->mConfig, &ConfigInformation::startConfigUpLoad, this, [this]
            {
                ui->progressBar->setMovingValue(d->mPrograssSI * 1);
                ui->listWidget->addItem(tr("Uploading contents data to SW"));
                ui->listWidget->addItem(tr("Uploading the initial settings"));
                ui->listWidget->addItem(tr("Uploading sequence data"));
            });

    connect(d->mConfig, &ConfigInformation::endConfigProtocol, this, [this]
            (bool isSequencing)
            {
                d->mHeartBeat.stop();
                d->mFullsequenceTimer.stop();

                d->mIsSequencing = isSequencing;

                connect(d->mSerialInstance, &SerialCtrl::onSequenceSendFinished, this, [this]
                {
                    ui->progressBar->setMovingValue(d->mPrograssSI * 2);
                    ui->listWidget->addItem(tr("Waiting for device initialization"));
                });

                connect(d->mSerialInstance, &SerialCtrl::onResponseResetEndCommand, this, [this]
                {
                    ui->progressBar->setMovingValue(100);
                    this->accept();
                });
            });

    connect(&d->mTimer, &QTimer::timeout, this, [this]
            {
                this->reject();
            });

    connect(&d->mFullsequenceTimer, &QTimer::timeout, d->mConfig, &ConfigInformation::onFullSequenceProcess);

    connect(d->mConfig, &ConfigInformation::onEmptySelectedContents,this ,[this]
            {
                d->mIsInitContentsData = false;
                d->mContentsErrorList.push_back(tr("Selected Contents List UnMatched"));
            });

    connect(d->mConfig, &ConfigInformation::onMissingSequenceFile,this ,[this] (QString contentsName)
            {
                d->mIsInitContentsData = false;
                d->mContentsErrorList.push_back(tr("Sequence Data File Missing Detected : [%1]").arg(contentsName));
            });

    connect(d->mConfig, &ConfigInformation::onContentsListMissMatch,this ,[this] (QString contentsName)
            {
                d->mIsInitContentsData = false;
                d->mContentsErrorList.push_back(tr("Contents List MissMatch Detected : [%1]").arg(contentsName));
            });

    connect(d->mConfig, &ConfigInformation::onTuningDataMissing,this ,[this] (QString contentsName)
            {
                d->mIsInitContentsData = false;
                d->mContentsErrorList.push_back(tr("TuningData Missing Detected : [%1]").arg(contentsName));
            });

    connect(d->mSerialInstance, &SerialCtrl::onRecvHeartBeatCommand, this, [this]
        {
            if(d->mHeartBeat.isActive() == true)
            {
                d->mHeartBeat.stop();

                if(this->start() == false)
                    d->mTimer.start();
            }
        }, Qt::UniqueConnection);

    connect(d->mSerialInstance, &SerialCtrl::onDeviceIsDisConnected, this, [this]
            {
                this->stopDeviceErrorStateForStopTimer();
                this->deviceShutDownProcess();
            });


    connect(d->mSerialInstance, &SerialCtrl::onDeviceErrorMsg, this, [this]
            (QString)
            {
                this->reject();
            });
}

bool InitDataDlg::nativeEvent(const QByteArray & /*eventType*/, void *msg, long * /*result*/)
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

        if(deviceType->dbch_devicetype != DBT_DEVTYP_PORT)
            return false;

        switch(message->wParam)
        {

        case DBT_DEVICEREMOVECOMPLETE:
            this->stopDeviceErrorStateForStopTimer();
            this->deviceShutDownProcess();
            break;
        }

    }break;

    }

    return false;
}

bool InitDataDlg::start()
{
    return d->mConfig->start();
}

void InitDataDlg::getData(bool& initContentsData, bool& isSequenceing, QStringList& errorList)
{
    initContentsData = d->mIsInitContentsData;
    isSequenceing = d->mIsSequencing;
    errorList = d->mContentsErrorList;
}

bool InitDataDlg::isDisconnectedProcess()
{
    return d->mIsDisconnectedProcess;
}

void InitDataDlg::sendHeartBeat()
{
    ui->listWidget->addItem(tr("Initializing S-Blot2 Please wait for moment"));

    connect(&d->mHeartBeat, &QTimer::timeout, this, [this]
            {
                auto heartBeatProtocol = WriteProtocolStatusRead::writeHeartBeat().toUtf8();
                d->mSerialInstance->writeQueuedData(heartBeatProtocol);
            });

    d->mHeartBeat.start();
}

void InitDataDlg::stopDeviceErrorStateForStopTimer()
{
    d->mHeartBeat.stop();
    d->mFullsequenceTimer.stop();
    d->mTimer.stop();
}

void InitDataDlg::deviceShutDownProcess()
{
    d->mIsDisconnectedProcess = true;

    auto connectionText = tr("The connection to the device has been lost. \n");
    auto noticeMessage =  tr("After 10 seconds, the SW will automatically shut down and please reboot the device. \n");
    auto cautionMessage =  tr("If the device does not restart before SW is run, abnormal behavior may occur. \n");

    QString msg;
    msg.append(connectionText);
    msg.append(noticeMessage);
    msg.append(cautionMessage);

    auto msgbox = new MessageShowDlg(msg, this);
    msgbox->exec();

    this->reject();
}
