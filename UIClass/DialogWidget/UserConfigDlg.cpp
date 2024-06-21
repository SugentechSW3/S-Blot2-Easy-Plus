#include <QFileDialog>
#include <QMenu>

#include "UserConfigDlg.h"
#include "ui_UserConfigDlg.h"

#include "Common/CLogWriter.h"
#include "Common/Io/INIFileControls/GenericIniDataControls.h"
#include "Common/CUtil.h"

#include "Network/PacketWriter/WriteProtocolStatusRead.h"
#include "Network/PacketWriter/WriteProtocolDispAspPage.h"
#include "Network/PacketWriter/WriteProtocolControlRead.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"
#include "Network/PacketReader/PacketReader.h"
#include "Network/SerialCtrl.h"

#include "UIClass/DialogWidget/PasswordChangeDig.h"
#include "UIClass/DialogWidget/InterfaceServerIPSettingDlg.h"

#include "Analysis/ConfigInformation.h"

#include "AboutAndOpenSourceDlg.h"
#include "HiddenBandCheckDlg.h"

USING_SUGENTECH

struct UserConfigDlg::privateStruct
{
    QVector<QCheckBox*> mCheckBoxList;
    SerialCtrl* mSerialInstance = nullptr;
    QString mPath;
};

UserConfigDlg::UserConfigDlg(UserConfigData initData, QWidget *parent) :
                                                                                    CDialogParentWidget(parent),
                                                                                    ui(new Ui_UserConfigDlg),
                                                                                    d(new privateStruct)
{
    ui->setupUi(this);
    d->mSerialInstance = SerialCtrl::getInstance();
    this->initVariable();
    this->initSignalSlots();
    this->initVersion(initData);
    this->initAuthority();
    this->loadConfigData();
    this->readASPBathPosition();
}

UserConfigDlg::~UserConfigDlg()
{
}

void UserConfigDlg::initVariable()
{   
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    ui->btnASPHome->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                       ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");
    ui->btnASPDown->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                       ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    d->mCheckBoxList << ui->checkClass0;
    d->mCheckBoxList << ui->checkClass1;
    d->mCheckBoxList << ui->checkClass2;
    d->mCheckBoxList << ui->checkClass3;
    d->mCheckBoxList << ui->checkClass4;
    d->mCheckBoxList << ui->checkClass5;
    d->mCheckBoxList << ui->checkClass6;

}

void UserConfigDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, &UserConfigDlg::onSaveConfigData);
    connect(ui->btnCancel, &QPushButton::clicked, this, &UserConfigDlg::reject);

    connect(ui->checkClass0, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass0);
    connect(ui->checkClass1, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass1);
    connect(ui->checkClass2, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass2);
    connect(ui->checkClass3, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass3);
    connect(ui->checkClass4, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass4);
    connect(ui->checkClass5, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass5);
    connect(ui->checkClass6, &QCheckBox::clicked, this, &UserConfigDlg::onClickHighlightClass6);
    connect(ui->btnLogoFileOpen, &QPushButton::clicked, this, &UserConfigDlg::onClickLogoFileOpen);

    connect(this, &UserConfigDlg::customContextMenuRequested, this, &UserConfigDlg::onAboutPopupMenu);

    connect(ui->btnASPHome, &QPushButton::clicked,this,&UserConfigDlg::onClickASPHome);
    connect(ui->btnASPDown, &QPushButton::clicked,this,&UserConfigDlg::onClickASPBath);
    connect(ui->btnACCount, &QPushButton::clicked,this,[]
    {
        PasswordChangeDig settingPassswordView;
        settingPassswordView.exec();
    });
    connect(d->mSerialInstance,&SerialCtrl::onRecvDataForSerial,this ,&UserConfigDlg::onRecvData);
    connect(ui->btnUserIPSettings, &QPushButton::clicked, this, [this]
    {
        InterfaceServerIPSettingDlg dlg(this);
        dlg.exec();
    });
}

void UserConfigDlg::initVersion(const UserConfigData & initData)
{
    ui->labelHW->setText(initData.hwVersion);
    ui->labelFW->setText(initData.fwVersion);
    ui->labelSW->setText(initData.swVersion);
}

void UserConfigDlg::initAuthority()
{
    auto config = ConfigInformation::getInstance();

    if(config->isAdminMode())
    {
        ui->groupCutOffSetting->show();        
        ui->btnACCount->show();
        ui->gbLogoFileUpload->show();        
        ui->grouptIgEZeroConditionSetting->show();
        ui->groupsIgEZeroConditionSetting->show();

        ui->widgetASPControl->hide();
    }

    else
    {
        ui->groupCutOffSetting->hide();        
        ui->btnACCount->hide();
        ui->gbLogoFileUpload->hide();        
        ui->grouptIgEZeroConditionSetting->hide();
        ui->groupsIgEZeroConditionSetting->hide();

        ui->widgetASPControl->show();
    }

}

void UserConfigDlg::saveHighLightClass()
{
    GlobalDataStruct::HIGH_LIGHT_DATA data;
    data.class0 = ui->checkClass0->isChecked();
    data.class1 = ui->checkClass1->isChecked();
    data.class2 = ui->checkClass2->isChecked();
    data.class3 = ui->checkClass3->isChecked();
    data.class4 = ui->checkClass4->isChecked();
    data.class5 = ui->checkClass5->isChecked();
    data.class6 = ui->checkClass6->isChecked();

    GenericIniDataControls genericData;
    genericData.writeClassHightLightData(data);
}

void UserConfigDlg::loadHighLightClass()
{
    GenericIniDataControls genericData;
    auto hightLightData = genericData.readClassHightLightData();

    ui->checkClass0->setChecked(hightLightData.class0);
    ui->checkClass1->setChecked(hightLightData.class1);
    ui->checkClass2->setChecked(hightLightData.class2);
    ui->checkClass3->setChecked(hightLightData.class3);
    ui->checkClass4->setChecked(hightLightData.class4);
    ui->checkClass5->setChecked(hightLightData.class5);
    ui->checkClass6->setChecked(hightLightData.class6);
}

void UserConfigDlg::saveUseLowLevelDataConfig()
{
    GenericIniDataControls io;
    auto isUseLowLevelData = ui->checkUseLowLevelData->isChecked();
    io.writeUseLowLevelConfigData(isUseLowLevelData);
}

void UserConfigDlg::loadUseLowLevelDataConfig()
{
    GenericIniDataControls io;
    auto isUseLowLevelData = io.readUseLowLevelConfigData();
    ui->checkUseLowLevelData->setChecked(isUseLowLevelData);
}

void UserConfigDlg::savePCCutOff()
{
    GenericIniDataControls io;
    auto value = ui->spinPCCutOff->value();
    io.writePCCutOff(value);
}

void UserConfigDlg::loadPCCutOff()
{
    GenericIniDataControls io;
    auto value = io.readPCCutOff();
    ui->spinPCCutOff->setValue(value);
}

void UserConfigDlg::saveTigeCutOff()
{
    GenericIniDataControls io;
    auto value = ui->spinTigeCutOff->value();
    io.writeTIGECutOff(value);
}

void UserConfigDlg::loadTigeCutOff()
{
    GenericIniDataControls io;
    auto value = io.readTIGECutOff();
    ui->spinTigeCutOff->setValue(value);
}

void UserConfigDlg::readASPBathPosition()
{
    auto protocolStr = WriteProtocolDispAspPage::writeAspPinBathRead();
    auto sendData = protocolStr.toUtf8();
    d->mSerialInstance->writeQueuedData(sendData);
}

void UserConfigDlg::savetIgEZeroConditionSetting()
{
    GenericIniDataControls io;
    auto value = ui->spintIgEZeroConditionSettingValue->value();
    io.writetIgEZeroConditionSetting(value);
}

void UserConfigDlg::loadtIgEZeroConditionSetting()
{
    GenericIniDataControls io;
    auto value = io.readtIgEZeroConditionSetting();
    ui->spintIgEZeroConditionSettingValue->setValue(value);
}

void UserConfigDlg::saveBackgroundCutoff()
{
    GenericIniDataControls io;
    auto value = ui->spinBackgroundCutoff->value();
    io.writeBackgroundCutoffData(value);
}

void UserConfigDlg::loadBackgroundCutoff()
{
    GenericIniDataControls io;
    auto value = io.readBackgroundCutoffData();
    ui->spinBackgroundCutoff->setValue(value);
}

void UserConfigDlg::saveUnmarkIUML()
{
    GenericIniDataControls io;
    auto value = ui->checktIgEUnmark->isChecked();
    io.writeUnmarkIUML(value);
}

void UserConfigDlg::loadUnmarkIUML()
{
    GenericIniDataControls io;
    auto value = io.readUnmarkIUML();
    ui->checktIgEUnmark->setChecked(value);
}

void UserConfigDlg::saveDisableDBTigECutoff()
{
    GenericIniDataControls io;
    auto value = ui->checkDisableDatabasetIgECutoff->isChecked();
    io.writeDisableDatabaseTigeCutoff(value);
}

void UserConfigDlg::loadDisableDBTigECutoff()
{
    GenericIniDataControls io;
    auto value = io.readDisableDatabaseTigeCutoff();
    ui->checkDisableDatabasetIgECutoff->setChecked(value);
}

void UserConfigDlg::saveUseXML()
{
    GenericIniDataControls io;
    auto isUseXML = ui->checkBoxXML->isChecked();
    io.writeUseXML(isUseXML);
}

void UserConfigDlg::loadUseXML()
{
    GenericIniDataControls io;
    auto isUseXML = io.readUseXML();
    ui->checkBoxXML->setChecked(isUseXML);
}

void UserConfigDlg::onSaveConfigData()
{
    this->saveConfigData();
    this->accept();
}

void UserConfigDlg::saveConfigData()
{
    this->saveHighLightClass();
    this->saveUseLowLevelDataConfig();
    this->savePCCutOff();
    this->saveTigeCutOff();
    this->saveBackgroundCutoff();
    this->savetIgEZeroConditionSetting();
    this->saveUnmarkIUML();
    this->saveUseXML();
    this->saveDisableDBTigECutoff();

    ui->widgetSigE->savesIgEData();
}

void UserConfigDlg::loadConfigData()
{
    this->loadHighLightClass();
    this->loadUseLowLevelDataConfig();
    this->loadPCCutOff();
    this->loadTigeCutOff();
    this->loadBackgroundCutoff();
    this->loadtIgEZeroConditionSetting();
    this->loadUnmarkIUML();
    this->loadUseXML();
    this->loadDisableDBTigECutoff();

    ui->widgetSigE->loadsIgEData();
}

void UserConfigDlg::onClickHighlightClass0()
{
    this->allCheckBoxDisable();
    this->setCheckBoxEnable(0);
}

void UserConfigDlg::onClickHighlightClass1()
{
    this->allCheckBoxDisable();
    this->setCheckBoxEnable(1);
}

void UserConfigDlg::onClickHighlightClass2()
{
    this->allCheckBoxDisable();
    this->setCheckBoxEnable(2);
}

void UserConfigDlg::onClickHighlightClass3()
{
    this->allCheckBoxDisable();
    this->setCheckBoxEnable(3);
}

void UserConfigDlg::onClickHighlightClass4()
{
    this->allCheckBoxDisable();
    this->setCheckBoxEnable(4);
}

void UserConfigDlg::onClickHighlightClass5()
{
    this->allCheckBoxDisable();
    this->setCheckBoxEnable(5);
}

void UserConfigDlg::onClickHighlightClass6()
{
    if(this->isUnCheckLastClass())
        return;

    this->allCheckBoxDisable();
    this->setCheckBoxEnable(6);
}

void UserConfigDlg::onAboutPopupMenu(const QPoint& point)
{
    QMenu menu;
    menu.addAction(tr("About"), this, &UserConfigDlg::onShowAboutDlg);

    auto globalPoint = this->mapToGlobal(point);
    menu.exec(globalPoint);
}

void UserConfigDlg::onShowAboutDlg()
{
    AboutAndOpenSourceDlg dlg(this);
    dlg.exec();
}

void UserConfigDlg::onClickASPHome()
{
    QString protocolStr = WriteProtocolControlRead::writeASPHome();
    QByteArray sendData = protocolStr.toUtf8();
    d->mSerialInstance->writeQueuedData(sendData);
}

void UserConfigDlg::onClickASPBath()
{
    auto value = ConfigInformation::getInstance()->getCurrentASPBathPosition();
    QString protocolStr = WriteProtocolDispAspPage::writeAspPinBathSet(value);
    QByteArray sendData = protocolStr.toUtf8();
    d->mSerialInstance->writeQueuedData(sendData);
}

void UserConfigDlg::onClickLogoFileOpen()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Select LogFile"), QDir::homePath(),
                                             tr("Images (*.png *.jpg);;All files (*.*)"));

    if(path.isEmpty() == false)
        d->mPath = path;

    CUtil::logoResizeImage(d->mPath);
}

void UserConfigDlg::onRecvData(QByteArray recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_QR_SETTING_MODE:
       {
            if(PacketReader::commandReader(recvData)== CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_BATH_ASP_PIN_HEIGHT_READ)
            {
                 auto position = PacketReader::dataReader(recvData).toUInt(nullptr, 16);
                 ConfigInformation::getInstance()->setCurrentASPBathPosition(position);

                 CLogWriter::printLog(QString("[UserConfigDlg::] : CMD_MOTOR_SETTING_MODE_BATH_ASP_PIN_HEIGHT_READ"));
            }
        }
        break;
    }
}

void UserConfigDlg::setCheckBoxEnable(int startPos)
{
    for(auto i = startPos ; i < d->mCheckBoxList.count(); i++)
        d->mCheckBoxList[i]->setChecked(true);
}

void UserConfigDlg::allCheckBoxDisable()
{
    for(auto&itr : d->mCheckBoxList)
        itr->setChecked(false);
}

bool UserConfigDlg::isUnCheckLastClass()
{
    for(auto i = 0 ; i < d->mCheckBoxList.count() - 1 ; i++)
    {
        auto itr = d->mCheckBoxList[i];

        if(itr->isChecked())
            return false;
    }

    return true;
}
