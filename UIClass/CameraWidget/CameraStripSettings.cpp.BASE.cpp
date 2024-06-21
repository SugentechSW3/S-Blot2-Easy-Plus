#include <QVariant>
#include "CameraStripSettings.h"
#include "ui_CameraStripSettings.h"

#include "UIClass/CameraWidget/SubWidget/BlobWidget.h"
#include "UIClass/CameraWidget/SubWidget/BoxCarWidget.h"
#include "UIClass/CameraWidget/SubWidget/DualBandWidget.h"
#include "UIClass/DialogWidget/CirclePrograssDialog.h"

#include "Analysis/ConfigInformation.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

USING_SUGENTECH

struct CameraStripSettings::privateStruct
{
    QScopedPointer<BoxCarWidget> mBoxCarWidget; // 추후에 컨텐츠별로 만들어야할 필요성......
    QScopedPointer<BlobWidget> mBlobWidget;
    QScopedPointer<DualBandWidget> mDualBandWidget;
    QScopedPointer<CirclePrograssDialog> mPrograssDlg;

    ConfigInformation* mConfigInstance = nullptr;
    QString mSelectedCompany;

    bool mIsInitContents = true;
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
    d->mPrograssDlg.reset(new CirclePrograssDialog(nullptr));
    this->setUpBoxCarWidget(ui->widget2ndPart, ui->gridLayout2nd);
    this->setUpBlobWidget(ui->widget2ndPart, ui->gridLayout2nd);
    this->setUpDualBandWidget(ui->widget2ndPart, ui->gridLayout2nd);

    ui->checkUseBlob->hide();

    d->mConfigInstance = ConfigInformation::getInstance();
    this->readConfigure();

    ui->checkUseDualBand->setEnabled(true);
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
    connect(ui->checkUseBlob, &QCheckBox::clicked, this, &CameraStripSettings::onUseChanged);
    connect(ui->checkUseDualBand, &QCheckBox::clicked, this, &CameraStripSettings::onUseChanged);

    connect(d->mConfigInstance, &ConfigInformation::onChangedIsUseBlob, this, [this]
            (bool status)
            {
                ui->checkUseBlob->setChecked(status);
            });

    connect(d->mConfigInstance, &ConfigInformation::onChangedIsUseDualBand, this, [this]
            (bool status)
            {
                ui->checkUseDualBand->setChecked(status);
            });

    connect(this, &CameraStripSettings::onConfigDataSendingStart, this, [this]
            {
                d->mPrograssDlg->start();
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

#if 0 //deprecated
    if(d->mBlobWidget.isNull() == false)
    {
        connect(d->BlobWidget.data(), QOverload<int,int>::of(&BlobWidget::setBoxCarRect), this, QOverload<int,int>::of(&CameraStripSettings::setBoxCarRect));
        connect(d->BlobWidget.data(), QOverload<QVector<int>>::of(&BlobWidget::setBoxCarRect), this, QOverload<QVector<int>>::of(&CameraStripSettings::setBoxCarRect));
    }
#endif
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
    ui->checkUseBlob->setChecked(d->mConfigInstance->getIsUseBlob());
    ui->checkUseDualBand->setChecked(d->mConfigInstance->getIsUseDualBand());

    ui->widgetIntensity->readConfigData();
    ui->widgetParticleRemoval->readConfigData();
    ui->widgetHousingRoi->readConfigData();

    auto isUseDualBand = ui->checkUseDualBand->isChecked();

    if(isUseDualBand == true)
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->readConfigData();
    }
    else
    {
        if(d->mBlobWidget.isNull() == false)
            d->mBlobWidget->readConfigData();

        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->readConfigData();
    }

    this->onUseChanged();

}

void CameraStripSettings::writeConfigure()
{
    emit onConfigDataSendingStart();

    auto isUseChecked = ui->checkUseBlob->isChecked();
    auto isUseDualBand = ui->checkUseDualBand->isChecked();

    this->writeUseBlob(isUseChecked);
    this->writeDualBand(isUseDualBand);

    ui->widgetIntensity->writeConfigData();
    ui->widgetParticleRemoval->writeConfigData();
    ui->widgetHousingRoi->writeConfigData();

    if(isUseDualBand == true)
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->writeConfigData();
    }
    else
    {
        if(d->mBlobWidget.isNull() == false)
            d->mBlobWidget->writeConfigData();

        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->writeConfigData();
    }

    d->mConfigInstance->sendDataEnd();

    this->onContentsChanged(ui->comboContents->currentIndex());
}

void CameraStripSettings::writeDualBand(bool isChecked)
{
    d->mConfigInstance->setIsUseDualBand(isChecked);
}

void CameraStripSettings::writeUseBlob(bool isChecked)
{
    d->mConfigInstance->setIsUseBlob(isChecked);
}

void CameraStripSettings::onContentsChanged(int idx)
{
    if(d->mIsInitContents == true)
    {
        d->mIsInitContents = false;
        return;
    }

    auto contentsList = d->mConfigInstance->getContentsList();

    if(idx < contentsList.count())
    {
        auto currentContentsName = contentsList[idx];
        d->mConfigInstance->setCurrentContents(currentContentsName);

        CUtil::messageBox(QString("ContentsSelected: %1").arg(currentContentsName));
    }
}

void CameraStripSettings::onUseChanged()
{
    auto isChecked = ui->checkUseBlob->isChecked();
    auto isDualBand = ui->checkUseDualBand->isChecked();

    if(isDualBand == true)
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->show();

        if(d->mBoxCarWidget.isNull() == false)
            d->mBoxCarWidget->hide();

        if(d->mBlobWidget.isNull() == false)
            d->mBlobWidget->hide();
    }
    else
    {
        if(d->mDualBandWidget.isNull() == false)
            d->mDualBandWidget->hide();

        if(isChecked == true)
        {
            if(d->mBoxCarWidget.isNull() == false)
                d->mBoxCarWidget->hide();

            if(d->mBlobWidget.isNull() == false)
                d->mBlobWidget->show();
        }
        else
        {
            if(d->mBoxCarWidget.isNull() == false)
                d->mBoxCarWidget->show();

            if(d->mBlobWidget.isNull() == false)
                d->mBlobWidget->hide();
        }
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

void CameraStripSettings::setUpBlobWidget(QWidget* part2nd, QGridLayout* layout)
{
    if(part2nd == nullptr)
        return;

    if(layout == nullptr)
        return;

    auto widget = new BlobWidget(part2nd); //auto removed;
    layout->addWidget(widget);
    d->mBlobWidget.reset(widget);
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

        if(ui->checkUseBlob->isChecked() == false)
        {
            if(i == 0)
                indexItem = new QTableWidgetItem(QString("PC"));
        }

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
