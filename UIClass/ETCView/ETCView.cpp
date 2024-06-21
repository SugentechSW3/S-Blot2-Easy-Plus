#include "ETCView.h"
#include "ui_ETCView.h"

#include "Analysis/ConfigInformation.h"
#include "Analysis/TemperatureCalcClass.h"

#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"

#include "Common/CLogWriter.h"

USING_SUGENTECH
#include "mainwindow.h"
#include "UIClass/DialogWidget/CirclePrograssDialog.h"

struct ETCView::privateStruct
{
    ConfigInformation* mConfigInstance = nullptr;
};

ETCView::ETCView(QWidget *parent) :
                                    CSerialNetworkWidget(parent),
                                    ui(new Ui_ETCView),
                                    d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    prograssCircleDlg.reset(new CirclePrograssDialog(this));
}

ETCView::~ETCView()
{
}

void ETCView::initVariables()
{
    d->mConfigInstance = ConfigInformation::getInstance();
    this->setMaximumStripCount();
}

void ETCView::initSignalSlots()
{
    connect(ui->btnFanTestON, SIGNAL(clicked()), SLOT(onClickedFanTestON()));
    connect(ui->btnFanTestOFF, SIGNAL(clicked()), SLOT(onClickedFanTestOFF()));

    connect(ui->btnStatusLEDON, SIGNAL(clicked()), SLOT(onClickedStatusLEDON()));
    connect(ui->btnStatusLEDOFF, SIGNAL(clicked()), SLOT(onClickedStatusLEDOFF()));

    connect(ui->btnPrimeSet, SIGNAL(clicked()), SLOT(onClickPrimeFunc()));
    connect(ui->btnDispSet, SIGNAL(clicked()), SLOT(onClickDispFunc()));
    connect(ui->btnAspRun, SIGNAL(clicked()), SLOT(onClickAspFunc()));

    connect(ui->btnIncubation, &QPushButton::clicked, this, &ETCView::onClickIncubationFuc);

    connect(ui->btnAirTemperature, &QPushButton::clicked, this, &ETCView::onClickTemperature);

    connect(ui->btnHeatingPadTempCh1Read, &QPushButton::clicked, this, &ETCView::onClickedTrayHeatingPadTempCh1Read);
    connect(ui->btnHeatingPadTempCh1Write, &QPushButton::clicked, this, &ETCView::onClickedTrayHeatingPadTempCh1Write);
    connect(ui->btnHeatingPadTempCh2Read, &QPushButton::clicked, this, &ETCView::onClickedTrayHeatingPadTempCh2Read);
    connect(ui->btnHeatingPadTempCh2Write, &QPushButton::clicked, this, &ETCView::onClickedTrayHeatingPadTempCh2Write);

    connect(ui->btnHeatingPadON, &QPushButton::clicked, this, &ETCView::onclickedTrayHeatingPadON);
    connect(ui->btnHeatingPadOFF, &QPushButton::clicked, this, &ETCView::onclickedTrayHeatingPadOFF);

    connect(ui->btnTrayTemperature, &QPushButton::clicked, this, &ETCView::onClickedTrayAdcReverseTemperature);

    connect(this, &ETCView::onReadReady, this, &ETCView::onRecvData);
    connect(ui->btnAutoclean, &QPushButton::clicked, this, &ETCView::onClickAutoClean);
}

void ETCView::startWidget()
{
    this->onClickedTrayHeatingPadTempCh1Read();
    this->onClickedTrayHeatingPadTempCh2Read();
}

void ETCView::stopWidget()
{

}

void ETCView::setMaximumStripCount()
{
    auto stripCount = d->mConfigInstance->getStripCount();

    ui->spinDispStripNumber->setMaximum(stripCount);
    ui->spinAspStrip->setMaximum(stripCount);
}

void ETCView::setInterlock(int ms)
{
    if(!prograssCircleDlg->isHidden())
        return;

    prograssCircleDlg->start();
    QTimer::singleShot(ms, this, [this]
    {
        prograssCircleDlg->stop();
    });
}

void ETCView::releaseInterlock()
{
    if(prograssCircleDlg->isHidden())
        return;
    prograssCircleDlg->stop();
}

void ETCView::onClickedFanTestON()
{
    QString protocolStr = WriteProtocolETCPage::writeFanTestONOFF(true);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickedFanTestOFF()
{
    QString protocolStr = WriteProtocolETCPage::writeFanTestONOFF(false);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickedStatusLEDON()
{
    QString protocolStr = WriteProtocolETCPage::writeStatusLEDONOFF(true);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickedStatusLEDOFF()
{
    QString protocolStr = WriteProtocolETCPage::writeStatusLEDONOFF(false);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickPrimeFunc()
{
    auto ch = ui->comboChannel->currentText().toInt();

    QString protocolStr = WriteProtocolETCPage::writePrimeTest(ch, ui->spinPrimeTime->value());
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickDispFunc()
{
    auto ch = ui->comboChannel->currentText().toInt();
    auto stripNumber = ui->spinDispStripNumber->value();

    QString protocolStr = WriteProtocolETCPage::writeDispenseTestSet(ch, stripNumber, ui->spinDispVolumn->value());
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
    setInterlock();
}

void ETCView::onClickAspFunc()
{
    auto stripNumber = ui->spinAspStrip->value();

    QString protocolStr = WriteProtocolETCPage::writeAspirationTestSet(stripNumber, ui->spinAspTime->value());
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
    setInterlock();
}

void ETCView::onClickIncubationFuc()
{
    ushort incubationTime = ui->spinIncubationTime->value();
    QString protocolStr = WriteProtocolETCPage::writeIncubation(incubationTime);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickTemperature()
{
    int volt = ui->spinAirTemperatureVolt->value() * 10;

    QString protocolStr = WriteProtocolETCPage::writeTemperatureValue(volt);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onclickedTrayHeatingPadON()
{
    QString protocolStr = WriteProtocolETCPage::writeHeatingPadONOFF(true);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onclickedTrayHeatingPadOFF()
{
    QString protocolStr = WriteProtocolETCPage::writeHeatingPadONOFF(false);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickedTrayAdcReverseTemperature()
{
    ushort volt = ui->spinTrayTemperatureVolt->value() * 100;
    ushort res = 0;

    TemperatureCalcClass tempClass;
    auto temp = tempClass.clacTempFromStripTemp(ui->spinStripTemp->value(), ui->spinTrayCompensationValue->value());
    ui->spinTrayTemperatureValue->setValue(temp);

    res = tempClass.adcRawDataCalcTemperature(ui->spinTrayTemperatureB->value(), ui->spinTrayTemperatureValue->value());

    QString protocolStr = WriteProtocolETCPage::writeAdcRawDataReverseTemperature(volt, res);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void ETCView::onClickedTrayHeatingPadTempCh1Read()
{
    TunnningDataProtocolSendHelper::writeHeatingPadTempCh1Read();
}

void ETCView::onClickedTrayHeatingPadTempCh1Write()
{
    TunnningDataProtocolSendHelper::writeHeatingPadTempCh1Write(ui->spinHeatingPadTempCh1->value());
}

void ETCView::onClickedTrayHeatingPadTempCh2Read()
{
    TunnningDataProtocolSendHelper::writeHeatingPadTempCh2Read();
}

void ETCView::onClickedTrayHeatingPadTempCh2Write()
{
    TunnningDataProtocolSendHelper::writeHeatingPadTempCh2Write(ui->spinHeatingPadTempCh2->value());
}

void ETCView::onRecvData(QByteArray recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_QC_MODE:
    {
        switch(PacketReader::commandReader(recvData))
        {
        case CMD_PROTOCOL_QC_MODE::CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMPERATURE_READ:
        {
            auto value = PacketReader::dataReader(recvData);
            auto res = value.toInt(nullptr, 16);

            TemperatureCalcClass tempClass;
            auto c = tempClass.clacTemperature(ui->spinAirTemperatureB->value(), res);
            ui->spinAirTemperatureResult->setValue(c);

            CLogWriter::printLog(QString("(ETCView::) : CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMPERATURE_READ"));
        }break;

        case CMD_PROTOCOL_QC_MODE::CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH1_READ:
        {
            auto value = PacketReader::dataReader(recvData);
            ui->spinHeatingPadTempCh1->setValue(value.toUInt(nullptr, 16));
            CLogWriter::printLog(QString("(ETCView::)Recv : CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH1_READ"));
        }break;

        case CMD_PROTOCOL_QC_MODE::CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH2_READ:
        {
            auto value = PacketReader::dataReader(recvData);
            ui->spinHeatingPadTempCh2->setValue(value.toUInt(nullptr, 16));
            CLogWriter::printLog(QString("(ETCView::)Recv : CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH2_READ"));
        }break;

        }

    }break;
    }
}

void ETCView::onClickAutoClean()
{
    auto mainWindow = MainWindow::getInstance();
    mainWindow->startAutoCleanProcess();
}
