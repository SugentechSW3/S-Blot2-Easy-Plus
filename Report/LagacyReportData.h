#ifndef LAGACYREPORTDATA_H
#define LAGACYREPORTDATA_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"

class LagacyReportData : public QObject
{
private:
    Q_OBJECT
//    Q_DISABLE_COPY(ReportData)

    struct privateStruct;
public:
    explicit LagacyReportData(QObject *parent = nullptr);
    ~LagacyReportData();

    void openExcelFile();
    bool closeExcelFile(const QString &saveFilePath);

    QString getDirPath();

    void writeExcelData(const GlobalDataStruct::AnalysisReportInfomation &result);
    void writeExcelSingleBand(const GlobalDataStruct::AnalysisReportInfomation& result);
    void writeExcelDualBand(const GlobalDataStruct::AnalysisReportInfomation& result);

    void drawCellClassColorSingleBand(const int rowIdx, int classLevel);
    void drawCellClassColorDualBand(const int rowIdx, int classLevel);

    static void writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result);
    static void writeIntensityData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result, bool useRawData);
private:
    QScopedPointer<privateStruct> d;
};

#endif // LAGACYREPORTDATA_H
