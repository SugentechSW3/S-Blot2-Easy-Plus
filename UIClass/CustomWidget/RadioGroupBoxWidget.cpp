#include <QMap>
#include <QVBoxLayout>
#include <QRadioButton>

#include "RadioGroupBoxWidget.h"
#include "ui_RadioGroupBoxWidget.h"

struct RadioGroupBoxWidget::privateStruct
{
    QScopedPointer<QVBoxLayout> mVerticalLayOut;
    QMap<QString, QRadioButton*> mRadioButtons;
};

RadioGroupBoxWidget::RadioGroupBoxWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_RadioGroupBoxWidget),
    d(new privateStruct)
{
    ui->setupUi(this);
    d->mVerticalLayOut.reset(new QVBoxLayout(ui->groupBox));

}

RadioGroupBoxWidget::RadioGroupBoxWidget(QString groupBoxName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui_RadioGroupBoxWidget),
    d(new privateStruct)
{
    ui->setupUi(this);
    ui->groupBox->setTitle(groupBoxName);
    d->mVerticalLayOut.reset(new QVBoxLayout(ui->groupBox));

}

RadioGroupBoxWidget::~RadioGroupBoxWidget()
{
}

void RadioGroupBoxWidget::setTitle(QString groupBoxTitle)
{
    ui->groupBox->setTitle(groupBoxTitle);
}

void RadioGroupBoxWidget::addRadioWidget(const QString& text, const QString& objectName)
{
    auto findObj = d->mRadioButtons[objectName];

    if(findObj != nullptr)
        return;

    auto radioButton = new QRadioButton(text, this);
    radioButton->setObjectName(objectName);

    d->mRadioButtons[objectName] = radioButton;
    d->mVerticalLayOut->addWidget(radioButton);
}

QRadioButton* RadioGroupBoxWidget::findObject(const QString& objectName)
{
    return d->mRadioButtons[objectName];
}

QRadioButton* RadioGroupBoxWidget::getCheckedRadioBox()
{
    QMapIterator<QString, QRadioButton*> itr(d->mRadioButtons);

    while(itr.hasNext())
    {
        itr.next();

        if(itr.value()->isChecked())
            return itr.value();
    }

    return nullptr;
}

QString RadioGroupBoxWidget::spinToRadioName(QString objectName)
{
    if(objectName.isEmpty())
        return QString();

    objectName.remove("spin");
    objectName.push_front("radio");

    return objectName;
}

QString RadioGroupBoxWidget::doubleSpinToRadioName(QString objectName)
{
    if(objectName.isEmpty())
        return QString();

    objectName.remove("doubleSpin");
    objectName.push_front("radio");

    return objectName;
}


QString RadioGroupBoxWidget::radioNameToSpinName(QString objectName)
{
    if(objectName.isEmpty())
        return QString();

    objectName.remove("radio");
    objectName.push_front("spin");

    return objectName;
}
