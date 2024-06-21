#include "GuideLineWidget.h"
#include "ui_GuideLineWidget.h"

GuideLineWidget::GuideLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_GuideLineWidget)
{
    ui->setupUi(this);
    this->initSignalSlots();

    ui->housingROIWidget->setGroupBoxName("Guide Line");
    ui->housingROIWidget->setGroupBoxNameColor(Qt::blue);
    this->readConfigData();
}

GuideLineWidget::~GuideLineWidget()
{
}

void GuideLineWidget::initSignalSlots()
{
    connect(ui->btnWrite, SIGNAL(clicked()), SLOT(onClickedWrite()));
}

void GuideLineWidget::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getGuideLine();

    ui->housingROIWidget->setX1(configData.x1);
    ui->housingROIWidget->setX2(configData.x2);
    ui->housingROIWidget->setY1(configData.y1);
    ui->housingROIWidget->setY2(configData.y2);
}

void GuideLineWidget::writeConfigData()
{
    auto instance = this->getConfigInstance();

    GlobalDataStruct::GUIDE_LINE configData;
    configData.x1 = ui->housingROIWidget->getX1();
    configData.x2 = ui->housingROIWidget->getX2();
    configData.y1 = ui->housingROIWidget->getY1();
    configData.y2 = ui->housingROIWidget->getY2();

    instance->setGuideLine(configData);
}

void GuideLineWidget::onClickedWrite()
{
    this->writeConfigData();
}
