#ifndef INCUBATIONWIDGET_H
#define INCUBATIONWIDGET_H

#include "CommonSequenceWidget.h"

class Ui_IncubationWidget;
class IncubationWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit IncubationWidget(QWidget *parent = nullptr);
    virtual ~IncubationWidget();

    enum SEQUENCE_INCUBATION
    {
        SEQUENCE_INCUBATION_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);
private:
    QScopedPointer<Ui_IncubationWidget> ui;
};

#endif // INCUBATIONWIDGET_H
