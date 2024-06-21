#ifndef CHECKBOXTABLEWIDGETITEM_H
#define CHECKBOXTABLEWIDGETITEM_H

#include "UIClass/CustomWidget/CustomTableWidgetItem.h"

class CheckBoxTableWidgetItem : public CustomTableWidgetItem
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CheckBoxTableWidgetItem(int row, int column, QWidget *parent = nullptr);
    virtual ~CheckBoxTableWidgetItem();

    bool isChecked();
    void setCheckBox(bool isCheck);

private slots:
    void onCheckStateChanged(int);

private:
    QScopedPointer<privateStruct> d;
};

#endif // CHECKBOXTABLEWIDGETITEM_H
