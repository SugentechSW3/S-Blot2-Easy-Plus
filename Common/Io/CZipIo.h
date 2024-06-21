#ifndef CZIPIO_H
#define CZIPIO_H

#include "Common/CUtilCommon.h"

#ifdef USE_QT

#include <QObject>



NS_SUGENTECH_BEGIN

class CZipIo : public QObject
{
    Q_OBJECT
public:
    explicit CZipIo(QObject *parent = nullptr);
    virtual ~CZipIo();

    static void zip(const QString& fileFullPath , const QString& zipFullPath);
    static void unZip(const QString& zipFullPath , const QString &unZipFullPath);
};

NS_SUGENTECH_END

#endif /* END OF USE_QT */

#endif // CZIPIO_H
