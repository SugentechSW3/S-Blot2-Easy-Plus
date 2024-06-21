#include "RelativeCheckBoxWidget.h"
#include "ui_RelativeCheckBoxWidget.h"
#include <QWidget>

struct RelativeCheckBoxWidget::PrivateStruct
{
    QVector<bool> mIsCheck;
    QScopedPointer<QWidget> mGroupWidget;
};

RelativeCheckBoxWidget::RelativeCheckBoxWidget(QWidget *parent) :
    QWidget(parent),d(new PrivateStruct),
    ui(new Ui_RelativeCheckBoxWidget)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

RelativeCheckBoxWidget::~RelativeCheckBoxWidget()
{

}

void RelativeCheckBoxWidget::createEditCheckBox(int count)
{
    auto baseWidget = new QWidget(ui->gbBase);
    d->mGroupWidget.reset(baseWidget);


    ui->gridLayout->addWidget(baseWidget,0,0,1,1);
    auto verticalLayout = new QVBoxLayout(baseWidget);
}

void RelativeCheckBoxWidget::setGroubBoxName(const QString &gbName)
{
    ui->gbBase->setTitle(gbName);
}

QVector<bool> RelativeCheckBoxWidget::getCheckBoxData()
{

}

void RelativeCheckBoxWidget::clearCheckBox()
{

}
