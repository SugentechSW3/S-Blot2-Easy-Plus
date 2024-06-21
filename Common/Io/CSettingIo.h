#ifndef SETTINGIO_H
#define SETTINGIO_H

#include "Common/CUtilCommon.h"

#ifdef USE_QT

#include <QObject>
#include <QScopedPointer>
#include <QMap>
#include <QString>
#include <QVariant>


NS_SUGENTECH_BEGIN

class CSettingIo : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CSettingIo(const bool& dirMake, QString fullPath, QObject *parent = nullptr);
    explicit CSettingIo(QString fileName = QString("Config"), QString addtionalDirPath = QString(""), QString elementText = QString(""), QObject *parent = nullptr);
    virtual ~CSettingIo();


    static void createDir(QString dirPath);

    QVariant getConfigureData(QString mainCategory, QString key);
    void setConfigureData(QString mainCategory, QString key, QVariant value);

    QString getPath();
    QString getFileName();

    void clear();
private:
    QScopedPointer<privateStruct> d;

};

NS_SUGENTECH_END

#endif /* END OF USE QT */

#endif // SETTINGIO_H
