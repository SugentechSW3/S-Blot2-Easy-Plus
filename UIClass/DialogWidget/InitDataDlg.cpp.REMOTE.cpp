#include <QTimer>
#include "InitDataDlg.h"
#include "ui_InitDataDlg.h"

#include "Common/CUtil.h"
#include "Analysis/ConfigInformation.h"

#include "CameraControl/CameraInstance.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h"

struct InitDataDlg::privateStruct
{
    SerialCtrl* mSerialInstance = nullptr;
    ConfigInformation* mConfig = nullptr;
    int mTotalCount = 0;
    float mPrograssSI = 0;

    bool mIsInitContentsData = true;
    bool mIsSequencing = false;
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
    d->mPrograssSI = 100 / (4 + d->mTotalCount) + 1;

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

    connect(d->mConfig, &ConfigInformation::startConfigDownLoad, this, [this]
            {
                ui->progressBar->setMovingValue(d->mPrograssSI * 1);
                ui->listWidget->addItem(tr("Downloads the initial settings"));
                ui->listWidget->addItem(tr("Downloading sequence data"));
            });

    connect(d->mConfig, &ConfigInformation::downloadGlobalData, this, [this]
            {
                ui->progressBar->setMovingValue(d->mPrograssSI * 2);
                ui->listWidget->addItem(tr("Downloading device settings"));
            });

    connect(d->mConfig, &ConfigInformation::downloadAnalysisData, this, [this]
            (int current)
            {
                ui->progressBar->setMovingValue(d->mPrograssSI * (3 + current));
                ui->listWidget->addItem(tr("DownLoading parameter settings %1/%2").arg(current).arg(d->mTotalCount));
            });

    connect(d->mConfig, &ConfigInformation::endConfigProtocol, this, [this]
            (bool isSequencing)
            {
                d->mHeartBeat.stop();
                d->mFullsequenceTimer.stop();

                ui->progressBar->setMovingValue(100);
                d->mIsSequencing = isSequencing;

                this->accept();
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
            });

    connect(d->mSerialInstance, &SerialCtrl::onNetworkErrorMsg, this, [this]
            (QString)
            {
                this->reject();
            });
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
    d->mTimer.start();
}
