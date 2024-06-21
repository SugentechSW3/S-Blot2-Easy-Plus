#include <QPrinter>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QPainter>
#include <QTranslator>
#include <QtSingleApplication.h>
#include <QPixmap>

#include "ResultPrintWidget.h"
#include "ui_ResultPrintWidget.h"

#include "Common/Io/INIFileControls/GenericIniDataControls.h"

#include "Analysis/ConfigInformation.h"
#include "UIClass/DialogWidget/InspectionOpinionDlg.h"
#include "UIClass/ResultView/SubWidget/ResultPrintWidgetClassTableItem.h"

struct ResultPrintWidget::privateStruct
{
    GlobalDataStruct::HIGH_LIGHT_DATA mHighlightClassData;
};

ResultPrintWidget::ResultPrintWidget(QWidget *parent) :
                                                        QWidget(parent),
                                                        ui(new Ui_ResultPrintWidget),
                                                        d(new privateStruct)
{
    ui->setupUi(this);

    this->createTableHeader();
    this->initTitleText();    
}

ResultPrintWidget::~ResultPrintWidget()
{
}

void ResultPrintWidget::initTitleText()
{
    ui->labelSubTitle->setText(tr("Allergy Test Report (Allergen-specific Human IgE Test Result)"));
    ui->labelPatientIDTitle->setText(tr("Patient ID:"));
    ui->labelPatientNameTitle->setText(tr("Patient Name:"));
    ui->labelAgeTitle->setText(tr("Age:"));
    ui->labelSexTitle->setText(tr("Sex:"));
    ui->labelInstitutionTitle->setText(tr("Institution:"));
    ui->labelTestingDateTitle->setText(tr("Testing Date:"));
    ui->labelTestedByTitle->setText(tr("Tested by:"));
    ui->labelPanelTitle->setText(tr("Panel:"));

    auto tableItemNotFound = new QTableWidgetItem(tr("Not found"));
    auto tableItemWeak = new QTableWidgetItem(tr("Weak"));
    auto tableItemModerate = new QTableWidgetItem(tr("Moderate"));
    auto tableItemModeratelyStrong = new QTableWidgetItem(tr("Moderately strong"));
    auto tableItemStrong = new QTableWidgetItem(tr("Strong"));
    auto tableItemVeryStrong = new QTableWidgetItem(tr("Very strong"));
    auto tableItemExtremelyStrong = new QTableWidgetItem(tr("Extremely strong"));

    ui->tableWidgetSpecific->setItem(1, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemNotFound);
    ui->tableWidgetSpecific->setItem(2, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemWeak);
    ui->tableWidgetSpecific->setItem(3, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemModerate);
    ui->tableWidgetSpecific->setItem(4, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemModeratelyStrong);
    ui->tableWidgetSpecific->setItem(5, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemStrong);
    ui->tableWidgetSpecific->setItem(6, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemVeryStrong);
    ui->tableWidgetSpecific->setItem(7, TABLE_COLUMN_SPECIFIC_LEVEL, tableItemExtremelyStrong);
}

void ResultPrintWidget::createTableHeader()
{
    ui->tableWidgetFront->setRowCount(0);
    ui->tableWidgetBack->setRowCount(0);

    auto header = ui->tableWidgetFront->horizontalHeader();
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_FRONT_NUMBER, 25);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_FRONT_ALLERGEN, 180);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_FRONT_CODE, 40);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_FRONT_IU, 40);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS, 110);

    header = ui->tableWidgetBack->horizontalHeader();
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_BACK_NUMBER, 25);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_BACK_ALLERGEN, 180);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_BACK_CODE, 40);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_BACK_IU, 40);
    header->resizeSection(TABLE_COLUMN_REPORT_RESULT_BACK_CLASS, 110);

    auto tableItemNumber = new QTableWidgetItem(tr("No"));
    auto tableItemName = new QTableWidgetItem(tr("Allergen"));
    auto tableItemCodeName = new QTableWidgetItem(tr("Code"));
    auto tableItemIU = new QTableWidgetItem(tr("IU/ml"));
    auto tableItemClass = new QTableWidgetItem(tr("Class¹"));

    auto tableItemNumber2 = new QTableWidgetItem(tr("No"));
    auto tableItemName2 = new QTableWidgetItem(tr("Allergen"));
    auto tableItemCodeName2 = new QTableWidgetItem(tr("Code"));
    auto tableItemIU2 = new QTableWidgetItem(tr("IU/ml"));
    auto tableItemClass2 = new QTableWidgetItem(tr("Class¹"));

    tableItemNumber->setBackgroundColor(QColor("#D9D9D9"));
    tableItemName->setBackgroundColor(QColor("#D9D9D9"));
    tableItemCodeName->setBackgroundColor(QColor("#D9D9D9"));
    tableItemIU->setBackgroundColor(QColor("#D9D9D9"));
    tableItemClass->setBackgroundColor(QColor("#D9D9D9"));

    tableItemNumber2->setBackgroundColor(QColor("#D9D9D9"));
    tableItemName2->setBackgroundColor(QColor("#D9D9D9"));
    tableItemCodeName2->setBackgroundColor(QColor("#D9D9D9"));
    tableItemIU2->setBackgroundColor(QColor("#D9D9D9"));
    tableItemClass2->setBackgroundColor(QColor("#D9D9D9"));

    ui->tableWidgetFront->insertRow(ui->tableWidgetFront->rowCount());
    ui->tableWidgetFront->setItem(0, TABLE_COLUMN_REPORT_RESULT_FRONT_NUMBER, tableItemNumber);
    ui->tableWidgetFront->setItem(0, TABLE_COLUMN_REPORT_RESULT_FRONT_ALLERGEN, tableItemName);
    ui->tableWidgetFront->setItem(0, TABLE_COLUMN_REPORT_RESULT_FRONT_CODE, tableItemCodeName);
    ui->tableWidgetFront->setItem(0, TABLE_COLUMN_REPORT_RESULT_FRONT_IU, tableItemIU);
    ui->tableWidgetFront->setItem(0, TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS, tableItemClass);

    ui->tableWidgetBack->insertRow(ui->tableWidgetBack->rowCount());
    ui->tableWidgetBack->setItem(0, TABLE_COLUMN_REPORT_RESULT_BACK_NUMBER, tableItemNumber2);
    ui->tableWidgetBack->setItem(0, TABLE_COLUMN_REPORT_RESULT_BACK_ALLERGEN, tableItemName2);
    ui->tableWidgetBack->setItem(0, TABLE_COLUMN_REPORT_RESULT_BACK_CODE, tableItemCodeName2);
    ui->tableWidgetBack->setItem(0, TABLE_COLUMN_REPORT_RESULT_BACK_IU, tableItemIU2);
    ui->tableWidgetBack->setItem(0, TABLE_COLUMN_REPORT_RESULT_BACK_CLASS, tableItemClass2);

    QString logoImageFileName = (QApplication::applicationDirPath() + "/Config/logo.png");
    QString signImageFileName = (QApplication::applicationDirPath() + "/Config/sign.png");

    auto logoImg = QImage(logoImageFileName);
    auto logoImgBuffer = QPixmap::fromImage(logoImg);
    ui->lbLogo->setPixmap(logoImgBuffer);

    auto signImg = QImage(signImageFileName);
    auto signImgBuffer = QPixmap::fromImage(signImg);
    ui->lbSign->setPixmap(signImgBuffer);
}

QPicture ResultPrintWidget::printWindow()
{
    QPicture image;
    QPainter painter(&image);
    this->render(&painter);

    return image;
}

void ResultPrintWidget::setData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
    this->setGeneralData(data);
    this->setRowData(data);
}

QString ResultPrintWidget::UpperToLowerForPanelName(const QString& panelName)
{
    QString orgName = panelName;

    if(orgName.count() <= 1)
        return orgName;

    QString newName;
    auto upperString = orgName.left(1);
    auto lowerString = orgName.right(orgName.count()-1);

    newName.push_back(upperString.toUpper());
    newName.push_back(lowerString.toLower());

    return newName;
}

void ResultPrintWidget::setGeneralData(const GlobalDataStruct::AnalysisReportInfomation& data)
{    
    auto panelName =  data.element.panelName;    
    auto productName = QApplication::instance()->applicationName();
    auto companyName = ConfigInformation::getInstance()->getCompanyName();
    auto dateTime = data.dateTime.toString("yyyy-MM-dd hh:mm");
    auto comment =  data.result.comment;

    ui->labelPatientID->setText(data.element.patientID);
    ui->labelPatientName->setText(data.element.name);
    ui->labelAge->setText(data.element.age);
    ui->labelSex->setText(data.element.sex);
    ui->labelInstitution->setText(companyName);
    ui->labelTestingDate->setText(dateTime);
    ui->labelTestedBy->setText(data.element.analyst);
    ui->labelPanel->setText(panelName);
    ui->editNote->setText(comment);
}

void ResultPrintWidget::setRowData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
#if 0
    auto ConfigInformation = ConfigInformation::getInstance();

    this->loadHighLightData();

    auto currentStatus = ConfigInformation->getStatus();
    auto isDualband = currentStatus.isDualBand;

    if(isDualband)
        this->setDualBandRowData(data);
    else
        this->setSingleRowData(data);
#endif
}

void ResultPrintWidget::setSingleRowData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
    auto ConfigInformation = ConfigInformation::getInstance();
    auto itemNameList = ConfigInformation->getCurrentStripNameList(data.element.panelName);
    auto itemCodeList = ConfigInformation->getCurrentStripCodeList(data.element.panelName);
    auto resultCount = data.result.resultValue.count();
    auto isBandHiddenCodes = ConfigInformation::getInstance()->getIsStripBehindeList(data.element.panelName);
    int hiddenCodeCount = 1;

    if(itemNameList.isEmpty() == true)
        return;

    if(itemCodeList.isEmpty() == true)
        return;

    for(int i = 0; i < resultCount; i++)
    {
        ui->tableWidgetFront->insertRow(ui->tableWidgetFront->rowCount());

        if(isBandHiddenCodes[i] == true)
        {
            hiddenCodeCount ++;
            continue;
        }
        int tableRowIdx = ui->tableWidgetFront->rowCount()-hiddenCodeCount;
        //auto tableRowIdx = i+1;
        auto tableItemNumber = new QTableWidgetItem(QString::number(tableRowIdx));
        auto tableItemName = new QTableWidgetItem(itemNameList[i]);
        auto tableItemCodeName = new QTableWidgetItem(itemCodeList[i]);
        auto tableItemIU = new QTableWidgetItem(data.result.resultValue[i]);
        auto tableItemClass =  new ResultPrintWidgetClassTableItem(ui->tableWidgetFront);

        tableItemClass->setClass(data.result.classType[i]);
        tableItemNumber->setBackgroundColor(QColor("#F2F2F2"));

        if(this->isHighLightColor(data.result.classType[i]))
            tableItemClass->setClassTextColor("#28a0ff");

        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_NUMBER, tableItemNumber);
        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_ALLERGEN, tableItemName);
        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_CODE, tableItemCodeName);
        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_IU, tableItemIU);
        ui->tableWidgetFront->setCellWidget(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS, tableItemClass);
    }
}

void ResultPrintWidget::setDualBandRowData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
#if 0
    auto ConfigInformation = ConfigInformation::getInstance();
    auto itemNameList = ConfigInformation->getCurrentStripNameList(data.element.panelName);
    auto itemCodeList = ConfigInformation->getCurrentStripCodeList(data.element.panelName);
    auto isBandHiddenCodes = ConfigInformation::getInstance()->getIsStripBehindeList(data.element.panelName);
    int frontHiddenCodeCount = 1;
    int backHiddenCodeCount = 1;

    if(itemNameList.isEmpty() == true)
        return;

    if(itemCodeList.isEmpty() == true)
        return;

    auto dualbandBackCount = data.result.resultValue.count();
    auto dualBandFrontCount = ConfigInformation->getDualBandSetting().dualBandFrontBlockCount;

    for(auto i = 0 ; i < dualBandFrontCount; i++)
    {
        if(dualbandBackCount <= dualBandFrontCount)
            break;

        QString itemName;
        QString itemCode;

        if(i < itemNameList.count())
            itemName = itemNameList[i];

        if(i < itemCodeList.count())
            itemCode = itemCodeList[i];

        ui->tableWidgetFront->insertRow(ui->tableWidgetFront->rowCount());

        if(isBandHiddenCodes[i] == true)
        {
            frontHiddenCodeCount ++;
            continue;
        }

        int tableRowIdx = ui->tableWidgetFront->rowCount() - frontHiddenCodeCount;

        auto tableItemNumber = new QTableWidgetItem(QString::number(tableRowIdx));
        auto tableItemName = new QTableWidgetItem(itemName);
        auto tableItemCodeName = new QTableWidgetItem(itemCode);
        auto tableItemIU = new QTableWidgetItem(data.result.resultValue[i]);
        auto tableItemClass =  new ResultPrintWidgetClassTableItem(ui->tableWidgetFront);

        tableItemClass->setClass(data.result.classType[i]);
        tableItemNumber->setBackgroundColor(QColor("#F2F2F2"));

        if(this->isHighLightColor(data.result.classType[i]))
            tableItemClass->setClassTextColor("#28a0ff");

        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_NUMBER, tableItemNumber);
        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_ALLERGEN, tableItemName);
        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_CODE, tableItemCodeName);
        ui->tableWidgetFront->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_IU, tableItemIU);
        ui->tableWidgetFront->setCellWidget(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS, tableItemClass);
    }

    for(auto i = dualBandFrontCount; i < dualbandBackCount; i++)
    {
        QString itemName;
        QString itemCode;

        if(i < itemNameList.count())
            itemName = itemNameList[i];

        if(i < itemCodeList.count())
            itemCode = itemCodeList[i];

        ui->tableWidgetBack->insertRow(ui->tableWidgetBack->rowCount());

        if(isBandHiddenCodes[i] == true)
        {
            backHiddenCodeCount ++;
            continue;
        }

        int tableRowIdx = ui->tableWidgetBack->rowCount() - backHiddenCodeCount;

        auto tableItemNumber = new QTableWidgetItem(QString::number(tableRowIdx));
        auto tableItemName = new QTableWidgetItem(itemName);
        auto tableItemCodeName = new QTableWidgetItem(itemCode);
        auto tableItemIU = new QTableWidgetItem(data.result.resultValue[i]);
        auto tableItemClass =  new ResultPrintWidgetClassTableItem(ui->tableWidgetBack);

        tableItemClass->setClass(data.result.classType[i]);
        tableItemNumber->setBackgroundColor(QColor("#F2F2F2"));

        if(this->isHighLightColor(data.result.classType[i]))
            tableItemClass->setClassTextColor("#28a0ff");

        ui->tableWidgetBack->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_BACK_NUMBER, tableItemNumber);
        ui->tableWidgetBack->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_BACK_ALLERGEN, tableItemName);
        ui->tableWidgetBack->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_BACK_CODE, tableItemCodeName);
        ui->tableWidgetBack->setItem(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_BACK_IU, tableItemIU);
        ui->tableWidgetBack->setCellWidget(tableRowIdx, TABLE_COLUMN_REPORT_RESULT_BACK_CLASS, tableItemClass);
    }
#endif
}

void ResultPrintWidget::loadHighLightData()
{
    GenericIniDataControls genericIO;
    d->mHighlightClassData = genericIO.readClassHightLightData();
}

bool ResultPrintWidget::isHighLightColor(const QString &classType)
{
    if(classType == "6")
        if(d->mHighlightClassData.class6)
            return true;

    if(classType == "5")
        if(d->mHighlightClassData.class5)
            return true;

    if(classType == "4")
        if(d->mHighlightClassData.class4)
            return true;

    if(classType == "3")
        if(d->mHighlightClassData.class3)
            return true;

    if(classType == "2")
        if(d->mHighlightClassData.class2)
            return true;

    if(classType == "1")
        if(d->mHighlightClassData.class1)
            return true;

    if(classType == "0")
        if(d->mHighlightClassData.class0)
            return true;

    return false;
}
