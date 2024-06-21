#include "MainView.h"
#include "ui_MainView.h"

#include "Common/CUtil.h"
#include "Common/CLogWriter.h"

#include "mainwindow.h"


#include "Analysis/ConfigInformation.h"

#include "UIClass/CameraWidget/CameraView.h"
#include "UIClass/SequenceWidget/SequenceView.h"
#include "UIClass/ResultView/ResultView.h"
#include "UIClass/WorkPlace/WorkPlaceView.h"
#include "UIClass/ETCView/ETCView.h"
#include "UIClass/UserButtonMenu.h"
#include "UIClass/AdminButtonMenu.h"
#include "UIClass/SemiAutoCalibration/SemiAutoCalibrationWidget.h"

#include "UIClass/DialogWidget/InitDataDlg.h"
#include "UIClass/DialogWidget/UserConfigDlg.h"
#include "UIClass/DialogWidget/LoginDialog.h"
#include "UIClass/DialogWidget/ContetnsSelectDlg.h"
#include "UIClass/DialogWidget/StatusDialog.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/AspControlDlg.h"
#include "UIClass/DialogWidget/InterfaceServerIPSettingDlg.h"
#include "UIClass/DialogWidget/LanguageSelectDlg.h"
#include "UIClass/DialogWidget/BatchUpAndDownLoadTunningDataDlg.h"
#include "UIClass/DialogWidget/ListToHiddenBandDlg.h"

#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h"
#include "Network/PacketWriter/WriteProtocolControlWrite.h"
#include "Network/PacketWriter/WriteProtocolBarcodePage.h"
#include "Network/PacketWriter/WriteProtocolETCPage.h"

#include "Analysis/ErrorResonIO.h"
#include <QMessageBox>

#include <QMessageBox>

USING_SUGENTECH

struct MainView::privateStruct
{
    QScopedPointer<CameraView> mAdminCameraView;
    QScopedPointer<SequenceView> mAdminSequenceView;
    QScopedPointer<UserButtonMenu> mUserButtonMenu;
    QScopedPointer<AdminButtonMenu> mAdminButtonMenu;
    QScopedPointer<SemiAutoCalibrationWidget> mAdminSemiAutoView;
    QScopedPointer<ETCView> mAdminETCView;

    QScopedPointer<WorkPlaceView> mUserWorkPlaceView;
    QScopedPointer<ResultView> mUserResultView;

    QMetaObject::Connection mWorkPlaceMemoryClearConnection;

    StatusDialog mWaitingAspDlg;

    QStringList mContentsErrorList;
    FW_VERSION mFWVersion;
    HW_VERSION mHWVersion;
    SW_VERSION mSWVersion;

    bool mIsSequenceing = false;
    bool mIsResetStatus = false;
    bool mIsInitContentsData = true;
    bool mIsSerialConnection = true;
    bool mIsOpenMessageShow = false;
};

MainView::MainView(QWidget *parent) :
    CSerialNetworkWidget(parent),
    ui(new Ui_MainWidget),
    d(new privateStruct)
{   
    ui->setupUi(this);
    this->init();

    this->onLoadDeviceVersion();
    this->onLoadQRTriplePosition();
    this->offLineGUIProcessing();

#ifdef IS_DEBUG_PC
    QMessageBox::warning(this, "Debug Mode", "Change Code(.pro)");
#endif
}

MainView::~MainView()
{
    this->stop();
}

void MainView::initVariables()
{
    qRegisterMetaType<QVector<GlobalDataStruct::AnalysisReportInfomation>>();
    this->initDownloadData();
    this->initLoginProcess();
    this->initGUI();
}

void MainView::initSignalSlots()
{
    auto config = ConfigInformation::getInstance();

    if(config->isAdminMode() == true)
    {
        if(d->mAdminButtonMenu.isNull() == true)
            return;

        if(d->mUserWorkPlaceView.isNull() == true)
            return;

        if(d->mUserResultView.isNull() == true)
            return;

        auto menuPtr = d->mAdminButtonMenu.data();

        connect(menuPtr, &AdminButtonMenu::onClickedETC, this, &MainView::onClickETCBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedCamera, this, &MainView::onClickCameraBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedSequence, this, &MainView::onClickSequenceBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedConfig, this, &MainView::onClickAdminModeConfigureBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedSemiAutoView, this, &MainView::onClickSemiautoBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedWorkPlace, this, &MainView::onClickWorkPlaceBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedResultView, this, &MainView::onClickResultViewBtn);
        connect(menuPtr, &AdminButtonMenu::onClickedServerIPSetting, this, &MainView::onClickIPSetting);
        connect(menuPtr, &AdminButtonMenu::onClickedLanguageSelect, this, &MainView::onClickLanguageSelect);
        connect(menuPtr, &AdminButtonMenu::onClickedDataExchange, this, &MainView::onClickDataExchange);
        connect(menuPtr, &AdminButtonMenu::onClickedHiddenBand, this, &MainView::onClickHiddenBand);

        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onResultShow, this, &MainView::onResult);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onEnableMenuWidget, this, &MainView::onEnableMenuWidget);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onDisableMenuWidget, this, &MainView::onDisableMenuWidget);
    }
    else
    {
        if(d->mUserButtonMenu.isNull() == true)
            return;

        if(d->mUserWorkPlaceView.isNull() == true)
            return;

        if(d->mUserResultView.isNull() == true)
            return;

        auto menuPtr = d->mUserButtonMenu.data();

        connect(menuPtr, &UserButtonMenu::onClickedWorkPlace, this, &MainView::onClickWorkPlaceBtn);
        connect(menuPtr, &UserButtonMenu::onClickedResultView, this, &MainView::onClickResultViewBtn);
        connect(menuPtr, &UserButtonMenu::onClickedReset, this, &MainView::onClickResetBtn);
        connect(menuPtr, &UserButtonMenu::onClickedConfigure, this, &MainView::onClickUserModeConfigureBtn);
        connect(menuPtr, &UserButtonMenu::onClickedContentsChange, this, &MainView::onClickContentsChangeBtn);
        connect(menuPtr, &UserButtonMenu::onClickedAutoClean, this, &MainView::onClickAutoCleanBtn);
        connect(menuPtr, &UserButtonMenu::onClickedStripScan, this, &MainView::onClickStripScan);
        connect(menuPtr, &UserButtonMenu::onClickedAspiration, this, &MainView::onClickAspration);

        connect(ui->btnMain, &QPushButton::clicked, ui->TitleBarWidget, &TitleStatusBarWidget::onStatusMain);

        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onPatientStarted, ui->TitleBarWidget, &TitleStatusBarWidget::onStatusPatientInfomation);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onReagentStarted, ui->TitleBarWidget, &TitleStatusBarWidget::onStatusReagent);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onRunningStarted, ui->TitleBarWidget, &TitleStatusBarWidget::onStatusRunningStatus);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onResultShow, this, &MainView::onResult);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onPageReset, this, &MainView::onResetGUI);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onEnableMenuWidget, this, &MainView::onEnableMenuWidget);
        connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onDisableMenuWidget, this, &MainView::onDisableMenuWidget);

        connect(d->mUserResultView.data(), &ResultView::onResultViewStarted, ui->TitleBarWidget, &TitleStatusBarWidget::onStatusResultView);
    }

    connect(ui->btnMain, &QPushButton::clicked, this, &MainView::onClickMainBtn);
    connect(ui->btnExit, &QPushButton::clicked, QApplication::instance(), &QApplication::closeAllWindows);
    connect(this, &MainView::onReadReady, this, &MainView::onRecvData);    
}

void MainView::initDownloadData()
{
    auto config = ConfigInformation::getInstance();

    InitDataDlg dlg;
    if(dlg.exec() == QDialog::Rejected)
    {
        d->mIsSerialConnection = false;

        QuestionDlg dlg;
        dlg.setQuestionMessage(tr("Do you want to enter offline mode? \n (Results can only be inquired and restarts \n are required when connecting the device.)"));

        if(dlg.exec() == QDialog::Accepted)
            config->setOffLine(true);
        else
        {
            SerialCtrl::getInstance()->closeApplication();
            ::exit(0);
            return;
        }
    }

    dlg.getData(d->mIsInitContentsData, d->mIsSequenceing, d->mContentsErrorList);
}

void MainView::initLoginProcess()
{
    auto config = ConfigInformation::getInstance();
    LoginDialog login(d->mIsInitContentsData, d->mIsSerialConnection, d->mIsSequenceing, d->mContentsErrorList);
//    LoginDialog login(true, true, false, d->mContentsErrorList);

    if(login.exec() == QDialog::Accepted)
    {
        if(login.getMode() == LoginDialog::LOGIN_MODE_ADMIN)
            config->setAdminMode(true);
    }
    else
    {
        SerialCtrl::getInstance()->closeApplication();
        ::exit(0);
        return;
    }
}

void MainView::initGUI()
{
    auto config = ConfigInformation::getInstance();

    if(config->isAdminMode() == true)
        this->initAdmin();
    else
        this->initUser();

    this->onClickMainBtn();
    this->initStyleSheetButton();
}

void MainView::initWaitDlg()
{
    auto topLevelWidget = MainWindow::getInstance();

    d->mWaitingAspDlg.setParent(topLevelWidget);
    d->mWaitingAspDlg.setStatusMessage(tr("Aspiration in progress. Please wait."));
    d->mWaitingAspDlg.stopWaiting();
}

void MainView::initAdmin()
{
    d->mUserWorkPlaceView.reset(new WorkPlaceView(ui->widgetMainView));
    d->mUserResultView.reset(new ResultView(ui->widgetMainView));

    d->mAdminButtonMenu.reset(new AdminButtonMenu(ui->widgetMainView));
    d->mAdminETCView.reset(new ETCView(ui->widgetMainView));
    d->mAdminSequenceView.reset(new SequenceView(ui->widgetMainView));
    d->mAdminSemiAutoView.reset(new SemiAutoCalibrationWidget(ui->widgetMainView));

    ui->gridLayoutMainView->addWidget(d->mUserWorkPlaceView.data(), 1, 0, 1, 1);
    ui->gridLayoutMainView->addWidget(d->mUserResultView.data(), 1, 0, 1, 1);
    ui->gridLayout->addWidget(d->mAdminETCView.data(), 1,0,1,1);
    ui->gridLayout->addWidget(d->mAdminSequenceView.data(), 1,0,1,1);
    ui->gridLayout->addWidget(d->mAdminSemiAutoView.data(), 1,0,1,1);

}

void MainView::initUser()
{
    d->mUserWorkPlaceView.reset(new WorkPlaceView(ui->widgetMainView));
    d->mUserResultView.reset(new ResultView(ui->widgetMainView));
    d->mUserButtonMenu.reset(new UserButtonMenu(ui->widgetMainView));

    ui->gridLayoutMainView->addWidget(d->mUserWorkPlaceView.data(), 1, 1, 1, 1);
    ui->gridLayoutMainView->addWidget(d->mUserResultView.data(), 1, 1, 1, 1);
    ui->gridLayoutMainView->addWidget(d->mUserButtonMenu.data(), 1, 1, 1, 1);
}

void MainView::initStyleSheetButton()
{
    ui->btnMain->setStyleSheetImage(":/Image/Image/Basic/home_normal.png",":/Image/Image/Basic/home_over.png",
                                    ":/Image/Image/Basic/home_focus.png",":/Image/Image/Basic/home_disable.png");
    ui->btnExit->setStyleSheetImage(":/Image/Image/Basic/power_normal.png",":/Image/Image/Basic/power_over.png",
                                    ":/Image/Image/Basic/power_focus.png",":/Image/Image/Basic/power_disable.png");
}

void MainView::startWidget()
{
    this->initWaitDlg();
}

void MainView::stopWidget()
{

}

void MainView::hideAllViews()
{
    if(d->mUserWorkPlaceView.isNull() == false)
    {
        d->mUserWorkPlaceView->hide();
        d->mUserWorkPlaceView->stop();
    }

    if(d->mUserResultView.isNull() == false)
    {
        d->mUserResultView->hide();
        d->mUserResultView->stop();
    }

    if(d->mUserButtonMenu.isNull() == false)
    {
        d->mUserButtonMenu->hide();
        d->mUserButtonMenu->stop();
    }

    if(d->mAdminCameraView.isNull() == false)
        d->mAdminCameraView.reset();

    if(d->mAdminButtonMenu.isNull() == false)
    {
        d->mAdminButtonMenu->hide();
        d->mAdminButtonMenu->stop();
    }

    if(d->mAdminETCView.isNull() == false)
    {
        d->mAdminETCView->hide();
        d->mAdminETCView->stop();
    }

    if(d->mAdminSequenceView.isNull() == false)
    {
        d->mAdminSequenceView->hide();
        d->mAdminSequenceView->stop();
    }

    if(d->mAdminSemiAutoView.isNull() == false)
    {
        d->mAdminSemiAutoView->hide();
        d->mAdminSemiAutoView->stop();
    }
}

QVector<GlobalDataStruct::AnalysisReportInfomation> MainView::readLastestDataBaseData()
{
    DataBaseCtrl db(DataBaseCtrl::DATABASE_NAME_LIST::BasicDB);

    QVector<GlobalDataStruct::AnalysisReportInfomation> infomation;
    QString patientTableName = "PATIENT_TABLE";

    QString patientColumnUniqueID = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), patientTableName);
    QString patientColumnPatientID = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ID), patientTableName);
    QString patientColumnName = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_NAME), patientTableName);
    QString patientColumnPanelName = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PANEL_NAME), patientTableName);
    QString patientColumnContentsName = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_CONTENTS_NAME), patientTableName);
    QString patientColumnStripNumber = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_STRIP_NUMBER), patientTableName);
    QString patientColumnLotNumber = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_LOT_NUMBER), patientTableName);
    QString patientColumnApplicant = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_APPLICANT), patientTableName);
    QString patientColumnAnalyst = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ANALYST), patientTableName);
    QString patientColumnReviewer = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_REVIEWER), patientTableName);
    QString patientColumnDate = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);
    QString patientColumnAge = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_AGE), patientTableName);
    QString patientColumnSex = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_SEX), patientTableName);
    QString patientColumnPhone = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PHONE), patientTableName);
    QString patientColumnEmail = db.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_EMAIL), patientTableName);

    QString whereSubQuery(QString("SELECT MAX(%1) FROM %2").arg(patientColumnDate).arg(patientTableName));
    QString lastDateQuery;
    lastDateQuery.append(QString("SELECT * "));
    lastDateQuery.append(QString("FROM %1 ").arg(patientTableName));
    lastDateQuery.append(QString("WHERE %1 = (%2)").arg(patientColumnDate).arg(whereSubQuery));

    auto patientQueryData =  db.select(lastDateQuery);

    for(auto itr : patientQueryData)
    {
        GlobalDataStruct::AnalysisReportInfomation element;

        element.uniqueID = itr[patientColumnUniqueID].toString();
        element.lotNumber = itr[patientColumnLotNumber].toString();
        element.dateTime = itr[patientColumnDate].toDateTime();
        element.element.patientID = itr[patientColumnPatientID].toString();
        element.element.name = itr[patientColumnName].toString();
        element.element.panelName = itr[patientColumnPanelName].toString();
        element.element.contentsName = itr[patientColumnContentsName].toString();

        element.element.stripNumber = itr[patientColumnStripNumber].toInt();
        element.element.applicant = itr[patientColumnApplicant].toString();
        element.element.analyst = itr[patientColumnAnalyst].toString();
        element.element.reviewer = itr[patientColumnReviewer].toString();
        element.element.age = itr[patientColumnAge].toString();
        element.element.sex = itr[patientColumnSex].toString();
        element.element.phone = itr[patientColumnPhone].toString();
        element.element.eMail = itr[patientColumnEmail].toString();

        infomation.push_back(element);
    }

    return infomation;
}

void MainView::moveToRunningStatus(const QVector<GlobalDataStruct::AnalysisReportInfomation> &dbData)
{
    d->mUserWorkPlaceView->toRunningStatus(dbData);
}

void MainView::onResetGUI()
{
    this->onClickMainBtn();
    this->onEnableMenuWidget();
}

void MainView::onClickMainBtn()
{
    if(d->mIsResetStatus == true)
    {
        auto dlg = new MessageShowDlg(tr("System Reset In Progress Please Wait"), this);
        dlg->raise();
        dlg->open();
        return;
    }

    this->hideAllViews();

    if(d->mUserButtonMenu.isNull() == false)
    {
        d->mUserButtonMenu->show();
        d->mUserButtonMenu->start();
    }

    if(d->mAdminButtonMenu.isNull() == false)
    {
        d->mAdminButtonMenu->show();
        d->mAdminButtonMenu->start();
    }
}

void MainView::onClickCameraBtn()
{
    this->hideAllViews();

    d->mAdminCameraView.reset(new CameraView(ui->widgetMainView));
    d->mAdminCameraView->start();
    ui->gridLayoutMainView->addWidget(d->mAdminCameraView.data(), 1,0,1,1);
}

void MainView::onClickETCBtn()
{
    this->hideAllViews();

    if(d->mAdminETCView.data() != nullptr)
    {
        d->mAdminETCView->show();
        d->mAdminETCView->start();
    }
}

void MainView::onClickSequenceBtn()
{
    this->hideAllViews();

    if(d->mAdminSequenceView.data() != nullptr)
    {
        d->mAdminSequenceView->show();
        d->mAdminSequenceView->start();
    }
}

void MainView::onClickSemiautoBtn()
{
    this->hideAllViews();

    if(d->mAdminSemiAutoView.data() != nullptr)
    {
        d->mAdminSemiAutoView->show();
        d->mAdminSemiAutoView->start();
    }
}

void MainView::onClickWorkPlaceBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Work Place---"));
    this->hideAllViews();

    if(d->mUserWorkPlaceView.data() != nullptr)
    {
        d->mUserWorkPlaceView->show();
        d->mUserWorkPlaceView->start();
    }
}

void MainView::onClickResultViewBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Result View---"));
    this->hideAllViews();

    if(d->mUserResultView.data() != nullptr)
    {
        d->mUserResultView->show();
        d->mUserResultView->start();
    }
}

void MainView::onClickResetBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Reset---"));

    QString protocolStr = WriteProtocolControlWrite::writeDeviceRESET();

    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
    this->onClickMainBtn();
}

void MainView::onClickAutoCleanBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Auto Clean---"));
    auto topLevelWidget = MainWindow::getInstance();
    topLevelWidget->startAutoCleanProcess();
}

void MainView::onClickLanguageSelect()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Language Select---"));
    LanguageSelectDlg dlg(this);
    dlg.exec();
}

void MainView::onClickIPSetting()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click IP Setting---"));
    InterfaceServerIPSettingDlg dlg(this);
    dlg.exec();
}

void MainView::onClickDataExchange()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Data Exchange---"));
    BatchUpAndDownLoadTunningDataDlg dlg(this);
    dlg.exec();
}

void MainView::onClickUserModeConfigureBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click User Mode Configure---"));

    QString hwVersion = QString("%1.%2").arg(d->mHWVersion.major).arg(d->mHWVersion.minor);
    QString fwVersion = QString("%1.%2").arg(d->mFWVersion.major).arg(d->mFWVersion.minor);
    QString swVersion = QString("%1.%2").arg(d->mSWVersion.major).arg(d->mSWVersion.minor);

    UserConfigDlg::UserConfigData data;
    data.hwVersion = hwVersion;
    data.fwVersion = fwVersion;
    data.swVersion = swVersion;

    UserConfigDlg dlg(data);
    dlg.exec();
}

void MainView::onClickAdminModeConfigureBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Admin Mode Configure---"));

    QString hwVersion = QString("%1.%2.%3.%4").arg(d->mHWVersion.major).arg(d->mHWVersion.minor).arg(d->mHWVersion.modified).arg(d->mHWVersion.bugfix);
    QString fwVersion = QString("%1.%2.%3.%4").arg(d->mFWVersion.major).arg(d->mFWVersion.minor).arg(d->mFWVersion.modified).arg(d->mFWVersion.bugfix);
    QString swVersion = QString("%1.%2.%3.%4").arg(d->mSWVersion.major).arg(d->mSWVersion.minor).arg(d->mSWVersion.modified).arg(d->mSWVersion.bugfix);

    UserConfigDlg::UserConfigData data;
    data.hwVersion = hwVersion;
    data.fwVersion = fwVersion;
    data.swVersion = swVersion;

    UserConfigDlg dlg(data);
    dlg.exec();
}

void MainView::onClickContentsChangeBtn()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Contents Change---"));
    ContetnsSelectDlg dlg;
    dlg.exec();
}

void MainView::onClickStripScan()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Strip Scan---"));
    this->onClickWorkPlaceBtn();

    if(d->mUserWorkPlaceView.isNull() == false)
        d->mUserWorkPlaceView->setForceStripScan();
}

void MainView::onClickAspration()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Aspration---"));

    AspControlDlg dlg(this);

    if(dlg.exec() == QDialog::Rejected)
        return;

    auto stripCount = dlg.getStripCount();

    QTimer::singleShot(25, this, [this] { d->mWaitingAspDlg.startWaiting();});

    QTimer::singleShot((stripCount*1150) + (14000), this, [this]
                       {
                           d->mWaitingAspDlg.stopWaiting();
    });
}

void MainView::onClickHiddenBand()
{
    CLogWriter::printLog(QString("[MainView::] : ---Click Hidden Band---"));
    ListToHiddenBandDlg dlg(this);
    dlg.exec();
}

void MainView::onResult(QVector<GlobalDataStruct::AnalysisReportInfomation> resultInfomation)
{
    emit onResultReset();
    auto config = ConfigInformation::getInstance();
    config->setLastAnalysisData(resultInfomation);

    d->mWorkPlaceMemoryClearConnection = connect(d->mUserWorkPlaceView.data(), &WorkPlaceView::onMemoryCleared, this, [this]
                                          {
                                              this->showResultData();
                                              this->disconnectMemoryConnection();
                                          });

    this->onClickResultViewBtn();
}

void MainView::onEnableMenuWidget()
{
    ui->btnMain->setEnabled(true);
    ui->btnExit->setEnabled(true);
}

void MainView::onDisableMenuWidget()
{
    ui->btnMain->setDisabled(true);
    ui->btnExit->setDisabled(true);
}

void MainView::onLoadDeviceVersion()
{
    QString hwProtocolStr = WriteProtocolStatusRead::writeHardwareVersion();
    QByteArray hwSendData = hwProtocolStr.toUtf8();
    this->onQueuedSend(hwSendData);

    QString fwProtocolStr = WriteProtocolStatusRead::writeFirmwareVersion();
    QByteArray fwSendData = fwProtocolStr.toUtf8();
    this->onQueuedSend(fwSendData);
}

void MainView::onLoadQRTriplePosition()
{
    auto positionProtocol = WriteProtocolBarcodePage::writeBarcodeQRTrayPosRead().toUtf8();
    this->onQueuedSend(positionProtocol);
}

void MainView::onRecvStatusRead(const QByteArray& recvData)
{
    switch(PacketReader::commandReader(recvData))
    {

    case CMD_STATUS_READ::CMD_STATUS_READ_FIRMWARE_VERSION:
    {
        auto value = PacketReader::dataReader(recvData);

        FW_VERSION version;
        version.major = value.mid(6,2).toInt(nullptr, 16);
        version.minor = value.mid(4,2).toInt(nullptr, 16);
        version.modified = value.mid(2,2).toInt(nullptr, 16);
        version.bugfix = value.mid(0,2).toInt(nullptr, 16);

        d->mFWVersion = version;

        CLogWriter::printLog(QString("[MainView::] : CMD_STATUS_READ_FIRMWARE_VERSION"));
    }   break;

    case CMD_STATUS_READ::CMD_STATUS_READ_HARDWARE_VERSION:
    {      
        HW_VERSION version;
        version.major = "1";
        version.minor = "0";
        version.modified = "0";
        version.bugfix = "0";

        d->mHWVersion = version;

        CLogWriter::printLog(QString("[MainView::] : CMD_STATUS_READ_HARDWARE_VERSION"));
    }   break;

    }
}

void MainView::onRecvQRDataSetting(const QByteArray& recvData)
{
    switch(PacketReader::commandReader(recvData))
    {
    case CMD_QR_SETTING_MODE::CMD_SETTING_MODE_QR_TRAY_POS_READ:
    {
        auto position = PacketReader::dataReader(recvData).toUInt(nullptr, 16);
        ConfigInformation::getInstance()->setQRTriplePosition(position);

        CLogWriter::printLog(QString("[MainView::] : CMD_SETTING_MODE_QR_TRAY_POS_READ"));
        if(!mLoading)
        {
            mLoading = true;
            CLogWriter::printLog(QString("[MainView::] : ---Program Boot Completed---"));
        }
    }break;
    }
}

void MainView::onRecvData(QByteArray recvData)
{
    switch(PacketReader::modeReader(recvData))
    {
    case MD_MODE::MD_MODE_STATUS_READ:
        this->onRecvStatusRead(recvData);
        break;

    case MD_MODE::MD_MODE_QR_SETTING_MODE:
        this->onRecvQRDataSetting(recvData);
        break;
    }
}

void MainView::checkReSequenceProcess()
{
    if(d->mIsSequenceing == true)
    {
        this->onClickWorkPlaceBtn();
        auto lastData = this->readLastestDataBaseData();
        ErrorResonIO::loadErrorReason(lastData);
        this->moveToRunningStatus(lastData);
        CLogWriter::printLog(QString("[MainView::] : Enabled Resequence Process IsSequenceingProcess : %1").arg(d->mIsSequenceing));
    }
}

void MainView::setResetStatus(bool status)
{
    d->mIsResetStatus = status;
}

void MainView::offLineGUIProcessing()
{
    auto config = ConfigInformation::getInstance();

    if(config->isOffLine() == false)
        return;

    ui->btnMain->setDisabled(true);

    if(d->mUserButtonMenu.isNull() == false)
        d->mUserButtonMenu->setOfflinMode();
}

void MainView::showResultData()
{
    auto config = ConfigInformation::getInstance();
    auto analystData = config->getLastAnalysisData();

    if(d->mUserResultView.isNull() == false)
        d->mUserResultView->onResultShow(analystData);
}

void MainView::disconnectMemoryConnection()
{
    disconnect(d->mWorkPlaceMemoryClearConnection);
    d->mWorkPlaceMemoryClearConnection = QMetaObject::Connection();
}

bool MainView::isRunningMainViewFunction()
{
   auto isActiveAsp = d->mWaitingAspDlg.isActivatedTimer();

   if(isActiveAsp)
       return true;
   else
       return false;
}
