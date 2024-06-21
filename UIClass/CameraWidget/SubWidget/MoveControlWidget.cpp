#include "Common/CLogWriter.h"

#include "MoveControlWidget.h"
#include "ui_MoveControlWidget.h"

#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/WriteProtocolBarcodePage.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"

#include "Network/PacketReader/PacketReader.h"
#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct MoveControlWidget::privateStruct
{
    ConfigInformation* mConfigInstance = nullptr;
};

MoveControlWidget::MoveControlWidget(QWidget *parent) :
    CSerialNetworkWidget(parent),
    ui(new Ui_MoveControlWidget),
    d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    this->setRange(1, ConfigInformation::getInstance()->getStripCount());
}

MoveControlWidget::~MoveControlWidget()
{
    this->stop();
}

void MoveControlWidget::initVariables()
{
    d->mConfigInstance = ConfigInformation::getInstance();
}

void MoveControlWidget::initSignalSlots()
{
    connect(ui->btnBack, SIGNAL(clicked()), SLOT(onClickedBack()));
    connect(ui->btnFront, SIGNAL(clicked()), SLOT(onClickedFront()));
    connect(ui->btnMove, SIGNAL(clicked()), SLOT(onClickedMove()));
    connect(ui->btnMoveQR, &QPushButton::clicked, this, &MoveControlWidget::onClickedQR);
    connect(d->mConfigInstance, &ConfigInformation::onChangedStripPoint, this, [this]
            (int point)
            {
                ui->spinMove->setValue(point);
            });
}

void MoveControlWidget::startWidget()
{
}

void MoveControlWidget::stopWidget()
{
}

void MoveControlWidget::onClickedBack()
{
    auto position = ui->spinMove->value();
    position--;

    ui->spinMove->setValue(position);
    CLogWriter::printLog("<MoveControlWidget::onClickedPreBack> : Clicked Back");
    this->sendMoveCommand(ui->spinMove->value());
}

void MoveControlWidget::onClickedMove()
{
    CLogWriter::printLog(QString("<MoveControlWidget::onClickedPreMove> : Clicked Move To : %1").arg(QString::number(ui->spinMove->value())));
    this->sendMoveCommand(ui->spinMove->value());
}

void MoveControlWidget::onClickedFront()
{
    auto position = ui->spinMove->value();
    position++;

    ui->spinMove->setValue(position);
    CLogWriter::printLog("<MoveControlWidget::onClickedPreFront> : Clicked Front");
    this->sendMoveCommand(ui->spinMove->value());
}

void MoveControlWidget::onClickedQR()
{
    this->moveTripleQR();
}

void MoveControlWidget::setRange(int min, int max)
{
    ui->spinMove->setMinimum(min);
    ui->spinMove->setMaximum(max);
}

void MoveControlWidget::sendMoveCommand(int position)
{

    QString protocolStr = WriteProtocolCameraLedSetting::writeSendCameraPosMoveReq(position);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);

    d->mConfigInstance->setCurrentStripPoint(position);
}

void MoveControlWidget::moveTripleQR()
{
    auto positionProtocol = WriteProtocolBarcodePage::writeQRTrayMove().toUtf8();
    this->onQueuedSend(positionProtocol);
}

