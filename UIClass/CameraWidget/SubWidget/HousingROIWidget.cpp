#include "HousingROIWidget.h"
#include "ui_HousingROIWidget.h"

#include "Analysis/GlobalDataStruct.h"


HousingROIWidget::HousingROIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_HousingROIWidget)
{
    ui->setupUi(this);
    this->setGroupBoxName("Housing ROI");
    this->setGroupBoxNameColor(Qt::red);
    this->readConfigData();
}

HousingROIWidget::~HousingROIWidget()
{
}

int HousingROIWidget::getX1()
{
    return ui->spinX1->value();
}

void HousingROIWidget::setX1(int x1)
{
    ui->spinX1->setValue(x1);
}

int HousingROIWidget::getX2()
{
    return ui->spinX2->value();
}

void HousingROIWidget::setX2(int x2)
{
    ui->spinX2->setValue(x2);
}

int HousingROIWidget::getY1()
{
    return ui->spinY1->value();
}

void HousingROIWidget::setY1(int y1)
{
    ui->spinY1->setValue(y1);
}

int HousingROIWidget::getY2()
{
    return ui->spinY2->value();
}

void HousingROIWidget::setY2(int y2)
{
    ui->spinY2->setValue(y2);
}

void HousingROIWidget::setGroupBoxName(const QString& name)
{
    ui->groupBox->setTitle(name);
}

void HousingROIWidget::setGroupBoxNameColor(Qt::GlobalColor color)
{
    QColor qcolor(color);
    ui->groupBox->setStyleSheet(QString("QGroupBox::title  {color: %1;}").arg(qcolor.name()));
}

void HousingROIWidget::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getHousingROI();

    this->setX1(configData.x1);
    this->setX2(configData.x2);
    this->setY1(configData.y1);
    this->setY2(configData.y2);
}

void HousingROIWidget::writeConfigData()
{
    auto instance = this->getConfigInstance();

    GlobalDataStruct::HOUSING_ROI configData;
    configData.x1 = this->getX1();
    configData.x2 = this->getX2();
    configData.y1 = this->getY1();
    configData.y2 = this->getY2();

    instance->setHousingROI(configData);
}
