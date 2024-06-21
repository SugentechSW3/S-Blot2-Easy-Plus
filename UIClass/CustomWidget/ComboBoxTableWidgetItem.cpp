#include <QComboBox>
#include <QWheelEvent>
#include "CustomComboBox.h"
#include "ComboBoxTableWidgetItem.h"

struct ComboBoxTableWidgetItem::privateStruct
{
    CustomComboBox* mComboBox = nullptr;
    bool mIsWheelDisalbled = false;
};

ComboBoxTableWidgetItem::ComboBoxTableWidgetItem(int row, int column, QWidget *parent) :
    CustomTableWidgetItem(row, column, parent),
    d(new privateStruct)
{
    d->mComboBox = new CustomComboBox(this);
    d->mComboBox->installEventFilter(this);

    this->addWidget(d->mComboBox);
    this->initSignalSlots();
    d->mComboBox->setMinimumHeight(25);
}

ComboBoxTableWidgetItem::~ComboBoxTableWidgetItem()
{

}

void ComboBoxTableWidgetItem::addItem(const QString& itemName)
{
    d->mComboBox->addItemForPanelColorText(itemName);
}

void ComboBoxTableWidgetItem::addItem(const QString& item, const QVariant& data)
{
    d->mComboBox->addItemForPanelColorText(item, data);
}

void ComboBoxTableWidgetItem::addItems(const QStringList& itemList)
{    
    d->mComboBox->addItemForPanelColorText(itemList);
}

void ComboBoxTableWidgetItem::removeItem(int index)
{
    d->mComboBox->removeItem(index);
}

void ComboBoxTableWidgetItem::clearItems()
{
    d->mComboBox->clear();
}

void ComboBoxTableWidgetItem::setCurrentComboBoxIndex(const QString& text)
{
    d->mComboBox->setCurrentText(text);
}

void ComboBoxTableWidgetItem::setCurrentComboBoxIndex(int idx)
{
    d->mComboBox->setCurrentIndex(idx);
}

QString ComboBoxTableWidgetItem::getCurrentComboBoxText()
{
    return d->mComboBox->currentText();
}

int ComboBoxTableWidgetItem::getCurrentComboBoxIndex()
{
    return d->mComboBox->currentIndex();
}

void ComboBoxTableWidgetItem::onIndexChanged(int)
{
    this->onChanged();
}

void ComboBoxTableWidgetItem::onClickedItem(int)
{
    int column = 0;
    int row = 0;

    this->getCurrentIndex(row, column);
    emit onActivatedItem(row);
}

void ComboBoxTableWidgetItem::initSignalSlots()
{
    connect(d->mComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onIndexChanged(int)));
    connect(d->mComboBox, SIGNAL(activated(int)), SLOT(onClickedItem(int)));
}

void ComboBoxTableWidgetItem::setWheelEventDisable(bool isDisable)
{
    d->mIsWheelDisalbled = isDisable;
}

void ComboBoxTableWidgetItem::setComboBoxMinimumWidth(int width)
{
    d->mComboBox->setMinimumWidth(width);
}

void ComboBoxTableWidgetItem::wheelEvent(QWheelEvent *event)
{
    if(d->mIsWheelDisalbled == true)
        event->ignore();
}

bool ComboBoxTableWidgetItem::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::Wheel)
        return true;

    return false;
}
