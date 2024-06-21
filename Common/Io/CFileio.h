#ifndef CFILEIO_H
#define CFILEIO_H

#include "Common/CUtilCommon.h"

#ifdef USE_QT

#include <QScopedPointer>
#include <QVector>
#include <QByteArray>


NS_SUGENTECH_BEGIN

class CFileIO  : public QObject
{
    Q_OBJECT
private:
    struct privateStruct;

public:
    explicit  CFileIO(QObject *parent = nullptr);
    virtual ~CFileIO();

    static void removeAllOldData();
    static void removeOneMonthAgoRawImage();
    static void removeXMLInterFaceFile();
    static void removeOldLogFile();
    static void removeOldResultFile();

private:
    static void removeOldFile(const QString& fileDirPath, int minusMonth);

private:
    QScopedPointer<privateStruct> d;


};
NS_SUGENTECH_END

#else

#endif /* END OF USE_QT */


#endif // CFILEIO_H
