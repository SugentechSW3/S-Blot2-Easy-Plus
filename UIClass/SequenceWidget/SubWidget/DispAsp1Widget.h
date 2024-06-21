#ifndef DISPASP1WIDGET_H
#define DISPASP1WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_DispAsp1Widget;
class DispAsp1Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit DispAsp1Widget(QWidget *parent = nullptr);
    virtual ~DispAsp1Widget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_DispAsp1Widget> ui;
};

#endif // DISPASP1WIDGET_H
