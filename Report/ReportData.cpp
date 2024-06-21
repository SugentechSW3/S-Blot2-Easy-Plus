#include "ReportData.h"
#include <QApplication>
#include <windows.h>

#include <Common/CLogWriter.h>
#include <Analysis/ConfigInformation.h>
#include <QPrintDialog>
#include <QPrinter>
#include <QDate>

using namespace sugentech;

struct ReportData::privateStruct
{
    QString mDefaultFileNamePath = "Allergy_PLUS_Test_Report.xlsx";

    QString mDefaultDirPath = "DocumentFormat";
    QString mFormFileFullPath;
    Book* mXlsx;
    Sheet* mXlsxSheet;
};

ReportData::ReportData(QObject *parent)
    :d(new privateStruct)
{
    d->mFormFileFullPath = QApplication::applicationDirPath() + "/" + d->mDefaultDirPath + "/" + d->mDefaultFileNamePath;
}

ReportData::~ReportData()
{

}

bool ReportData::openExcelFile()
{
    d->mXlsx = xlCreateXMLBook();
    d->mXlsx->setKey(L"Dae-gil Song",L"windows-202a2e0305c6e0046bbb6967a1t7n1o4e62e58f9");
    d->mXlsx->setRgbMode(true);

    if(!d->mXlsx->load(d->mFormFileFullPath.toStdWString().c_str()))
        return false;

    return true;
}

bool ReportData::closeExcelFile(const QString &saveFilePath)
{
    if(d->mXlsx == nullptr)
        return false;
    if(saveFilePath.isEmpty())
        return false;

    QString error(d->mXlsx->errorMessage());

    if(error != "ok")
        CLogWriter::printLog(QString("[ReportData] Report Error : %1").arg(error));

    d->mXlsx->save(saveFilePath.toStdWString().c_str());
    d->mXlsx->release();

    return true;
}

QString ReportData::getDirPath()
{
    return QApplication::applicationDirPath() + "/" + d->mDefaultDirPath;
}

void ReportData::writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation> &result)
{
    auto xlsx = xlCreateXMLBook();
    auto path = QApplication::applicationDirPath() + "/" + "DocumentFormat"+ "/" + QString("test-fulldata-%1.xlsx").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm"));
    xlsx->setRgbMode(true);
    xlsx->setKey(QString("Dae-gil Song").toStdWString().c_str(), QString("windows-202a2e0305c6e0046bbb6967a1t7n1o4e62e58f9").toStdWString().c_str());

    int columnGap = 0;
    auto sheet = xlsx->addSheet(QString("TestData").toStdWString().c_str());

    if(sheet == nullptr)
        return;

    for(int column = 0 ; column < result.count(); column++)
    {
        auto itr = result[column];
        auto config = ConfigInformation::getInstance();
        auto itemNameList = config->getCurrentStripNameList(itr.element.panelName);

        if(itemNameList.isEmpty() == true)
            continue;

        sheet->writeStr(1, (column*5) + columnGap + 1, QString("UserID : %1").arg(itr.element.patientID).toStdWString().c_str());
        sheet->writeStr(2, (column*5) + columnGap + 1, QString("Name").toStdWString().c_str());
        sheet->writeStr(2, (column*5) + columnGap + 2, QString("IU/ml").toStdWString().c_str());
        sheet->writeStr(2, (column*5) + columnGap + 3, QString("RawIntensity").toStdWString().c_str());
        sheet->writeStr(2, (column*5) + columnGap + 4, QString("ClassType").toStdWString().c_str());

        //excel은 0이 아닌 1부터시작.
        for(int row = 0; row < itr.result.resultValue.count(); row++)
        {
            if(row < itemNameList.count())
                sheet->writeStr(row+3, (column*5) + columnGap + 1, itemNameList[row].toStdWString().c_str());

            sheet->writeStr(row+3, (column*5) + columnGap + 2, itr.result.resultValue[row].toStdWString().c_str());
            sheet->writeNum(row+3, (column*5) + columnGap + 3, itr.result.rawIntensityValue[row]);
            sheet->writeStr(row+3, (column*5) + columnGap + 4, itr.result.classType[row].toStdWString().c_str());
        }

        columnGap++;
    }

    xlsx->save(path.toStdWString().c_str());
    xlsx->release();
}

void ReportData::writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation> &result, bool useRawData)
{
    QString mode("intensity");

    if(useRawData)
        mode = "rawdata";

    auto xlsx = xlCreateXMLBook();
    auto path = QApplication::applicationDirPath() + "/" + "DocumentFormat"+ "/" + QString("test-%1-%2.xlsx").arg(mode).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm"));
    xlsx->setRgbMode(true);
    xlsx->setKey(QString("Dae-gil Song").toStdWString().c_str(), QString("windows-202a2e0305c6e0046bbb6967a1t7n1o4e62e58f9").toStdWString().c_str());

    int columnGap = 0;
    auto sheet = xlsx->addSheet(QString("TestData").toStdWString().c_str());

    if(sheet == nullptr)
        return;

    if(useRawData)
    {
        for(int column = 0 ; column < result.count(); column++)
        {
            auto itr = result[column];
            auto itemNameList = ConfigInformation::getInstance()->getCurrentStripNameList(itr.element.panelName);

            if(itemNameList.isEmpty() == true)
                continue;

            sheet->writeStr(1, (column*3) + columnGap + 1, QString("UserID : %1").arg(itr.element.patientID).toStdWString().c_str());
            sheet->writeStr(2, (column*3) + columnGap + 1, QString("Name").toStdWString().c_str());
            sheet->writeStr(2, (column*3) + columnGap + 2, QString("RawIntensity").toStdWString().c_str());
            sheet->writeStr(2, (column*3) + columnGap + 3, QString("Intensity").toStdWString().c_str());

            for(int row = 0; row < itr.result.resultValue.count(); row++)
            {

                if(row < itemNameList.count())
                    sheet->writeStr(row+3, (column*3) + columnGap + 1, itemNameList[row].toStdWString().c_str());

                sheet->writeNum(row+3, (column*3) + columnGap + 2, itr.result.rawIntensityValue[row]);
                sheet->writeStr(row+3, (column*3) + columnGap + 3, itr.result.resultValue[row].toStdWString().c_str());
            }

            columnGap++;
        }
    }
    else
    {
        for(int column = 0 ; column < result.count(); column++)
        {
            auto resultElement = result[column];
            auto itemNameList = ConfigInformation::getInstance()->getCurrentStripNameList(resultElement.element.panelName);

            if(itemNameList.isEmpty() == true)
                continue;

            sheet->writeStr(1, (column*3) + columnGap + 1, QString("StripNumber : %1").arg(column+1).toStdWString().c_str());
            sheet->writeStr(2, (column*3) + columnGap + 1, QString("Name").toStdWString().c_str());
            sheet->writeStr(2, (column*3) + columnGap + 2, QString("IU/ml").toStdWString().c_str());
            sheet->writeStr(2, (column*3) + columnGap + 3, QString("ClassType").toStdWString().c_str());

            for(int row = 0; row < resultElement.result.resultValue.count(); row++)
            {
                if(row < itemNameList.count())
                    sheet->writeStr(row+3, (column*3) + columnGap + 1, itemNameList[row].toStdWString().c_str());

                sheet->writeStr(row+3, (column * 3) + columnGap + 2, resultElement.result.resultValue[row].toStdWString().c_str());
                sheet->writeStr(row+3, (column * 3) + columnGap + 3, resultElement.result.classType[row].toStdWString().c_str());
            }

            columnGap++;
        }
    }

    xlsx->save(path.toStdWString().c_str());
    xlsx->release();
}

void ReportData::writeQStringToXlsx(const int &row, const int &col, const QString &str)
{
    d->mXlsxSheet->writeStr(row, col, QT_TR_NOOP(str.toStdWString().c_str()));
}

void ReportData::writeExcelDataV5(const GlobalDataStruct::AnalysisReportInfomation &result)
{
    if(d->mXlsx == nullptr)
        return ;

    auto config = ConfigInformation::getInstance();

    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(result.element.panelName);

    if(itemNameList.isEmpty() == true)
        return;

    if(itemCodeList.isEmpty() == true)
        return;

    d->mXlsxSheet = d->mXlsx->getSheet(0);

    d->mXlsxSheet->writeStr(12, getColumnNo("E") - 1, result.element.patientID.toStdWString().c_str());
    d->mXlsxSheet->writeStr(13, getColumnNo("E") - 1, result.element.name.toStdWString().c_str());
    d->mXlsxSheet->writeStr(14, getColumnNo("E") - 1, result.element.age.toStdWString().c_str());
    d->mXlsxSheet->writeStr(15, getColumnNo("E") - 1, result.element.sex.toStdWString().c_str());

    d->mXlsxSheet->writeStr(12, getColumnNo("T") - 1, result.element.applicant.toStdWString().c_str());
    d->mXlsxSheet->writeStr(13, getColumnNo("T") - 1, result.dateTime.toString("yyyy-MM-dd hh:mm").toStdWString().c_str());
    d->mXlsxSheet->writeStr(14, getColumnNo("T") - 1, result.element.analyst.toStdWString().c_str());
    d->mXlsxSheet->writeStr(15, getColumnNo("T") - 1, result.element.reviewer.toStdWString().c_str());

    d->mXlsxSheet->writeStr(19, getColumnNo("H") - 1, result.element.panelName.toStdWString().c_str());
    d->mXlsxSheet->writeStr(11, getColumnNo("AL") - 1, result.element.panelName.toStdWString().c_str());
    d->mXlsxSheet->writeStr(11, getColumnNo("BP") - 1, result.element.panelName.toStdWString().c_str());

    d->mXlsxSheet->writeStr(58, getColumnNo("B") - 1, result.result.comment.toStdWString().c_str());

    auto date = QDate::currentDate();
    d->mXlsxSheet->writeStr(86, getColumnNo("B") - 1, QString("Printed %1").arg(date.toString("yyyy-MM-dd")).toStdWString().c_str());
    d->mXlsxSheet->writeStr(86, getColumnNo("AF") - 1,  + QString("Printed %1").arg(date.toString("yyyy-MM-dd")).toStdWString().c_str());
    d->mXlsxSheet->writeStr(86, getColumnNo("BJ") - 1,  + QString("Printed %1").arg(date.toString("yyyy-MM-dd")).toStdWString().c_str());

    int logoImageIndex = d->mXlsx->addPicture(QString("Config/logo.png").toStdWString().c_str());           // 1500 * 150
    int logo1ImageIndex = d->mXlsx->addPicture(QString("Config/logo1.png").toStdWString().c_str());         // 1500 * 150
    int signImageIndex = d->mXlsx->addPicture(QString("Config/sign.png").toStdWString().c_str());           // 500 * 250

    d->mXlsxSheet->setPicture2(0, getColumnNo("B") - 1, logoImageIndex, 1500, 150, 25, 22, POSITION_ONLY_MOVE);         // PNG 32bit
    d->mXlsxSheet->setPicture2(77, getColumnNo("B") - 1, logo1ImageIndex, 1500, 150, 25, 22, POSITION_ONLY_MOVE);
    d->mXlsxSheet->setPicture2(0, getColumnNo("AF") - 1, logoImageIndex, 1500, 150, 25, 22, POSITION_ONLY_MOVE);
    d->mXlsxSheet->setPicture2(77, getColumnNo("AF") - 1, logo1ImageIndex, 1500, 150, 25, 22, POSITION_ONLY_MOVE);
    d->mXlsxSheet->setPicture2(0, getColumnNo("BJ") - 1, logoImageIndex, 1500, 150, 25, 22, POSITION_ONLY_MOVE);
    d->mXlsxSheet->setPicture2(77, getColumnNo("BJ") - 1, logo1ImageIndex, 1500, 150, 25, 22, POSITION_ONLY_MOVE);

    d->mXlsxSheet->setPicture2(58, getColumnNo("T") - 1, signImageIndex, 500, 250, 0, 0, POSITION_ONLY_MOVE);

    d->mXlsxSheet->writeStr(9, getColumnNo("AH") - 1, result.element.patientID.toStdWString().c_str());
    d->mXlsxSheet->writeStr(9, getColumnNo("AL") - 1, result.element.name.toStdWString().c_str());

    d->mXlsxSheet->writeStr(9, getColumnNo("BL") - 1, result.element.patientID.toStdWString().c_str());
    d->mXlsxSheet->writeStr(9, getColumnNo("BP") - 1, result.element.name.toStdWString().c_str());

    this->writeExcelBandV5(result);
}

void ReportData::writeExcelBandV5(const GlobalDataStruct::AnalysisReportInfomation &result)
{
    auto config = ConfigInformation::getInstance();

    QString summaryTige;
    QString summaryCCD;
    QVector<QStringList> classSummaryItems(6);

    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(result.element.panelName);
    auto itemGroupList = config->getCurrentStripGroupList(result.element.panelName);
    auto itemSecondNameList = config->getCurrentStripSecondNameList(result.element.panelName);

    auto bandCount = result.result.resultValue.count();

    auto hiddenBandList = config->getHiddenBandList(result.element.panelName);

    int excelRowIdx = 14;

    int itemNo = 1;
    bool isFront = true;
    bool isFirstPage = true;
    bool writeSecondBand = (config->getExportChangeLanguage() != config->getExportChangeSecondLanguage());

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
            summaryTige = (result.result.classType[i] == "P") ? "Positive" : "Normal";

        if(itemCode.compare("o214", Qt::CaseInsensitive) == 0)
            summaryCCD = QString("Class %1").arg(result.result.classType[i]);

        d->mXlsxSheet->writeNum(excelRowIdx,getBandTableColumnNo("AF", isFront, isFirstPage) - 1, itemNo);
        writeQStringToXlsx(excelRowIdx,     getBandTableColumnNo("AH", isFront, isFirstPage) - 1, itemName);
        if(writeSecondBand) writeQStringToXlsx(excelRowIdx+1,   getBandTableColumnNo("AH", isFront, isFirstPage) - 1, itemSecondName);
        writeQStringToXlsx(excelRowIdx,     getBandTableColumnNo("AK", isFront, isFirstPage) - 1, itemCode);
        writeQStringToXlsx(excelRowIdx,     getBandTableColumnNo("AG", isFront, isFirstPage) - 1, itemGroup);
        writeQStringToXlsx(excelRowIdx,     getBandTableColumnNo("AL", isFront, isFirstPage) - 1, result.result.resultValue[i]);

        if(i < result.result.classType.count())
        {
            writeQStringToXlsx(excelRowIdx, getBandTableColumnNo("AM", isFront, isFirstPage) - 1, result.result.classType[i]);
            this->drawCellClassColorBandV5(excelRowIdx, result.result.classType[i].toInt(), isFront, isFirstPage);

            int classType = result.result.classType[i].toInt();
            if(classType > 0 && classType <= 6)
            {
                classSummaryItems[classType - 1].append(itemNameList[i]);
            }
        }

        if(itemCode.compare("PC", Qt::CaseInsensitive) == 0)
        {
            writeQStringToXlsx(excelRowIdx,     getBandTableColumnNo("AF", isFront, isFirstPage) - 1, "-");
        }
        else
        {
            itemNo++;
        }

        excelRowIdx += 2;

        switch(itemNo)
        {
            case 31:
                isFront = false;
                excelRowIdx = 14;
            break;
            case 61:
                isFront = true;
                isFirstPage = false;
                excelRowIdx = 14;
            break;
            case 91:
                isFront = false;
                excelRowIdx = 14;
            break;
        }
    }

    this->writeClassMemoV5(summaryTige, summaryCCD, classSummaryItems);
    this->mergeAllergyDualGroupV5(bandCount);
}

int ReportData::getBandTableColumnNo(QString strColumn, bool isFront, bool isFirstPage)
{
    int conlumnNo = getColumnNo(strColumn);
    if(!isFront)
        conlumnNo += 15;

    return (isFirstPage) ? conlumnNo : conlumnNo + 30;
}

void ReportData::drawCellClassColorBandV5(const int rowIdx, int classLevel, bool isFront, bool isFirstPage)
{
    Format* cellFormat = d->mXlsx->addFormat();
    cellFormat->setFillPattern(FILLPATTERN_SOLID);
    cellFormat->setBorder(BORDERSTYLE_THIN);
    cellFormat->setBorderColor(d->mXlsx->colorPack(128, 128, 128));

    int startColumn = getBandTableColumnNo("AN", isFront, isFirstPage) - 1;

    switch(classLevel)
    {
    case 1:
        cellFormat->setPatternForegroundColor(d->mXlsx->colorPack(220, 230, 241));
        break;

    case 2:
        cellFormat->setPatternForegroundColor(d->mXlsx->colorPack(184, 204, 228));
        break;

    case 3:
        cellFormat->setPatternForegroundColor(d->mXlsx->colorPack(149, 179, 215));
        break;

    case 4:
        cellFormat->setPatternForegroundColor(d->mXlsx->colorPack(79, 129, 189));
        break;

    case 5:
        cellFormat->setPatternForegroundColor(d->mXlsx->colorPack(54, 86, 146));
        break;

    case 6:
        cellFormat->setPatternForegroundColor(d->mXlsx->colorPack(36, 64, 98));
        break;
    }

    for(auto i = startColumn; i < startColumn + classLevel; i++)
    {
        d->mXlsxSheet->writeBlank(rowIdx, i, cellFormat);
    }
}

void ReportData::writeClassMemoV5(QString tigeNote, QString ccdNote, QVector<QStringList>& classNoteItems)
{
    d->mXlsxSheet->writeStr(23, getColumnNo("D") - 1, tigeNote.toStdWString().c_str());
    d->mXlsxSheet->writeStr(24, getColumnNo("D") - 1, ccdNote.toStdWString().c_str());

    int row = 0;
    for(int i=classNoteItems.count()-1 ; i>=0 ; i--)
    {
        if(classNoteItems[i].count() > 0)
        {
            d->mXlsxSheet->writeStr(26 + row, getColumnNo("D") - 1, QString("Class %1 : ").arg(i + 1).toStdWString().c_str());

            QStringList allergyNote = getAllergyMemoNewLine(classNoteItems[i]);
            for(int j=0 ; j<allergyNote.count(); j++)
            {
                d->mXlsxSheet->writeStr(26 + row, getColumnNo("E") - 1, allergyNote[j].toStdWString().c_str());
                row++;
            }
        }
    }
}

QStringList ReportData::getAllergyMemoNewLine(QStringList classNoteItems)
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

void ReportData::mergeAllergyDualGroupV5(int bandCount)
{
    int mergeCount = 0;
    bool isFront = true;
    bool isFirstPage = true;
    int startRow = 14;
    int endRow = 0;

    QString readCell;
    QString nextCell;

    for(int i = 0; i < bandCount; i++)
    {
        endRow = startRow + (mergeCount++ * 2);

        readCell = QString::fromWCharArray(d->mXlsxSheet->readStr(startRow, getBandTableColumnNo("AG", isFront, isFirstPage) - 1));
        nextCell = QString::fromWCharArray(d->mXlsxSheet->readStr(endRow, getBandTableColumnNo("AG", isFront, isFirstPage) - 1));

        if(readCell != nextCell)
        {
            d->mXlsxSheet->setMerge(startRow, endRow - 1, getBandTableColumnNo("AG", isFront, isFirstPage) - 1, getBandTableColumnNo("AG", isFront, isFirstPage) - 1);
            startRow = endRow;
            mergeCount = 0;
            i--;
        }

        if(!isFirstPage && !isFront && nextCell.isEmpty())  break;


        switch(i)
        {
            case 31:
                isFront = false;
                startRow = 14;
            break;
            case 61:
                isFront = true;
                isFirstPage = false;
                startRow = 14;
            break;
            case 91:
                isFront = false;
                startRow = 14;
            break;
        }
    }
}

int ReportData::getColumnNo(QString strColumn)
{
    if (strColumn.isNull() && strColumn.isEmpty()) {
        return 0;
    }

    QString strUpper = strColumn.toUpper();

    int value = 0;
    char text;
    for(int i=0 ; i<strUpper.length() ; i++)
    {
        text = strUpper.at(i).toLatin1();
        value = value * 26 + (text - 'A' + 1);
    }

    return value;
}
