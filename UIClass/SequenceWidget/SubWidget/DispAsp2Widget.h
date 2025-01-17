#ifndef DISPASP2WIDGET_H
#define DISPASP2WIDGET_H

#include "CommonSequenceWidget.h"

class Ui_DispAsp2Widget;
class DispAsp2Widget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit DispAsp2Widget(QWidget *parent = nullptr);
    virtual ~DispAsp2Widget();

    enum SEQUENCE_DISP_ASP_2
    {
        SEQUENCE_DISP_ASP_2_PUMP1,
        SEQUENCE_DISP_ASP_2_PUMP2,
        SEQUENCE_DISP_ASP_2_VOLUME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_DispAsp2Widget> ui;
};

#endif // DISPASP2WIDGET_H
