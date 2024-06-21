#include <QHBoxLayout>
#include <QKeyEvent>
#include "CustomTableWidgetItem.h"

struct CustomTableWidgetItem::privateStruct
{
    QHBoxLayout* mLayout = nullptr;
    int mColumn = 0;
    int mRow = 0;
};


CustomTableWidgetItem::CustomTableWidgetItem(int row, int column, QWidget *parent) :
    QWidget(parent),
    d(new privateStruct)
{
    d->mRow = row;
    d->mColumn = column;

    d->mLayout = new QHBoxLayout(this);
    d->mLayout->setAlignment(Qt::AlignCenter);
    d->mLayout->setContentsMargins(0,0,0,0);
}

CustomTableWidgetItem::~CustomTableWidgetItem()
{

}

void CustomTableWidgetItem::addWidget(QWidget* widget)
{
    d->mLayout->addWidget(widget);
}

void CustomTableWidgetItem::onChanged()
{
    emit onItemChanged(d->mRow, d->mColumn);
}

int CustomTableWidgetItem::getCurrentRow()
{
    return d->mRow;
}

void CustomTableWidgetItem::getCurrentIndex(int& row, int& column )
{
    column = d->mColumn;
    row = d->mRow;
}

void CustomTableWidgetItem::setCurrentIndex(int row, int column)
{
    d->mColumn = column;
    d->mRow = row;
}
