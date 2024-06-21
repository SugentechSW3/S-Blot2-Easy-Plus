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

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
   QScopedPointer<Ui_Dispense2Widget> ui;
};

#endif // DISPENSE2WIDGET_H
