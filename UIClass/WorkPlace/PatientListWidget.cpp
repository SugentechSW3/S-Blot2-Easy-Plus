#include <QFileDialog>
#include <QDateTime>
#include <QKeyEvent>
#include <QMenu>
#include <QUuid>

#include "Common/CUtil.h"
#include "Common/CLogWriter.h"
#include "Common/Io/CSettingIo.h"
#include "Common/Network/HL7InterFaceAdapter.h"
#include "Common/Network/HL7InterFaceReader.h"

#include "DataBase/DataBaseCtrl.h"

#include "UIClass/CustomWidget/CheckBoxTableWidgetItem.h"
#include "UIClass/CustomWidget/PlaceHolderTableWidgetItem.h"
#include "UIClass/CustomWidget/ComboBoxTableWidgetItem.h"
#include "UIClass/CustomWidget/SpinTableWidgetItem.h"
#include "UIClass/DialogWidget/ProgressBarDlg.h"
#include "UIClass/DialogWidget/UserSequenceDialog.h"
#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "UIClass/DialogWidget/StatusDialog.h"

#include "Network/PacketReader/PacketReader.h"

#include "Analysis/ConfigInformation.h"

#include "mainwindow.h"

#include "PatientListWidget.h"
#include "ui_PatientListWidget.h"

USING_SUGENTECH

struct PatientListWidget::privateStruct
{
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> mTableItem;
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> mClipBoard;
    QScopedPointer<HL7InterFaceAdapter> mLISServerAdapter;
    QScopedPointer<StatusDialog> mLISWaitMessageDlg;

    ProgressBarDlg mProgressDlg;

    SerialCtrl* mNetworkInstance = nullptr;

    int mLastReadBarcordIdx = 0;
    bool mIsBreakItemChanged = false;
    bool mIsOverFlowPanel = false;
    bool mIsSelectedStripScan = false;
};

PatientListWidget::PatientListWidget(QWidget *parent) :
                                                        CSerialNetworkWidget(parent),
                                                        ui(new Ui_PatientListWidget),
                                                        d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    this->initStyleSheet();
    this->reAnalyzeLastFailureStripProcess();
}

PatientListWidget::~PatientListWidget()
{
}

void PatientListWidget::initVariables()
{
    this->initEditItem();
    this->initTableItem();
    this->initValidator();
    this->initVerticalHeader();

    d->mNetworkInstance = this->getSerialInstance();

    auto config = ConfigInformation::getInstance();
    auto panelList = config->getStatus().panelList;
    panelList.push_front("PANEL");

    config->setLastSelectedSequence(ConfigInformation::STEP_LIST_WETTING);
    ui->comboBatchControl->addItemForPanelColorText(panelList);
}

void PatientListWidget::initSignalSlots()
{
    connect(ui->btnSave, &CustomImageButtonWidget::clicked, this, &PatientListWidget::onClickedSave);
    connect(ui->btnLoad, &CustomImageButtonWidget::clicked, this, &PatientListWidget::onClickedLoad);
    connect(ui->btnReset, &CustomImageButtonWidget::clicked, this, &PatientListWidget::onClickedReset);
    connect(ui->btnRemove, &CustomImageButtonWidget::clicked, this, &PatientListWidget::onClickRemoveRows);

    connect(ui->btnInterlockLIS, &QPushButton::clicked, this, &PatientListWidget::onClickedInterLockingLIS);
    connect(ui->btnAutoID, &QPushButton::clicked, this, &PatientListWidget::onClickedAutoID);

    connect(ui->radioFullSequence, &QRadioButton::clicked, this, &PatientListWidget::onClickedSelectSequence);
    connect(ui->radioUserSeuqnece,&QRadioButton::clicked, this, &PatientListWidget::onClickedSelectSequence);
    connect(ui->radioStripScan, &QRadioButton::clicked, this, &PatientListWidget::onClickedSelectSequence);

    connect(ui->tablePatientInfomationWidget, &QTableWidget::itemSelectionChanged, this, &PatientListWidget::onItemUpdate);
    connect(ui->tablePatientInfomationWidget, &QTableWidget::cellChanged, this, &PatientListWidget::onItemCellChanged);
    connect(ui->tablePatientInfomationWidget, &QTableWidget::customContextMenuRequested, this, &PatientListWidget::onTableWidgetPopupMenu);
    connect(ui->tablePatientInfomationWidget, &TableExcelStyleWidget::onKeyPressed, this, &PatientListWidget::onKeyPressedPatientTableWidgetItem);

    connect(ui->editAge, &QLineEdit::textChanged, this, &PatientListWidget::onEditAgeChanged);
    connect(ui->editTEL, &QLineEdit::textChanged, this, &PatientListWidget::onEditPhoneChanged);
    connect(ui->editEMail, &QLineEdit::textChanged, this, &PatientListWidget::onEditEmailChanged);

    connect(ui->radioButtonMale, &QRadioButton::clicked, this, &PatientListWidget::onEditSexChanged);
    connect(ui->radioButtonFemale, &QRadioButton::clicked, this, &PatientListWidget::onEditSexChanged);

    connect(ui->checkApplicantFixed, &QCheckBox::clicked, this, &PatientListWidget::onCheckedFixedApplicant);
    connect(ui->checkAnalystFixed, &QCheckBox::clicked, this, &PatientListWidget::onCheckedFixedAnalyst);
    connect(ui->checkReviewerFixed, &QCheckBox::clicked, this, &PatientListWidget::onCheckedFixedReviewer);

    connect(ui->comboBatchControl, QOverload<const QString&>::of(&QComboBox::activated), this, &PatientListWidget::onBatchChangeForPanel);
}

void PatientListWidget::startWidget()
{
    auto topLevelWidget = MainWindow::getInstance();
    d->mProgressDlg.setParent(topLevelWidget);

    d->mLISWaitMessageDlg.reset(new StatusDialog(topLevelWidget));
    d->mLISWaitMessageDlg->setStatusMessage(tr("Connecting to LIS server. Please wait a moment."));
    d->mLISWaitMessageDlg->stopWaiting();
}

void PatientListWidget::stopWidget()
{
}

void PatientListWidget::initStyleSheet()
{
    ui->btnSave->setStyleSheetImage(":/Image/Image/Information/add_normal.png", ":/Image/Image/Information/add_over.png",
                                    ":/Image/Image/Information/add_focus.png", ":/Image/Image/Information/add_disable.png");

    ui->btnLoad->setStyleSheetImage(":/Image/Image/Information/load_normal.png", ":/Image/Image/Information/load_over.png",
                                    ":/Image/Image/Information/load_focus.png", ":/Image/Image/Information/load_disable.png");

    ui->btnReset->setStyleSheetImage(":/Image/Image/Information/barcodebt_normal.png", ":/Image/Image/Information/barcodebt_over.png",
                                     ":/Image/Image/Information/barcodebt_focus.png", ":/Image/Image/Information/barcodebt_disable.png");

    ui->btnRemove->setStyleSheetImage(":/Image/Image/Information/barcodebt_normal.png", ":/Image/Image/Information/barcodebt_over.png",
                                      ":/Image/Image/Information/barcodebt_focus.png", ":/Image/Image/Information/barcodebt_disable.png");

    ui->btnInterlockLIS->setStyleSheetImage(":/Image/Image/Information/barcodebt_normal.png", ":/Image/Image/Information/barcodebt_over.png",
                                            ":/Image/Image/Information/barcodebt_focus.png", ":/Image/Image/Information/barcodebt_disable.png");

    ui->btnAutoID->setStyleSheetImage(":/Image/Image/Information/barcodebt_normal.png", ":/Image/Image/Information/barcodebt_over.png",
                                     ":/Image/Image/Information/barcodebt_focus.png", ":/Image/Image/Information/barcodebt_disable.png");

    //ui->btnRemove->setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG_RIGHT);

}

void PatientListWidget::initEditItem()
{
    auto config = ConfigInformation::getInstance();
    auto fixedUserData = config->getFixedUser();

    auto applicant = fixedUserData.applicant;
    auto analyst = fixedUserData.analyst;
    auto reviewer = fixedUserData.reviewer;

    if(applicant.isEmpty() == false)
    {
        ui->checkApplicantFixed->setChecked(true);
        ui->editApplicant->setText(applicant);
        ui->editApplicant->setDisabled(true);
    }

    if(analyst.isEmpty() == false)
    {
        ui->checkAnalystFixed->setChecked(true);
        ui->editAnalyst->setText(analyst);
        ui->editAnalyst->setDisabled(true);
    }

    if(reviewer.isEmpty() == false)
    {
        ui->checkReviewerFixed->setChecked(true);
        ui->editReviewer->setText(reviewer);
        ui->editReviewer->setDisabled(true);
    }
}

void PatientListWidget::initTableItem()
{
    d->mIsBreakItemChanged = true;
    ui->tablePatientInfomationWidget->clearContents();

    auto stripCount = ConfigInformation::getInstance()->getStripCount();

    ui->tablePatientInfomationWidget->setRowCount(stripCount);
    ui->dateTime->setDateTime(QDateTime::currentDateTime());
    d->mTableItem.clear();
    d->mTableItem.resize(stripCount);

    this->setPatientTableHeaderSize();

    auto config = ConfigInformation::getInstance();
    auto panelList = config->getStatus().panelList;
    panelList.push_front("PANEL");

    for(int i = 0; i< stripCount; i ++)
    {
        auto id = new QTableWidgetItem;
        auto patientName = new QTableWidgetItem;
        auto panel = new ComboBoxTableWidgetItem(i, PATIENT_TABLE_COLUMN_PANEL, ui->tablePatientInfomationWidget);

        panel->setWheelEventDisable(true);
        panel->addItems(panelList);

        connect(panel, &ComboBoxTableWidgetItem::onActivatedItem, this, &PatientListWidget::onComboBoxItemActivated);

        ui->tablePatientInfomationWidget->setItem(i, PATIENT_TABLE_COLUMN_ID, id);
        ui->tablePatientInfomationWidget->setItem(i, PATIENT_TABLE_COLUMN_PATIENT_NAME, patientName);
        ui->tablePatientInfomationWidget->setCellWidget(i, PATIENT_TABLE_COLUMN_PANEL, panel);

        d->mTableItem[i].panelName = panel->getCurrentComboBoxText();
        d->mTableItem[i].sex = "M";
    }

    ui->comboBatchControl->setCurrentIndex(0);

    d->mIsBreakItemChanged = false;

    this->onSyncItem();
}

void PatientListWidget::initValidator()
{
    ui->editAge->setValidator(new QRegExpValidator(QRegExp("[0-9]\\d{0,2}"),this));
    ui->editTEL->setValidator(new QRegExpValidator(QRegExp("[0-9]\\d{0,10}"),this));
}

void PatientListWidget::initVerticalHeader()
{
    auto verticalHeader = ui->tablePatientInfomationWidget->verticalHeader();

    for(int i = 0 ; i < verticalHeader->count(); i++)
    {
        auto verticalHeaderItem = new QTableWidgetItem(QString("     "));
        ui->tablePatientInfomationWidget->setVerticalHeaderItem(i,verticalHeaderItem);
    }
}

bool PatientListWidget::isSelectedStripScan()
{
    return d->mIsSelectedStripScan;
}

void PatientListWidget::savePatientListData()
{
    QString filePath = QString("%1/Documents/Blot-2/PatientList").arg(QDir::homePath());
    CSettingIo::createDir(filePath);

    QString fileName = QString("%1/PatientListData-%2")
                           .arg(filePath)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));

    CSettingIo io(false, fileName);

    for(int i = 0; i < d->mTableItem.count(); i++)
    {
        auto tableNumber = QString("Number_%1").arg(QString::number(i+1));
        io.setConfigureData(tableNumber, "PATIENT_ID", d->mTableItem[i].patientID);
        io.setConfigureData(tableNumber, "PANEL", d->mTableItem[i].panelName);

        io.setConfigureData(tableNumber, "NAME", d->mTableItem[i].name);
        io.setConfigureData(tableNumber, "SEX", d->mTableItem[i].sex);
        io.setConfigureData(tableNumber, "AGE", d->mTableItem[i].age);
        io.setConfigureData(tableNumber, "PHONE", d->mTableItem[i].phone);
        io.setConfigureData(tableNumber, "EMAIL", d->mTableItem[i].eMail);
    }
}

QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> PatientListWidget::getData()
{
    auto copiedData = this->createCopiedData();
    return copiedData;
}

QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> PatientListWidget::createCopiedData()
{
    auto copiedData = d->mTableItem;

    for(int i = 0; i < copiedData.count(); i++)
    {
        copiedData[i].stripNumber = i+1;
        auto isEmptyID = copiedData[i].patientID.isEmpty();
        auto isEmptyName = copiedData[i].name.isEmpty();

        copiedData[i].applicant = ui->editApplicant->text();
        copiedData[i].analyst = ui->editAnalyst->text();
        copiedData[i].reviewer = ui->editReviewer->text();

        if(isEmptyID && (isEmptyName == false))
            copiedData[i].patientID = QUuid::createUuid().toString();
    }

    return copiedData;
}

QMap<int, QString> PatientListWidget::createPatientIDList()
{
    QMap<int, QString> idList;
    int index = 1;

    for(auto i = 0 ; i < d->mTableItem.count(); i++)
    {
        if(this->isEmptyID(i))
            continue;

        auto itr = d->mTableItem[i];

        idList[index] = itr.patientID;
        index++;
    }

    return idList;
}

bool PatientListWidget::isEmptyID(int currentRow)
{
    if(d->mTableItem.count() <= currentRow)
        return false;

    return d->mTableItem[currentRow].patientID.isEmpty();
}

bool PatientListWidget::isEmptyIDAndUnEmptyName(int currentRow)
{
    if(d->mTableItem.count() <= currentRow)
        return false;

    auto isEmptyID = d->mTableItem[currentRow].patientID.isEmpty();
    auto isEmptyName = d->mTableItem[currentRow].name.isEmpty();

    if(isEmptyID && (isEmptyName == false))
        return true;

    return false;
}

bool PatientListWidget::isEmptyIDAndEmptyName(int currentRow)
{
    if(d->mTableItem.count() <= currentRow)
        return true;

    auto isEmptyID = d->mTableItem[currentRow].patientID.isEmpty();
    auto isEmptyName = d->mTableItem[currentRow].name.isEmpty();

    if(isEmptyID && isEmptyName)
        return true;

    return false;
}

bool PatientListWidget::isAllEmptyData()
{
    for(auto i = 0 ; i < d->mTableItem.count(); i++)
    {
        if(this->isEmptyIDAndEmptyName(i) == false)
            return false;
    }

    return true;
}

bool PatientListWidget::isAllEmptyID()
{
    for(auto i = 0 ; i < d->mTableItem.count(); i++)
    {
        if(this->isEmptyID(i) == false)
            return false;
    }

    return true;
}

void PatientListWidget::createUUID(int currentRow)
{
    if(d->mTableItem.count() <= currentRow)
        return;

    QString autoID = QString("%1-%2").arg(currentRow+1).arg(QUuid::createUuid().toString());
    d->mTableItem[currentRow].patientID = autoID;
}

void PatientListWidget::showLISWaitMessage()
{
    d->mLISWaitMessageDlg->startWaiting();
}

void PatientListWidget::hideLISWaitMessage()
{
    d->mLISWaitMessageDlg->stopWaiting();
}

int PatientListWidget::findRowHasPatientID(const QString& patientID)
{
    int row = -1;

    for(auto i = 0 ; i < d->mTableItem.count(); i++)
    {
        auto itr = d->mTableItem[i];

        if(itr.patientID == patientID)
        {
            row = i;
            break;
        }
    }

    return row;
}

void PatientListWidget::updateLISData(const QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA> &data)
{
    for(auto& itr : data)
    {
        auto row = this->findRowHasPatientID(itr.patientID);

        if(row < 0)
            continue;

        if(itr.sex.isEmpty() == false)
            d->mTableItem[row].sex = itr.sex;

        d->mTableItem[row].age = itr.birthDate;
        d->mTableItem[row].name = itr.name;
        d->mTableItem[row].panelName = itr.panelName;
    }
}

void PatientListWidget::setData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data)
{
    this->onClickedReset();
    this->copyToTableItem(data);
    this->copyInspectorData(data);
    this->onSyncItem();
    this->onItemUpdate();
}


void PatientListWidget::moveNextTableWidgetItem(int row, int column)
{
    auto totalRowCount = ui->tablePatientInfomationWidget->rowCount();
    auto nextRowIndex = row + 1;

    if(nextRowIndex < totalRowCount)
        ui->tablePatientInfomationWidget->setCurrentCell(nextRowIndex, column);
}

void PatientListWidget::copyToTableItem(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data)
{
    if(d->mTableItem.count() < data.count())
        return;

    for(auto i = 0; i < data.count(); i++)
        d->mTableItem[i] = data[i];
}

void PatientListWidget::reAnalyzeLastFailureStripProcess()
{
    if(this->checkLastFailureStripData() == false)
    {
        auto config = ConfigInformation::getInstance();

        if(config->getLastAnalysisData().isEmpty() == false)
        {
            auto nullData = QVector<GlobalDataStruct::AnalysisReportInfomation>();
            config->setLastAnalysisData(nullData);
        }
        return;
    }

    this->confirmFailureStripData();
    this->drawingColorForAnalysisFailItems();
    this->disableUIButtonsForRescanning();
    this->setForceStripScanRadioButton();

    this->onSyncItem();
    this->onItemUpdate();
}

bool PatientListWidget::checkLastFailureStripData()
{
    auto config = ConfigInformation::getInstance();
    auto lastAnalysisData = config->getLastAnalysisData();

    for(auto& itr : lastAnalysisData)
    {
        if(itr.result.isSuccess == false)
        {
            auto topLevelWidget = MainWindow::getInstance();

            QuestionDlg dlg(topLevelWidget);
            dlg.setQuestionMessage(tr("Some of the last items you performed analysis failed. Would you like to confirm?"));

            if(dlg.exec() == QDialog::Accepted)
                return true;
            else
                return false;
        }
    }

    return false;
}

void PatientListWidget::confirmFailureStripData()
{
    auto config = ConfigInformation::getInstance();
    auto lastAnalysisData = config->getLastAnalysisData();

    for(int i = 0 ; i < lastAnalysisData.count(); i++)
    {
        if(d->mTableItem.count() <= i)
            break;

        auto currentElement= lastAnalysisData[i].element;
        auto currentIsSuccess = lastAnalysisData[i].result.isSuccess;

        currentElement.isSuccess = currentIsSuccess;
        d->mTableItem[i] = currentElement;
    }
}

void PatientListWidget::drawingColorForAnalysisFailItems()
{
    auto backupData = d->mTableItem;

    for(int row = 0 ; row < d->mTableItem.count() ; row++)
    {
        auto itr = d->mTableItem[row];

        if(itr.patientID.isEmpty() == true)
            continue;

        if(itr.isSuccess == false)
        {
            auto item =  ui->tablePatientInfomationWidget->item(row, PATIENT_TABLE_COLUMN_ID);

            if(item != nullptr)
                item->setBackgroundColor(Qt::yellow);
        }
    }

    d->mTableItem = backupData;
}

void PatientListWidget::disableUIButtonsForRescanning()
{
    this->setDisabled(true);
}

void PatientListWidget::setForceStripScanRadioButton()
{
    ui->radioStripScan->setChecked(true);
    this->onClickedSelectSequence(true);
}

bool PatientListWidget::changePanelDataForSingle(int row, const QString& panel)
{
    if(d->mTableItem.count() <= row)
        return false;

    this->changePanelForSingleControlling(row, panel);

    return true;
}

bool PatientListWidget::changePanelDataForBatch(const QString& panel)
{
    this->changePanelForBatchControlling(panel);

    return true;
}

void PatientListWidget::changePanelForSingleControlling(int row, const QString& panel)
{
    auto currentItem = qobject_cast<ComboBoxTableWidgetItem*>(ui->tablePatientInfomationWidget->cellWidget(row, PATIENT_TABLE_COLUMN_PANEL));

    if(currentItem == nullptr)
        return;

    d->mIsBreakItemChanged = true;

    currentItem->setCurrentComboBoxIndex(panel);

    if(row < d->mTableItem.count())
        d->mTableItem[row].panelName = panel;

    d->mIsBreakItemChanged = false;
}

void PatientListWidget::changePanelForBatchControlling(const QString& panel)
{
    auto items =  ui->tablePatientInfomationWidget->selectionModel()->selectedIndexes();

    if(items.isEmpty())
        return;

    d->mIsBreakItemChanged = true;

    for(auto& itemsItr : items)
    {
        auto selectedRow =  itemsItr.row();
        auto selectedColumn = itemsItr.column();
        auto currentItem = qobject_cast<ComboBoxTableWidgetItem*>(ui->tablePatientInfomationWidget->cellWidget(selectedRow, PATIENT_TABLE_COLUMN_PANEL));

        if(selectedColumn != PATIENT_TABLE_COLUMN_PANEL)
            continue;

        if(currentItem == nullptr)
            continue;

        currentItem->setCurrentComboBoxIndex(panel);

        if(selectedRow < d->mTableItem.count())
            d->mTableItem[selectedRow].panelName = panel;
    }

    d->mIsBreakItemChanged = false;
}

int PatientListWidget::findNextEmptyRow()
{
    int row = 0;

    for(auto i = 0; i < d->mTableItem.count(); i++)
    {
        auto isEmptyPatientID = d->mTableItem[i].patientID.isEmpty();
        auto isEmptyName = d->mTableItem[i].name.isEmpty();

        if(isEmptyPatientID && isEmptyName)
        {
            row = i;
            break;
        }
    }

    return row;
}

void PatientListWidget::copyInspectorData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data)
{
    if(data.isEmpty() == true)
        return;

    auto analyst = data.first().analyst;
    auto reviewer = data.first().reviewer;
    auto applicant = data.first().applicant;

    ui->editAnalyst->setText(analyst);
    ui->editReviewer->setText(reviewer);
    ui->editApplicant->setText(applicant);
}

void PatientListWidget::updateLISData(const HL7_ORDER_DATA& data)
{
    for(auto& itr : data.data)
    {
        auto row = this->findRowHasPatientID(itr.patientID);

        if(row < 0)
            continue;

        auto tableItemItr = d->mTableItem[row];

        auto panelName = HL7InterFaceReader::panelInitalToPanelName(itr.panelInitial);
        auto currentAge = tableItemItr.age;
        auto currentSex = tableItemItr.sex;
        auto currentName = tableItemItr.name;
        auto currentPhone = tableItemItr.phone;
        auto currentPanelName = tableItemItr.panelName;

        tableItemItr.age = itr.birthDate;
        tableItemItr.sex = itr.sex;
        tableItemItr.name = itr.name;
        tableItemItr.phone = itr.phoneNumber;
        tableItemItr.panelName = panelName;

        d->mTableItem[row] = tableItemItr;

        if(this->changePanelDataForSingle(row, panelName) == false)
        {
            tableItemItr.age = currentAge;
            tableItemItr.sex = currentSex;
            tableItemItr.name = currentName;
            tableItemItr.phone = currentPhone;
            tableItemItr.panelName = currentPanelName;
            break;
        }
    }
}

void PatientListWidget::getSelectedData()
{
    auto items =  ui->tablePatientInfomationWidget->selectionModel()->selectedRows();
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> selectedData;

    for(auto& itr : items)
    {
        auto row = itr.row();

        if(row < d->mTableItem.count())
            selectedData.push_back(d->mTableItem[row]);
    }

    d->mClipBoard = selectedData;
}

void PatientListWidget::setSelectedData()
{
    auto items =  ui->tablePatientInfomationWidget->selectionModel()->selectedRows();
    auto pasteData = d->mClipBoard;

    for(auto& itr : items)
    {
        auto row = itr.row();

        if(row >= d->mTableItem.count())
            continue;

        if(pasteData.isEmpty())
            break;

        d->mTableItem[row] = pasteData.takeFirst();
    }

    this->onSyncItem();
    this->onItemUpdate();
}

int PatientListWidget::getEmptyRowCount()
{
    auto emptyCount = 0;

    for(auto itr : d->mTableItem)
    {
        auto isEmptyPatientID = itr.patientID.isEmpty();
        auto isEmptyName = itr.name.isEmpty();

        if(isEmptyPatientID && isEmptyName)
            emptyCount++;
    }

    return emptyCount;
}

int PatientListWidget::getSelectedPanelCount()
{
    auto items =  ui->tablePatientInfomationWidget->selectionModel()->selectedIndexes();
    auto panelCount = 0;

    for(auto& itr : items)
    {
        if(itr.column() == PATIENT_TABLE_COLUMN_PANEL)
            panelCount++;
    }

    return panelCount;
}

void PatientListWidget::setPatientTableHeaderSize()
{
    auto itemNameHeader = ui->tablePatientInfomationWidget->horizontalHeader();
    itemNameHeader->resizeSection(PATIENT_TABLE_COLUMN_ID, 250);
    itemNameHeader->resizeSection(PATIENT_TABLE_COLUMN_PATIENT_NAME, 60);
    itemNameHeader->resizeSection(PATIENT_TABLE_COLUMN_PANEL, 130);
}

void PatientListWidget::onClickedSave()
{
    QString filePath = QString("%1/Documents/S-Blot2-Easy-PLUS/PatientList").arg(QDir::homePath());

    CSettingIo::createDir(filePath);

    QString fileName = QString("%1/PatientListData-%2")
                           .arg(filePath)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));

    QString selectPath = QFileDialog::getSaveFileName(this, tr("Save File"), fileName, tr("INI File (*.ini)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(selectPath.isEmpty() == true)
        return;

    CSettingIo io(false, selectPath);

    for(int i = 0; i < d->mTableItem.count(); i++)
    {
        auto tableNumber = QString("Number_%1").arg(QString::number(i+1));
        io.setConfigureData(tableNumber, "PATIENT_ID", d->mTableItem[i].patientID);
        io.setConfigureData(tableNumber, "PANEL", d->mTableItem[i].panelName);

        io.setConfigureData(tableNumber, "NAME", d->mTableItem[i].name);
        io.setConfigureData(tableNumber, "SEX", d->mTableItem[i].sex);
        io.setConfigureData(tableNumber, "AGE", d->mTableItem[i].age);
        io.setConfigureData(tableNumber, "PHONE", d->mTableItem[i].phone);
        io.setConfigureData(tableNumber, "EMAIL", d->mTableItem[i].eMail);
    }
}

void PatientListWidget::onClickedLoad()
{
    QString filePath = QString("%1/Documents/S-Blot2-Easy-PLUS/PatientList").arg(QDir::homePath());
    CSettingIo::createDir(filePath);

    QString selectPath = QFileDialog::getOpenFileName(this, tr("Load File"), filePath, tr("INI File (*.ini)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(selectPath.isEmpty() == true)
        return;

    this->onClickedReset();

    CSettingIo io(false, selectPath);

    for(int i = 0; i < d->mTableItem.count(); i++)
    {
        auto rowIdx = QString("Number_%1").arg(QString::number(i + 1));
        d->mTableItem[i].patientID = io.getConfigureData(rowIdx, "PATIENT_ID").toString();
        d->mTableItem[i].panelName = io.getConfigureData(rowIdx, "PANEL").toString();
        d->mTableItem[i].name = io.getConfigureData(rowIdx, "NAME").toString();
        d->mTableItem[i].sex = io.getConfigureData(rowIdx, "SEX").toString();
        d->mTableItem[i].age = io.getConfigureData(rowIdx, "AGE").toString();
        d->mTableItem[i].phone = io.getConfigureData(rowIdx, "PHONE").toString();
        d->mTableItem[i].eMail = io.getConfigureData(rowIdx, "EMAIL").toString();
    }

    this->onSyncItem();
    this->onItemUpdate();
}

void PatientListWidget::onClickedReset()
{
    this->initTableItem();
    this->onClearEdit();
}

void PatientListWidget::onRemoveRow(int row)
{
    if(d->mTableItem.count() <= row)
        return;

    if(row < 0)
        return;

    auto config = ConfigInformation::getInstance();
    auto panelList = config->getStatus().panelList;
    panelList.push_front("PANEL");
    auto firstPanel = panelList.first();

    d->mIsBreakItemChanged = true;

    auto currentRow = row;
    d->mTableItem[currentRow].age = QString("");
    d->mTableItem[currentRow].sex = QString("M");
    d->mTableItem[currentRow].name = QString("");
    d->mTableItem[currentRow].eMail = QString("");
    d->mTableItem[currentRow].phone = QString("");
    d->mTableItem[currentRow].panelName = firstPanel;
    d->mTableItem[currentRow].patientID = QString("");
    d->mTableItem[currentRow].stripNumber = 0;
    d->mTableItem[currentRow].contentsName = QString("");

    d->mIsBreakItemChanged = false;

    this->onSyncItem();
}

void PatientListWidget::onClickRemoveRows()
{
    auto items =  ui->tablePatientInfomationWidget->selectionModel()->selectedRows();
    auto config = ConfigInformation::getInstance();
    auto panelList = config->getStatus().panelList;
    panelList.push_front("PANEL");
    auto firstPanel = panelList.first();

    d->mIsBreakItemChanged = true;

    for(int i =0; i < items.count(); i++)
    {
        auto currentRow = items.at(i).row();

        d->mTableItem[currentRow].age = QString("");
        d->mTableItem[currentRow].sex = QString("M");
        d->mTableItem[currentRow].name = QString("");
        d->mTableItem[currentRow].eMail = QString("");
        d->mTableItem[currentRow].phone = QString("");
        d->mTableItem[currentRow].panelName = firstPanel;
        d->mTableItem[currentRow].patientID = QString("");
        d->mTableItem[currentRow].stripNumber = 0;
        d->mTableItem[currentRow].contentsName = QString("");
    }

    d->mIsBreakItemChanged = false;

    this->onSyncItem();
}

void PatientListWidget::onClickedInterLockingLIS()
{
    GenericIniDataControls io;
    if(io.readUseXML())
    {
        U2BioXmlLisProtocol lis;
        auto loadData = lis.loadU2BioXmlInData();

        this->updateLISData(loadData);

        this->onSyncItem();
        this->onItemUpdate();
        return;
    }

    if(this->isAllEmptyID())
    {
        auto topLevelWidget = MainWindow::getInstance();
        auto msgDlg = new MessageShowDlg(tr("At least one ID is required for LIS interlocking."), topLevelWidget);
        msgDlg->exec();
        return;
    }

    this->onDisableTopLevelWidget();
    this->showLISWaitMessage();

    d->mLISServerAdapter.reset(new HL7InterFaceAdapter(this));

    QDateTime dateTime = QDateTime::currentDateTime();

    connect(d->mLISServerAdapter.data(), &HL7InterFaceAdapter::onTimeOut, this, [this]
            {
                this->hideLISWaitMessage();
                this->onEnableTopLevelWidget();

                auto topLevelWidget = MainWindow::getInstance();
                auto msgDlg = new MessageShowDlg(tr("Unable to connect to server or no response."), topLevelWidget);
                msgDlg->exec();
            });

    connect(d->mLISServerAdapter.data(), &HL7InterFaceAdapter::onRespFinished, this, [this]
            (const HL7_ORDER_DATA data)
            {
                this->updateLISData(data);
                this->onSyncItem();
                this->onItemUpdate();
                this->hideLISWaitMessage();
                this->onEnableTopLevelWidget();
            });

    auto patientIDList = this->createPatientIDList();
    d->mLISServerAdapter->downloadLISData(dateTime, patientIDList);
}



void PatientListWidget::onClickedAutoID()
{
    d->mIsBreakItemChanged = true;

    auto selectedItems = ui->tablePatientInfomationWidget->selectionModel()->selectedIndexes();

    for(auto& itr : selectedItems)
    {
        if(itr.column() != PATIENT_TABLE_COLUMN_ID)
            continue;

        auto currentRow = itr.row();

        if(d->mTableItem.count() <= currentRow)
            continue;

        this->createUUID(currentRow);
    }

    d->mIsBreakItemChanged = false;

    this->onSyncItem();
}

void PatientListWidget::onSyncItem()
{
    if(d->mIsBreakItemChanged)
        return;

    for(int i =0; i< d->mTableItem.count(); i++)
    {
        auto currentData = d->mTableItem[i];

        auto columnId = ui->tablePatientInfomationWidget->item(i, PATIENT_TABLE_COLUMN_ID);
        auto patientName = ui->tablePatientInfomationWidget->item(i, PATIENT_TABLE_COLUMN_PATIENT_NAME);
        auto comboBox = qobject_cast<ComboBoxTableWidgetItem*> (ui->tablePatientInfomationWidget->cellWidget(i, PATIENT_TABLE_COLUMN_PANEL));

        if(columnId == nullptr)
            Q_ASSERT("columnId TableWidget Item Is Null");

        if(patientName == nullptr)
            Q_ASSERT("patientName TableWidget Item Is Null");

        if(comboBox == nullptr)
            Q_ASSERT("ComboBox TableWidget Item Is Null");

        columnId->setText(currentData.patientID);
        patientName->setText(currentData.name);
        comboBox->setCurrentComboBoxIndex(currentData.panelName);
     }
}

void PatientListWidget::onItemUpdate()
{
    if(d->mIsBreakItemChanged)
        return;

    auto currentRowIdx = ui->tablePatientInfomationWidget->currentIndex().row();

    if(currentRowIdx < 0)
        return;

    if(d->mTableItem.isEmpty())
        return;

    auto tableItem = d->mTableItem[currentRowIdx];

    auto columnId = ui->tablePatientInfomationWidget->item(currentRowIdx, PATIENT_TABLE_COLUMN_ID);
    auto patientName = ui->tablePatientInfomationWidget->item(currentRowIdx, PATIENT_TABLE_COLUMN_PATIENT_NAME);
    auto comboBox = qobject_cast<ComboBoxTableWidgetItem*> (ui->tablePatientInfomationWidget->cellWidget(currentRowIdx, PATIENT_TABLE_COLUMN_PANEL));

    if(columnId == nullptr)
        Q_ASSERT("columnId TableWidget Item Is Null");

    if(patientName == nullptr)
        Q_ASSERT("patientName TableWidget Item Is Null");

    if(comboBox == nullptr)
        Q_ASSERT("ComboBox TableWidget Item Is Null");

    columnId->setText(tableItem.patientID);
    patientName->setText(tableItem.name);
    comboBox->setCurrentComboBoxIndex(tableItem.panelName);

    if(d->mTableItem[currentRowIdx].sex == QString("M"))
        ui->radioButtonMale->setChecked(true);
    else
        ui->radioButtonFemale->setChecked(true);

    ui->editAge->setText(d->mTableItem[currentRowIdx].age);
    ui->editTEL->setText(d->mTableItem[currentRowIdx].phone);
    ui->editEMail->setText(d->mTableItem[currentRowIdx].eMail);
}

QString PatientListWidget::getStartMode()
{
    bool isFullSeq   = ui->radioFullSequence->isChecked();
    bool isUserSeq   = ui->radioUserSeuqnece->isChecked();
    bool isStripScan = ui->radioStripScan->isChecked();

    if(isFullSeq) return "FULL SEQUENCE";
    if(isUserSeq)
    {
        ConfigInformation *config = ConfigInformation::getInstance();
        QString step = config->convertStepEnumToStepName(config->getLastSelectedSequence());
        return QString("USER SEQUENCE : %1").arg(step);
    }
    if(isStripScan) return "STRIP SCAN";
}

void PatientListWidget::onClickedSelectSequence(bool)
{
    ConfigInformation *config = ConfigInformation::getInstance();
    auto isFullSequence = (ui->radioFullSequence->isChecked());
    auto isUserSequence = (ui->radioUserSeuqnece->isChecked());
    d->mIsSelectedStripScan = (ui->radioStripScan->isChecked());

    if(isFullSequence == true)
    {
        config->setLastSelectedSequence(ConfigInformation::STEP_LIST_WETTING);
        return;
    }
    else if(isUserSequence == true)
    {
        auto topLevelWidget = MainWindow::getInstance();
        UserSequenceDialog dlg(config->getLastSelectedSequence(), topLevelWidget);

        if(dlg.exec() == QDialog::Accepted)
        {
            config->setLastSelectedSequence(dlg.getCurrentIdx());
            return;
        }
    }
    else if (d->mIsSelectedStripScan == true)
    {

        config->setLastSelectedSequence(ConfigInformation::STEP_LIST_ANALYSIS);
        return;
    }

    ui->radioFullSequence->setChecked(true);
}

void PatientListWidget::onClearEdit()
{
    ui->dateTime->setDateTime(QDateTime::currentDateTime());
    ui->radioButtonMale->setChecked(true);
    ui->editAge->setText("");
    ui->editTEL->setText("");
    ui->editEMail->setText("");
}

void PatientListWidget::onItemCellChanged(int row, int column)
{
    if(d->mIsBreakItemChanged == true)
        return;

    if(d->mTableItem.count() <= row)
        return;

    if(row < 0)
        return;

    if(column < 0)
        return;

    switch(column)
    {
    case PATIENT_TABLE_COLUMN_ID:
    {
        auto item = ui->tablePatientInfomationWidget->item(row,column);

        if(item != nullptr)
            d->mTableItem[row].patientID = item->text();

    }   break;

    case PATIENT_TABLE_COLUMN_PATIENT_NAME:
    {
        auto item = ui->tablePatientInfomationWidget->item(row,column);

        if(item != nullptr)
            d->mTableItem[row].name = item->text();

    }   break;

    }
}

void PatientListWidget::onKeyPressedPatientTableWidgetItem(int row, int column, int qtKey)
{
    auto isEnter = (qtKey == Qt::Key::Key_Enter);
    auto isReturn = (qtKey == Qt::Key::Key_Return);

    if(isEnter || isReturn)
        this->moveNextTableWidgetItem(row, column);
}

void PatientListWidget::onSpinBoxActivated(int row, int column)
{
    ui->tablePatientInfomationWidget->setCurrentCell(row, column);
}

void PatientListWidget::onSpinMoveHelper(int qtKey)
{
    if(d->mIsBreakItemChanged == true)
        return;

    auto currentRow = ui->tablePatientInfomationWidget->currentRow();
    auto currentColumn = ui->tablePatientInfomationWidget->currentColumn();

    switch(qtKey)
    {
    case Qt::Key_Up:

        if(currentRow -1 >= 0)
            ui->tablePatientInfomationWidget->setCurrentCell(currentRow -1, currentColumn);

        break;

    case Qt::Key_Down:

        if(currentRow + 1 < ui->tablePatientInfomationWidget->rowCount())
            ui->tablePatientInfomationWidget->setCurrentCell(currentRow + 1, currentColumn);

        break;

    case Qt::Key_Left:

        if(currentColumn - 1 >= 0)
            ui->tablePatientInfomationWidget->setCurrentCell(currentRow, currentColumn - 1);

        break;

    case Qt::Key_Right:

        if(currentColumn + 1 < ui->tablePatientInfomationWidget->columnCount())
            ui->tablePatientInfomationWidget->setCurrentCell(currentRow, currentColumn + 1);

        break;

    case Qt::Key_Enter:
        this->moveNextTableWidgetItem(currentRow, currentColumn);
        break;
    }
}

void PatientListWidget::onComboBoxItemActivated(int row)
{
    auto item =  qobject_cast<ComboBoxTableWidgetItem*> (ui->tablePatientInfomationWidget->cellWidget(row, PATIENT_TABLE_COLUMN_PANEL));
    auto selected =  ui->tablePatientInfomationWidget->selectionModel()->selectedIndexes();

    if(item == nullptr)
        return;

    auto panel = item->getCurrentComboBoxText();

//    if(selected.isEmpty() || selected.count() == 1)
//        this->changePanelDataForSingle(row, panel);
//    else
//        this->changePanelDataForBatch(panel);
    this->changePanelDataForSingle(row, panel);

    this->onSyncItem();
    this->onItemUpdate();
}

void PatientListWidget::onEditNameChanged(QString text)
{
    if(d->mIsBreakItemChanged == true)
        return;

    auto currentRow = ui->tablePatientInfomationWidget->currentRow();

    if(currentRow < 0)
    {
        currentRow = 0;
        ui->tablePatientInfomationWidget->setCurrentCell(0,0);
    }

    d->mTableItem[currentRow].name = text;
}

void PatientListWidget::onEditSexChanged()
{
    if(d->mIsBreakItemChanged == true)
        return;

    auto currentRow = ui->tablePatientInfomationWidget->currentRow();

    if(currentRow < 0)
    {
        currentRow = 0;
        ui->tablePatientInfomationWidget->setCurrentCell(0,0);
    }

    if(ui->radioButtonMale->isChecked() == true)
        d->mTableItem[currentRow].sex = "M";
    else
        d->mTableItem[currentRow].sex = "F";
}

void PatientListWidget::onEditAgeChanged(QString text)
{
    if(d->mIsBreakItemChanged == true)
        return;

    auto currentRow = ui->tablePatientInfomationWidget->currentRow();

    if(currentRow < 0)
    {
        currentRow = 0;
        ui->tablePatientInfomationWidget->setCurrentCell(0,0);
    }

    d->mTableItem[currentRow].age = text;
}

void PatientListWidget::onEditPhoneChanged(QString text)
{
    if(d->mIsBreakItemChanged == true)
        return;

    auto currentRow = ui->tablePatientInfomationWidget->currentRow();

    if(currentRow < 0)
    {
        currentRow = 0;
        ui->tablePatientInfomationWidget->setCurrentCell(0,0);
    }

    d->mTableItem[currentRow].phone = text;
}

void PatientListWidget::onEditEmailChanged(QString text)
{
    if(d->mIsBreakItemChanged == true)
        return;

    auto currentRow = ui->tablePatientInfomationWidget->currentRow();

    if(currentRow < 0)
    {
        currentRow = 0;
        ui->tablePatientInfomationWidget->setCurrentCell(0,0);
    }

    d->mTableItem[currentRow].eMail = text;
}

void PatientListWidget::onCopyShortCutData()
{
    this->getSelectedData();
}

void PatientListWidget::onPasteShortCutData()
{
    this->setSelectedData();
}

void PatientListWidget::onCheckedFixedApplicant(bool isChecked)
{
    auto config = ConfigInformation::getInstance();
    auto userData = config->getFixedUser();

    if(isChecked == true)
    {
        userData.applicant = ui->editApplicant->text();
        ui->editApplicant->setDisabled(true);
    }
    else
    {
        userData.applicant = "";
        ui->editApplicant->setEnabled(true);
    }

    config->setFixedUser(userData);
}

void PatientListWidget::onCheckedFixedAnalyst(bool isChecked)
{
    auto config = ConfigInformation::getInstance();
    auto userData = config->getFixedUser();

    if(isChecked == true)
    {
        userData.analyst = ui->editAnalyst->text();
        ui->editAnalyst->setDisabled(true);
    }
    else
    {
        userData.analyst = "";
        ui->editAnalyst->setEnabled(true);
    }

    config->setFixedUser(userData);
}

void PatientListWidget::onCheckedFixedReviewer(bool isChecked)
{
    auto config = ConfigInformation::getInstance();
    auto userData = config->getFixedUser();

    if(isChecked == true)
    {
        userData.reviewer = ui->editReviewer->text();
        ui->editReviewer->setDisabled(true);
    }
    else
    {
        userData.reviewer = "";
        ui->editReviewer->setEnabled(true);
    }

    config->setFixedUser(userData);
}

void PatientListWidget::onBatchChangeForPanel()
{
    auto items =  ui->tablePatientInfomationWidget->selectionModel()->selectedIndexes();
    auto panel = ui->comboBatchControl->currentText();

    if(items.isEmpty())
        return;

    this->changePanelDataForBatch(panel);

    this->onSyncItem();
    this->onItemUpdate();
}

void PatientListWidget::onTableWidgetPopupMenu(const QPoint& point)
{
    QMenu menu;
    menu.addAction(tr("Copy Row Data"), this, &PatientListWidget::onCopyShortCutData);
    menu.addAction(tr("Paste Row Data"),this, &PatientListWidget::onPasteShortCutData);
    menu.addAction(tr("Remove"), this, &PatientListWidget::onClickRemoveRows);

    auto globalPoint = ui->tablePatientInfomationWidget->mapToGlobal(point);
    menu.exec(globalPoint);
}

void PatientListWidget::onDisableTopLevelWidget()
{
    auto topLevelWidget = MainWindow::getInstance();
    topLevelWidget->setDisableTopLevelWidget(true);
}

void PatientListWidget::onEnableTopLevelWidget()
{
    auto topLevelWidget = MainWindow::getInstance();
    topLevelWidget->setDisableTopLevelWidget(false);
}

bool PatientListWidget::event(QEvent *event)
{
    if (event->type() == QEvent::Resize)
    {
        auto resizedWidth = this->width() - 828;
        ui->groupPatient->setMinimumWidth(resizedWidth);
    }

    return CSerialNetworkWidget::event(event);
}
