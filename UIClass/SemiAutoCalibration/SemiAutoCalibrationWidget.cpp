#include "SemiAutoCalibrationWidget.h"
#include "ui_SemiAutoCalibrationWidget.h"

#include "Network/PacketWriter/WriteProtocolDIWaterPage.h"
#include "Network/PacketWriter/WriteProtocolGeneralSetting.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"
#include "Network/PacketWriter/WriteProtocolControlRead.h"
#include "Network/PacketWriter/WriteProtocolTraySetting.h"
#include "Network/PacketWriter/WriteProtocolDispAspPage.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"
#include "Network/PacketReader/PacketReader.h"

struct SemiAutoCalibrationWidget::privateStruct
{
    int mAmountBath = 0;    
};

SemiAutoCalibrationWidget::SemiAutoCalibrationWidget(QWidget *parent) :
                                                                        CSerialNetworkWidget(parent),
                                                                        ui(new Ui_SemiAutoCalibrationWidget),
                                                                        d(new privateStruct)
{
    ui->setupUi(this);
    ui->btnWasteBottleSensorON->setVisible(false);
    ui->groupBox_6->setVisible(false); // TODO : 추후 아예 삭제 || 다른데서 처리    
    ui->groupBox_17->setVisible(false);
    this->init();
}

SemiAutoCalibrationWidget::~SemiAutoCalibrationWidget()
{
}

void SemiAutoCalibrationWidget::initVariables()
{
}

void SemiAutoCalibrationWidget::initSignalSlots()
{
    connect(ui->btnReadAll, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickReadAllParam);
    connect(ui->btnWasteBottleSensorON, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickWasteBottleSensorON);

    connect(ui->btnShakeSpeedMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeSpeedMove);
    connect(ui->btnShakePeriodMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakePeriodMove);

    connect(this, &SemiAutoCalibrationWidget::onReadReady, this, &SemiAutoCalibrationWidget::onRecvData);

    this->pumpSettingSignalSlots();
    this->xAxisSettingSignalSlots();
    this->shakeSettingSignalSlots();
    this->aspPinSettingSignalSlots();

}

void SemiAutoCalibrationWidget::startWidget()
{
    SemiAutoCalibrationWidget::onClickReadAllParam(true);
}

void SemiAutoCalibrationWidget::stopWidget()
{

}

void SemiAutoCalibrationWidget::xAxisSettingSignalSlots()
{
    connect(ui->btnXAxisHome, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickXAxisHome);
    connect(ui->btnXAxisBathMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickXAxisBathMove);
    connect(ui->btnXAxisASPMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickXAxisASPMove);
    connect(ui->btnXAxisDSPMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickXAxisDSPMove);

    connect(ui->btnReadXAxisBathMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeXAxisBathPosRead();
    });
    connect(ui->btnReadXAxisASPMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeXAxisAspPosRead();
    });
    connect(ui->btnReadXAxisDSPMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeXAxisDspPosRead();
    });
}

void SemiAutoCalibrationWidget::pumpSettingSignalSlots()
{
    connect(ui->btnPumpApply, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickPumpOffsetSet);
    connect(ui->btnPumpSet, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickPumpTimeSet);
    connect(ui->btnReadPumpVolumn, &QPushButton::clicked, this, [=]
    {
        auto pumpChannel = ui->comboPumpChannel->currentText().toInt();
        TunnningDataProtocolSendHelper::writePumpOffsetRead(pumpChannel);
    });
}

void SemiAutoCalibrationWidget::shakeSettingSignalSlots()
{
    connect(ui->btnShakeHome, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeHome);
    connect(ui->btnShakeAspMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeAspMove);
    connect(ui->btnShakeDSPMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeDspMove);
    connect(ui->btnShakeDryMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeDryMove);
    connect(ui->btnShakeAnalysisMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeAnalysisMove);
    connect(ui->btnShakeSampleDSPMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeSampleDspMove);
    connect(ui->btnShakeHomeMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickShakeHomeMove);

    connect(ui->btnReadShakeAspMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeShakeAspPosRead();
    });
    connect(ui->btnReadShakeDSPMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeShakeDspPosRead();
    });
    connect(ui->btnReadShakeDryMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeShakeDryPosRead();
    });
    connect(ui->btnReadShakeAnalysisMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeShakeCameraPosRead();
    });
    connect(ui->btnReadShakeSampleDSPMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeShakeSampleDspRead();
    });
    connect(ui->btnReadShakeHomeMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeShakeHomeRead();
    });

}

void SemiAutoCalibrationWidget::aspPinSettingSignalSlots()
{
    connect(ui->btnASPHome, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickAspHome);
    connect(ui->btnASPPinMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickAspPinMove);
    connect(ui->btnASPBathMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickAspBathMove);
    connect(ui->btnASPMove, &QPushButton::clicked, this, &SemiAutoCalibrationWidget::onClickAspTrayMove);

    connect(ui->btnReadASPBathMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeAspPinBathRead();
    });
    connect(ui->btnReadASPMove, &QPushButton::clicked, this, [=]
    {
        TunnningDataProtocolSendHelper::writeAspPinTrayRead();
    });
}

void SemiAutoCalibrationWidget::onClickReadAllParam(bool)
{
    auto pumpChannel = ui->comboPumpChannel->currentText().toInt();

    TunnningDataProtocolSendHelper::writePumpOffsetRead(pumpChannel);
    TunnningDataProtocolSendHelper::writePumpUseTimeRead(pumpChannel);

    TunnningDataProtocolSendHelper::writeXAxisBathPosRead();
    TunnningDataProtocolSendHelper::writeXAxisAspPosRead();
    TunnningDataProtocolSendHelper::writeXAxisDspPosRead();

    TunnningDataProtocolSendHelper::writeShakeAspPosRead();
    TunnningDataProtocolSendHelper::writeShakeDspPosRead();
    TunnningDataProtocolSendHelper::writeShakeDryPosRead();
    TunnningDataProtocolSendHelper::writeShakeCameraPosRead();
    TunnningDataProtocolSendHelper::writeShakeSampleDspRead();
    TunnningDataProtocolSendHelper::writeShakeHomeRead();

    TunnningDataProtocolSendHelper::writeAspPinBathRead();
    TunnningDataProtocolSendHelper::writeAspPinTrayRead();
//    TunnningDataProtocolSendHelper::writeShakeSpeedSpeedRead();
//    TunnningDataProtocolSendHelper::writeShakeSpeedPeriodRead();
}
//do not use
void SemiAutoCalibrationWidget::onClickWasteBottleSensorON(bool status)
{
    QString protocolStr = WriteProtocolGeneralSetting::writeWasteSensorOnOff(status);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);

    if(status == true)
        ui->btnWasteBottleSensorON->setText("OFF");
    else
        ui->btnWasteBottleSensorON->setText("ON");
}

void SemiAutoCalibrationWidget::onClickPumpOffsetSet(bool)
{
    auto channel = ui->comboPumpChannel->currentText().toInt();
    auto volumn = ui->spinPumpVolumn->value();

    TunnningDataProtocolSendHelper::writePumpOffsetWrite(channel, volumn);
}

void SemiAutoCalibrationWidget::onClickPumpTimeSet(bool)
{
    auto channel = ui->comboPumpChannel->currentText().toInt();
    auto value = ui->spinPumpTime->value();
    ui->spinPumpTimeOFUse->setValue(value);

    TunnningDataProtocolSendHelper::writePumpUseTimeWrite(channel, value);
}

void SemiAutoCalibrationWidget::onClickXAxisHome(bool)
{
    QString protocolStr = WriteProtocolControlRead::writeXAxisHome();
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void SemiAutoCalibrationWidget::onClickXAxisBathMove(bool)
{
    auto value = ui->spinXAxisBathValue->value();

    TunnningDataProtocolSendHelper::writeXAxisBathPosWrite(value);
    ui->spinXAxisBathCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickXAxisASPMove(bool)
{
    auto value = ui->spinXAxisASPValue->value();

    TunnningDataProtocolSendHelper::writeXAxisAspPosWrite(value);
    ui->spinXAxisASPCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickXAxisDSPMove(bool)
{
    auto value = ui->spinXAxisDSPValue->value();

    TunnningDataProtocolSendHelper::writeXAxisDspPosWrite(value);
    ui->spinXAxisDSPCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeHome(bool)
{
    QString protocolStr = WriteProtocolTraySetting::writeTrayHome();
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void SemiAutoCalibrationWidget::onClickShakeAspMove(bool)
{   
    auto value = ui->spinShakeASPValue->value();

    TunnningDataProtocolSendHelper::writeShakeAspPosWrite(value);
    ui->spinShakeASPCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeDspMove(bool)
{
    auto value = ui->spinShakeDSPValue->value();

    TunnningDataProtocolSendHelper::writeShakeDspPosWrite(value);
    ui->spinShakeDSPCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeDryMove(bool)
{
    auto value = ui->spinShakeDryValue->value();

    TunnningDataProtocolSendHelper::writeShakeDryPosWrite(value);
    ui->spinShakeDryCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeAnalysisMove(bool)
{
    auto value = ui->spinShakeAnalysisValue->value();

    TunnningDataProtocolSendHelper::writeShakeCameraPosWrite(value);
    ui->spinShakeAnalysisCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeSampleDspMove(bool)
{
    auto value = ui->spinShakeSampleDSPValue->value();

    TunnningDataProtocolSendHelper::writeShakeSampleDspWrite(value);
    ui->spinShakeSampleDSPCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeHomeMove(bool)
{
    auto value = ui->spinShakeHomeValue->value();

    TunnningDataProtocolSendHelper::writeShakeHomeWrite(value);
    ui->spinShakeHomeCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickAspHome(bool)
{
    QString protocolStr = WriteProtocolControlRead::writeASPHome();
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void SemiAutoCalibrationWidget::onClickAspPinMove(bool)
{
    auto value = ui->spinASPPinValue->value();

    QString protocolStr = WriteProtocolDispAspPage::writeAspPinMove(value);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void SemiAutoCalibrationWidget::onClickAspBathMove(bool)
{
    auto value = ui->spinASPBathValue->value();

    TunnningDataProtocolSendHelper::writeAspPinBathWrite(value);
    ui->spinASPBathCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickAspTrayMove(bool)
{
    auto value = ui->spinASPTrayValue->value();

    TunnningDataProtocolSendHelper::writeAspPinTrayWrite(value);
    ui->spinASPTrayCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakeSpeedMove(bool)
{
    auto value = ui->spinShakeSpeedValue->value();

    TunnningDataProtocolSendHelper::writeShakeSpeedSpeedWrite(value);
    ui->spinShakeSpeedCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onClickShakePeriodMove(bool)
{
    auto value = ui->spinShakePeriodValue->value();

    TunnningDataProtocolSendHelper::writeShakeSpeedPeriodWrite(value);
    ui->spinShakePeriodCurrent->setValue(value);
}

void SemiAutoCalibrationWidget::onRecvData(QByteArray recvData)
{
    switch(PacketReader::modeReader(recvData))
    {



    case MD_MODE::MD_MODE_PUMP_SETTING_MODE:
    {
        auto value = PacketReader::dataReader(recvData).right(4).toUInt(nullptr,16);

        switch(PacketReader::commandReader(recvData))
        {
        case CMD_PROTOCOL_PUMP_SETTING_MODE::CMD_PROTOCOL_PUMP_SETTING_MODE_OFFSET_SET :
        {            
            ui->spinPumpCurrentVolumn->setValue(value);
            ui->spinPumpVolumn->setValue(value);
        } break;

        case CMD_PROTOCOL_PUMP_SETTING_MODE::CMD_PROTOCOL_PUMP_SETTING_MODE_USE_TIME_READ :
        {
            ui->spinPumpTimeOFUse->setValue(value);
            ui->spinPumpTime->setValue(value);
        } break;

        }

    }   break;

    case MD_MODE::MD_MODE_MOTOR_SETTING_MODE:
    {
        auto value = PacketReader::dataReader(recvData).right(4).toUInt(nullptr,16);

        switch(PacketReader::commandReader(recvData))
        {

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_BATH_X_POS_READ:
        {            
            ui->spinXAxisBathCurrent->setValue(value);
            ui->spinXAxisBathValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_ASP_X_START_READ:
        {            
            ui->spinXAxisASPCurrent->setValue(value);
            ui->spinXAxisASPValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_DISP_X_START_READ:
        {            
            ui->spinXAxisDSPCurrent->setValue(value);
            ui->spinXAxisDSPValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_ASP_POS_READ:
        {            
            ui->spinShakeASPCurrent->setValue(value);
            ui->spinShakeASPValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_DSP_POS_READ:
        {            
            ui->spinShakeDSPCurrent->setValue(value);
            ui->spinShakeDSPValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_DRY_POS_READ:
        {
            ui->spinShakeDryCurrent->setValue(value);
            ui->spinShakeDryValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_CAMERA_POS_READ:
        {            
            ui->spinShakeAnalysisCurrent->setValue(value);
            ui->spinShakeAnalysisValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_SAMPLE_DSP_POS_READ:
        {            
            ui->spinShakeSampleDSPCurrent->setValue(value);
            ui->spinShakeSampleDSPValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_HOME_POS_READ:
        {
            ui->spinShakeHomeCurrent->setValue(value);
            ui->spinShakeHomeValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_ASP_JOG_RUN:
        {            
            ui->spinASPPinCurrent->setValue(value);
            ui->spinASPPinValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_BATH_ASP_PIN_HEIGHT_READ:
        {            
            ui->spinASPBathCurrent->setValue(value);
            ui->spinASPBathValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_ASP_PIN_HEIGHT_READ:
        {            
            ui->spinASPTrayCurrent->setValue(value);
            ui->spinASPTrayValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_READ:
        {            
            ui->spinShakeSpeedCurrent->setValue(value);
            ui->spinShakeSpeedValue->setValue(value);
        } break;

        case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_PERIOD_READ:
        {            
            ui->spinShakePeriodCurrent->setValue(value);
            ui->spinShakePeriodValue->setValue(value);
        } break;
        }

    }   break;

    } /*end of switch 1st*/
}
