#ifndef DISPENSE2WIDGET_H
#define DISPENSE2WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_Dispense2Widget;
class Dispense2Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit Dispense2Widget(QWidget *parent = nullptr);
    virtual ~Dispense2Widget();

    enum SEQUENCE_DISPENSE2
    {
        SEQUENCE_DISPENSE2_PUMP_CHANNEL_1,
        SEQUENCE_DISPENSE2_PUMP_CHANNEL_2,
        SEQUENCE_DISPENSE2_PUMP_VOLUME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
   QScopedPointer<Ui_Dispense2Widget> ui;
};

#endif // DISPENSE2WIDGET_H
