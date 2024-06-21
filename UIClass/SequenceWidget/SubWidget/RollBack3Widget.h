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

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_RollBack3Widget> ui;
};

#endif // ROLLBACK3WIDGET_H
