#include <QAction>
#include <QToolTip>
#include <QItemSelectionModel>
#include <QTranslator>

#include "Analysis/Interfaces/IDataClassifier.h"
#include "Analysis/Bases/DataClassifierBase.h"
#include "Analysis/ConfigInformation.h"
#include "Analysis/ClassHelper.h"

#include "Report/ReportData.h"
#include "Report/LagacyReportData.h"

#include "DataBase/DataBaseCtrl.h"
#include "DataBase/BasicDatabaseCtrl.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

#include "Common/Network/HL7InterFaceAdapter.h"
#include "Common/Network/HL7InterFaceReader.h"
#include "Common/Network/HL7InterFaceWriter.h"
#include "Common/Network/U2BioXmlLisProtocol.h"

#include "Common/Io/ExcelExportClass.h"
#include "Common/Io/INIFileControls/GenericIniDataControls.h"
#include "Common/Io/JsonFIleControls/ReportJsonDataControls.h"

#include "UIClass/DialogWidget/ResultPrintSelectLangDlg.h"
#include "UIClass/DialogWidget/SearchDialog.h"
#include "UIClass/DialogWidget/ResultPrintDlg.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "UIClass/DialogWidget/QuestionDlg.h"
#include "UIClass/DialogWidget/RemoveDlg.h"
#include "UIClass/DialogWidget/ResultExportDlg.h"
#include "UIClass/DialogWidget/StatusDialog.h"
#include "UIClass/DialogWidget/LanguageSelectDlg.h"

#include "mainwindow.h"

#include "ResultView.h"
#include "ui_ResultView.h"


USING_SUGENTECH

struct ResultView::privateStruct
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> mResult;

    QScopedPointer<HL7InterFaceAdapter> mLISServerAdapter;
    QScopedPointer<DataBaseCtrl> mBasicDataBase;
    QScopedPointer<DataBaseCtrl> mCodeFuncDataBase;

    QScopedPointer<StatusDialog> mLISWaitMessageDlg;
    QScopedPointer<StatusDialog> mExportWaitMessageDlg;
    QScopedPointer<StatusDialog> mPrintWaitMessageDlg;

    QTimer mPrintTimer;
    QVector<int> mSelectedRowIndex;

    QString mLastSelectQuery;

    GlobalDataStruct::HIGH_LIGHT_DATA mHighlightClassData;
    bool mUseXML;
};

ResultView::ResultView(QWidget *parent) :
                                          CSerialNetworkWidget(parent),
                                          ui(new Ui_ResultView),
                                          d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    this->initStyleSheet();
    this->hideAdminUI();
}

ResultView::~ResultView()
{
    this->stop();
}

void ResultView::initVariables()
{
    d->mBasicDataBase.reset(new DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::BasicDB));
    d->mCodeFuncDataBase.reset(new DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::DBCodeFunc));

    this->setupTableWidgetHeader();
    this->setupAlignRule();
}

void ResultView::initSignalSlots()
{
    connect(ui->widgetComment, &OpinionSelectWidget::onApply, this, &ResultView::onUpdateComment);

    connect(ui->btnSearch, &QPushButton::clicked, this, &ResultView::onClickedSearch);
    connect(ui->btnRemove, &QPushButton::clicked, this, &ResultView::onClickedRemove);
    connect(ui->btnClean, &QPushButton::clicked, this, &ResultView::onClickClean);
    connect(ui->btnPrint, &QPushButton::clicked, this, &ResultView::onClickedPrint);
    connect(ui->btnExport, &QPushButton::clicked, this, &ResultView::onClickedExport);
    connect(ui->btnInterLockingLIS, &QPushButton::clicked, this, &ResultView::onClickLISInterLocking);
    connect(ui->btnTest, &QPushButton::clicked, this, &ResultView::onClickTest);
    connect(ui->btnTest2, &QPushButton::clicked, this, &ResultView::onClickTest2);
    connect(ui->checkRawData, &QCheckBox::clicked, this, [this]
            {
                this->onChangedPatientRow();
            });

    connect(ui->tablePatientList, &TableExcelStyleWidget::itemSelectionChanged,  this, &ResultView::onChangedPatientRow);
    connect(ui->tablePatientList, &TableExcelStyleWidget::onSortRequest, this, &ResultView::onUpdateSort);

    connect(&d->mPrintTimer, &QTimer::timeout, this, &ResultView::onPrintProcessing);

    this->initAction();
}

void ResultView::initStyleSheet()
{
    ui->btnSearch->setStyleSheetImage(":/Image/Image/ResultView/search_normal.png", ":/Image/Image/ResultView/search_over.png",
                                     ":/Image/Image/ResultView/search_focus.png", ":/Image/Image/ResultView/search_disable.png");

    ui->btnRemove->setStyleSheetImage(":/Image/Image/ResultView/delete_normal.png", ":/Image/Image/ResultView/delete_over.png",
                                     ":/Image/Image/ResultView/delete_focus.png", ":/Image/Image/ResultView/delete_disable.png");

    ui->btnClean->setStyleSheetImage(":/Image/Image/ResultView/delete_normal.png", ":/Image/Image/ResultView/delete_over.png",
                                      ":/Image/Image/ResultView/delete_focus.png", ":/Image/Image/ResultView/delete_disable.png");

    ui->btnExport->setStyleSheetImage(":/Image/Image/ResultView/export_normal.png", ":/Image/Image/ResultView/export_over.png",
                                      ":/Image/Image/ResultView/export_focus.png", ":/Image/Image/ResultView/export_disable.png");

    ui->btnPrint->setStyleSheetImage(":/Image/Image/ResultView/print_normal.png", ":/Image/Image/ResultView/print_over.png",
                                     ":/Image/Image/ResultView/print_focus.png", ":/Image/Image/ResultView/print_disable.png");

    ui->btnInterLockingLIS->setStyleSheetImage(":/Image/Image/ResultView/export_normal.png", ":/Image/Image/ResultView/export_over.png",
                                               ":/Image/Image/ResultView/export_focus.png", ":/Image/Image/ResultView/export_disable.png");
}

void ResultView::initWaitMessageDialog()
{
    auto topLevelWidget = MainWindow::getInstance();
    d->mLISWaitMessageDlg.reset(new StatusDialog(topLevelWidget));
    d->mLISWaitMessageDlg->setStatusMessage(tr("Connecting to LIS server. Please wait a moment."));
    d->mLISWaitMessageDlg->stopWaiting();

    d->mExportWaitMessageDlg.reset(new StatusDialog(topLevelWidget));
    d->mExportWaitMessageDlg->setStatusMessage(tr("Generating report file Please wait a moment."));
    d->mExportWaitMessageDlg->stopWaiting();

    d->mPrintWaitMessageDlg.reset(new StatusDialog(topLevelWidget));
    d->mPrintWaitMessageDlg->stopWaiting();
}

void ResultView::initAction()
{
    QAction* adminMenuSignal = new QAction(this);
    adminMenuSignal->setShortcut(Qt::CTRL | Qt::ALT| Qt::Key_H);
    adminMenuSignal->setShortcutContext(Qt::WindowShortcut);

    connect(adminMenuSignal, &QAction::triggered, this, &ResultView::showAdminUI);
    this->addAction(adminMenuSignal);
}

void ResultView::startWidget()
{
    auto config = ConfigInformation::getInstance();
    d->mLastSelectQuery = QString();
    d->mResult.clear();
    d->mSelectedRowIndex.clear();
    d->mPrintTimer.stop();
    ui->widgetComment->clearComment();

    this->cleanWidgets();
    this->loadHighLightData();
    this->loadUseXML();
    this->onLoadRecentData();

    if(config->isAdminMode() == true)
        this->showAdminUI();
    else
        this->hideAdminUI();

    this->initWaitMessageDialog();

    emit onResultViewStarted();
}

void ResultView::stopWidget()
{

}

void ResultView::setupTableWidgetHeader()
{
    auto itemNameHeader1 = ui->tableStripList1->horizontalHeader();
    auto itemNameHeader2 = ui->tableStripList2->horizontalHeader();
    auto stripNumberHeader = ui->tablePatientList->horizontalHeader();

    itemNameHeader1->resizeSection(RESULT_TABLE_RESULT_COLUMN_ITEM_NAME, 300);
    itemNameHeader2->resizeSection(RESULT_TABLE_RESULT_COLUMN_ITEM_NAME, 300);
    stripNumberHeader->resizeSection(PATIENT_TABLE_RESULT_COLUMN_STRIP_NUMBER, 100);
}

void ResultView::setupAlignRule()
{
    ui->btnSearch->setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG_CENTER);
    ui->btnRemove->setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG_CENTER);
    ui->btnClean->setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG_CENTER);
    ui->btnExport->setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG_CENTER);
    ui->btnPrint->setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG_CENTER);

    if(ConfigInformation::getDefaultLanguage() == GlobalDataStruct::LANGUAGE_LIST_KOR)
        ui->btnExport->setAlignMargin(ui->btnExport->getAlignMargin() + 5);
}

void ResultView::cleanWidgets()
{
    ui->labelImage->clear();
    ui->labelPanelName->clear();
    ui->tablePatientList->clearContents();
    ui->tableStripList1->clearContents();
    ui->tableStripList2->clearContents();
}

void ResultView::loadHighLightData()
{
    GenericIniDataControls generalData;
    d->mHighlightClassData = generalData.readClassHightLightData();
}

void ResultView::loadUseXML()
{
    GenericIniDataControls io;
    d->mUseXML = io.readUseXML();
}

QVector<GlobalDataStruct::AnalysisReportInfomation> ResultView::setUpElementData(const QString& query)
{
    QString patientTableName = "PATIENT_TABLE";
    QString resultTableName = "RESULT_TABLE";

    QVector<GlobalDataStruct::AnalysisReportInfomation> infomation;
    auto db = d->mBasicDataBase.data();
    auto patientQueryData =  db->select(query);

    QString patientColumnUniqueID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), patientTableName);
    QString patientColumnPatientID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ID), patientTableName);
    QString patientColumnName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_NAME), patientTableName);
    QString patientColumnPanelName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PANEL_NAME), patientTableName);
    QString patientColumnContentsName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_CONTENTS_NAME), patientTableName);
    QString patientColumnStripNumber = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_STRIP_NUMBER), patientTableName);
    QString patientColumnLotNumber = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_LOT_NUMBER), patientTableName);

    QString patientColumnApplicant = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_APPLICANT), patientTableName);
    QString patientColumnAnalyst = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ANALYST), patientTableName);
    QString patientColumnReviewer = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_REVIEWER), patientTableName);
    QString patientColumnDate = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);
    QString patientColumnAge = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_AGE), patientTableName);
    QString patientColumnSex = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_SEX), patientTableName);
    QString patientColumnPhone = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PHONE), patientTableName);
    QString patientColumnEmail = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_EMAIL), patientTableName);

    QString resultColumnUniqueID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), resultTableName);
    QString resultColumnIntensity = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_INTENSITY), resultTableName);
    QString resultColumnTestResult = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_TEST_RESULT), resultTableName);
    QString resultColumnImagePath = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_IMAGE_PATH), resultTableName);
    QString resultColumnComment = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_COMMENT), resultTableName);
    QString resultColumnError = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_ERROR_REASON), resultTableName);

    for(auto& itr : patientQueryData)
    {
        GlobalDataStruct::AnalysisReportInfomation element;

        element.uniqueID = itr[patientColumnUniqueID].toString();
        element.lotNumber = itr[patientColumnLotNumber].toString();
        element.element.patientID = itr[patientColumnPatientID].toString();
        element.element.name = itr[patientColumnName].toString();
        element.element.panelName = itr[patientColumnPanelName].toString();
        element.element.contentsName = itr[patientColumnContentsName].toString();

        element.element.stripNumber = itr[patientColumnStripNumber].toInt();
        element.element.applicant = itr[patientColumnApplicant].toString();
        element.element.analyst = itr[patientColumnAnalyst].toString();
        element.element.reviewer = itr[patientColumnReviewer].toString();
        element.dateTime = itr[patientColumnDate].toDateTime();
        element.element.age = itr[patientColumnAge].toString();
        element.element.sex = itr[patientColumnSex].toString();
        element.element.phone = itr[patientColumnPhone].toString();
        element.element.eMail = itr[patientColumnEmail].toString();

        element.result.rawIntensityValue = BasicDatabaseCtrl::makeStringToInstensityValue(itr[resultColumnIntensity].toString());
        element.result.isSuccess = itr[resultColumnTestResult].toBool();
        element.result.resultImagePath = itr[resultColumnImagePath].toString();
        element.result.comment = itr[resultColumnComment].toString();
        element.result.errorReason = itr[resultColumnError].toString();

        infomation.push_back(element);
    }

    return infomation;
}

QString ResultView::makeSearchQuery(const GlobalDataStruct::QUERY_CONDITION_DATA& dataStruct)
{
    auto db = d->mBasicDataBase.data();
    QString patientTableName = "PATIENT_TABLE";
    QString resultTableName = "RESULT_TABLE";

    QString patientColumnUniqueID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), patientTableName);
    QString patientColumnPatientID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ID), patientTableName);
    QString patientColumnName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_NAME), patientTableName);
    QString patientColumnPanelName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PANEL_NAME), patientTableName);
    QString patientColumnStripNumber = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_STRIP_NUMBER), patientTableName);
    QString patientColumnDate = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);
    QString resultColumnUniqueID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), resultTableName);

    QString fromSubQuery;
    fromSubQuery.append("SELECT * ");
    fromSubQuery.append(QString("FROM %1, %2 ").arg(patientTableName).arg(resultTableName));
    fromSubQuery.append(QString("WHERE %1.%2 = %3.%4 ").arg(patientTableName).arg(patientColumnUniqueID)
                            .arg(resultTableName).arg(resultColumnUniqueID));

    QString where;
    DataBaseCtrl::makeWhereBetweenDate(patientTableName, where, patientColumnDate, dataStruct.fromDate, dataStruct.toDate);
    DataBaseCtrl::makeWhereCondition(patientTableName, where, patientColumnPatientID, dataStruct.patientID);
    DataBaseCtrl::makeWhereCondition(patientTableName, where, patientColumnName, dataStruct.patientName);

    if(dataStruct.patientPanel != "ALL")
        DataBaseCtrl::makeWhereCondition(patientTableName, where, patientColumnPanelName, dataStruct.patientPanel);

    QString lastDateQuery;
    lastDateQuery.append("SELECT * ");
    lastDateQuery.append(QString("FROM (%1) ").arg(fromSubQuery));

    if(where.isEmpty() == false)
        lastDateQuery.append(QString("WHERE %1 ").arg(where));

    lastDateQuery.append(QString("ORDER BY %1 DESC, %2").arg(patientColumnDate).arg(patientColumnStripNumber));

    return lastDateQuery;
}

QString ResultView::makeRemoveQuery(const GlobalDataStruct::QUERY_CONDITION_DATA& dataStruct)
{
    auto db = d->mBasicDataBase.data();
    QString patientTableName = "PATIENT_TABLE";
    QString resultTableName = "RESULT_TABLE";

    QString patientColumnPatientID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ID), patientTableName);
    QString patientColumnName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_NAME), patientTableName);
    QString patientColumnPanelName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PANEL_NAME), patientTableName);
    QString patientColumnDate = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);

    QString where;
    DataBaseCtrl::makeWhereBetweenDate(patientTableName, where, patientColumnDate, dataStruct.fromDate, dataStruct.toDate);
    DataBaseCtrl::makeWhereCondition(patientTableName, where, patientColumnPatientID, dataStruct.patientID);
    DataBaseCtrl::makeWhereCondition(patientTableName, where, patientColumnName, dataStruct.patientName);

    if(dataStruct.patientPanel != "ALL")
        DataBaseCtrl::makeWhereCondition(patientTableName, where, patientColumnPanelName, dataStruct.patientPanel);

    QString query;
    query.append("DELETE ");
    query.append(QString("FROM %1 ").arg(patientTableName));

    if(where.isEmpty() == false)
        query.append(QString("WHERE %1 ").arg(where));

    return query;
}

QString ResultView::makeLastDataQuery()
{
    auto db = d->mBasicDataBase.data();

    QString patientTableName = "PATIENT_TABLE";
    QString resultTableName = "RESULT_TABLE";

    QString patientColumnUniqueID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), patientTableName);
    QString patientColumnPatientID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ID), patientTableName);
    QString patientColumnName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_NAME), patientTableName);
    QString patientColumnPanelName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PANEL_NAME), patientTableName);
    QString patientColumnContentsName = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_CONTENTS_NAME), patientTableName);
    QString patientColumnStripNumber = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_STRIP_NUMBER), patientTableName);
    QString patientColumnLotNumber = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_LOT_NUMBER), patientTableName);

    QString patientColumnApplicant = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_APPLICANT), patientTableName);
    QString patientColumnAnalyst = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ANALYST), patientTableName);
    QString patientColumnReviewer = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_REVIEWER), patientTableName);
    QString patientColumnDate = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);
    QString patientColumnAge = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_AGE), patientTableName);
    QString patientColumnSex = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_SEX), patientTableName);
    QString patientColumnPhone = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PHONE), patientTableName);
    QString patientColumnEmail = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_EMAIL), patientTableName);

    QString resultColumnUniqueID = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), resultTableName);
    QString resultColumnIntensity = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_INTENSITY), resultTableName);
    QString resultColumnTestResult = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_TEST_RESULT), resultTableName);
    QString resultColumnImagePath = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_IMAGE_PATH), resultTableName);
    QString resultColumnComment = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_COMMENT), resultTableName);
    QString resultColumnError = db->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_ERROR_REASON), resultTableName);

    QString fromSubQuery;
    fromSubQuery.append("SELECT * ");
    fromSubQuery.append(QString("FROM %1, %2 ").arg(patientTableName).arg(resultTableName));
    fromSubQuery.append(QString("WHERE %1.%2 = %3.%4 ").arg(patientTableName).arg(patientColumnUniqueID)
                            .arg(resultTableName).arg(resultColumnUniqueID));

    QString query;
    query.append("SELECT * ");
    query.append(QString("FROM (%1) ").arg(fromSubQuery));
    query.append(QString("WHERE %1.%2 = (SELECT MAX(%1.%2) FROM %1) ")
                             .arg(patientTableName).arg(patientColumnDate));
    query.append(QString("ORDER BY %1 DESC, %2").arg(patientColumnDate).arg(patientColumnStripNumber));

    return query;
}

void ResultView::onResultShow(QVector<GlobalDataStruct::AnalysisReportInfomation> resultInfomation)
{
    d->mResult = resultInfomation;
    this->onUpdatePatientList();

    emit onResultViewStarted();

    if(d->mUseXML)
    {
        U2BioXmlLisProtocol lis;
        lis.saveU2BioXmlOutData(resultInfomation, d->mCodeFuncDataBase.data());
    }
}

void ResultView::onUpdateComment()
{
    auto rowItems = ui->tablePatientList->selectionModel()->selectedRows();

    if(rowItems.isEmpty())
        return;

    for(auto& itr : rowItems)
        this->updateCommentData(itr.row());
}

void ResultView::onClickedSearch()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---onClickedSearch---"));
    SearchDialog dlg;

    if(dlg.exec() == QDialog::Rejected)
        return;

    d->mResult.clear();

    this->cleanWidgets();
    auto dataStruct = dlg.getData();

    auto query = this->makeSearchQuery(dataStruct);
    auto infomation =  this->setUpElementData(query);

    QScopedPointer<IDataClassifier> calcClassData(new DataClassifierBase);
    calcClassData->calcClassData(infomation,d->mCodeFuncDataBase.data());

    //BasicSupremeIAnalysis::calcClassData(infomation, d->mCodeFuncDataBase.data());
    d->mResult = infomation;
    d->mLastSelectQuery = query;
    this->onUpdatePatientList();
}


void ResultView::onClickedRemove()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---onClickedRemove---"));
    RemoveDlg dlg(this);

    if(dlg.exec() == QDialog::Rejected)
        return;

    if(dlg.getSelectedMode() ==  RemoveDlg::REMOVE_DIALOG_SELECLECTION_MODE_SELECTED_ROW)
    {
        if(this->removeSelectedRows() == false)
            return;
    }
    else
    {
        auto data = dlg.getData();
        auto dataStruct = dlg.getData();

        this->deleteConditionDBData(dataStruct);
        this->loadLastQueryData();
    }

    this->onUpdatePatientList();
}

void ResultView::onClickClean()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---onClickClean---"));
    auto currentDate = QDate::currentDate();
    auto pastDateString = currentDate.addYears(-1).toString("yyyy-MM-dd");

    QuestionDlg dlg(this);
    dlg.setQuestionMessage(tr("Are you sure you want to delete old data? Data prior to %1").arg(pastDateString));

    if(dlg.exec() == QDialog::Rejected)
        return;

    this->deletingDBDataYearAgo();
    this->loadLastQueryData();
    this->onUpdatePatientList();
}

void ResultView::onClickedExport()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---onClickedExport---"));
    this->saveExportDataForUserSelectPath();
}

void ResultView::onClickedPrint()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---onClickedPrint---"));
    ResultPrintSelectLangDlg selectLangDlg(this);

    if(selectLangDlg.exec() == QDialog::Rejected)
        return ;

    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    if(selectedRows.isEmpty())
    {
        for(auto i = 0; i < d->mResult.count(); i++)
            d->mSelectedRowIndex << i;
    }
    else
    {
        for(int i = 0 ; i < selectedRows.count(); i++)
        {
            auto row = selectedRows.at(i).row();
            d->mSelectedRowIndex << row;
        };
    }

    QTimer::singleShot(100, this, [this]
                       {
                           d->mPrintWaitMessageDlg->startWaiting();
                           d->mPrintTimer.start(200);
                       });
}

void ResultView::onClickTest()
{
    CLogWriter::printLog(QString("[WorkPlaceView::] : ---onClickTest---"));
    if(d->mResult.isEmpty() == true)
        return;

    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    if(selectedRows.isEmpty())
        ReportData::writeIntensityData(d->mResult);
    else
        this->selectedTest1ReportOut();
}

void ResultView::onClickTest2()
{
    if(d->mResult.isEmpty() == true)
        return;

    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    if(selectedRows.isEmpty())
        ReportData::writeIntensityData(d->mResult, ui->checkRawData->isChecked());
    else
        this->selectedTest2ReportOut();
}

void ResultView::selectedTest1ReportOut()
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> data;

    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    for(auto& itr : selectedRows)
    {
        auto row = itr.row();

        if(d->mResult.count() <= row)
            continue;

        data << d->mResult[row];
    }

    if(data.isEmpty() == false)
        ReportData::writeIntensityData(data);
}

void ResultView::selectedTest2ReportOut()
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> data;

    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    for(auto& itr : selectedRows)
    {
        auto row = itr.row();

        if(d->mResult.count() <= row)
            continue;

        data << d->mResult[row];
    }

    if(data.isEmpty() == false)
        ReportData::writeIntensityData(d->mResult, ui->checkRawData->isChecked());
}

void ResultView::onClickLISInterLocking()
{
    if(d->mUseXML)
        this->saveXmlOutData();
    else
        this->sendResultToLIS();
}

QStringList ResultView::saveAllResultRowData(const QString& dirPath)
{
    QStringList savePathList;

    for(int i = 0 ; i < ui->tablePatientList->rowCount(); i++)
        savePathList << this->saveResultRowData(dirPath, i);

    return savePathList;
}

QStringList ResultView::saveSelectedResultRowData(const QString& dirPath)
{
    QStringList savePathList;

    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    for(auto& itr : selectedRows)
    {
        auto row = itr.row();
        savePathList << this->saveResultRowData(dirPath, row);
    }
    return savePathList;
}

QString ResultView::saveResultRowData(const QString& dirPath, int selectedRow)
{
    if(selectedRow < 0)
        return QString();

    if(d->mResult.count() <= selectedRow)
        return QString();

    auto timeStamp = d->mResult[selectedRow].dateTime.toString("yyyy-MM-dd-hh-mm");

    ReportData data;
    data.openExcelFile();

    CUtil::createDirectory(dirPath);
    QString fileName = QString("%1/(Report-%2-%3)%4.xlsx")
                           .arg(dirPath)
                           .arg(d->mResult[selectedRow].element.patientID)
                           .arg(d->mResult[selectedRow].element.panelName)
                           .arg(timeStamp);

    data.writeExcelDataV5(d->mResult[selectedRow]);
    data.closeExcelFile(fileName);

    return fileName;
}

bool ResultView::isHighLightColor(const QString &classType)
{
    if(classType == "6")
        if(d->mHighlightClassData.class6 == true)
            return true;

    if(classType == "5")
        if(d->mHighlightClassData.class5 == true)
            return true;

    if(classType == "4")
        if(d->mHighlightClassData.class4 == true)
            return true;

    if(classType == "3")
        if(d->mHighlightClassData.class3 == true)
            return true;

    if(classType == "2")
        if(d->mHighlightClassData.class2 == true)
            return true;

    if(classType == "1")
        if(d->mHighlightClassData.class1 == true)
            return true;

    if(classType == "0")
        if(d->mHighlightClassData.class0 == true)
            return true;

    return false;
}

bool ResultView::isSampleMissing(const QString& errorReason)
{
    if(errorReason.isEmpty())
        return false;

    auto errorToken = errorReason.split(":");
    auto errorHeader = errorToken.first();

    if(errorHeader.contains("[ETC-S]"))
        return true;

    return false;
}

void ResultView::deleteConditionDBData(const GlobalDataStruct::QUERY_CONDITION_DATA& conditionData)
{
    auto imagePathList = this->findFromDBForImageDataPathForRemove(conditionData);
    this->removeImageDataFromFullPath(imagePathList);

    auto query = this->makeRemoveQuery(conditionData);
    d->mBasicDataBase->writeData(query);
}

void ResultView::deletingDBDataYearAgo()
{
    auto currentDate = QDate::currentDate();
    auto pastYearDate = currentDate.addYears(-1).addDays(1);

    QString patientTableName = "PATIENT_TABLE";
    QString patientColumnDate = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);
    d->mBasicDataBase->removeDataBeforeDate(patientTableName, patientColumnDate, pastYearDate);
}

bool ResultView::removeSelectedRows()
{
    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    if(selectedRows.isEmpty() == true)
        return false;

    if(d->mResult.isEmpty() == true)
        return false;

    auto imagePathList = this->findFromDBForImageDataPathForRemove(selectedRows);
    this->removeImageDataFromFullPath(imagePathList);
    this->removeSelectedDBAndUpdateResultVector(selectedRows);

    return true;
}
void ResultView::removeImageDataFromFullPath(const QVector<QString>& pathList)
{
    for(auto&itr : pathList)
        QFile::remove(itr);
}

void ResultView::removeSelectedDBAndUpdateResultVector(const QModelIndexList& selectedRows)
{
    QString patientTableName = "PATIENT_TABLE";
    QString patientColumnUniqueID = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), patientTableName);

    QVector<QString> removeList;

    for(int i = 0 ; i < selectedRows.count(); i++)
    {
        auto row = selectedRows.at(i).row();
        removeList.push_back(d->mResult[row].uniqueID);
        d->mBasicDataBase->removeData(patientTableName, patientColumnUniqueID, d->mResult[row].uniqueID);
    }

    for(auto& itr : removeList)
    {
        for(int idx = 0; idx < d->mResult.count(); idx++)
        {
            if(d->mResult[idx].uniqueID == itr)
            {
                d->mResult.remove(idx);
                break;
            }
        }
    }
}

void ResultView::loadLastQueryData()
{
    this->cleanWidgets();
    d->mResult.clear();

    auto infomation =  this->setUpElementData(d->mLastSelectQuery);

    QScopedPointer<IDataClassifier> calcClassData(new DataClassifierBase);
    calcClassData->calcClassData(infomation,d->mCodeFuncDataBase.data());

    d->mResult = infomation;
}

void ResultView::clearStripResultTable()
{
    ui->tableStripList1->clearContents();
    ui->tableStripList2->clearContents();
    ui->widgetComment->clearComment();
    ui->labelImage->setPixmap(QPixmap());
    ui->labelPanelName->setText("");
}

void ResultView::setupResultStripDataForSingleRow(int row)
{
    if(row < 0 )
        return;

    if(d->mResult.count() <= row)
        return;

    auto info = d->mResult[row];

    ui->widgetComment->setOpinionText(info.result.comment);

    if(info.result.image.data == nullptr)
        ui->labelImage->setPixmap(QPixmap(info.result.resultImagePath));
    else
        ui->labelImage->setPixmap(CUtil::convertMatToQPixmap(info.result.image));

    ui->labelPanelName->setText(info.element.panelName);
}

void ResultView::setSingleBandData(int row)
{
    if(row < 0 )
        return ;

    if(d->mResult.count() <= row)
        return;

    auto info = d->mResult[row];

    if(info.result.resultValue.count() <= 1)
    {
        ui->tableStripList1->setRowCount(0);
        return;
    }

    auto itemNameList = ConfigInformation::getInstance()->getCurrentStripNameList(info.element.panelName);
    auto isBandBehindList = ConfigInformation::getInstance()->getIsStripBehindeList(info.element.panelName);
    ui->tableStripList1->setRowCount(0);

    for(int i = 0; i <info.result.rawIntensityValue.count(); i++)
    {
        QString stripName;
        if(isBandBehindList[i] == true)
            continue;
        ui->tableStripList1->insertRow(ui->tableStripList1->rowCount());
        int tableRowIdx = ui->tableStripList1->rowCount()-1;

        if(i < itemNameList.count())
            stripName = itemNameList[i];

        ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_ITEM_NAME, new QTableWidgetItem(stripName));

        if(i < info.result.resultValue.count())
        {
            if(ui->checkRawData->isChecked() == true)
                ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_INTENSITY, new QTableWidgetItem(QString::number(info.result.rawIntensityValue[i])));
            else
                ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_INTENSITY, new QTableWidgetItem(info.result.resultValue[i]));
        }

        if(i < info.result.classType.count())
        {
            auto classItem = new QTableWidgetItem(info.result.classType[i]);
            ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_CLASS, classItem);

            if(this->isHighLightColor(info.result.classType[i]) == true)
            {
                classItem->setTextColor(QColor(40, 160, 255));
                auto currentText = classItem->text();
                classItem->setText(currentText.append("*"));
            }
        }
    }

    ui->tableStripList2->hide();
}

void ResultView::setMultiBandData(int row)
{
    if(row < 0 )
        return ;

    if(d->mResult.count() <= row)
        return;

    if(ui->tableStripList2->isHidden() == true)
        ui->tableStripList2->show();

    auto info = d->mResult[row];

    if(info.result.resultValue.count() <= 1)
    {
        ui->tableStripList1->setRowCount(0);
        ui->tableStripList2->setRowCount(0);
        return;
    }

    auto config = ConfigInformation::getInstance();
    auto resultCount = config->getBoxCarAnalysis().boxCarbandCount/2;
    auto itemNameList = config->getCurrentStripNameList(info.element.panelName);
    auto isBandBehindList = ConfigInformation::getInstance()->getIsStripBehindeList(info.element.panelName);

    ui->tableStripList1->setRowCount(0);
    ui->tableStripList2->setRowCount(0);

    for(int i = 0; i < resultCount; i++)
    {
        QString stripName;
        if(isBandBehindList[i] == true)
            continue;
        ui->tableStripList1->insertRow(ui->tableStripList1->rowCount());
        int tableRowIdx = ui->tableStripList1->rowCount()-1;
//        int tableRowIdx = i;

        if(i < itemNameList.count())
            stripName = itemNameList[i];

        ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_ITEM_NAME, new QTableWidgetItem(stripName));

        if(i < info.result.resultValue.count())
        {
            if(ui->checkRawData->isChecked() == true)
                ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_INTENSITY, new QTableWidgetItem(QString::number(info.result.rawIntensityValue[i])));
            else
                ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_INTENSITY, new QTableWidgetItem(info.result.resultValue[i]));
        }

        if(i < info.result.classType.count())
        {
            auto classItem = new QTableWidgetItem(info.result.classType[i]);
            ui->tableStripList1->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_CLASS, classItem);

            if(this->isHighLightColor(info.result.classType[i]) == true)
            {
                classItem->setTextColor(QColor(40, 160, 255));
                auto currentText = classItem->text();
                classItem->setText(currentText.append("*"));
            }
        }
    }

    for(int i = resultCount; i < info.result.resultValue.count(); i++)
    {
        QString stripName;

        if(isBandBehindList[i] == true)
            continue;
        ui->tableStripList2->insertRow(ui->tableStripList2->rowCount());
        int tableRowIdx = ui->tableStripList2->rowCount()-1;
        //int tableRowIdx = i - resultCount;

        if(i < itemNameList.count())
            stripName = itemNameList[i];

        ui->tableStripList2->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_ITEM_NAME, new QTableWidgetItem(stripName));

        if(i < info.result.resultValue.count())
        {
            if(ui->checkRawData->isChecked() == true)
                ui->tableStripList2->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_INTENSITY, new QTableWidgetItem(QString::number(info.result.rawIntensityValue[i])));
            else
                ui->tableStripList2->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_INTENSITY, new QTableWidgetItem(info.result.resultValue[i]));
        }

        if(i < info.result.classType.count())
        {
            auto classItem = new QTableWidgetItem(info.result.classType[i]);
            ui->tableStripList2->setItem(tableRowIdx, RESULT_TABLE_RESULT_COLUMN_CLASS, classItem);

            if(this->isHighLightColor(info.result.classType[i]) == true)
            {
                classItem->setTextColor(QColor(40, 160, 255));
                auto currentText = classItem->text();
                classItem->setText(currentText.append("*"));
            }
        }
    }
}

QVector<QString> ResultView::findFromDBForImageDataPathForRemove(const GlobalDataStruct::QUERY_CONDITION_DATA& conditionData)
{
    QVector<QString> removeImagePathList;
    auto query = this->makeSearchQuery(conditionData);
    auto queryResult = d->mBasicDataBase->select(query);

    QString patientTableName = "PATIENT_TABLE";
    QString patientColumnDate = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), patientTableName);

    for(auto& itr: queryResult)
        removeImagePathList << itr[patientColumnDate].toString();

    return removeImagePathList;
}

QVector<QString> ResultView::findFromDBForImageDataPathForRemove(const QModelIndexList &selectedRows)
{
    QVector<QString> imagePathList;
    QString patientTableName = "PATIENT_TABLE";
    QString patientColumnUniqueID = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), patientTableName);

    for(int i = 0 ; i < selectedRows.count(); i++)
    {
        auto row = selectedRows.at(i).row();
        imagePathList.push_back(d->mResult[row].result.resultImagePath);
    }

    return imagePathList;
}

void ResultView::saveExportDataForUserSelectPath()
{
    ResultExportDlg dlg(this);

    if(dlg.exec() == QDialog::Rejected)
        return ;

    auto dirPath = dlg.getPath();
    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    ExcelExportClass* excelExport = new ExcelExportClass(d->mResult, selectedRows, dirPath);

    connect(excelExport, &ExcelExportClass::onStartExport, this, [this]
            {
                this->showExportWaitMessage();
            });

    connect(excelExport, &ExcelExportClass::onFinishedExport, this, [this]
            {
                auto config = ConfigInformation::getInstance();
                config->changeCurrentLanguage(config->getDefaultLanguage());
                this->hideExportWaitMessage();
            });

    excelExport->startThread();
}

void ResultView::updateCommentData(const int& row)
{
    if(row < 0)
        return;

    d->mResult[row].result.comment = ui->widgetComment->getOpinionText();

    QString resultTableName = "RESULT_TABLE";

    QVariantMap updateMap;
    QString resultColumnComment = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_COMMENT), resultTableName);
    updateMap[resultColumnComment] = d->mResult[row].result.comment;

    QString resultColumnUniqueID = d->mBasicDataBase->convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), resultTableName);
    d->mBasicDataBase->updateData(resultTableName, updateMap, resultColumnUniqueID, d->mResult[row].uniqueID);
}

void ResultView::setBackGroundYellowColor(QTableWidgetItem* item)
{
    if(item == nullptr)
        return;

    item->setBackgroundColor(Qt::yellow);
}

void ResultView::setBackGroundOrangeColor(QTableWidgetItem* item)
{
    if(item == nullptr)
        return;

    item->setBackgroundColor(QColor("orange"));
}

void ResultView::setToolTipMessage(QTableWidgetItem* item, const QString& message)
{
    if(item == nullptr)
        return;

    item->setToolTip(message);
}

void ResultView::sendResultToLIS()
{
    this->onDisableTopLevelWidget();
    this->showLISWaitMessage();

    d->mLISServerAdapter.reset(new HL7InterFaceAdapter(this));

    QDateTime dateTime = QDateTime::currentDateTime();
    auto sendData = this->getResultToLISProtocol();
    auto topLevelWidget = MainWindow::getInstance();

    if(sendData.isEmpty())
    {
        this->hideLISWaitMessage();
        this->onEnableTopLevelWidget();

        auto msgDlg = new MessageShowDlg(tr("At least one data to transfer must be selected."), topLevelWidget);
        msgDlg->exec();

        return;
    }

    auto sendResult = d->mLISServerAdapter->sendData(sendData);

    this->hideLISWaitMessage();
    this->onEnableTopLevelWidget();

    if(sendResult == false)
    {
        auto msgDlg = new MessageShowDlg(tr("Unable to connect to server or no response."), topLevelWidget);
        msgDlg->exec();
    }
}

void ResultView::saveXmlOutData()
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> result;
    auto selectedRows = ui->tablePatientList->selectionModel()->selectedRows();

    for(auto& itr : selectedRows)
    {
        auto row = itr.row();

        if(d->mResult.count() <= row)
            continue;

        result << d->mResult[row];
    }

    U2BioXmlLisProtocol lis;
    lis.saveU2BioXmlOutData(result, d->mCodeFuncDataBase.data());
}

void ResultView::showLISWaitMessage()
{
    d->mLISWaitMessageDlg->startWaiting();
}

void ResultView::hideLISWaitMessage()
{
    d->mLISWaitMessageDlg->stopWaiting();
}

void ResultView::showExportWaitMessage()
{
    d->mExportWaitMessageDlg->startWaiting();
}

void ResultView::hideExportWaitMessage()
{
    d->mExportWaitMessageDlg->stopWaiting();
}

void ResultView::showAdminUI()
{
    ui->widgetAdminMenu->show();
}

void ResultView::hideAdminUI()
{
    ui->widgetAdminMenu->hide();
}

QByteArray ResultView::getResultToLISProtocol()
{
    QByteArray data;

    auto items = ui->tablePatientList->selectionModel()->selectedRows();

    if(items.isEmpty())
        return data;

    auto mshProtocol = HL7InterFaceWriter::createMSH(this->getResultMSHData());
    data.append(mshProtocol);

    for(auto& itr : items)
    {
        auto i = itr.row();

        if(d->mResult.count() <= i)
            continue;

        auto pidData = this->getResultPIDData(i);
        auto obrData = this->getResultOBRData(i);
        auto obxData = this->getResultOBXData(i);

        data.append(HL7InterFaceWriter::createPID(pidData));
        data.append(HL7InterFaceWriter::createOBR(obrData));
        data.append(HL7InterFaceWriter::createOBX(obxData));
    }

    return data;
}

HL7_MSH_DATA ResultView::getResultMSHData()
{
    HL7_MSH_DATA data;
    data.msgType = HL7_MSH_MESSAGE_TYPE_ORU;
    data.dateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    data.deviceID = HL7_DEVICE_ID;

    return data;
}

HL7_PID_DATA ResultView::getResultPIDData(int idx)
{
    HL7_PID_DATA pidData;
    auto itr = d->mResult[idx];

    pidData.index = QString::number(idx + 1);
    pidData.sex = itr.element.sex;
    pidData.name = itr.element.name;
    pidData.birthDate = itr.element.age;
    pidData.patientID = itr.element.patientID;
    pidData.phoneNumber = itr.element.phone;

    return pidData;
}

HL7_OBR_DATA ResultView::getResultOBRData(int idx)
{
    HL7_OBR_DATA obrData;
    auto itr = d->mResult[idx];

    obrData.index = QString::number(idx + 1);
    obrData.dateTime = itr.dateTime.toString("yyyyMMddhhmmss");
    obrData.patientID = itr.element.patientID;
    obrData.panelInitial = HL7InterFaceReader::panelNameToPanelInitial(itr.element.panelName);
    obrData.resultImagePath = d->mResult[idx].result.resultImagePath;
    return obrData;
}

HL7_OBX_DATA ResultView::getResultOBXData(int idx)
{
    HL7_OBX_DATA obxData;
    QVector<HL7_OBX_ELEMENT> obxElement;
    auto itr = d->mResult[idx];
    auto config = ConfigInformation::getInstance();
    auto itemCodeList = config->getCurrentStripCodeList(itr.element.panelName);

    obxData.patientID = itr.element.patientID;

    for(auto i = 0 ; i < itr.result.resultValue.count(); i++)
    {
        QString bandCode;
        QString iuml;
        QString result;

        if(i < itemCodeList.count())
            bandCode = itemCodeList[i];

        if(i < itr.result.classType.count())
            iuml = itr.result.classType[i];

        if(i < itr.result.resultValue.count())
            result = itr.result.resultValue[i];

        HL7_OBX_ELEMENT data;

        data.bandCode = bandCode;
        data.iumlClass = iuml;
        data.iumlResult = result;

        obxElement.push_back(data);
    }

    obxData.dataResult = obxElement;

    return obxData;
}

QString ResultView::getWaitTitleMessageString()
{
    auto language = ConfigInformation::getDefaultLanguage();
    QString waitMessage;

    switch(language)
    {
    case GlobalDataStruct::LANGUAGE_LIST_KOR:
        waitMessage = "잠시만 기다려 주십시오";
        break;

    case GlobalDataStruct::LANGUAGE_LIST_RU:
        waitMessage = "Подождите несколько секунд";
        break;

    default:
        waitMessage = "Wait for a few minutes";
        break;
    }

    return waitMessage;
}

QString ResultView::getExportWaitMessageString()
{
    auto language = ConfigInformation::getDefaultLanguage();
    QString waitMessage;

    switch(language)
    {
    case GlobalDataStruct::LANGUAGE_LIST_KOR:
        waitMessage = "리포트 파일을 생성중입니다 잠시만 기다려 주십시오.";
        break;

    case GlobalDataStruct::LANGUAGE_LIST_RU:
        waitMessage = "Создание файла отчета. Пожалуйста, подождите немного.";
        break;

    default:
        waitMessage = "Generating report file Please wait a moment.";
        break;
    }

    return waitMessage;
}

QString ResultView::getPrintWaitMessageString()
{
    auto language = ConfigInformation::getDefaultLanguage();
    QString waitMessage;

    switch(language)
    {
    case GlobalDataStruct::LANGUAGE_LIST_KOR:
        waitMessage = "리포트가 출력중입니다";
        break;

    case GlobalDataStruct::LANGUAGE_LIST_RU:
        waitMessage = "Отчет печатается";
        break;

    default:
        waitMessage = "The report is being printed";
        break;
    }

    return waitMessage;
}

bool ResultView::sortStripNumberData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b)
{
    if(a.element.stripNumber == b.element.stripNumber)
        return a.dateTime < b.dateTime;

    return a.element.stripNumber < b.element.stripNumber;
}

bool ResultView::sortIDData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b)
{
    if(a.element.patientID == b.element.patientID)
        return ResultView::sortStripNumberData(a,b);

    return a.element.patientID < b.element.patientID;
}

bool ResultView::sortNameData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b)
{
    if(a.element.name == b.element.name)
        return ResultView::sortStripNumberData(a,b);

    return a.element.name < b.element.name;
}

bool ResultView::sortPanelData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b)
{
    if(a.element.panelName == b.element.panelName)
        return ResultView::sortStripNumberData(a,b);

    return a.element.panelName < b.element.panelName;
}

bool ResultView::sortTestDate(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b)
{
    if(a.dateTime == b.dateTime)
        return ResultView::sortStripNumberData(a,b);

    return a.dateTime < b.dateTime;
}

void ResultView::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
//        ui->retranslateUi(this);
    }
}

void ResultView::onUpdateSort(int column)
{
    switch(column)
    {
    case ResultView::PATIENT_TABLE_RESULT_COLUMN_STRIP_NUMBER:
        std::sort(d->mResult.begin(), d->mResult.end(), &ResultView::sortStripNumberData);
        break;

    case ResultView::PATIENT_TABLE_RESULT_COLUMN_ID:
        std::sort(d->mResult.begin(), d->mResult.end(), &ResultView::sortIDData);
        break;

    case ResultView::PATIENT_TABLE_RESULT_COLUMN_NAME:
        std::sort(d->mResult.begin(), d->mResult.end(), &ResultView::sortNameData);
        break;

    case ResultView::PATIENT_TABLE_RESULT_COLUMN_PANEL:
        std::sort(d->mResult.begin(), d->mResult.end(), &ResultView::sortPanelData);
        break;

    case ResultView::PATIENT_TABLE_RESULT_COLUMN_TEST_DATE:
        std::sort(d->mResult.begin(), d->mResult.end(), &ResultView::sortTestDate);
        break;
    }

    this->onUpdatePatientList();
}

void ResultView::onUpdatePatientList()
{
    ui->tablePatientList->clearContents();
    ui->tablePatientList->setRowCount(d->mResult.count());

    if(d->mResult.isEmpty())
        return;

    for(int i = 0; i < d->mResult.count(); i++)
    {
        auto info = d->mResult[i];

        auto stripNumber = new QTableWidgetItem(QString::number(info.element.stripNumber));
        auto id = new QTableWidgetItem(info.element.patientID);
        auto name = new QTableWidgetItem(info.element.name);
        auto panel = new QTableWidgetItem(info.element.panelName);
        auto date = new QTableWidgetItem(info.dateTime.toString("yyyy-MM-dd-hh:mm:ss"));

        ui->tablePatientList->setItem(i, PATIENT_TABLE_RESULT_COLUMN_STRIP_NUMBER, stripNumber);
        ui->tablePatientList->setItem(i, PATIENT_TABLE_RESULT_COLUMN_ID, id);
        ui->tablePatientList->setItem(i, PATIENT_TABLE_RESULT_COLUMN_NAME, name);
        ui->tablePatientList->setItem(i, PATIENT_TABLE_RESULT_COLUMN_PANEL, panel);
        ui->tablePatientList->setItem(i, PATIENT_TABLE_RESULT_COLUMN_TEST_DATE, date);

        if(info.result.isSuccess == false)
        {
            auto isSampleMissing = this->isSampleMissing(info.result.errorReason);

            if(isSampleMissing)
            {
                this->setBackGroundOrangeColor(stripNumber);
                this->setBackGroundOrangeColor(id);
                this->setBackGroundOrangeColor(name);
                this->setBackGroundOrangeColor(panel);
                this->setBackGroundOrangeColor(date);
            }
            else
            {
                this->setBackGroundYellowColor(stripNumber);
                this->setBackGroundYellowColor(id);
                this->setBackGroundYellowColor(name);
                this->setBackGroundYellowColor(panel);
                this->setBackGroundYellowColor(date);
            }

            QString message = info.result.errorReason;
            this->setToolTipMessage(stripNumber, message);
            this->setToolTipMessage(id, message);
            this->setToolTipMessage(name, message);
            this->setToolTipMessage(panel, message);
            this->setToolTipMessage(date, message);
        }
    }
}

void ResultView::onChangedPatientRow()
{
    this->clearStripResultTable();

    auto row = ui->tablePatientList->currentRow();

    if(row < 0 )
        return ;

    if(d->mResult.count() <= row)
        return;

    if(d->mResult[row].result.resultValue.isEmpty() == true)
        return;

    this->setupResultStripDataForSingleRow(row);

     auto isMultiBand = ConfigInformation::getInstance()->getStatus().isMultiBand;

     if(isMultiBand == false)
         this->setSingleBandData(row);
     else
         this->setMultiBandData(row);

     auto currentComment = d->mResult[row].result.comment;
     ui->widgetComment->setOpinionText(currentComment);
}

void ResultView::onLoadRecentData()
{
    d->mResult.clear();
    this->cleanWidgets();

    auto query = this->makeLastDataQuery();
    auto infomation =  this->setUpElementData(query);

    QScopedPointer<IDataClassifier> calcClassData(new DataClassifierBase);
    calcClassData->calcClassData(infomation,d->mCodeFuncDataBase.data());

    d->mResult = infomation;
    d->mLastSelectQuery = query;

    this->onUpdatePatientList();
}

void ResultView::onDisableTopLevelWidget()
{
    auto topLevelWidget = MainWindow::getInstance();
    topLevelWidget->setDisableTopLevelWidget(true);
}

void ResultView::onEnableTopLevelWidget()
{
    auto topLevelWidget = MainWindow::getInstance();
    topLevelWidget->setDisableTopLevelWidget(false);
}

void ResultView::onPrintProcessing()
{
    static int statusCount = 0;

    if(d->mSelectedRowIndex.count() <= statusCount)
    {
        d->mPrintTimer.stop();
        d->mPrintWaitMessageDlg->stopWaiting();
        d->mSelectedRowIndex.clear();

        auto config = ConfigInformation::getInstance();
        config->changeCurrentLanguage(config->getDefaultLanguage());

        statusCount = 0;

        return;
    }

    d->mPrintWaitMessageDlg->setStatusMessage(QString("%1(%2/%3)").arg(this->getPrintWaitMessageString()).arg(statusCount + 1).arg(d->mSelectedRowIndex.count()));

    auto row = d->mSelectedRowIndex[statusCount];
    auto data = d->mResult[row];

    ResultPrintDlg dlg(data);
    dlg.hide();
    dlg.startPrint();

    statusCount++;
}
