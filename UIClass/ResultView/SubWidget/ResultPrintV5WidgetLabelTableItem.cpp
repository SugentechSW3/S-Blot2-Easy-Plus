#include "ResultPrintV5WidgetLabelTableItem.h"

ResultPrintV5WidgetLabelTableItem::ResultPrintV5WidgetLabelTableItem(const QString &text, QWidget *parent) :
                                                                                        QLabel(parent)
{
    this->setText(text);
    this->initStyleSheet();
}

ResultPrintV5WidgetLabelTableItem::~ResultPrintV5WidgetLabelTableItem()
{

}

void ResultPrintV5WidgetLabelTableItem::initStyleSheet()
{
    QString styleString = QString("QLabel { font-size:6pt; font-family:Arial; background-color : #F2F2F2; qproperty-alignment: AlignCenter; }");
    this->setStyleSheet(styleString);
}
