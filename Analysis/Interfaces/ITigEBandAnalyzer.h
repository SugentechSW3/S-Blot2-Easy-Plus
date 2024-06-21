#ifndef ITIGEBANDANALYZER_H
#define ITIGEBANDANALYZER_H

#include <QObject>

#include <Analysis/GlobalDataStruct.h>
class ITigEBandAnalyzer : public QObject
{
public:
    explicit ITigEBandAnalyzer(QObject* parent = nullptr)
        :QObject(parent){}
    virtual ~ITigEBandAnalyzer() = default;

    //virtual bool analyzeTigEBand(QVector<GlobalDataStruct::AnalysisReportInfomation>& element) = 0;
    virtual bool analyzeTigEBand(QVector<GlobalDataStruct::AnalysisReportInfomation>& element, QMap<QString, GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT> lotDataMap) = 0;
};
#endif // ITIGEBANDANALYZER_H
