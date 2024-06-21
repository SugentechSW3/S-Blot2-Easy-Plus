#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QApplication>
#include <QDateTime>
#include "CDataBase.h"
#include "Common/CLogWriter.h"


// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2018-03-19
// Modified Date : 2018-07-01

NS_SUGENTECH_BEGIN

struct CDataBase::privateStruct
{
    QMap<QString ,QSqlDatabase> mDB;
};

CDataBase::CDataBase(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
}

CDataBase::~CDataBase()
{
    for(auto itr : d->mDB)
        itr.close();
}

#if USE_SQL_LITE
bool CDataBase::openDataBase(const QString& databaseName)
{
    if( d->mDB[databaseName].isDriverAvailable("QSQLITE") == false)
        return false;

    if( d->mDB[databaseName].isOpen() == true)
        return true;


     d->mDB[databaseName] = QSqlDatabase::addDatabase("QSQLITE");

    QString fullPath;
    fullPath = QApplication::applicationDirPath();
    fullPath.append("/");
    fullPath.append(databaseName);
    fullPath.append(".db");

     d->mDB[databaseName].setDatabaseName(fullPath);

    if(! d->mDB[databaseName].open())
    {
        CLogWriter::printLog(QString("(CDataBase::openDataBase) DataBase Open Fail : %1").arg( d->mDB[databaseName].lastError().text()));
        return false;
    }

    return true;
}


#elif USE_SQL_ACCESS

bool CDataBase::openDataBase(const QString& databaseName)
{
    if(d->mDB[databaseName].isDriverAvailable("QODBC") == false)
        return false;

    if(d->mDB[databaseName].isOpen() == true)
        return true;


    QString fullPath ;
    fullPath.append("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=");
    fullPath.append(QApplication::applicationDirPath());
    fullPath.append("/");
    fullPath.append(databaseName);
    fullPath.append(".accdb");

    d->mDB[databaseName] = QSqlDatabase::addDatabase("QODBC", fullPath);
    d->mDB[databaseName].setDatabaseName(fullPath);

    if(d->mDB[databaseName].open() == false)
    {
        CLogWriter::printLog(QString("(CDataBase::openDataBase) DataBase Open Fail : %1").arg(d->mDB[databaseName].lastError().text()),
                             CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    return true;
}

bool CDataBase::openDataBase(const QString& databaseName, const QString& password)
{
    if(d->mDB[databaseName].isDriverAvailable("QODBC") == false)
        return false;

    if(d->mDB[databaseName].isOpen() == true)
        return true;

    QString fullPath ;
    fullPath.append("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=");
    fullPath.append(QApplication::applicationDirPath());
    fullPath.append("/");
    fullPath.append(databaseName);
    fullPath.append(".accdb"); // use .accdb need 32bit .accdb driver (2016 upper driver)

    d->mDB[databaseName] = QSqlDatabase::addDatabase("QODBC", fullPath);
    d->mDB[databaseName].setDatabaseName(fullPath);
    d->mDB[databaseName].setPassword(password);

    if(d->mDB[databaseName].open() == false)
    {
        CLogWriter::printLog(QString("(CDataBase::openDataBase) DataBase Open Fail : %1").arg(d->mDB[databaseName].lastError().text()),
                             CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    return true;
}

#endif /* [CDataBase::op enDataBase]USE_SQL */

//select
QSqlQuery CDataBase::readData(const QString& databaseName,const QString& sql)
{
    QSqlQuery query(d->mDB[databaseName]);
    query.prepare(sql);

    if(query.exec() == false)
    {
        qDebug() << "read" <<sql;
        CLogWriter::printLog(QString("(CDataBase::readData) : ") + query.lastError().text(),
                             CLogWriter::LOG_TYPE_RELEASE);
        return query;
    }

    return query;
}

//remove, insert, etc
bool CDataBase::writeData(const QString& databaseName,const QString& sql)
{
    QSqlQuery query( d->mDB[databaseName]);
    query.prepare(sql);

     d->mDB[databaseName].transaction();

    if(query.exec() == false)
    {
        qDebug() << "write" <<sql;
        CLogWriter::printLog(QString("(CDataBase::writeData) : ") + query.lastError().text(),
                             CLogWriter::LOG_TYPE_RELEASE);

         d->mDB[databaseName].rollback();
        return false;
    }

     d->mDB[databaseName].commit();
    return true;
}

bool CDataBase::writeAllData(const QString &databaseName, const QVector<QString>& data)
{
     d->mDB[databaseName].transaction();

    for(int i = 0 ; i < data.count(); i++)
    {
        QSqlQuery query( d->mDB[databaseName]);
        query.prepare(data[i]);

        if(query.exec() == false)
        {
            qDebug() << "write all" << data[i];
            CLogWriter::printLog(QString("(CDataBase::writeData) : ") + query.lastError().text(),
                                 CLogWriter::LOG_TYPE_RELEASE);

             d->mDB[databaseName].rollback();
            return false;
        }
    }

     d->mDB[databaseName].commit();
    return true;
}

QVector<QVariantMap> CDataBase::readSqlData2D(const QSqlQuery& data)
{
    QVector<QVariantMap> sqlData;
    QSqlQuery query(data);

    while(query.next())
    {
        QVariantMap rowData;

        for(int i =0 ; i < query.record().count(); i++)
        {
            auto columnName = query.record().fieldName(i);
            rowData[columnName] = query.record().value(columnName);
        }
        sqlData.push_back(rowData);
    }

    return sqlData;
}

QVector<QString> CDataBase::writeSqlInsertData2D(const QString& tableName, const QVector<QVariantMap>& data) const
{
    QVector<QString> queryList;

    for(int i = 0; i< data.count() ; i++)
    {
        auto query = this->createInsertQuery(tableName, data[i]);
        queryList.push_back(query);
    }

    return queryList;
}

QVector<QString> CDataBase::writeSqlUpdateData2D(const QString& tableName, const QVector<QVariantMap>& data, const QVector<QString>& where) const
{
    QVector<QString> queryList;

    for(int i = 0; i< data.count() ; i++)
    {
        QString whereQuery;

        for(auto itr : where)
        {
            if(whereQuery.isEmpty() == false)
                whereQuery.push_back(" AND ");

          auto uniqueColumnAndKey  =  QString("%1 = '%2' ").arg(itr).arg(data[i][itr].toString());
          whereQuery.push_back(uniqueColumnAndKey);
        }

        auto query = this->createUpdateQuery(tableName, data[i], whereQuery);
        queryList.push_back(query);
    }

    return queryList;
}

QString CDataBase::createInsertQuery(const QString& tableName, const QVariantMap& data) const
{
    QString key;
    QString value;

    QMapIterator<QString, QVariant> itr(data);

    while(itr.hasNext())
    {
        itr.next();

        if(key.isEmpty() == true)
            key.append("(");
        else
            key.append(", ");

        if(value.isEmpty() == true)
            value.append("(");
        else
            value.append(", ");

        key.append(itr.key());

        if(itr.value().type() == QVariant::String)
            value.append("'" + itr.value().toString()+"'");
        else
            value.append(itr.value().toString());
    }

    if(key.isEmpty() == false)
        key.append(") ");

    if(value.isEmpty() == false)
        value.append(") ");

    QString query = QString("INSERT INTO %1 %2 VALUES %3").arg(tableName).arg(key).arg(value);

    return query;
}

QString CDataBase::createUpdateQuery(const QString& tableName, const QVariantMap& data, const QString& where) const
{
    QString updateQuery;
    QMapIterator<QString, QVariant> itr(data);

    while(itr.hasNext())
    {
        itr.next();

        if(updateQuery.isEmpty() == false)
            updateQuery.append(", ");

        updateQuery.append(itr.key());
        updateQuery.append(" = ");

        if(itr.value().type() == QVariant::String)
            updateQuery.append("'" + itr.value().toString()+"'");
        else
            updateQuery.append(itr.value().toString());
    }

    QString query = QString("UPDATE %1 SET %2 WHERE %3").arg(tableName)
                                                          .arg(updateQuery)
                                                          .arg(where);
    return query;
}

QString CDataBase::getLastError(const QString& databaseName) const
{
    return  d->mDB[databaseName].lastError().text(); //3022
}

QString CDataBase::getLastNativeError(const QString& databaseName) const
{
    return d->mDB[databaseName].lastError().nativeErrorCode();
}

NS_SUGENTECH_END
