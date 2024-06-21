#include <QString>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QCoreApplication>
#include "CameraConfigWatcher.h"

struct CameraConfigWatcher::privateStruct
{
    QFileSystemWatcher mWatcher;
    QString mConfigDirPath;
};

CameraConfigWatcher::CameraConfigWatcher(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{

#ifndef QT_DEBUG
    d->mConfigDirPath = QCoreApplication::applicationDirPath() + "/" + "Config";
#else
    d->mConfigDirPath = QCoreApplication::applicationDirPath().remove("/debug") + "/" + "Config";
#endif /* end of QT_DEBUG */

    connect(&d->mWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
}

CameraConfigWatcher::~CameraConfigWatcher()
{

}

void CameraConfigWatcher::onFileChanged(QString path)
{
    QString fileNameStr = path.split("/").last();
    auto fileName = fileNameStr.remove(".ini");

    emit onConfigFileChanged(fileName);
}


void CameraConfigWatcher::addWatchFile(QString fileName)
{
    d->mWatcher.addPath(d->mConfigDirPath + "/" + fileName + ".ini");
}
