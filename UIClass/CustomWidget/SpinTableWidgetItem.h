#ifndef SPINTABLEWIDGETITEM_H
#define SPINTABLEWIDGETITEM_H

#include "UIClass/CustomWidget/CustomTableWidgetItem.h"

class SpinTableWidgetItem : public CustomTableWidgetItem
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit SpinTableWidgetItem(int row, int column, QWidget *parent = nullptr);
    virtual ~SpinTableWidgetItem();

    int getSpinValue();
    void setSpinValue(int value);

    void setSpinMaximumValue(int value);
    void setSpinMinimumValue(int value);

    int getSpinMaximumValue();
    int getSpinMinimumValue();

    void setSpinEditFocus();

signals:
    void onEditEnd(int qtKey);
    void onSelectedThisWidget(int row, int column);

private slots:
    void onValueChanged(int);
    void onMoveChanged(int qtKey);
    void onFocusInEventSpinBoxCallBack();

private:
    void initSignalSlots();

private:
    QScopedPointer<privateStruct> d;
};

#endif // SPINTABLEWIDGETITEM_H
