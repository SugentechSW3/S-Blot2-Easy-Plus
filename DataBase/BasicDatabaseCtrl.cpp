#include "BasicDatabaseCtrl.h"

#include <Common/CUtil.h>

BasicDatabaseCtrl::BasicDatabaseCtrl(QObject *parent)
    :DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::BasicDB,parent)
{

}

QString BasicDatabaseCtrl::makeIntensityValueToStrings(const QVector<double> &intensityValueList)
{
    QString valueString;

    foreach(auto itr, intensityValueList)
    {
        valueString.append(QString::number(itr,'f'));
        valueString.append("/");
    }

    if(valueString.isEmpty() == false)
        valueString.remove(valueString.count() - 1, 1); // remove'/'

    return valueString;
}

QVector<double> BasicDatabaseCtrl::makeStringToInstensityValue(const QString &value)
{
    QVector<double> values;
    QString valueStr = value;
    auto valueList = valueStr.split("/");

    foreach(auto itr, valueList)
        values.push_back(itr.toDouble());

    return  values;
}
// TDOD : 테스트 진행 해봐야함
QVariantMap BasicDatabaseCtrl::makePatientData(const GlobalDataStruct::AnalysisReportInfomation &report)
{
    QVariantMap query;
    QString tableName("PATIENT_TABLE");
    BasicDatabaseCtrl dataBaseCtrl;

    QString patientColumnUniqueID = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_UNIQUE_ID), tableName);
    QString patientColumnPatientID = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ID), tableName);
    QString patientColumnName = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_NAME), tableName);
    QString patientColumnPanelName = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PANEL_NAME), tableName);
    QString patientColumnContentsName = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_CONTENTS_NAME), tableName);
    QString patientColumnStripNumber = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_STRIP_NUMBER), tableName);
    QString patientColumnLotNumber = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_LOT_NUMBER), tableName);

    QString patientColumnApplicant = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_APPLICANT), tableName);
    QString patientColumnAnalyst = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_ANALYST), tableName);
    QString patientColumnReviewer = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_REVIEWER), tableName);
    QString patientColumnDate = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_DATE), tableName);
    QString patientColumnAge = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_AGE), tableName);
    QString patientColumnSex = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_SEX), tableName);
    QString patientColumnPhone = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_PHONE), tableName);
    QString patientColumnEmail = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::PATIENT_TABLE::PATIENT_EMAIL), tableName);

    query[patientColumnUniqueID] = report.uniqueID;
    query[patientColumnLotNumber] = report.lotNumber;
    query[patientColumnPatientID] = report.element.patientID;
    query[patientColumnName] = report.element.name;
    query[patientColumnPanelName] = report.element.panelName;
    query[patientColumnContentsName] = report.element.contentsName;
    query[patientColumnStripNumber] = report.element.stripNumber;

    query[patientColumnApplicant] = report.element.applicant;
    query[patientColumnAnalyst] = report.element.analyst;
    query[patientColumnReviewer] = report.element.reviewer;
    query[patientColumnDate] =  report.dateTime.toString("yyyy-MM-dd hh:mm:ss");
    query[patientColumnAge] = report.element.age;
    query[patientColumnSex] = report.element.sex;
    query[patientColumnPhone] = report.element.phone;
    query[patientColumnEmail] = report.element.eMail;

    return query;
}
// TDOD : 테스트 진행 해봐야함
QVariantMap BasicDatabaseCtrl::makeResultData(const GlobalDataStruct::AnalysisReportInfomation &report)
{
    QVariantMap query;
    QString tableName("RESULT_TABLE");
    BasicDatabaseCtrl dataBaseCtrl;

    QString resultColumnUniqueIDStr = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_UNIQUE_ID), tableName);
    QString resultColumnIntensity = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_INTENSITY), tableName);
    QString resultColumnTestResult = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_TEST_RESULT), tableName);
    QString resultColumnImagePath = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_IMAGE_PATH), tableName);
    QString resultColumnErrorReason = dataBaseCtrl.convertEnumToQString(static_cast<int>(DataBaseCtrl::RESULT_TABLE::RESULT_ERROR_REASON), tableName);

    query[resultColumnUniqueIDStr] = report.uniqueID;
    query[resultColumnIntensity] = BasicDatabaseCtrl::makeIntensityValueToStrings(report.result.rawIntensityValue);
    query[resultColumnTestResult] = report.result.isSuccess;
    query[resultColumnImagePath] = report.result.resultImagePath;
    query[resultColumnErrorReason] = report.result.errorReason;

    return query;
}

