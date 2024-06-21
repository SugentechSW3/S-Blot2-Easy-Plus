#ifndef REPORTJSONDATACONTROLS_H
#define REPORTJSONDATACONTROLS_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"
#include "Common/CUtilCommon.h"

class ReportJsonDataControls: public QObject
{
private:
    Q_OBJECT
    struct privateSturct;
public:
    explicit ReportJsonDataControls(QObject *parent = nullptr);
    virtual ~ReportJsonDataControls();

    void convertJsonAnalysisReportImformation(const GlobalDataStruct::AnalysisReportInfomation& result,const QString& filePath);
    bool isExportJson(const QString password);

private:
    QJsonObject getElementObject(const GlobalDataStruct::AnalysisReportInfomation& result);
    QJsonObject getAnalysisResultObject(const GlobalDataStruct::AnalysisReportInfomation& result);
    QJsonObject getBandNameList(const GlobalDataStruct::AnalysisReportInfomation& result);

    void saveJsonFile(const QString fileName, const QJsonDocument& doc);

private:
    QScopedPointer<privateSturct> d;
};

#endif // REPORTJSONDATACONTROLS_H
