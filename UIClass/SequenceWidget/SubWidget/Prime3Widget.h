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

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);

private:
    QScopedPointer<Ui_Prime3Widget> ui;
};

#endif // PRIME3WIDGET_H
