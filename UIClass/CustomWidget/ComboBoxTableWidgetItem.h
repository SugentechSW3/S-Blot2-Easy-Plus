#ifndef COMBOBOXTABLEWIDGETITEM_H
#define COMBOBOXTABLEWIDGETITEM_H

#include "UIClass/CustomWidget/CustomTableWidgetItem.h"

class ComboBoxTableWidgetItem : public CustomTableWidgetItem
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ComboBoxTableWidgetItem(int row, int column, QWidget *parent = nullptr);
    virtual ~ComboBoxTableWidgetItem();


    void setCurrentComboBoxIndex(const QString& text);
    void setCurrentComboBoxIndex(int idx);
    QString getCurrentComboBoxText();
    int getCurrentComboBoxIndex();

    void addItem(const QString& itemName);
    void addItem(const QString& item, const QVariant& data);
    void addItems(const QStringList& itemList);

    void removeItem(int index);
    void clearItems();

    void setWheelEventDisable(bool isDisable);

    void setComboBoxMinimumWidth(int width);

signals:
    void onActivatedItem(int row);

private slots:
    void onIndexChanged(int);
    void onClickedItem(int);

private:
    void initSignalSlots();

    virtual void wheelEvent(QWheelEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *e) override;

private:
    QScopedPointer<privateStruct> d;

};

#endif // COMBOBOXTABLEWIDGETITEM_H
