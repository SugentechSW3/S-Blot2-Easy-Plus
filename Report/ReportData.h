#ifndef REPORTDATA_H
#define REPORTDATA_H
#include <QObject>
#include <Analysis/GlobalDataStruct.h>
#include "libxl.h"

using namespace libxl;

class ReportData
{
    struct privateStruct;
    enum CELLTYPE
    {
        STRING,
        NUMBER,
        IMAGE,
    };

public:
    explicit ReportData(QObject *parent = nullptr);
    ~ReportData();

    bool openExcelFile();
    bool closeExcelFile(const QString &saveFilePath);

    QString getDirPath();

    static void writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result);
    static void writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result, bool useRawData);

    void writeExcelDataV5(const GlobalDataStruct::AnalysisReportInfomation &result);

private:
    void writeQStringToXlsx(const int& row, const int&col, const QString& str);
    void writeExcelBandV5(const GlobalDataStruct::AnalysisReportInfomation& result);

    int getColumnNo(QString strColumn);
    int getBandTableColumnNo(QString strColumn, bool isFront, bool isFirstPage = true);
    void writeClassMemoV5(QString tigeNote, QString ccdNote, QVector<QStringList>& classSummaryItem);
    void drawCellClassColorBandV5(const int rowIdx, int classLevel, bool isFront, bool isFirstPage = true);
    QStringList getAllergyMemoNewLine(QStringList classSummaryItems);
    void mergeAllergyDualGroupV5(int bandCount);

private:
    QScopedPointer<privateStruct> d;
};

#endif // REPORTDATA_H
