#ifndef ROLLBACK2WIDGET_H
#define ROLLBACK2WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_RollBack2Widget;
class RollBack2Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit RollBack2Widget(QWidget *parent = nullptr);
    virtual ~RollBack2Widget();

    enum SEQUENCE_ROLL_BACK_2
    {
        SEQUENCE_ROLL_BACK_2_PUMP_CHANNEL_1,
        SEQUENCE_ROLL_BACK_2_PUMP_CHANNEL_2,
        SEQUENCE_ROLL_BACK_2_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_RollBack2Widget> ui;
};

#endif // ROLLBACK2WIDGET_H
