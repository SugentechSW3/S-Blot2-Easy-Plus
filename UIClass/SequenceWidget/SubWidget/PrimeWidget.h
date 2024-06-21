#ifndef PRIMEWIDGET_H
#define PRIMEWIDGET_H

#include "CommonSequenceWidget.h"

class Ui_PrimeWidget;
class PrimeWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit PrimeWidget(QWidget *parent = nullptr);
    virtual ~PrimeWidget();

    enum SEQUENCE_PRIME
    {
        SEQUENCE_PRIME_PUMP_CHANNEL,
        SEQUENCE_PRIME_PUMP_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);
private:
    QScopedPointer<Ui_PrimeWidget> ui;
};

#endif // PRIMEWIDGET_H
