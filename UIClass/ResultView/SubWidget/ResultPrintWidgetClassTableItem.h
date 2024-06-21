#ifndef RESULTPRINTWIDGETCLASSTABLEITEM_H
#define RESULTPRINTWIDGETCLASSTABLEITEM_H

#include <QWidget>

class Ui_ResultPrintWidgetClassTableItem;
class ResultPrintWidgetClassTableItem : public QWidget
{
    Q_OBJECT

public:
    explicit ResultPrintWidgetClassTableItem(QWidget *parent = nullptr);
    ~ResultPrintWidgetClassTableItem();

    void setClass(const QString& classData);
    void setClassTextColor(const QString &hexColorCode);
    void setClassReportV5(const QString& classData);

private:
    QString getLabelClassStyleSheet(QString bgColor);

private:
    QScopedPointer<Ui_ResultPrintWidgetClassTableItem> ui;
};

#endif // RESULTPRINTWIDGETCLASSTABLEITEM_H
