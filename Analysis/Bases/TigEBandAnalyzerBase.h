#ifndef TIGEBANDANALYZERBASE_H
#define TIGEBANDANALYZERBASE_H

#include <Analysis/Interfaces/ITigEBandAnalyzer.h>

class TigEBandAnalyzerBase : public ITigEBandAnalyzer
{
public:
    explicit TigEBandAnalyzerBase(QObject* parent = nullptr);

    virtual ~TigEBandAnalyzerBase() = default;
    bool analyzeTigEBand(QVector<GlobalDataStruct::AnalysisReportInfomation>& element, QMap<QString, GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT> lotDataMap);

private:
    GlobalDataStruct::AnalysisReportInfomation findForSameIDData(const GlobalDataStruct::AnalysisReportInfomation& target,
                                                                            const QVector<GlobalDataStruct::AnalysisReportInfomation>& element);
    void tIgEMagnificationCalc(int currentTigeIdx, GlobalDataStruct::AnalysisReportInfomation& itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData, int cutoff);

    bool tIgEAllZeroCondition(int currentTigeIdx, GlobalDataStruct::AnalysisReportInfomation& itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData, int cutoff);
    void sIgECalc(int currentTigeIdx, GlobalDataStruct::AnalysisReportInfomation& itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData, int cutoff);
    double getTigeConcentrationValue(const double& calculatedTIGE, const int cutoff, const bool isPValueTige);
    QString getTigeResultText(const QString &orgText, const double &concentrationValue);
    bool isHasNonZeroClass(const GlobalDataStruct::AnalysisReportInfomation& itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData);

    int getsIgEConditionClass(const GlobalDataStruct::AnalysisReportInfomation& itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData);
    double findsIgEMagnification(int classNumber);
};

#endif // TIGEBANDANALYZERBASE_H
