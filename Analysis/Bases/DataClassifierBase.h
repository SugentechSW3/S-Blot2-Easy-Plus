#ifndef DATACLASSIFIERBASE_H
#define DATACLASSIFIERBASE_H

#include "Analysis/Interfaces/IDataClassifier.h"

class DataClassifierBase : public IDataClassifier
{
public:
    explicit DataClassifierBase(QObject* parent =nullptr);
    virtual ~DataClassifierBase() = default;

    bool calcClassData(QVector<GlobalDataStruct::AnalysisReportInfomation>& element,
                                          DataBaseCtrl* codeFuncDatabase) override;

private:
    double calcContentsValue(const double& intensity, const GlobalDataStruct::FORMULA_PARAMETER& parameter);
    void appendClassTypeAndResultValue(GlobalDataStruct::AnalysisReportInfomation& itr, double intensity, double calcValue,
                                                  const QString& code, const QString& funcRel, const int& tigECutoff = 0);
};

#endif // DATACLASSIFIERBASE_H
