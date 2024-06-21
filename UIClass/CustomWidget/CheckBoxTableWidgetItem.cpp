#include <QCheckBox>
#include "CheckBoxTableWidgetItem.h"

#include "Common/CommonParentWidget/CCheckBoxParentWidget.h"

struct CheckBoxTableWidgetItem::privateStruct
{
    CCheckBoxParentWidget* mCheckBox = nullptr;
};


CheckBoxTableWidgetItem::CheckBoxTableWidgetItem(int row, int column, QWidget *parent) :
    CustomTableWidgetItem(row, column, parent),
    d(new privateStruct)
{
    d->mCheckBox = new CCheckBoxParentWidget(this);
    this->addWidget(d->mCheckBox);

    connect(d->mCheckBox, SIGNAL(stateChanged(int)), SLOT(onCheckStateChanged(int)));
}

CheckBoxTableWidgetItem::~CheckBoxTableWidgetItem()
{

}

bool CheckBoxTableWidgetItem::isChecked()
{
    return d->mCheckBox->isChecked();
};

void CheckBoxTableWidgetItem::setCheckBox(bool isCheck)
{
    d->mCheckBox->setChecked(isCheck);
}

void CheckBoxTableWidgetItem::onCheckStateChanged(int)
{
    this->onChanged();
}
