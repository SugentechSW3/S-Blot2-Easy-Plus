#ifndef CDATABASE_H
#define CDATABASE_H

#include "Common/CUtilCommon.h"

#ifdef USE_QT

#include <QObject>
#include <QScopedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>

NS_SUGENTECH_BEGIN

class CDataBase : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(CDataBase)

    struct privateStruct;

    explicit CDataBase(QObject *parent = nullptr);
    virtual ~CDataBase();


public:
    static CDataBase* getInstance()
    {
        static CDataBase instance;
        return &instance;
    }

    QSqlQuery readData(const QString& databaseName, const QString& sql);
    bool writeData(const QString& databaseName, const QString& sql);
    bool writeDuplicatedValue(const QString& databaseName,const QString& sql);
    bool writeAllData(const QString& databaseName, const QVector<QString>& data);

    QVector<QVariantMap> readSqlData2D(const QSqlQuery& data);
    QVector<QString> writeSqlInsertData2D(const QString& tableName, const QVector<QVariantMap>& data) const;
    QVector<QString> writeSqlUpdateData2D(const QString& tableName, const QVector<QVariantMap>& data, const QVector<QString>& where) const;

#if USE_SQL_LITE
    bool openDataBase(const QString& databaseName);
#elif USE_SQL_ACCESS
    bool openDataBase(const QString& databaseName);
    bool openDataBase(const QString& databaseName, const QString &password);
#endif

    QString createInsertQuery(const QString& tableName, const QVariantMap& data) const;
    QString createUpdateQuery(const QString& tableName, const QVariantMap& data, const QString &where) const;

    QString getLastError(const QString& databaseName) const;
    QString getLastNativeError(const QString& databaseName) const;

private:
    QScopedPointer<privateStruct> d;
};

NS_SUGENTECH_END

#endif/* USE_QT */

#endif // CDATABASE_H
