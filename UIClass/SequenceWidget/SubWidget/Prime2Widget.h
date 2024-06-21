#ifndef PRIME2WIDGET_H
#define PRIME2WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_Prime2Widget;
class Prime2Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit Prime2Widget(QWidget *parent = nullptr);
    virtual ~Prime2Widget();

    enum SEQUENCE_PRIME2
    {
        SEQUENCE_PRIME2_PUMP_CHANNEL_1,
        SEQUENCE_PRIME2_PUMP_CHANNEL_2,
        SEQUENCE_PRIME2_PUMP_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);

private:
    QScopedPointer<Ui_Prime2Widget> ui;
};

#endif // PRIME2WIDGET_H
