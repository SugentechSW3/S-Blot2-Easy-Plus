#include "CFileio.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2018-03-19
//this code is experimental

#ifdef USE_QT

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QDirIterator>

NS_SUGENTECH_BEGIN

struct CFileIO::privateStruct
{

};

CFileIO::CFileIO(QObject *parent) :
                                    QObject(parent),
                                    d(new privateStruct)
{
}

CFileIO::~CFileIO()
{
}

void CFileIO::removeAllOldData()
{
    CFileIO::removeOneMonthAgoRawImage();
    CFileIO::removeXMLInterFaceFile();
    CFileIO::removeOldLogFile();
    CFileIO::removeOldResultFile();
}

void CFileIO::removeOneMonthAgoRawImage()
{
    QString defaultDirPath("AnalysisImage");
    CFileIO::removeOldFile(defaultDirPath, -1);
}

void CFileIO::removeXMLInterFaceFile()
{
    QString defaultDirInPath = QString("%1/Documents/Blot-2/In").arg(QDir::homePath());
    QString defaultDirOutPath = QString("%1/Documents/Blot-2/Out").arg(QDir::homePath());

    CFileIO::removeOldFile(defaultDirInPath, -1);
    CFileIO::removeOldFile(defaultDirOutPath, -1);
}

void CFileIO::removeOldLogFile()
{
    QString defaultDirPath("Log");
    CFileIO::removeOldFile(defaultDirPath, -3);
}

void CFileIO::removeOldResultFile()
{
    QString defaultDirPath("ResultImage");
    CFileIO::removeOldFile(defaultDirPath, -3);
}

void CFileIO::removeOldFile(const QString& fileDirPath, int minusMonth)
{
    QDir directory(fileDirPath);

    QDirIterator itr(directory, QDirIterator::Subdirectories);
    auto monthAgo = QDateTime::currentDateTime().addMonths(minusMonth);

    while(itr.hasNext())
    {
        itr.next();
        auto currentFileInfo = itr.fileInfo();

        if(currentFileInfo.birthTime() < monthAgo)
            QFile::remove(itr.filePath());
    }
}

NS_SUGENTECH_END

#endif /*END OF USE_QT*/



