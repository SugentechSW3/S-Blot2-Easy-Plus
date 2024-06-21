#ifndef PRIME3WIDGET_H
#define PRIME3WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_Prime3Widget;
class Prime3Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit Prime3Widget(QWidget *parent = nullptr);
    virtual ~Prime3Widget();

    enum SEQUENCE_PRIME3
    {
        SEQUENCE_PRIME3_PUMP_CHANNEL_1,
        SEQUENCE_PRIME3_PUMP_CHANNEL_2,
        SEQUENCE_PRIME3_PUMP_CHANNEL_3,
        SEQUENCE_PRIME3_PUMP_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);

private:
    QScopedPointer<Ui_Prime3Widget> ui;
};

#endif // PRIME3WIDGET_H
