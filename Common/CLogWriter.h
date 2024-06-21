#ifndef C_LOGWRITER_H
#define C_LOGWRITER_H

#include "CUtilCommon.h"

#include <QString>
#include <QFile>
#include <QMutex>

NS_SUGENTECH_BEGIN

class CLogWriter
{
public:
    enum LOG_TYPE
    {
        LOG_TYPE_HIDE = 0,
        LOG_TYPE_DEBUG,
        LOG_TYPE_RELEASE
    };

    explicit CLogWriter();
    virtual ~CLogWriter();

    static QString createDirectory();
    static void printLog(QString logMsg, CLogWriter::LOG_TYPE type = LOG_TYPE_DEBUG, QString fileName = QString(""));
    static void printLotLog(QString logMsg);

private:
     static QMap<QString, QFile*> mFile;
     static QMutex mMutex;
};

NS_SUGENTECH_END

#endif // CLOGWRITER_H
