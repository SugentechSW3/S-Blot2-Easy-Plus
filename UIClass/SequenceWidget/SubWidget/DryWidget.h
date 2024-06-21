#ifndef DRYWIDGET_H
#define DRYWIDGET_H

#include "CommonSequenceWidget.h"

class Ui_DryWidget;
class DryWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit DryWidget(QWidget *parent = nullptr);
    virtual ~DryWidget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);
private:
    QScopedPointer<Ui_DryWidget> ui;
};

#endif // DRYWIDGET_H
