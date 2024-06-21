#ifndef CPRINTSUPPORT_H
#define CPRINTSUPPORT_H

#include "Common/CUtilCommon.h"

#ifdef USE_QT

#include <QThread>
#include <QPicture>

NS_SUGENTECH_BEGIN
class CPrintSupport : public QThread
{
private:
    Q_OBJECT
    struct privateStruct;
    void run() override;

public:
    explicit CPrintSupport(QObject *parent = nullptr);
    virtual ~CPrintSupport();

    void setPrintData(const QVector<QPicture>& data, const int printPage = 1);

private:
    QScopedPointer<privateStruct> d;
};
NS_SUGENTECH_END

#else

#endif /* END OF USE_QT */

#endif // CPRINTSUPPORT_H
