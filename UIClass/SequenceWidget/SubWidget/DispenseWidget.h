#ifndef DISPENSEWIDGET_H
#define DISPENSEWIDGET_H

#include "CommonSequenceWidget.h"

class Ui_DispenseWidget;
class DispenseWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit DispenseWidget(QWidget *parent = nullptr);
    virtual ~DispenseWidget();

    enum SEQUENCE_DISPENSE
    {
        SEQUENCE_DISPENSE_PUMP_CHANNEL,
        SEQUENCE_DISPENSE_PUMP_VOLUME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_DispenseWidget> ui;
};

#endif // DISPENSEWIDGET_H
