#ifndef BASICDATABASECTRL_H
#define BASICDATABASECTRL_H

#include "DataBase/DataBaseCtrl.h"

class BasicDatabaseCtrl: public DataBaseCtrl
{
public:
    explicit BasicDatabaseCtrl(QObject* parent = nullptr);
    virtual ~BasicDatabaseCtrl() = default;

    static QString makeIntensityValueToStrings(const QVector<double>& intensityValueList);
    static QVector<double> makeStringToInstensityValue(const QString& value);
    static QVariantMap makePatientData(const GlobalDataStruct::AnalysisReportInfomation& report);
    static QVariantMap makeResultData(const GlobalDataStruct::AnalysisReportInfomation& report);
};

#endif // BASICDATABASECTRL_H
