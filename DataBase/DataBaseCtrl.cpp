#include "DataBaseCtrl.h"

#include "Common/DataBase/CDataBase.h"
#include "Common/CUtil.h"
#include "Common/CLogWriter.h"

USING_SUGENTECH

struct DataBaseCtrl::privateStruct
{
    QString mDatabaseName;
    QMap<QString, QString> mPasswordMap;
    CDataBase* mDatabase = nullptr;
};

DataBaseCtrl::DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST database, QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    this->initData();

    auto dbName = this->convertEnumToQString((int)database, "DATABASE_NAME_LIST");
    d->mDatabase->openDataBase(dbName, this->findPassword(dbName));
    d->mDatabaseName = dbName;
}

DataBaseCtrl::~DataBaseCtrl()
{

}

void DataBaseCtrl::initData()
{
    d->mPasswordMap[this->convertEnumToQString((int)DATABASE_NAME_LIST::BasicDB, "DATABASE_NAME_LIST")] = "reader_admin";
    d->mPasswordMap[this->convertEnumToQString((int)DATABASE_NAME_LIST::DBCodeFunc, "DATABASE_NAME_LIST")] = "reader_admin";
    d->mDatabase = CDataBase::getInstance();
}

QVector<QVariantMap> DataBaseCtrl::select(const QString& query) const
{
    return d->mDatabase->readSqlData2D(d->mDatabase->readData(d->mDatabaseName, query));
}

bool DataBaseCtrl::writeData(const QString& query)
{
    return d->mDatabase->writeData(d->mDatabaseName,query);
}

bool DataBaseCtrl::insertData(const QString &tableName, const QVariantMap& data)
{
    return d->mDatabase->writeData(d->mDatabaseName, d->mDatabase->createInsertQuery(tableName, data));
}

bool DataBaseCtrl::insertData(const QString& tableName, const QVector<QVariantMap>&data)
{
    auto sqlVector = d->mDatabase->writeSqlInsertData2D(tableName, data);

    return d->mDatabase->writeAllData(d->mDatabaseName, sqlVector);
}

bool DataBaseCtrl::updateData(const QString& tableName, const QVariantMap& data,
                                  const QString& uniqueIDColumnStr, const QString& uniqueID)
{
    QString where = QString("%1 = '%2' ").arg(uniqueIDColumnStr).arg(uniqueID);
    auto sql = d->mDatabase->createUpdateQuery(tableName, data, where);
    return d->mDatabase->writeData(d->mDatabaseName, sql);
}

bool DataBaseCtrl::updateData(const QString& tableName, const QVector<QVariantMap>&data, const QVector<QString>& whereColumn)
{
    auto sql = d->mDatabase->writeSqlUpdateData2D(tableName, data, whereColumn);
    return d->mDatabase->writeAllData(d->mDatabaseName, sql);
}

bool DataBaseCtrl::removeData(const QString& tableName,
                                  const QString& uniqueIDColumnStr, const QString& uniqueId)
{
    QString removeQuery = QString("DELETE FROM %1 WHERE %2 = '%3'").arg(tableName)
                                                                   .arg(uniqueIDColumnStr)
                                                                   .arg(uniqueId);

    return d->mDatabase->writeData(d->mDatabaseName, removeQuery);
}

bool DataBaseCtrl::removeDataBeforeDate(const QString& tableName,
                              const QString& uniqueIDColumnStr, const QDate& date)
{
    QString removeQuery = QString("DELETE FROM %1 WHERE %2 <= #%3#").arg(tableName)
                              .arg(uniqueIDColumnStr)
                              .arg(date.toString("yyyy-MM-dd"));

    return d->mDatabase->writeData(d->mDatabaseName, removeQuery);
}

void DataBaseCtrl::makeWhereCondition(const QString& tableName,  QString& whereClause,
                                      const QString& columnName, const QVariant& key )
{
    QString keyString = key.toString();

    if(keyString.isEmpty() == false)
    {
        if(whereClause.isEmpty() == false)
            whereClause.append(" AND ");

        if(key.type() == QVariant::String)
            whereClause.append(QString("%1.%2 = '%3'").arg(tableName).arg(columnName).arg(keyString));
        else if(key.type() == QVariant::Date)
            whereClause.append(QString("DATEVALUE(%1.%2) = #%3#").arg(tableName).arg(columnName).arg(keyString));
        else
            whereClause.append(QString("%1.%2 = %3").arg(tableName).arg(columnName).arg(keyString));
    }
}


void DataBaseCtrl::makeWhereBetweenDate(const QString& tableName,  QString& whereClause,
                                        const QString& columnName, const QDate &fromDate, const QDate &toDate)
{
    QString fromDateString = QVariant(fromDate).toString();
    QString toDateString = QVariant(toDate).toString();

    if(fromDateString.isEmpty() == false || toDateString.isEmpty() == false)
    {
        if(whereClause.isEmpty() == false)
            whereClause.append(" AND ");

            whereClause.append(QString("DATEVALUE(%1.%2) BETWEEN #%3# AND #%4#").arg(tableName).arg(columnName).arg(fromDateString).arg(toDateString));
    }
}

QString DataBaseCtrl::convertEnumToQString(int enumIdx, const QString& tableName) const
{
   return CUtil::convertQEnumToQString(enumIdx, tableName, staticMetaObject);
}

int DataBaseCtrl::convertQStringToEnum(const QString& key, const QString& tableName) const
{
    return CUtil::convertQStringToQEnum(key, tableName, staticMetaObject);
}

QString DataBaseCtrl::getLastError() const
{
    return d->mDatabase->getInstance()->getLastError(d->mDatabaseName);
}

QString DataBaseCtrl::getLastNativeDatabaseError() const
{
    return d->mDatabase->getInstance()->getLastNativeError(d->mDatabaseName);
}

QString DataBaseCtrl::findPassword(const QString& dbName)
{
    return d->mPasswordMap[dbName];
}

