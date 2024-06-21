#include <QDate>
#include <QApplication>
#include <QFileDialog>
#include <QTranslator>
#include <qtsingleapplication.h>

#include "LagacyReportData.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include "xlsxchart.h"
#include "xlsxcellrange.h"

#include "Analysis/ConfigInformation.h"


QXLSX_USE_NAMESPACE

struct LagacyReportData::privateStruct
{
    QString mDefaultFileNamePath = "Sugentech_Allergy_New_Report.xlsx";
    QString mDefaultDirPath = "DocumentFormat";
    QString mFormFileFullPath;
    QScopedPointer<Document> mXlsx;
};

LagacyReportData::LagacyReportData(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    d->mFormFileFullPath = QApplication::applicationDirPath() + "/" + d->mDefaultDirPath + "/" + d->mDefaultFileNamePath;
}

LagacyReportData::~LagacyReportData()
{
    d->mXlsx.reset();
}

void LagacyReportData::openExcelFile()
{
    d->mXlsx.reset(new Document(d->mFormFileFullPath));
}

bool LagacyReportData::closeExcelFile(const QString& saveFilePath)
{
    if(d->mXlsx.isNull() == true)
        return false;

    if(saveFilePath.isEmpty() == true)
        return false;

    d->mXlsx->saveAs(saveFilePath);
    d->mXlsx.reset();
    return true;
}

QString LagacyReportData::getDirPath()
{
    return QApplication::applicationDirPath() + "/" + d->mDefaultDirPath;
}

void LagacyReportData::writeExcelData(const GlobalDataStruct::AnalysisReportInfomation& result)
{
#if 0
    if(d->mXlsx.isNull() == true)
        return ;

    auto config = ConfigInformation::getInstance();

    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(result.element.panelName);

    if(itemNameList.isEmpty() == true)
        return;

    if(itemCodeList.isEmpty() == true)
        return;

    QString logoImageFileName = QApplication::applicationDirPath() + "/Config/" + "logo.png";
    QString signImageFileName = QApplication::applicationDirPath() + "/Config/" + "sign.png";
    //QImage logoImage(":/Image/Image/ResultView/Result_Logo.png");
    QImage logoImage(logoImageFileName);
    QImage signImage(signImageFileName);
    d->mXlsx->write(CellReference("B4"), QT_TR_NOOP("Allergy Test Report(Allergen-specific Human IgE Test Result)"));

    d->mXlsx->write(CellReference("B6"), tr("Patient ID :"));
    d->mXlsx->write(CellReference("B7"), tr("Patient Name :"));
    d->mXlsx->write(CellReference("B8"), tr("Age :"));
    d->mXlsx->write(CellReference("B9"), tr("Sex :"));

    d->mXlsx->write(CellReference("Q6"), tr("Institution :"));
    d->mXlsx->write(CellReference("Q7"), QT_TR_NOOP("Testing Date :"));
    d->mXlsx->write(CellReference("Q8"), QT_TR_NOOP("Tested by :"));
    d->mXlsx->write(CellReference("Q9"), tr("Panel :"));

    d->mXlsx->write(CellReference("E6"), result.element.patientID);
    d->mXlsx->write(CellReference("E7"), result.element.name);
    d->mXlsx->write(CellReference("E8"), result.element.age);
    d->mXlsx->write(CellReference("E9"), result.element.sex);

    d->mXlsx->write(CellReference("U6"), config->getCompanyName());
    d->mXlsx->write(CellReference("U7"), result.dateTime.toString("yyyy-MM-dd hh:mm"));
    d->mXlsx->write(CellReference("U8"), result.element.analyst);
    d->mXlsx->write(CellReference("U9"), result.element.panelName);

    d->mXlsx->write(CellReference("E46"), tr("Not found"));
    d->mXlsx->write(CellReference("E47"), tr("Weak"));
    d->mXlsx->write(CellReference("E48"), tr("Moderate"));
    d->mXlsx->write(CellReference("E49"), tr("Moderately strong"));
    d->mXlsx->write(CellReference("E50"), tr("Strong"));
    d->mXlsx->write(CellReference("E51"), tr("Very strong"));
    d->mXlsx->write(CellReference("E52"), tr("Extremely strong"));

    d->mXlsx->write(CellReference("Q45"), result.result.comment);

    d->mXlsx->insertImage(53,21,signImage);
    d->mXlsx->insertImage(3,21,logoImage);

    auto isDualBand = config->getStatus().isDualBand;

    if(isDualBand)
        this->writeExcelDualBand(result);
    else
        this->writeExcelSingleBand(result);
#endif
}

void LagacyReportData::writeExcelSingleBand(const GlobalDataStruct::AnalysisReportInfomation& result)
{
    auto itemNameList = ConfigInformation::getInstance()->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = ConfigInformation::getInstance()->getCurrentStripCodeList(result.element.panelName);

   auto resultCount = result.result.resultValue.count();

    if(resultCount <= 0)
        return;

    for(int i = 0; i < resultCount; i++)
    {
        int excelRowIdx = 12 + i;

        QString name = QString("C%1").arg(excelRowIdx);
        QString code = QString("G%1").arg(excelRowIdx);
        QString iu = QString("H%1").arg(excelRowIdx);
        QString classLevel = QString("I%1").arg(excelRowIdx);

        if(i < itemNameList.count())
            d->mXlsx->write(CellReference(name), itemNameList[i]);

        if(i < itemCodeList.count())
            d->mXlsx->write(CellReference(code), itemCodeList[i]);

        d->mXlsx->write(CellReference(iu), result.result.resultValue[i]);

        if(i < result.result.classType.count())
        {
            d->mXlsx->write(CellReference(classLevel), result.result.classType[i]);
            this->drawCellClassColorSingleBand(excelRowIdx, result.result.classType[i].toInt());
        }
    }
}

void LagacyReportData::writeExcelDualBand(const GlobalDataStruct::AnalysisReportInfomation& result)
{
#if 0
    int hideBandCount = 0;
    auto config = ConfigInformation::getInstance();

    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(result.element.panelName);
    auto isHindeTheBandList = config->getIsStripBehindeList(result.element.panelName);

    auto dualbandBackCount = result.result.resultValue.count();
    auto dualBandFrontCount = config->getDualBandSetting().dualBandFrontBlockCount;


    for(int i = 0; i < dualBandFrontCount; i++)
    {
        if(dualbandBackCount <= dualBandFrontCount)
            break;

        if(isHindeTheBandList[i] == true)
        {
            hideBandCount ++;
            continue;
        }

        int excelRowIdx = 12 + i - hideBandCount;

        QString name = QString("C%1").arg(excelRowIdx);
        QString code = QString("G%1").arg(excelRowIdx);
        QString iu = QString("H%1").arg(excelRowIdx);
        QString classLevel = QString("I%1").arg(excelRowIdx);

        if(i < itemNameList.count())
            d->mXlsx->write(CellReference(name), itemNameList[i]);

        if(i < itemCodeList.count())
            d->mXlsx->write(CellReference(code), itemCodeList[i]);

        d->mXlsx->write(CellReference(iu), result.result.resultValue[i]);

        if(i < result.result.classType.count())
        {
            d->mXlsx->write(CellReference(classLevel), result.result.classType[i]);
            this->drawCellClassColorSingleBand(excelRowIdx, result.result.classType[i].toInt());
        }
    }

    hideBandCount = 0;

    for(int i = dualBandFrontCount; i < dualbandBackCount; i++)
    {
        if(isHindeTheBandList[i] == true)
        {
            hideBandCount ++;
            continue;
        }

        int excelRowIdx = 12 + i - dualBandFrontCount -hideBandCount;
        auto dualBandIdx = i;

        QString nextPageName = QString("R%1").arg(excelRowIdx);
        QString nextPagecode = QString("V%1").arg(excelRowIdx);
        QString nextPageIu = QString("W%1").arg(excelRowIdx);
        QString nextPageClassLevel = QString("X%1").arg(excelRowIdx);

        if(dualBandIdx < itemNameList.count())
            d->mXlsx->write(CellReference(nextPageName), itemNameList[dualBandIdx]);

        if(dualBandIdx < itemCodeList.count())
            d->mXlsx->write(CellReference(nextPagecode), itemCodeList[dualBandIdx]);

        d->mXlsx->write(CellReference(nextPageIu), result.result.resultValue[dualBandIdx]);

        if(dualBandIdx < result.result.classType.count())
        {
            d->mXlsx->write(CellReference(nextPageClassLevel), result.result.classType[dualBandIdx]);
            this->drawCellClassColorDualBand(excelRowIdx, result.result.classType[dualBandIdx].toInt());
        }
    }
#endif
}

void LagacyReportData::drawCellClassColorSingleBand(const int rowIdx, int classLevel)
{
    QColor color;
    int range = 0;

    switch(classLevel)
    {
    case 1:
        color = QColor("#DCE6F1");
        range = 11;
        break;

    case 2:
        color = QColor("#B8CCE4");
        range = 12;
        break;

    case 3:
        color = QColor("#95B3D7");
        range = 13;
        break;

    case 4:
        color = QColor("#4F81BD");
        range = 14;
        break;

    case 5:
        color = QColor("#366092");
        range = 15;
        break;

    case 6:
        color = QColor("#244062");
        range = 16;
        break;
    }

    for(auto i = 10; i < range; i++)
    {
        auto cell = d->mXlsx->cellAt(rowIdx, i);

        if(cell == nullptr)
            return;

        auto format = cell->format();
        format.setPatternBackgroundColor(color);
        d->mXlsx->write(CellReference(rowIdx,i),"", format);
    }
}

void LagacyReportData::drawCellClassColorDualBand(const int rowIdx, int classLevel)
{
    QColor color;
    int range = 0;

    switch(classLevel)
    {
    case 1:
        color = QColor("#DCE6F1");
        range = 26;
        break;

    case 2:
        color = QColor("#B8CCE4");
        range = 27;
        break;

    case 3:
        color = QColor("#95B3D7");
        range = 28;
        break;

    case 4:
        color = QColor("#4F81BD");
        range = 29;
        break;

    case 5:
        color = QColor("#366092");
        range = 30;
        break;

    case 6:
        color = QColor("#244062");
        range = 31;
        break;
    }

    for(auto i = 25; i < range; i++)
    {
        auto cell = d->mXlsx->cellAt(rowIdx, i);

        if(cell == nullptr)
            return;

        auto format = cell->format();
        format.setPatternBackgroundColor(color);
        d->mXlsx->write(CellReference(rowIdx,i),"", format);
    }
}


void LagacyReportData::writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    Document xlsx(QApplication::applicationDirPath() + "/" + "DocumentFormat"+ "/" + QString("test-fulldata-%1.xlsx").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));

    int columnGap = 0;

    for(int column = 0 ; column < result.count(); column++)
    {
        auto itr = result[column];
        auto itemNameList = ConfigInformation::getInstance()->getCurrentStripNameList(itr.element.panelName);

        if(itemNameList.isEmpty() == true)
            continue;

        xlsx.write(1, (column*4) + columnGap + 1, QString("UserID : %1").arg(itr.element.patientID));
        xlsx.write(2, (column*4) + columnGap + 1, "Name");
        xlsx.write(2, (column*4) + columnGap + 2, "IU/ml");
        xlsx.write(2, (column*4) + columnGap + 3, "Intensity");
        xlsx.write(2, (column*4) + columnGap + 4, "ClassType");

        //excel은 0이 아닌 1부터시작.
        for(int row = 0; row < itr.result.resultValue.count(); row++)
        {
            if(row < itemNameList.count())
                xlsx.write(row+3, (column*4) + columnGap + 1, itemNameList[row]);

            xlsx.write(row+3, (column*4) + columnGap + 2, itr.result.resultValue[row]);
            xlsx.write(row+3, (column*4) + columnGap + 3, itr.result.rawIntensityValue[row]);
            xlsx.write(row+3, (column*4) + columnGap + 4, itr.result.classType[row]);
        }

        columnGap++;
    }

    //숫자넣고 코드삭제

    xlsx.save();
}

void LagacyReportData::writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result, bool useRawData)
{
    QString mode("intensity");

    if(useRawData == true)
        mode = "rawdata";

    //excel start idx 1
    Document xlsx(QApplication::applicationDirPath() + "/" + "DocumentFormat"+ "/" + QString("test-%1-%2.xlsx").arg(mode).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
    int columnGap = 0;

    if(useRawData == true)
    {
        for(int column = 0 ; column < result.count(); column++)
        {
            auto itr = result[column];
            auto itemNameList = ConfigInformation::getInstance()->getCurrentStripNameList(itr.element.panelName);

            if(itemNameList.isEmpty() == true)
                continue;

            xlsx.write(1, (column*4) + columnGap + 1, QString("UserID : %1").arg(itr.element.patientID));
            xlsx.write(2, (column*2) + columnGap + 1, "Name");
            xlsx.write(2, (column*2) + columnGap + 2, "Intensity");

            for(int row = 0; row < itr.result.resultValue.count(); row++)
            {

                if(row < itemNameList.count())
                    xlsx.write(row+3, (column*2) + columnGap + 1, itemNameList[row]);

                xlsx.write(row+3, (column*2) + columnGap + 2, itr.result.rawIntensityValue[row]);
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

            xlsx.write(1, (column*3) + columnGap + 1, QString("StripNumber : %1").arg(column+1));
            xlsx.write(2, (column*3) + columnGap + 1, "Name");
            xlsx.write(2, (column*3) + columnGap + 2, "IU/ml");
            xlsx.write(2, (column*3) + columnGap + 3, "ClassType");

            for(int row = 0; row < resultElement.result.resultValue.count(); row++)
            {
                if(row < itemNameList.count())
                    xlsx.write(row+3, (column*3) + columnGap + 1, itemNameList[row]);

                xlsx.write(row+3, (column * 3) + columnGap + 2, resultElement.result.resultValue[row]);
                xlsx.write(row+3, (column * 3) + columnGap + 3, resultElement.result.classType[row]);
            }

            columnGap++;
        }
    }

    xlsx.save();
}
