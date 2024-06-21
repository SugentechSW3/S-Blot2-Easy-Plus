#ifndef PLACEHOLDERTABLEWIDGETITEM_H
#define PLACEHOLDERTABLEWIDGETITEM_H

#include "UIClass/CustomWidget/CustomTableWidgetItem.h"

class PlaceHolderTableWidgetItem : public CustomTableWidgetItem
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit PlaceHolderTableWidgetItem(int row, int column, QWidget *parent = nullptr);
    virtual ~PlaceHolderTableWidgetItem();


    QString getText();
    void setText(const QString& text);

    QString getPlaceHolderText();
    void setPlaceHolderText(const QString& text);

    void setFrameless(bool isFrameless);

private slots:
    void onChangedText(QString);

private:
    QScopedPointer<privateStruct> d;
};

#endif // PLACEHOLDERTABLEWIDGETITEM_H
