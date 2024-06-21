#include "UIClass/CustomWidget/SpinRemoveUpDown.h"
#include "SpinTableWidgetItem.h"

struct SpinTableWidgetItem::privateStruct
{
    SpinRemoveUpDown* mSpinBox = nullptr;
};

SpinTableWidgetItem::SpinTableWidgetItem(int row, int column, QWidget *parent) :
                                                                                 CustomTableWidgetItem(row, column, parent),
                                                                                 d(new privateStruct)
{
    d->mSpinBox = new SpinRemoveUpDown(this);
    d->mSpinBox->setWheelEventDisable(true);

    this->addWidget(d->mSpinBox);
    this->initSignalSlots();
}

SpinTableWidgetItem::~SpinTableWidgetItem()
{

}

void SpinTableWidgetItem::initSignalSlots()
{
    connect(d->mSpinBox, SIGNAL(valueChanged(int)), SLOT(onValueChanged(int)));
    connect(d->mSpinBox, SIGNAL(onPressedArrowKeyBoard(int)), SLOT(onMoveChanged(int)));
    connect(d->mSpinBox, &SpinRemoveUpDown::onFocusInEventSpinBox, this, &SpinTableWidgetItem::onFocusInEventSpinBoxCallBack);
}

void SpinTableWidgetItem::onValueChanged(int)
{
    this->onChanged();
}

int SpinTableWidgetItem::getSpinValue()
{
    return d->mSpinBox->value();
}

void SpinTableWidgetItem::setSpinValue(int value)
{
    d->mSpinBox->setValue(value);
}

void SpinTableWidgetItem::setSpinMaximumValue(int value)
{
    d->mSpinBox->setMaximum(value);
}

void SpinTableWidgetItem::setSpinMinimumValue(int value)
{
    d->mSpinBox->setMinimum(value);
}

int SpinTableWidgetItem::getSpinMaximumValue()
{
    return d->mSpinBox->maximum();
}

int SpinTableWidgetItem::getSpinMinimumValue()
{
    return d->mSpinBox->minimum();
}

void SpinTableWidgetItem::setSpinEditFocus()
{
    d->mSpinBox->setFocus();
}

void SpinTableWidgetItem::onMoveChanged(int qtKey)
{
    emit onEditEnd(qtKey);
}

void SpinTableWidgetItem::onFocusInEventSpinBoxCallBack()
{
    int row = 0;
    int column = 0;

    this->getCurrentIndex(row, column);

    emit onSelectedThisWidget(row, column);
}
