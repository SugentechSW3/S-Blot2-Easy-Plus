#ifndef CPACKETTIMER_H
#define CPACKETTIMER_H
#include "Common/CUtilCommon.h"

#ifdef USE_QT
#include <QTimer>
#include <QScopedPointer>

NS_SUGENTECH_BEGIN

class CPacketTimer : public QTimer
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CPacketTimer(QObject* parent = nullptr);
    virtual ~CPacketTimer();

    void startPacket(int msec);
    void stopPacket();
    bool isSended();

signals:
    void packetTimeOut();

private slots:
    void timeOutPacket();

private:
    QScopedPointer<CPacketTimer::privateStruct> d;

};

NS_SUGENTECH_END


#endif /* END OF USE_QT */

#endif // CPACKETTIMER_H
