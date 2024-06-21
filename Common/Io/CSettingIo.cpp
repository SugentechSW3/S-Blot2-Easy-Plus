#include <QCoreApplication>
#include <QSettings>
#include <QStringList>
#include <QDir>
#include <QMutex>

#include "CSettingIo.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2018-03-19
#ifdef USE_QT
NS_SUGENTECH_BEGIN

struct CSettingIo::privateStruct
{
    QScopedPointer<QSettings> mSettings;
    QString mPath;
    QString mFileName;
};

CSettingIo::CSettingIo(const bool& dirMake, QString fullPath,  QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    QString path(fullPath);
    QStringList pathList = fullPath.split("/");

    if(path.contains(".ini") == false)
        path.append(".ini");

    if(dirMake == true)
    {
        QString dirPath;

        for(int i = 0; i < pathList.count() - 1; i++)
            dirPath.append(pathList.at(i));

        QDir dir;
        dir.mkpath(dirPath);
    }

    d->mSettings.reset(new QSettings(path, QSettings::IniFormat, this));
    d->mSettings->setIniCodec("UTF-8");
    d->mPath = path;
    d->mFileName = pathList.last();
}

CSettingIo::CSettingIo(QString fileName, QString addtionalDirPath, QString elementText, QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    //InI포맷으로 저장 + 로컬 설정 적용
//    QString dirPath(QDir::currentPath()+"/Config");
    QString dirPath("Config");

    if(addtionalDirPath.isEmpty() == false)
    {
        dirPath.append("/");
        dirPath.append(addtionalDirPath);
    }

    if(elementText.isEmpty() == false)
    {
        dirPath.append("/");
        dirPath.append(elementText);
    }

    QDir dir;
    dir.mkpath(dirPath);

    QString path = dirPath +"/"+ fileName;

    if(path.contains(".ini") == false)
        path.append(".ini");

    d->mSettings.reset(new QSettings(path,QSettings::IniFormat, this));
    d->mSettings->setIniCodec("UTF-8");
    d->mPath = path;
    d->mFileName = fileName;
}

CSettingIo::~CSettingIo()
{
}

void CSettingIo::createDir(QString dirPath)
{
    QDir dir;
    dir.mkpath(dirPath);
}

void CSettingIo::setConfigureData(QString mainCategory, QString key, QVariant value)
{
    QString category = QString("%1/%2").arg(mainCategory).arg(key);
    d->mSettings->setValue(category, value);
}

QVariant CSettingIo::getConfigureData(QString mainCategory, QString key)
{
    QString keyString = QString("%1/%2").arg(mainCategory).arg(key);
    auto ret = d->mSettings->value(keyString);

    return ret;
}

QString CSettingIo::getPath()
{
    return d->mPath;
}

QString CSettingIo::getFileName()
{
    return d->mFileName;
}

void CSettingIo::clear()
{
    d->mSettings->clear();
}

NS_SUGENTECH_END

#endif
