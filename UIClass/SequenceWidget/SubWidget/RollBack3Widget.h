#ifndef ROLLBACK3WIDGET_H
#define ROLLBACK3WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_RollBack3Widget;
class RollBack3Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit RollBack3Widget(QWidget *parent = nullptr);
    virtual ~RollBack3Widget();

    enum SEQUENCE_ROLL_BACK_3
    {
        SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_1,
        SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_2,
        SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_3,
        SEQUENCE_ROLL_BACK_3_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_RollBack3Widget> ui;
};

#endif // ROLLBACK3WIDGET_H
