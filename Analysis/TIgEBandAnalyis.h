#ifndef TIGEBANDANALYIS_H
#define TIGEBANDANALYIS_H

#include <QObject>
#include "GlobalDataStruct.h"

class TIgEBandAnalyis :public QObject
{
    Q_OBJECT
public:
    explicit TIgEBandAnalyis(QObject* parent = nullptr);
    virtual ~ TIgEBandAnalyis();

    static void calcTIgEBandData(int currentTigeidx, GlobalDataStruct::AnalysisReportInfomation& element);
    static void calcTIgEBandData( GlobalDataStruct::AnalysisReportInfomation& element);

private:
    static int bandClassSum(const GlobalDataStruct::AnalysisReportInfomation& element);
};

#endif // TIGEBANDANALYIS_H
