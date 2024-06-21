#include "AspControlDlg.h"
#include "ui_AspControlDlg.h"

#include "UIClass/DialogWidget/StatusDialog.h"

#include "Analysis/ConfigInformation.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"

struct AspControlDlg::privateStruct
{
    SerialCtrl* mSerial = nullptr;
};

AspControlDlg::AspControlDlg(QWidget *parent) :
                                                CDialogParentWidget(parent),
                                                ui(new Ui_AspControlDlg),
                                                d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
}

AspControlDlg::~AspControlDlg()
{

}

void AspControlDlg::init()
{
    d->mSerial = SerialCtrl::getInstance();
    auto config = ConfigInformation::getInstance();
    auto stripCount = config->getStripCount();

    ui->spinAspStrip->setMaximum(stripCount);

    connect(ui->btnAspFunc, &QPushButton::clicked, this, &AspControlDlg::onClickAspFunc);
    connect(ui->btnClose, &QPushButton::clicked, this, &AspControlDlg::reject);
}

void AspControlDlg::initStyleSheet()
{
    ui->btnAspFunc->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnClose->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void AspControlDlg::onClickAspFunc()
{
    auto stripNumber = ui->spinAspStrip->value();
    QString protocolStr = WriteProtocolETCPage::writeAspirationTestSet(stripNumber, 2);
    QByteArray sendData = protocolStr.toUtf8();
    d->mSerial->writeQueuedData(sendData);

    this->accept();
}

int AspControlDlg::getStripCount()
{
    return ui->spinAspStrip->value();
}
