#ifndef CUSTOMDELAYBUTTONWIDGET_H
#define CUSTOMDELAYBUTTONWIDGET_H

#include "UIClass/CustomWidget/CustomImageButtonWidget.h"

class CustomDelayButtonWidget : public CustomImageButtonWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CustomDelayButtonWidget(QWidget *parent = nullptr);
    ~CustomDelayButtonWidget();


    void setEnabledChange();
    void setDisabled(bool);

    void saveButtonText();

private:
    void setDelayTime(int msDelayTime);
    void showRemainTime();

private:
    QScopedPointer<privateStruct> d;
};

#endif // CUSTOMDELAYBUTTONWIDGET_H
