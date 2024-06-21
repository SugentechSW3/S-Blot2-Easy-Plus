#include <QPrinter>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QPainter>

#include "ResultPrintV5Widget.h"
#include "ui_ResultPrintV5Widget.h"

#include "Common/Io/INIFileControls/GenericIniDataControls.h"

#include "Analysis/ConfigInformation.h"
#include "UIClass/DialogWidget/InspectionOpinionDlg.h"
#include "UIClass/ResultView/SubWidget/ResultPrintWidgetClassTableItem.h"
#include "UIClass/ResultView/SubWidget/ResultPrintV5WidgetLabelTableItem.h"

struct ResultPrintV5Widget::privateStruct
{
    GlobalDataStruct::HIGH_LIGHT_DATA mHighlightClassData;
};

ResultPrintV5Widget::ResultPrintV5Widget(QWidget *parent) :
                                                            QWidget(parent),
                                                            ui(new Ui_ResultPrintV5Widget),
                                                            d(new privateStruct)
{
    ui->setupUi(this);

    this->createPage1();
    this->createPage2and3();
}

ResultPrintV5Widget::~ResultPrintV5Widget()
{
}

void ResultPrintV5Widget::setData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
    this->setGeneralData(data);
    this->setRowData(data);
}

QString ResultPrintV5Widget::UpperToLowerForPanelName(const QString& panelName)
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

void ResultPrintV5Widget::setGeneralData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
    auto panelName =  data.element.panelName;
    auto dateTime = data.dateTime.toString("yyyy-MM-dd hh:mm");
    auto comment =  data.result.comment;

    ui->labelPatientID->setText(data.element.patientID);
    ui->labelPatientName->setText(data.element.name);
    ui->labelAge->setText(data.element.age);
    ui->labelSex->setText(data.element.sex);
    ui->labelApplicant->setText(data.element.applicant);
    ui->labelTestingDate->setText(dateTime);
    ui->labelTestedBy->setText(data.element.analyst);
    ui->labelPage1Panel->setText(panelName);
    ui->labelPage2Panel->setText(panelName);
    ui->textEditNote->setText(comment);

    ui->labelPatientID2->setText(data.element.patientID);
    ui->labelPatientName2->setText(data.element.name);
}

void ResultPrintV5Widget::setRowData(const GlobalDataStruct::AnalysisReportInfomation& data)
{
    QFont fnt;
    fnt.setPointSize(4);
    fnt.setFamily("Arial");

    auto config = ConfigInformation::getInstance();

    QString summaryTige;
    QString summaryCCD;
    QVector<QStringList> classSummaryItems(6);

    auto itemNameList = config->getCurrentStripNameList(data.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(data.element.panelName);
    auto itemGroupList = config->getCurrentStripGroupList(data.element.panelName);
    auto itemSecondNameList = config->getCurrentStripSecondNameList(data.element.panelName);

    auto bandCount = data.result.resultValue.count();

    auto hiddenBandList = config->getHiddenBandList(data.element.panelName);

    int itemNo = 1;
    bool isFront = true;
    bool isFirstPage = true;
    bool writeSecondBand = (config->getExportChangeLanguage() != config->getExportChangeSecondLanguage());

    auto selectedBandListTable = ui->tableBandList1Page2;

    for(int i = 0; i < bandCount; i++)
    {
        QString itemName;
        QString itemCode;
        QString itemGroup;
        QString itemSecondName;

        if(i < itemNameList.count())
            itemName = itemNameList[i];

        if(i < itemCodeList.count())
            itemCode = itemCodeList[i];

        if(i < itemGroupList.count())
            itemGroup = itemGroupList[i];

        if(i < itemSecondNameList.count())
            itemSecondName = itemSecondNameList[i];

        if(hiddenBandList.contains(itemCodeList[i]))
            continue;

        if(itemCode.compare("PM", Qt::CaseInsensitive) == 0 || itemCode.compare("-", Qt::CaseInsensitive) == 0)
            continue;

        if(itemCode.compare("tIgE", Qt::CaseInsensitive) == 0)
            summaryTige = (data.result.classType[i] == "P") ? "Positive" : "Normal";

        if(itemCode.compare("o214", Qt::CaseInsensitive) == 0)  // CCD
            summaryCCD = QString("Class %1").arg(data.result.classType[i]);

        if(i < data.result.classType.count())
        {
            int classType = data.result.classType[i].toInt();
            if(classType > 0 && classType <= 6)
            {
                classSummaryItems[classType - 1].append(itemNameList[i]);
            }
        }

        auto currentRow = selectedBandListTable->rowCount();
        auto tableRowIdx = currentRow;

        auto tableItemNo = new ResultPrintV5WidgetLabelTableItem(QString::number(itemNo));
        auto tableitemGroup = new QTableWidgetItem(itemGroup);

        if(writeSecondBand) itemName = itemName + "\n" + itemSecondName;
        auto tableItemName = new QTableWidgetItem(itemName);
        auto tableItemCode = new ResultPrintV5WidgetLabelTableItem(itemCode);
        auto tableItemIU = new QTableWidgetItem(data.result.resultValue[i]);
        auto tableItemClass = new ResultPrintV5WidgetLabelTableItem(data.result.classType[i]);
        auto tableItemClassBar = new ResultPrintWidgetClassTableItem(selectedBandListTable);
        tableItemClassBar->setClassReportV5(data.result.classType[i]);

        selectedBandListTable->insertRow(tableRowIdx);
        selectedBandListTable->setCellWidget(tableRowIdx, TABLE_COLUMN_BANDLIST_NO, tableItemNo);
        selectedBandListTable->setItem(tableRowIdx, TABLE_COLUMN_BANDLIST_GROUP, tableitemGroup);
        selectedBandListTable->setItem(tableRowIdx, TABLE_COLUMN_BANDLIST_ALLERGEN, tableItemName);
        selectedBandListTable->setCellWidget(tableRowIdx, TABLE_COLUMN_BANDLIST_CODE, tableItemCode);
        selectedBandListTable->setItem(tableRowIdx, TABLE_COLUMN_BANDLIST_IUML, tableItemIU);
        selectedBandListTable->setCellWidget(tableRowIdx, TABLE_COLUMN_BANDLIST_CLASS, tableItemClass);
        selectedBandListTable->setCellWidget(tableRowIdx, TABLE_COLUMN_BANDLIST_CLASS_BAR, tableItemClassBar);

        selectedBandListTable->item(tableRowIdx, TABLE_COLUMN_BANDLIST_IUML)->setTextAlignment(Qt::AlignCenter);

        if(itemCode.compare("PC", Qt::CaseInsensitive) == 0)
        {
            selectedBandListTable->setCellWidget(tableRowIdx, TABLE_COLUMN_BANDLIST_NO, new ResultPrintV5WidgetLabelTableItem("-"));
        }
        else
        {
            itemNo++;
        }

        switch(itemNo)
        {
            case 31:
                isFront = false;
                selectedBandListTable = ui->tableBandList2Page2;
            break;
            case 61:
                isFront = true;
                isFirstPage = false;
                selectedBandListTable = ui->tableBandList1Page3;
            break;
            case 91:
                isFront = false;
                selectedBandListTable = ui->tableBandList2Page3;
            break;
        }
    }

    this->writeClassMemoV5(summaryTige, summaryCCD, classSummaryItems);
    this->mergeAllergyGroupV5(bandCount);
}

void ResultPrintV5Widget::mergeAllergyGroupV5(int bandCount)
{
    int mergeCount = 0;
    bool isFront = true;
    bool isFirstPage = true;
    int startRow = 0;
    int endRow = 0;

    QString readCell;
    QString nextCell;
    auto selectedBandListTable = ui->tableBandList1Page2;

    for(int i = 0; i < bandCount; i++)
    {
        endRow = startRow + ++mergeCount;

        readCell = selectedBandListTable->item(startRow, TABLE_COLUMN_BANDLIST_GROUP)->text();
        nextCell =
        (endRow < selectedBandListTable->rowCount()) ?
        selectedBandListTable->item(endRow, TABLE_COLUMN_BANDLIST_GROUP)->text() : "";

        if(readCell != nextCell)
        {
            if(mergeCount != 1) selectedBandListTable->setSpan(startRow, 1, mergeCount, 1);
            selectedBandListTable->item(startRow, TABLE_COLUMN_BANDLIST_GROUP)->setTextAlignment(Qt::AlignCenter);
            startRow = endRow;
            mergeCount = 0;
        }

        if(!isFirstPage && !isFront && nextCell.isEmpty())  break;

        if(startRow >= selectedBandListTable->rowCount())
        {
            startRow = 0;

            if(isFirstPage)
            {
                if(isFront)
                {
                    isFront = false;
                    selectedBandListTable = ui->tableBandList2Page2;
                }
                else
                {
                    isFront = true;
                    isFirstPage = false;
                    selectedBandListTable = ui->tableBandList1Page3;
                }
            }
            else
            {
                selectedBandListTable = ui->tableBandList2Page3;
            }
        }
    }
}

void ResultPrintV5Widget::writeClassMemoV5(QString tigeNote, QString ccdNote, QVector<QStringList>& classNoteItems)
{
    ui->tableMemo->setRowCount(0);

    int tableRowIdx = 0;

    ui->tableMemo->insertRow(tableRowIdx);
    ui->tableMemo->setRowHeight(tableRowIdx, 12);
    ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_VHEADER, new QTableWidgetItem("Total IgE : "));
    ui->tableMemo->item(tableRowIdx, TABLE_COLUMN_MEMO_VHEADER)->setTextAlignment(Qt::AlignRight);
    ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_VALUE, new QTableWidgetItem(tigeNote));
    ui->tableMemo->item(tableRowIdx, TABLE_COLUMN_MEMO_VALUE)->setTextAlignment(Qt::AlignCenter);
    tableRowIdx++;

    ui->tableMemo->insertRow(tableRowIdx);
    ui->tableMemo->setRowHeight(tableRowIdx, 12);
    ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_VHEADER, new QTableWidgetItem("CCD : "));
    ui->tableMemo->item(tableRowIdx, TABLE_COLUMN_MEMO_VHEADER)->setTextAlignment(Qt::AlignRight);
    ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_VALUE, new QTableWidgetItem(ccdNote));
    ui->tableMemo->item(tableRowIdx, TABLE_COLUMN_MEMO_VALUE)->setTextAlignment(Qt::AlignCenter);
    tableRowIdx++;

    ui->tableMemo->insertRow(tableRowIdx);
    ui->tableMemo->setRowHeight(tableRowIdx, 12);
    ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_VHEADER, new QTableWidgetItem("Specific IgE : "));
    ui->tableMemo->item(tableRowIdx, TABLE_COLUMN_MEMO_VHEADER)->setTextAlignment(Qt::AlignRight);


    for(int i=classNoteItems.count()-1 ; i>=0 ; i--)
    {
        if(classNoteItems[i].count() > 0)
        {
            QStringList allergyNote = getAllergyMemoNewLine(classNoteItems[i]);
            for(int j=0 ; j<allergyNote.count(); j++)
            {
                if(j == 0)
                {
                    ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_VALUE, new QTableWidgetItem(QString("Class %1 : ").arg(i + 1)));
                    ui->tableMemo->item(tableRowIdx, TABLE_COLUMN_MEMO_VALUE)->setTextAlignment(Qt::AlignCenter);
                }
                ui->tableMemo->setItem(tableRowIdx, TABLE_COLUMN_MEMO_ALLERGEN, new QTableWidgetItem(allergyNote[j]));
                tableRowIdx++;
                ui->tableMemo->insertRow(tableRowIdx);
                ui->tableMemo->setRowHeight(tableRowIdx, 12);
            }
        }
    }
}

QStringList ResultPrintV5Widget::getAllergyMemoNewLine(QStringList classNoteItems)
{
    QStringList allergyLine;

    QString tempLine = "";
    QString item;
    for(int i=0 ; i<classNoteItems.count() ; i++)
    {
        tempLine += classNoteItems[i];
        if(tempLine.length() > 80)
        {
            allergyLine.append(tempLine);
            tempLine = "";
        }
        else
        {
            tempLine += ", ";
        }
    }

    if(tempLine.right(2) == ", ")   tempLine.chop(2);
    if(tempLine != "")              allergyLine.append(tempLine);

    return allergyLine;
}


void ResultPrintV5Widget::createPage1()
{
    ui->labelPage1Title->setText("     Allergy-specific Human IgE Test Report");
    ui->labelPage1SubTitle->setText("     SGTi-Allergy Screen Test");
    ui->labelRemark->setText("     Remark");
    ui->labelSign->setText("  Assistant/Physician");

    auto logoImg = QImage("Config/logo_print.png");         // 700 * 75
    auto logo1Img = QImage("Config/logo1_print.png");       // 700 * 75
    auto signImg = QImage("Config/sign_print.png");         // 250 * 125

    auto imgLogoBuffer = QPixmap::fromImage(logoImg);
    auto imgLogo1Buffer = QPixmap::fromImage(logo1Img);
    auto imgSignBuffer = QPixmap::fromImage(signImg);

    ui->labelPage1LogoImage->setPixmap(imgLogoBuffer);
    ui->labelPage1Logo1Image->setPixmap(imgLogo1Buffer);
    ui->labelPage1SignImage->setPixmap(imgSignBuffer);

    initMemoTable();
}

void ResultPrintV5Widget::initMemoTable()
{
    ui->tableMemo->horizontalHeader()->resizeSection(TABLE_COLUMN_MEMO_VHEADER, 60);
    ui->tableMemo->horizontalHeader()->resizeSection(TABLE_COLUMN_MEMO_VALUE, 40);
    ui->tableMemo->horizontalHeader()->resizeSection(TABLE_COLUMN_MEMO_ALLERGEN, 560);
}

void ResultPrintV5Widget::createPage2and3()
{
    ui->labelPage2SubTitle->setText("     SGTi-Allergy Screen PLUS");
    ui->labelPage3SubTitle->setText("     SGTi-Allergy Screen PLUS");
    ui->labelSpecific->setText("  Class of specific IgE1");
    ui->labelTotal->setText("  Total IgE");

    auto logoImg = QImage("Config/logo_print.png");         // 700 * 75
    auto logo1Img = QImage("Config/logo1_print.png");       // 250 * 125

    auto imgLogoBuffer = QPixmap::fromImage(logoImg);
    auto imgLogo1Buffer = QPixmap::fromImage(logo1Img);

    ui->labelPage2LogoImage->setPixmap(imgLogoBuffer);
    ui->labelPage2Logo1Image->setPixmap(imgLogo1Buffer);

    ui->labelPage3LogoImage->setPixmap(imgLogoBuffer);
    ui->labelPage3Logo1Image->setPixmap(imgLogo1Buffer);

    initPage2TableHeader();
    initPage2Tablebody();

    initPage3TableHeader();
    initPage3Tablebody();

    initETCTable();

    auto date = QDate::currentDate();
    ui->labelDatePage1->setText("Printed " + date.toString("yyyy-MM-dd"));
    ui->labelDatePage2->setText("Printed " + date.toString("yyyy-MM-dd"));
    ui->labelDatePage3->setText("Printed " + date.toString("yyyy-MM-dd"));
}

QVector<QPicture> ResultPrintV5Widget::printWindow()
{
    QVector<QPicture> imageItems;

    QPicture image1, image2, image3;
    QPainter painter1(&image1);
    ui->widgetPage1->render(&painter1);

    QPainter painter2(&image2);
    ui->widgetPage2->render(&painter2);

    QPainter painter3(&image3);
    ui->widgetPage3->render(&painter3);

    imageItems.push_back(image1);
    imageItems.push_back(image2);
    imageItems.push_back(image3);

    return imageItems;
}

void ResultPrintV5Widget::initPage2Tablebody()
{
    ui->tableBandList1Page2->resizeRowsToContents();
    auto header = ui->tableBandList1Page2->horizontalHeader();

    for(int col=TABLE_COLUMN_BANDLIST_HEADER_NO ; col<=TABLE_COLUMN_BANDLIST_HEADER_CLASS1 ; col++)
    {
        header->resizeSection(col, ui->tableBandList1HeaderPage2->horizontalHeader()->sectionSize(col));
        if(col == TABLE_COLUMN_BANDLIST_HEADER_CLASS1)
            header->resizeSection(col, ui->tableBandList1HeaderPage2->horizontalHeader()->sectionSize(col) * 6);
    }

    ui->tableBandList2Page2->setHorizontalHeader(ui->tableBandList1Page2->horizontalHeader());
}

void ResultPrintV5Widget::initPage3Tablebody()
{
    ui->tableBandList1Page3->resizeRowsToContents();
    auto header = ui->tableBandList1Page3->horizontalHeader();

    for(int col=TABLE_COLUMN_BANDLIST_HEADER_NO ; col<=TABLE_COLUMN_BANDLIST_HEADER_CLASS1 ; col++)
    {
        header->resizeSection(col, ui->tableBandList1HeaderPage3->horizontalHeader()->sectionSize(col));
        if(col == TABLE_COLUMN_BANDLIST_HEADER_CLASS1)
            header->resizeSection(col, ui->tableBandList1HeaderPage3->horizontalHeader()->sectionSize(col) * 6);
    }

    ui->tableBandList2Page3->setHorizontalHeader(ui->tableBandList1Page3->horizontalHeader());
}

void ResultPrintV5Widget::initPage2TableHeader()
{
    ui->tableBandList1HeaderPage2->setRowCount(1);
    ui->tableBandList1HeaderPage2->setRowHeight(0, 16);

    auto header = ui->tableBandList1HeaderPage2->horizontalHeader();
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_NO, 16);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_GROUP, 44);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_ALLERGEN, 113);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CODE, 29);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_IUML, 29);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS, 29);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS1, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS2, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS3, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS4, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS5, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS6, 14);

    auto tableItemNumber = new QTableWidgetItem("No");
    auto tableItemGroup = new QTableWidgetItem("");
    auto tableItemName = new QTableWidgetItem("Allergen");
    auto tableItemCodeName = new QTableWidgetItem("Code");
    auto tableItemIU = new QTableWidgetItem("IU/mL");
    auto tableItemClass = new QTableWidgetItem("Class¹");
    auto tableItemClass1 = new QTableWidgetItem("1");
    auto tableItemClass2 = new QTableWidgetItem("2");
    auto tableItemClass3 = new QTableWidgetItem("3");
    auto tableItemClass4 = new QTableWidgetItem("4");
    auto tableItemClass5 = new QTableWidgetItem("5");
    auto tableItemClass6 = new QTableWidgetItem("6");

    tableItemNumber->setBackgroundColor(QColor("#D9D9D9"));
    tableItemGroup->setBackgroundColor(QColor("#D9D9D9"));
    tableItemName->setBackgroundColor(QColor("#D9D9D9"));
    tableItemCodeName->setBackgroundColor(QColor("#D9D9D9"));
    tableItemIU->setBackgroundColor(QColor("#D9D9D9"));
    tableItemClass->setBackgroundColor(QColor("#D9D9D9"));
    tableItemClass1->setBackgroundColor(QColor("#DDEBF7"));
    tableItemClass2->setBackgroundColor(QColor("#BDD7EE"));
    tableItemClass3->setBackgroundColor(QColor("#9BC2E6"));
    tableItemClass4->setBackgroundColor(QColor("#5B9BD5"));
    tableItemClass5->setBackgroundColor(QColor("#2F75B5"));
    tableItemClass6->setBackgroundColor(QColor("#1F4E78"));

    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_NO, tableItemNumber);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_GROUP, tableItemGroup);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_ALLERGEN, tableItemName);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CODE, tableItemCodeName);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_IUML, tableItemIU);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS, tableItemClass);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS1, tableItemClass1);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS2, tableItemClass2);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS3, tableItemClass3);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS4, tableItemClass4);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS5, tableItemClass5);
    ui->tableBandList1HeaderPage2->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS6, tableItemClass6);

    for(int col= TABLE_COLUMN_BANDLIST_HEADER_NO ; col<= TABLE_COLUMN_BANDLIST_HEADER_CLASS6 ; col++)
        ui->tableBandList1HeaderPage2->item(0, col)->setTextAlignment(Qt::AlignCenter);


    ui->tableBandList2HeaderPage2->setRowCount(1);
    ui->tableBandList2HeaderPage2->setRowHeight(0, 16);

    ui->tableBandList2HeaderPage2->setHorizontalHeader(ui->tableBandList1HeaderPage2->horizontalHeader());

    for(int col= TABLE_COLUMN_BANDLIST_HEADER_NO ; col<= TABLE_COLUMN_BANDLIST_HEADER_CLASS6 ; col++)
        ui->tableBandList2HeaderPage2->setItem(0, col, ui->tableBandList1HeaderPage2->item(0, col)->clone());
}

void ResultPrintV5Widget::initPage3TableHeader()
{
    ui->tableBandList1HeaderPage3->setRowCount(1);
    ui->tableBandList1HeaderPage3->setRowHeight(0, 16);

    auto header = ui->tableBandList1HeaderPage3->horizontalHeader();
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_NO, 16);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_GROUP, 44);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_ALLERGEN, 113);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CODE, 29);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_IUML, 29);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS, 29);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS1, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS2, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS3, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS4, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS5, 14);
    header->resizeSection(TABLE_COLUMN_BANDLIST_HEADER_CLASS6, 14);


    auto tableItemNumber = new QTableWidgetItem("No");
    auto tableItemGroup = new QTableWidgetItem("");
    auto tableItemName = new QTableWidgetItem("Allergen");
    auto tableItemCodeName = new QTableWidgetItem("Code");
    auto tableItemIU = new QTableWidgetItem("IU/mL");
    auto tableItemClass = new QTableWidgetItem("Class¹");
    auto tableItemClass1 = new QTableWidgetItem("1");
    auto tableItemClass2 = new QTableWidgetItem("2");
    auto tableItemClass3 = new QTableWidgetItem("3");
    auto tableItemClass4 = new QTableWidgetItem("4");
    auto tableItemClass5 = new QTableWidgetItem("5");
    auto tableItemClass6 = new QTableWidgetItem("6");

    tableItemNumber->setBackgroundColor(QColor("#D9D9D9"));
    tableItemGroup->setBackgroundColor(QColor("#D9D9D9"));
    tableItemName->setBackgroundColor(QColor("#D9D9D9"));
    tableItemCodeName->setBackgroundColor(QColor("#D9D9D9"));
    tableItemIU->setBackgroundColor(QColor("#D9D9D9"));
    tableItemClass->setBackgroundColor(QColor("#D9D9D9"));
    tableItemClass1->setBackgroundColor(QColor("#DDEBF7"));
    tableItemClass2->setBackgroundColor(QColor("#BDD7EE"));
    tableItemClass3->setBackgroundColor(QColor("#9BC2E6"));
    tableItemClass4->setBackgroundColor(QColor("#5B9BD5"));
    tableItemClass5->setBackgroundColor(QColor("#2F75B5"));
    tableItemClass6->setBackgroundColor(QColor("#1F4E78"));

    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_NO, tableItemNumber);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_GROUP, tableItemGroup);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_ALLERGEN, tableItemName);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CODE, tableItemCodeName);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_IUML, tableItemIU);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS, tableItemClass);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS1, tableItemClass1);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS2, tableItemClass2);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS3, tableItemClass3);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS4, tableItemClass4);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS5, tableItemClass5);
    ui->tableBandList1HeaderPage3->setItem(0, TABLE_COLUMN_BANDLIST_HEADER_CLASS6, tableItemClass6);

    for(int col= TABLE_COLUMN_BANDLIST_HEADER_NO ; col<= TABLE_COLUMN_BANDLIST_HEADER_CLASS6 ; col++)
        ui->tableBandList1HeaderPage3->item(0, col)->setTextAlignment(Qt::AlignCenter);

    ui->tableBandList2HeaderPage3->setRowCount(1);
    ui->tableBandList2HeaderPage3->setRowHeight(0, 16);

    ui->tableBandList2HeaderPage3->setHorizontalHeader(ui->tableBandList1HeaderPage3->horizontalHeader());

    for(int col= TABLE_COLUMN_BANDLIST_HEADER_NO ; col<= TABLE_COLUMN_BANDLIST_HEADER_CLASS6 ; col++)
        ui->tableBandList2HeaderPage3->setItem(0, col, ui->tableBandList1HeaderPage3->item(0, col)->clone());
}

void ResultPrintV5Widget::initETCTable()
{
    ui->tableSpecific->horizontalHeader()->resizeSection(TABLE_COLUMN_SPECIFIC_VHEADER, 44);
    ui->tableSpecific->horizontalHeader()->resizeSection(TABLE_COLUMN_SPECIFIC_CLASS4, 46);
    ui->tableSpecific->item(0, TABLE_COLUMN_SPECIFIC_VHEADER)->setText("IU/mL");
    ui->tableSpecific->item(1, TABLE_COLUMN_SPECIFIC_VHEADER)->setText("Class");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_VHEADER)->setText("Level");

    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS0)->setText("Not found");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS1)->setText("Weak");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS2)->setText("Moderate");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS3)->setText("Moderately strong");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS4)->setText("Strong");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS5)->setText("Very strong");
    ui->tableSpecific->item(2, TABLE_COLUMN_SPECIFIC_CLASS6)->setText("Extremely strong");

    ui->tableSpecific->setRowHeight(0, 12);
    ui->tableSpecific->setRowHeight(1, 12);
    ui->tableSpecific->setRowHeight(2, 24);

    ui->tableTotalige->horizontalHeader()->resizeSection(TABLE_COLUMN_TOTAL_VHEADER, 44);
    ui->tableTotalige->setColumnWidth(TABLE_COLUMN_TOTAL_NORMAL, 84);
    ui->tableTotalige->setColumnWidth(TABLE_COLUMN_TOTAL_POSITIVE, 84);
    ui->tableTotalige->item(0, TABLE_COLUMN_TOTAL_VHEADER)->setText("Class");
    ui->tableTotalige->item(1, TABLE_COLUMN_TOTAL_VHEADER)->setText("Level");

    ui->tableTotalige->item(1, TABLE_COLUMN_TOTAL_NORMAL)->setText("Normal");
    ui->tableTotalige->item(1, TABLE_COLUMN_TOTAL_POSITIVE)->setText("Positive");

    ui->tableTotalige->setRowHeight(0, 12);
    ui->tableTotalige->setRowHeight(1, 12);
}
