#ifndef DISPASP3WIDGET_H
#define DISPASP3WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_DispAsp3Widget;
class DispAsp3Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit DispAsp3Widget(QWidget *parent = nullptr);
    virtual ~DispAsp3Widget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_DispAsp3Widget> ui;
};

#endif // DISPASP3WIDGET_H
