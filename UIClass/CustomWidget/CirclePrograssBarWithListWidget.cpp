#include "CirclePrograssBarWithListWidget.h"
#include "ui_CirclePrograssBarWithListWidget.h"

#include "CircleListWidgetItem.h"

struct CirclePrograssBarWithListWidget::privateStruct
{
    QVector<CircleListWidgetItem*> mItemList;
};

CirclePrograssBarWithListWidget::CirclePrograssBarWithListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_CirclePrograssBarWithListWidget),
    d(new privateStruct)
{
    ui->setupUi(this);
}

CirclePrograssBarWithListWidget::~CirclePrograssBarWithListWidget()
{
}

void CirclePrograssBarWithListWidget::addItem(const QString& labelText, bool isHideCircle)
{
    auto listWidgetItem = new CircleListWidgetItem(this);
    listWidgetItem->setLabelText(labelText);

    if(isHideCircle == true)
        listWidgetItem->hideCircle();

    ui->verticalLayoutItem->addWidget(listWidgetItem);

    d->mItemList.push_back(listWidgetItem);
}

void CirclePrograssBarWithListWidget::removeItem(int idx)
{
    if(d->mItemList.count() == 0)
        return;

    if(d->mItemList.count() <= idx)
        return;

    if(idx < 0)
        return;

    delete d->mItemList.takeAt(idx);
}

void CirclePrograssBarWithListWidget::clearItem()
{
    while(d->mItemList.count() != 0)
        this->removeItem(0);
}

CircleListWidgetItem* CirclePrograssBarWithListWidget::getItem(int idx)
{
    if(d->mItemList.count() <= 0)
        return nullptr;

    if(d->mItemList.count() <= idx)
        return nullptr;

    if(idx < 0)
        return nullptr;

    return d->mItemList[idx];
}

int CirclePrograssBarWithListWidget::getCount()
{
    return d->mItemList.count();
}
