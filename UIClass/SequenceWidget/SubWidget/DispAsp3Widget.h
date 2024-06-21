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


    enum SEQUENCE_DISP_ASP_3
    {
        SEQUENCE_DISP_ASP_3_PUMP1,
        SEQUENCE_DISP_ASP_3_PUMP2,
        SEQUENCE_DISP_ASP_3_PUMP3,
        SEQUENCE_DISP_ASP_3_VOLUME
    };

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_DispAsp3Widget> ui;
};

#endif // DISPASP3WIDGET_H
