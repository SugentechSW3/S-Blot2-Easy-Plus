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

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);
private:
    QScopedPointer<Ui_PrimeWidget> ui;
};

#endif // PRIMEWIDGET_H
