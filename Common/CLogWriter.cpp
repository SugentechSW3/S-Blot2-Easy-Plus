#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>

#include "CLogWriter.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2017-08-17
// Modify Date  : 2018-08-07
// Modify Date  : 2019-06-10

NS_SUGENTECH_BEGIN

QMap<QString, QFile*> CLogWriter::mFile;
QMutex CLogWriter::mMutex;

CLogWriter::CLogWriter()
{

}

CLogWriter::~CLogWriter()
{
    for(auto itr : mFile)
    {
        if(itr == nullptr)
            continue;

        if(itr->isOpen() == true)
            itr->close();

        itr->deleteLater();
    }
}

QString CLogWriter::createDirectory()
{
    QString path("Log");
    static bool isCreated = false;

    if(isCreated == false)
    {
        QDir dir;
        dir.mkpath(path);
        isCreated = true;
    }

    return path;
}

void CLogWriter::printLog(QString logMsg, CLogWriter::LOG_TYPE type, QString fileName)
{
    static auto logDirPath = CLogWriter::createDirectory();
    static QString lastDate = QDate::currentDate().toString("yyyy-MM-dd");

    auto filePtr = mFile[fileName];

    if(filePtr == nullptr)
    {
        filePtr = new QFile(nullptr);
        mFile[fileName] = filePtr;
    }

    mMutex.lock();

    if(filePtr->isOpen() == false)
    {
        QString fullPath;
        fullPath.append(logDirPath);
        fullPath.append("/");

        if(fileName.isEmpty() == true)
            fullPath.append(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss").append(".txt"));
        else
            fullPath.append(fileName.append(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")).append(".txt")); //기본 로그가 아닌경우 최초 생성된 날자고정

        filePtr->setFileName(fullPath);

        if(filePtr->open(QIODevice::WriteOnly) == false)
        {
            mMutex.unlock();
            return;
        }
    }

    //파일명 없는 기본 로그 기록의 경우 날자가 변경되면 1일단위로 새파일을 생성
    if(lastDate != QDate::currentDate().toString("yyyy-MM-dd"))
    {
        auto logControlObject = mFile[""];

        if(logControlObject == nullptr)
        {
            logControlObject = new QFile(nullptr);
            mFile[""]  = logControlObject;
        }

        if(logControlObject->isOpen() == true)
            logControlObject->close();

        lastDate = QDate::currentDate().toString("yyyy-MM-dd");

        QString fullPath;
        fullPath.append(logDirPath);
        fullPath.append("/");
        fullPath.append(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss").append(".txt"));
        logControlObject->setFileName(fullPath);

        if(logControlObject->open(QIODevice::WriteOnly) == false)
        {
            mMutex.unlock();
            return;
        }
    }

    QTextStream in(filePtr);
    in << QString("[%1] / %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(logMsg)<< "\r\n";
    in.flush();

    mMutex.unlock();
}

void CLogWriter::printLotLog(QString logMsg)
{
    static auto logDirPath = CLogWriter::createDirectory();
    static QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");

    QString fullPath;
    fullPath.append(logDirPath);
    fullPath.append("/");
    fullPath.append("LOT-").append(currentDate).append(".txt");

    QFile file(fullPath);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        CLogWriter::printLog("[printLotLog] Lot log save fail", CLogWriter::LOG_TYPE_RELEASE);
        return;
    }

    QTextStream out(&file);
    out << QString("[%1] / %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(logMsg)<< "\r\n";
    file.flush();
    file.close();
}

NS_SUGENTECH_END

