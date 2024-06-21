#ifndef INCUBATIONWSWIDGET_H
#define INCUBATIONWSWIDGET_H

#include "CommonSequenceWidget.h"


class Ui_IncubationWSWidget;
class IncubationWSWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit IncubationWSWidget(QWidget *parent = nullptr);
    virtual ~IncubationWSWidget();


    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);
private:
    QScopedPointer<Ui_IncubationWSWidget> ui;
};

#endif // INCUBATIONWSWIDGET_H
