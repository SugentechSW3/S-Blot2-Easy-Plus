#ifndef RESULTPRINTV5WIDGETLABELTABLEITEM_H
#define RESULTPRINTV5WIDGETLABELTABLEITEM_H

#include <QLabel>

class ResultPrintV5WidgetLabelTableItem : public QLabel
{
    Q_OBJECT
public:
    explicit ResultPrintV5WidgetLabelTableItem(const QString &text, QWidget *parent = nullptr);
    ~ResultPrintV5WidgetLabelTableItem();

private:
    void initStyleSheet();
};

#endif // RESULTPRINTV5WIDGETLABELTABLEITEM_H
