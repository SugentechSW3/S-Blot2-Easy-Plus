#ifndef CUSTOMTABLEWIDGETITEM_H
#define CUSTOMTABLEWIDGETITEM_H

#include <QWidget>

class CustomTableWidgetItem : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit CustomTableWidgetItem(int row, int column, QWidget *parent = nullptr);
    virtual ~CustomTableWidgetItem();


    int getCurrentRow();
    void getCurrentIndex(int& row, int& column );
    void setCurrentIndex(int row, int column);

signals:
    void onItemChanged(int row, int column);
    void onEnterKeyData();

protected:
    void addWidget(QWidget* widget);

protected slots:
    void onChanged();

private:
    QScopedPointer<privateStruct> d;
};

#endif // CUSTOMTABLEWIDGETITEM_H
