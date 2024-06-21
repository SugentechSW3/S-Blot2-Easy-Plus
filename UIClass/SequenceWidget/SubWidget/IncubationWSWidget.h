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

    enum SEQUENCE_INCUBATION_WS
    {
        SEQUENCE_INCUBATION_WS_TIME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap& parameter);
private:
    QScopedPointer<Ui_IncubationWSWidget> ui;
};

#endif // INCUBATIONWSWIDGET_H
