#include <QLineEdit>
#include <QHBoxLayout>
#include "PlaceHolderTableWidgetItem.h"


struct PlaceHolderTableWidgetItem::privateStruct
{
    QLineEdit* mEditBox = nullptr;
};

PlaceHolderTableWidgetItem::PlaceHolderTableWidgetItem(int row, int column, QWidget *parent) :
    CustomTableWidgetItem(row, column, parent),
    d(new privateStruct)
{

    d->mEditBox = new QLineEdit(this);
    d->mEditBox->setFocusPolicy(Qt::StrongFocus);
    this->addWidget(d->mEditBox);

    connect(d->mEditBox, SIGNAL(textChanged(QString)), SLOT(onChangedText(QString)));
}

PlaceHolderTableWidgetItem::~PlaceHolderTableWidgetItem()
{

}

QString PlaceHolderTableWidgetItem::getText()
{
    return d->mEditBox->text();
}

void PlaceHolderTableWidgetItem::setText(const QString& text)
{
    d->mEditBox->setText(text);
}

QString PlaceHolderTableWidgetItem::getPlaceHolderText()
{
    return d->mEditBox->placeholderText();
}

void PlaceHolderTableWidgetItem::setPlaceHolderText(const QString& text)
{
    d->mEditBox->setPlaceholderText(text);
}

void PlaceHolderTableWidgetItem::onChangedText(QString)
{
    this->onChanged();
}

void PlaceHolderTableWidgetItem::setFrameless(bool isFrameless)
{
    d->mEditBox->setFrame(!isFrameless);
}
