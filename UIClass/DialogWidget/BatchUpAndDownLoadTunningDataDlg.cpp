#include <QFileDialog>

#include "BatchUpAndDownLoadTunningDataDlg.h"
#include "ui_BatchUpAndDownLoadTunningDataDlg.h"

#include "Common/Io/INIFileControls/BatchUpAndDownLoadForTunningData.h"

#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/StatusDialog.h"

#include "Network/SerialCtrl.h"

#include "mainwindow.h"
//실행부 - 별도 클래스 , ui부를 분리
//경고메시지 기재 - EEPROM은 최대 10만번까지 쓰기가 가능하며 잦은 사용은 수명을 줄이는 결과가 된다.


struct BatchUpAndDownLoadTunningDataDlg::privateStruct
{
    BatchUpAndDownLoadForTunningData mBatchControl;
    QString mSelectedDirName;
    QScopedPointer<StatusDialog> mWaitingMessageDlg;
    SerialCtrl* mSerial = nullptr;
};

BatchUpAndDownLoadTunningDataDlg::BatchUpAndDownLoadTunningDataDlg(QWidget *parent) :
                                                                                      CDialogParentWidget(parent),
                                                                                      ui(new Ui_BatchUpAndDownLoadTunningDataDlg),
                                                                                      d(new privateStruct)
{
    ui->setupUi(this);

    this->initVariables();
    this->initMessage();
    this->initSignalSlots();
}

BatchUpAndDownLoadTunningDataDlg::~BatchUpAndDownLoadTunningDataDlg()
{
}

void BatchUpAndDownLoadTunningDataDlg::initVariables()
{
    d->mSerial = SerialCtrl::getInstance();
}

void BatchUpAndDownLoadTunningDataDlg::initMessage()
{
    d->mWaitingMessageDlg.reset(new StatusDialog(this));
    d->mWaitingMessageDlg->setStatusMessage("Exchange contents data. Please wait a moment.");
}

void BatchUpAndDownLoadTunningDataDlg::initSignalSlots()
{
    connect(ui->btnDownLoad, &QPushButton::clicked, this, [this]
            {
                this->onStartMessage();
                d->mBatchControl.downLoadAlldata();
            });

    connect(ui->btnExit, &QPushButton::clicked, this, [this]
            {
                this->reject();
            });

    connect(d->mSerial, &SerialCtrl::onDeviceIsDisConnected, this, [this]
            {
                this->onStopMessage();
                this->reconnectedMessage();
            });

    connect(&d->mBatchControl, &BatchUpAndDownLoadForTunningData::onExchangeSuccess, this, [this]
            {
                this->onStopMessage();
            });
}

QString BatchUpAndDownLoadTunningDataDlg::getDirFolderName()
{
    return d->mSelectedDirName;
}

void BatchUpAndDownLoadTunningDataDlg::emptyPathErrorMsg()
{
    MessageShowDlg* dlg = new MessageShowDlg("No Storage path selected",this);
    dlg->exec();
}

void BatchUpAndDownLoadTunningDataDlg::unsupportedDirNameMsg()
{
    auto getSupportedDirList = BatchUpAndDownLoadForTunningData::getSupportedDirList();
    QString supportName;

    for(auto& itr : getSupportedDirList)
        supportName.append(itr + " ");

    MessageShowDlg* dlg = new MessageShowDlg(QString("This folder has an unsupported name. You can only use the following names : %1").arg(supportName), this);
    dlg->exec();
}

void BatchUpAndDownLoadTunningDataDlg::reconnectedMessage()
{
    MessageShowDlg* dlg = new MessageShowDlg(QString("Network Error Detected"), this);
    dlg->exec();
}

void BatchUpAndDownLoadTunningDataDlg::onStartMessage()
{
    d->mWaitingMessageDlg->startWaiting();
}

void BatchUpAndDownLoadTunningDataDlg::onStopMessage()
{
    d->mWaitingMessageDlg->stopWaiting();
}
