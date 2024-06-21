#include <QVariant>
#include "CameraStripSettings.h"
#include "ui_CameraStripSettings.h"

#include "UIClass/CameraWidget/SubWidget/BoxCarWidget.h"
#include "UIClass/CameraWidget/SubWidget/DualBandWidget.h"
#include "UIClass/DialogWidget/CirclePrograssDialog.h"

#include "Analysis/ConfigInformation.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

USING_SUGENTECH

struct CameraStripSettings::privateStruct
{
    QScopedPointer<BoxCarWidget> mBoxCarWidget;
    QScopedPointer<DualBandWidget> mDualBandWidget;
    QScopedPointer<CirclePrograssDialog> mPrograssDlg;

    ConfigInformation* mConfigInstance = nullptr;
    QString mSelectedCompany;
};

CameraStripSettings::CameraStripSettings(QWidget *parent) :
                                                            CSerialNetworkWidget(parent),
                                                            ui(new Ui_CameraStripSettings),
                                                            d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
}

CameraStripSettings::~CameraStripSettings()
{
    this->stop();
}

void CameraStripSettings::initVariables()
{
    d->mConfigInstance = ConfigInformation::getInstance();

    d->mPrograssDlg.reset(new CirclePrograssDialog(nullptr));

    this->setUpBoxCarWidget(ui->widget2ndPart, ui->gridLayout2nd);
    this->setUpDualBandWidget(ui->widget2ndPart, ui->gridLayout2nd);
    this->readConfigure();

    ui->checkUseMultiBand->setEnabled(true);
}

void CameraStripSettings::initSignalSlots()
{
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onSelectedSaveImage, this, &CameraStripSettings::onSelectedSaveImage);
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onSelectedLoadImage, this, &CameraStripSettings::onSelectedLoadImage);
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onSelectedFindPC, this, &CameraStripSettings::onSelectedFindPC);
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onSelectedAnalysis, this, &CameraStripSettings::onSelectedAnalysis);
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onSelectedAngleReset, this, &CameraStripSettings::onSelectedAngleReset);
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onReadQRTest, this, &CameraStripSettings::onReadQRTest);
    connect(ui->widgetCommandMenu, &CommandMenuWidget::onGraphHide, this, &CameraStripSettings::onGraphHide);

    connect(ui->btnRecipeSave, &QPushButton::clicked, this, &CameraStripSettings::writeConfigure);
    connect(ui->checkUseMultiBand, &QCheckBox::toggled, this, &CameraStripSettings::onUseChanged);
    connect(ui->comboContents, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CameraStripSettings::onContentsChanged);

    connect(d->mConfigInstance, &ConfigInformation::onChangedIsUseMultiBand, this, [this]
            (bool status)
            {
                ui->checkUseMultiBand->setChecked(status);
            });

    connect(this, &CameraStripSettings::onConfigDataSendingStart, this, [this]
            {
#ifndef IS_DEBUG_PC
                d->mPrograssDlg->start();
#endif
            });

    connect(d->mConfigInstance, &ConfigInformation::endConfigProtocol, this, [this]
            (bool)
            {
                d->mPrograssDlg->stop();
            });

    if(d->mBoxCarWidget.isNull() == false)
    {
        connect(d->mBoxCarWidget.data(), QOverload<int,int>::of(&BoxCarWidget::setBoxCarRect), this, QOverload<int,int>::of(&CameraStripSettings::setBoxCarRect));
        connect(d->mBoxCarWidget.data(), QOverload<QVector<int>>::of(&BoxCarWidget::setBoxCarRect), this, QOverload<QVector<int>>::of(&CameraStripSettings::setBoxCarRect));
    }
}

void CameraStripSettings::startWidget()
{

}

void CameraStripSettings::stopWidget()
{

}

void CameraStripSettings::readConfigure()
{
    auto contentsList = d->mConfigInstance->getContentsList();
    auto contentsIdx = contentsList.indexOf(d->mConfigInstance->getCurrentContents());

    if(contentsIdx == -1)
        contentsIdx = 0;

    ui->comboContents->addItems(contentsList);
    ui->comboContents->setCurrentIndex(contentsIdx);
    ui->checkUseMultiBand->setChecked(d->mConfigInstance->getIsUseMultiBand());

    ui->widgetIntensity->readConfigData();
    ui->widgetParticleRemoval->readConfigData();
    ui->widgetHousingRoi->readConfigData();

    auto isUseMultiBand = ui->checkUseMultiBand->isChecked();

    if(isUseMultiBand == true)
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->readConfigData();
    }
    else
    {
        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->readConfigData();
    }

    this->onUseChanged();

}

void CameraStripSettings::writeConfigure()
{
    emit onConfigDataSendingStart();

    auto isUseMultiBand = ui->checkUseMultiBand->isChecked();

    this->writeDualBand(isUseMultiBand);

    ui->widgetIntensity->writeConfigData();
    ui->widgetParticleRemoval->writeConfigData();
    ui->widgetHousingRoi->writeConfigData();

    if(isUseMultiBand == true)
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->writeConfigData();
    }
    else
    {
        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->writeConfigData();
    }

    d->mConfigInstance->sendDataEnd();

    this->onContentsChanged(ui->comboContents->currentIndex());

    d->mConfigInstance->saveContentsTunningINIData(d->mConfigInstance->getCurrentContents());
}

void CameraStripSettings::writeDualBand(bool isChecked)
{
    d->mConfigInstance->setIsUseMultiBand(isChecked);
}

void CameraStripSettings::onContentsChanged(int idx)
{
    auto contentsList = d->mConfigInstance->getContentsList();

    if(idx < contentsList.count())
    {
        auto currentContentsName = contentsList[idx];
        d->mConfigInstance->setCurrentContents(currentContentsName);

        CUtil::messageBox(QString("ContentsChanged: %1").arg(currentContentsName));
    }
}

void CameraStripSettings::onUseChanged()
{
    auto isMultiBand = ui->checkUseMultiBand->isChecked();

    if(isMultiBand == true)
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->show();

        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->hide();
    }
    else
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->hide();

        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->show();
    }
}

void CameraStripSettings::setUpBoxCarWidget(QWidget* part2nd, QGridLayout* layout)
{
    if(part2nd == nullptr)
        return;

    if(layout == nullptr)
        return;

    auto widget = new BoxCarWidget(part2nd); //auto removed;
    layout->addWidget(widget);
    d->mBoxCarWidget.reset(widget);
}

void CameraStripSettings::setUpDualBandWidget(QWidget* part2nd, QGridLayout* layout)
{
    if(part2nd == nullptr)
        return;

    if(layout == nullptr)
        return;

    auto widget = new DualBandWidget(part2nd); //auto removed;
    layout->addWidget(widget);
    d->mDualBandWidget.reset(widget);
}

void CameraStripSettings::setResultValue(const QVector<double>& intensity, const QVector<double>& background, const QVector<double>& band)
{
    ui->tableResult->clearContents();
    ui->tableResult->setRowCount(intensity.count());

    for(int i = 0; i < intensity.count(); i++)
    {
        QTableWidgetItem* indexItem = nullptr;

        if(i == 0)
            indexItem = new QTableWidgetItem(QString("PC"));

        if(indexItem == nullptr)
            indexItem = new QTableWidgetItem(QString::number(i));

        ui->tableResult->setItem(i, INDEX, indexItem);

        if(i < band.count())
        {
            QTableWidgetItem* bandItem = new QTableWidgetItem(QString::number(band[i]));
            ui->tableResult->setItem(i, BAND, bandItem);
        }

        if(i < background.count())
        {
            QTableWidgetItem* backgroundItem = new QTableWidgetItem(QString::number(background[i]));
            ui->tableResult->setItem(i, BACKGROUND, backgroundItem);
        }

        QTableWidgetItem* itensityItem = new QTableWidgetItem(QString::number(intensity[i]));
        ui->tableResult->setItem(i, INTENSITY, itensityItem);
    }
}
