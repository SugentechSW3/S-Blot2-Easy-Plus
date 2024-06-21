#ifndef DATABASEINSTANCE_H
#define DATABASEINSTANCE_H

#include <QObject>
#include <QVariant>
#include <QVector>

#include "Analysis/GlobalDataStruct.h"

class DataBaseCtrl : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:

    enum class PATIENT_TABLE
    {
        //Mandentory
        PATIENT_UNIQUE_ID, //PK
        PATIENT_ID,
        PATIENT_NAME,
        PATIENT_CONTENTS_NAME,
        PATIENT_PANEL_NAME,
        PATIENT_STRIP_NUMBER,
        PATIENT_LOT_NUMBER,
        //optinal
        PATIENT_APPLICANT,
        PATIENT_ANALYST,
        PATIENT_REVIEWER,
        PATIENT_DATE,
        PATIENT_AGE,
        PATIENT_SEX,
        PATIENT_PHONE,
        PATIENT_EMAIL,
    };

    enum class RESULT_TABLE
    {
        RESULT_UNIQUE_ID, //PK
        RESULT_INTENSITY,
        RESULT_TEST_RESULT,
        RESULT_IMAGE_PATH,
        RESULT_COMMENT,
        RESULT_ERROR_REASON
    };

    enum class STRIP_REL_TABLE
    {
        LOT_NUMBER, //PK
        FUNC_LIST
    };

    enum class FUNC_TABLE
    {
        FUNC, //PK
        FVALUE_A,
        FVALUE_B,
        FVALUE_C,
        FVALUE_D,
        FVALUE_E,
        FVALUE_F,
        FVALUE_G
    };
    enum class LOT_OPTIONAL_TABLE
    {
        LOT_NUMBER, //pk
        CUT_OFF,
        EXP_DATE
    };

    enum class DATABASE_BASIC_DB_TABLE_LIST
    {
        PATIENT_TABLE, // 환자정보
        RESULT_TABLE    // 결과 저정보
    };
    enum class DATABASE_DB_CODEFUNC_TABLE_LIST
    {
        FUNC_TABLE, // func table
        LOT_OPTIONAL_TABLE, // lot information
        STRIP_REL_TABLE // lot funclist
    };

    enum class DATABASE_NAME_LIST
    {
        BasicDB,
        DBCodeFunc
    };

    explicit DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST database, QObject *parent = nullptr);
    virtual ~DataBaseCtrl();


    Q_ENUM(PATIENT_TABLE)
    Q_ENUM(RESULT_TABLE)
    Q_ENUM(STRIP_REL_TABLE)
    Q_ENUM(FUNC_TABLE)
    Q_ENUM(LOT_OPTIONAL_TABLE)
    Q_ENUM(DATABASE_BASIC_DB_TABLE_LIST)
    Q_ENUM(DATABASE_DB_CODEFUNC_TABLE_LIST)
    Q_ENUM(DATABASE_NAME_LIST)    

    //CRUD
    QVector<QVariantMap> select(const QString& query) const;

    bool writeData(const QString& query);
    bool insertData(const QString& tableName, const QVariantMap& data);
    bool insertData(const QString& tableName, const QVector<QVariantMap>& data);
    bool updateData(const QString& tableName, const QVariantMap& data,
                    const QString &uniqueIDColumnStr, const QString& uniqueID);
    bool updateData(const QString& tableName, const QVector<QVariantMap>&data, const QVector<QString> &whereColumn);
    bool removeData(const QString& tableName, const QString& uniqueIDColumnStr, const QString &uniqueID);
    bool removeDataBeforeDate(const QString& tableName, const QString& uniqueIDColumnStr, const QDate &date);

    QString getLastError() const;
    QString getLastNativeDatabaseError() const;

    static void makeWhereCondition(const QString &tableName, QString& whereClause, const QString &columnName, const QVariant &key);
    static void makeWhereBetweenDate(const QString &tableName, QString& whereClause, const QString &columnName, const QDate &fromDate, const QDate &toDate);

    QString convertEnumToQString(int enumIdx, const QString& tableName) const;
    int convertQStringToEnum(const QString& key, const QString& tableName) const;

private:
    void initData();
    QString findPassword(const QString& dbName);

private:
    QScopedPointer<privateStruct> d;
};

#endif // DATABASEINSTANCE_H
