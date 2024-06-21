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

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);

private:
    QScopedPointer<Ui_Prime2Widget> ui;
};

#endif // PRIME2WIDGET_H
