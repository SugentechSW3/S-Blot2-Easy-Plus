#include <QVector>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

#include "BandGapWidget.h"
#include "ui_BandGapWidget.h"

#include "Analysis/ConfigInformation.h"

struct BandGapWidget::privateStruct
{
    QVector<QSpinBox*> mEditWidgets;
    QScopedPointer<QWidget> mGroupWidget;
};

BandGapWidget::BandGapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_BandGapWidget),
    d(new privateStruct)
{
    ui->setupUi(this);
    this->setGroupBoxNameColor(Qt::green);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

BandGapWidget::~BandGapWidget()
{
}

void BandGapWidget::CreateEditBox(int count)
{
    auto baseWidget = new QWidget(ui->groupBox);
    d->mGroupWidget.reset(baseWidget);

    ui->gridLayout_2->addWidget(baseWidget,0,0,1,1);

    auto verticalLayout = new QVBoxLayout(baseWidget);
    baseWidget->setLayout(verticalLayout);

    QVector<QVBoxLayout*> widgetSet;

    for(int i = 0; i < count ; i++)
    {
        int idx = i + 1;

        QLabel* createdLabel = new QLabel(baseWidget);
        createdLabel->setText(QString::number(idx));
        createdLabel->setObjectName(QString("label%1").arg(idx));

        QSpinBox* createdSpin = new QSpinBox(baseWidget);
        createdSpin->setObjectName(QString("spin%1").arg(idx));
        createdSpin->setButtonSymbols(QSpinBox::NoButtons);
        createdSpin->setBaseSize(40,20);
        createdSpin->setMaximumSize(60,20);
        createdSpin->setMaximum(2000);

        QVBoxLayout* createdVBox = new QVBoxLayout();
        createdVBox->setObjectName(QString("layoutV%1").arg(idx));

        createdVBox->addWidget(createdLabel);
        createdVBox->addWidget(createdSpin);

        d->mEditWidgets.push_back(createdSpin);
        widgetSet.push_back(createdVBox);

        createdLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        createdSpin->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

        connect(createdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &BandGapWidget::onChangedSpinValue);
    }

    QHBoxLayout* layout = nullptr;

    for(int i = 0; i < widgetSet.count(); i++)
    {
        if(i % 10 == 0) //10개단위로 UI 배치
        {
            layout = new QHBoxLayout(); //take onwership
            verticalLayout->addLayout(layout);
        }

       layout->addLayout(widgetSet[i]);
    }

    this->repaint();
}

void BandGapWidget::reCreateEditBox(int count)
{
    this->clearEditBox();
    this->CreateEditBox(count);
}

void BandGapWidget::clearEditBox()
{
    ui->gridLayout_2->removeWidget(d->mGroupWidget.data());
    d->mGroupWidget.reset(nullptr);
    d->mEditWidgets.clear();
    this->repaint();
}

int BandGapWidget::getEditBoxData(int index)
{
    if(d->mEditWidgets.count() < index)
        return 0;

    auto spin = d->mEditWidgets.at(index);

    if(spin != nullptr)
       return spin->value();
    else
        return 0;
}

void BandGapWidget::setEditBoxData(int index, int value)
{
    if(d->mEditWidgets.count() < index)
        return ;

    auto spin = d->mEditWidgets.at(index);

    if(spin != nullptr)
        spin->setValue(value);
}

int BandGapWidget::getWidgetCount()
{
    return d->mEditWidgets.count();
}

void BandGapWidget::setGroupBoxName(const QString& str)
{
    ui->groupBox->setTitle(str);
}

void BandGapWidget::setGroupBoxNameColor(Qt::GlobalColor color)
{
    QColor qcolor(color);
    ui->groupBox->setStyleSheet(QString("QGroupBox::title  {color: %1;}").arg(qcolor.name()));
}

void BandGapWidget::setDisableSpin()
{
    ui->groupBox->setDisabled(true);
}

void BandGapWidget::setEnableSpin()
{
    ui->groupBox->setEnabled(true);
}

void BandGapWidget::setMaximumValue(int val)
{
    foreach(auto itr, d->mEditWidgets)
        itr->setMaximum(val);
}

void BandGapWidget::setMinimumValue(int val)
{
    foreach(auto itr, d->mEditWidgets)
        itr->setMinimum(val);
}

void BandGapWidget::onChangedSpinValue(int val)
{
    auto spinObject = qobject_cast<QSpinBox*>(this->sender());

    if( spinObject != nullptr )
    {
        auto idx = spinObject->objectName().remove("spin").toInt() - 1;
        emit onChangedSpin(idx, val);
    }

}
