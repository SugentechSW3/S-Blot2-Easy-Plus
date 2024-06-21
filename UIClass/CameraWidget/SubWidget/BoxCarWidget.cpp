#include "BoxCarWidget.h"
#include "ui_BoxCarWidget.h"

#include "Common/CLogWriter.h"
USING_SUGENTECH

struct BoxCarWidget::privateStruct
{
    ConfigInformation* mConfigInstance = nullptr;
};

BoxCarWidget::BoxCarWidget(QWidget *parent) :
                                              QWidget(parent),
                                              ui(new Ui_BoxCarWidget),
                                              d(new privateStruct)
{
    ui->setupUi(this);
    this->initWidget();
    this->initSignalSlots();
    this->readConfigData();
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

BoxCarWidget::~BoxCarWidget()
{
}

void BoxCarWidget::initWidget()
{
    d->mConfigInstance = this->getConfigInstance();
    auto configData = d->mConfigInstance->getBoxCarAnalysis();

    ui->relativeWidget->CreateEditBox(configData.boxCarbandCount);

    this->setEnableBoxCar(this->checkValidTuingData());
}

void BoxCarWidget::initSignalSlots()
{
    connect(ui->spinBandGapSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &BoxCarWidget::onChangedBoxCarGapSize);
    connect(ui->relativeWidget, &RelativePosWidget::onChangedBoxCarSpin, this, &BoxCarWidget::onChangedBoxCarData);
    connect(d->mConfigInstance, &ConfigInformation::onChangedUseBoxCarAnalaysis, this, &BoxCarWidget::onConfigChangedAnalysis);
    connect(d->mConfigInstance, &ConfigInformation::onChangedUseBoxCarBandGap, this, &BoxCarWidget::onConfigChangedBandGap);
    connect(ui->spinBandCount,  QOverload<int>::of(&QSpinBox::valueChanged), this, [this]
            (int value)
            {
                ui->relativeWidget->reCreateEditBox(value);

                auto configDataBandGap = d->mConfigInstance->getBoxCarBandGap();
                for(int i = 0; i < value; i++)
                {
                    {
                        int relData = 0;

                        if(i < configDataBandGap.boxCarBandGap.size())
                            relData = configDataBandGap.boxCarBandGap[i];

                        ui->relativeWidget->setMaximumValue(this->getMaximumRoiXPos());
                        ui->relativeWidget->setEditBoxData(i, relData);
                    }
                }
            });
}

void BoxCarWidget::readConfigData()
{
    auto configDataBandGap = d->mConfigInstance->getBoxCarBandGap();
    auto configDataBandAnalysis = d->mConfigInstance->getBoxCarAnalysis();
    auto bandCount = configDataBandAnalysis.boxCarbandCount;

    for(int i = 0; i < bandCount; i++)
    {
        int relData = 0;

        if(i < configDataBandGap.boxCarBandGap.size())
            relData = configDataBandGap.boxCarBandGap[i];

        ui->relativeWidget->setMaximumValue(this->getMaximumRoiXPos());
        ui->relativeWidget->setEditBoxData(i, relData);
    }

    this->setBandCount(configDataBandAnalysis.boxCarbandCount);
    this->setWidth(configDataBandAnalysis.width);
    this->setHeight(configDataBandAnalysis.height);

    ui->pcLineWidget->setXStart(configDataBandAnalysis.xStart);
    ui->pcLineWidget->setXRange(configDataBandAnalysis.xRange);
    ui->pcLineWidget->setPCThres(configDataBandAnalysis.pcThres);
    ui->pcLineWidget->setHousingThres(configDataBandAnalysis.housingThres);
}

void BoxCarWidget::writeConfigData()
{
    GlobalDataStruct::USE_BOXCAR_ANALYSIS configAnalaysis;
    GlobalDataStruct::USE_BOXCAR_BAND configBandData;

    auto bandCount = ui->relativeWidget->getWidgetCount();

    if(this->getBandCount() < bandCount)
        bandCount = this->getBandCount();

#pragma omp parallel for ordered
    for(int i = 0; i < bandCount; i++)
    {
#pragma omp ordered
        {
            int relValue = ui->relativeWidget->getEditBoxData(i);
            configBandData.boxCarBandGap.push_back(relValue);
        }
    }

    configAnalaysis.boxCarbandCount = this->getBandCount();
    configAnalaysis.width = this->getWidth();
    configAnalaysis.height = this->getHeight();

    configAnalaysis.xStart = ui->pcLineWidget->getXStart();
    configAnalaysis.xRange = ui->pcLineWidget->getXRange();
    configAnalaysis.pcThres = ui->pcLineWidget->getPCThres();
    configAnalaysis.housingThres = ui->pcLineWidget->getHousingThres();

    d->mConfigInstance->setBoxCarAnalysis(configAnalaysis);
    d->mConfigInstance->setBoxCarBandGap(configBandData);
}

void BoxCarWidget::onConfigChangedAnalysis(GlobalDataStruct::USE_BOXCAR_ANALYSIS configData)
{
    ui->relativeWidget->reCreateEditBox(configData.boxCarbandCount);

    this->readConfigData();
    this->setEnableBoxCar(this->checkValidTuingData());
}

void BoxCarWidget::onConfigChangedBandGap(GlobalDataStruct::USE_BOXCAR_BAND)
{
    this->readConfigData();
}

int BoxCarWidget::getBandCount()
{
    return ui->spinBandCount->value();
}

void BoxCarWidget::setBandCount(int count)
{
    ui->spinBandCount->setValue(count);
}

int BoxCarWidget::getWidth()
{
    return ui->spinWidth->value();
}

void BoxCarWidget::setWidth(int width)
{
    ui->spinWidth->setValue(width);
}

int BoxCarWidget::getHeight()
{
    return ui->spinHeight->value();
}

void BoxCarWidget::setHeight(int height)
{
    ui->spinHeight->setValue(height);
}

void BoxCarWidget::onChangedBoxCarGapSize(int value)
{
    if(value == 0)
        return;

    QVector<int> pos;

    for(int i =0; i < ui->relativeWidget->getWidgetCount(); i++)
    {
        ui->relativeWidget->setEditBoxData(i, this->calcBandGap(i+1, value));
        pos.push_back(ui->relativeWidget->getEditBoxData(i));
    }

    emit setBoxCarRect(pos);
}

void BoxCarWidget::onChangedBoxCarData(int idx, int value)
{
    emit setBoxCarRect(idx, value);
}

int BoxCarWidget::calcBandGap(int idx, int bandGapSize)
{
    return (idx*bandGapSize);
}

int BoxCarWidget::validRoiPos(int targetPos)
{
    int resultPos = 0;
    auto analysis = d->mConfigInstance->getBoxCarAnalysis();

    int roiMaxWidth = this->getMaximumRoiXPos();
    int targetPoint = targetPos + analysis.width;

    if(roiMaxWidth <= targetPoint)
        resultPos = roiMaxWidth;
    else
        resultPos = targetPos;

    return resultPos;
}

int BoxCarWidget::getMaximumRoiXPos()
{
    auto housingRoi = d->mConfigInstance->getHousingROI();
    auto analysis = d->mConfigInstance->getBoxCarAnalysis();
    int roiMaxWidth = (housingRoi.x2 - housingRoi.x1) - (analysis.xStart- housingRoi.x1) - analysis.xRange;

    return roiMaxWidth - 1;
}

bool BoxCarWidget::checkValidTuingData()
{
    auto analysisConfig = d->mConfigInstance->getBoxCarAnalysis();

    if(analysisConfig.width == 0)
        return false;

    if(analysisConfig.height == 0)
        return false;

    if(analysisConfig.xRange == 0)
        return false;

    if(analysisConfig.xStart == 0)
        return false;

    if(analysisConfig.pcThres == 0)
        return false;

    if(analysisConfig.housingThres == 0)
        return false;

    if(analysisConfig.boxCarbandCount == 0)
        return false;

    return true;
}

void BoxCarWidget::setEnableBoxCar(bool isEnable)
{
    if(isEnable == true)
    {
        ui->spinBandGapSize->setEnabled(true);
        ui->relativeWidget->setEnableSpin();
    }
    else
    {
        ui->spinBandGapSize->setDisabled(true);
        ui->relativeWidget->setDisableSpin();
    }
}
