#include "QThread"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "CameraView.h"
#include "ui_CameraView.h"

#include "CameraControl/CameraInstance.h"

#include "Common/Barcode/CQRCode.h"
#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

#include "Analysis/ConfigInformation.h"
#include "Analysis/BasicSupremeIAnalysis.h"

#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/WriteProtocolControlRead.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"

USING_SUGENTECH

struct CameraView::privateStruct
{
    ConfigInformation* mConfigInformation = nullptr;
    QRect mImgRoi;
    QVector<GlobalDataStruct::rectBandResult> mRectBandResult;
};

CameraView::CameraView(QWidget *parent) :
                                          CSerialNetworkWidget(parent),
                                          ui(new Ui_CameraView),
                                          d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
}

CameraView::~CameraView()
{
    this->stop();
}

void CameraView::initVariables()
{
    d->mConfigInformation = ConfigInformation::getInstance();
    this->onUpdatedConfigure(ConfigInformation::getInstance()->getHousingROI());
}

void CameraView::initSignalSlots()
{
    connect(ui->widgetImage, &CameraWidget::onDrawGraph, this, &CameraView::onDrawGraphRequest);
    connect(ui->widgetImage, &CameraWidget::requestEnableWidget, this, &CameraView::onEnableWidget);
    connect(ui->widgetImage, &CameraWidget::requestDisableWidget, this, &CameraView::onDisableWidget);
    connect(ui->widgetCtrl, &CameraSettings::onSelectedSaveImage, ui->widgetImage, &CameraWidget::onCameraSaveImage);
    connect(ui->widgetCtrl, &CameraSettings::onSelectedLoadImage, ui->widgetImage, &CameraWidget::onCameraLoadImage);
    connect(ui->widgetCtrl, &CameraSettings::onSelectedFindPC, this, &CameraView::onFindPC);
    connect(ui->widgetCtrl, &CameraSettings::onSelectedAnalysis, this, &CameraView::onAnalysisImage);
    connect(ui->widgetCtrl, &CameraSettings::onSelectedAngleReset, this, &CameraView::onAngleReset);
    connect(ui->widgetCtrl, &CameraSettings::onReadQRTest, this, &CameraView::onReadQRTest);
    connect(ui->widgetCtrl, QOverload<int,int>::of(&CameraSettings::setBoxCarRect), this, QOverload<int,int>::of(&CameraView::onUpdateFindPcPosX));
    connect(ui->widgetCtrl, QOverload<QVector<int>>::of(&CameraSettings::setBoxCarRect), this, QOverload<QVector<int>>::of(&CameraView::onUpdateFindPcPosX));
    connect(ui->widgetCtrl, &CameraSettings::onGraphHide, this, [this]
            (bool isHide)
            {
                if(isHide == true)
                    ui->widgetGraph->hide();
                else
                    ui->widgetGraph->show();
            });

    connect(d->mConfigInformation, &ConfigInformation::onChangedHousingROI, this, &CameraView::onUpdatedConfigure);
}

void CameraView::startWidget()
{
    ui->widgetImage->start();
    ui->widgetCtrl->start();
}

void CameraView::stopWidget()
{
    ui->widgetImage->stop();
    ui->widgetCtrl->stop();

    this->onLedOFF();
    this->onSendXHome();
}

void CameraView::onUpdatedConfigure(GlobalDataStruct::HOUSING_ROI configData)
{
    auto x1 = configData.x1;
    auto x2 = configData.x2;
    auto y1 = configData.y1;
    auto y2 = configData.y2;

    d->mImgRoi = QRect(x1, y1, x2 - x1, y2 - y1);
}

void CameraView::onDrawGraphRequest(QPoint pos, cv::Mat image)
{
    cv::Mat roiMat;

    if(d->mImgRoi.isNull() == true || d->mImgRoi.isValid() == false)
        return;

    auto isWidth = d->mImgRoi.x() + d->mImgRoi.width() < image.size().width;
    auto isHeight = d->mImgRoi.y() + d->mImgRoi.height() < image.size().height;

    if(isWidth == false || isHeight == false)
        return;

    auto isPoint = pos.y() < image.size().height;

    if(isPoint == false)
        return;

    if(d->mImgRoi.contains(pos) == false)
        return;

    cv::Rect roi = cv::Rect(d->mImgRoi.x(), d->mImgRoi.y(), d->mImgRoi.width(),  d->mImgRoi.height());
    auto dynamicPos = pos.y() - d->mImgRoi.y();
    roiMat = image(roi);

    ui->widgetGraph->drawLine(roiMat, dynamicPos, d->mImgRoi.x());
}

void CameraView::onEnableWidget()
{
    this->setEnabled(true);
}

void CameraView::onDisableWidget()
{
    this->setDisabled(true);
}

void CameraView::onFindPC()
{
    GlobalDataStruct::AnalysisResult result;

    for(int retryCount = 0; retryCount < 3 ; retryCount++)
    {
        auto image = ui->widgetImage->getCurrentImage();

        if(image.data == nullptr)
        {
            this->thread()->sleep(1); //1초 지연후 재측정.
            continue;
        }

        auto isStatic = ui->widgetImage->isStaticImage();
        int stripPoint = d->mConfigInformation->getCurrentStripPoint();

        BasicSupremeIAnalysis analysis;
        result =  analysis.analysisSingleStrip(image, d->mConfigInformation->getCurrentContents(), true, isStatic, stripPoint);

        //분석용 사진으로  분석할경우
        if(isStatic == true)
        {
            CameraInstance::ImageRotation(image, ConfigInformation::getInstance()->getRotation());
            ui->widgetImage->setCurrentImage(image);
        }

        if(result.isSuccess == false)
        {
            qDebug() << result.errorReason;
            CLogWriter::printLog(QString("[BoxCarScanFail] : %1").arg(result.errorReason));
            this->thread()->sleep(1); //1초 지연후 재측정.
        }
        else
            break;
    }

    ui->widgetImage->setBoxCarRect(result.rectBandResult);
    ui->widgetImage->setBandGapLine(result.frontPMXPosition, result.backPMXPosition);
    d->mRectBandResult = result.rectBandResult;
}

void CameraView::onAnalysisImage()
{
    GlobalDataStruct::AnalysisResult result;
    cv::Mat lastImage;
    int stripPoint = d->mConfigInformation->getCurrentStripPoint();

    for(int retryCount = 0; retryCount < 3 ; retryCount++)
    {
        auto image = ui->widgetImage->getCurrentImage();

        if(image.data == nullptr)
        {
            this->thread()->sleep(1);
            continue;
        }

        auto isStatic = ui->widgetImage->isStaticImage();
        lastImage = image;


        BasicSupremeIAnalysis analysis;
        result =  analysis.analysisSingleStrip(image, d->mConfigInformation->getCurrentContents(), false, isStatic, stripPoint);

        //분석용 사진으로  분석할경우
        if(isStatic)
        {
            CameraInstance::ImageRotation(image, ConfigInformation::getInstance()->getRotation());
            ui->widgetImage->setCurrentImage(image);
        }

        if(result.isSuccess == false)
        {
            qDebug() << result.errorReason;
            CLogWriter::printLog(QString("[BoxCarScanFail] : %1").arg(result.errorReason));
            this->thread()->sleep(1);
            continue;
        }
        else
            break;
    }

    BasicSupremeIAnalysis analysis;
    analysis.analysisRawImageSave(lastImage, stripPoint);

    ui->widgetCtrl->setResultValue(result.rawIntensityValue, result.backgroundValue, result.bandValue);
    ui->widgetImage->setBoxCarRect(result.rectBandResult);
    ui->widgetImage->setBandGapLine(result.frontPMXPosition, result.backPMXPosition);
    d->mRectBandResult = result.rectBandResult;
}

void CameraView::onReadQRTest()
{
    auto camera = CameraInstance::getInstance();
    auto image = camera->getFullConfigureImage();

    if(image.data == nullptr)
        return;

    auto currentFlip = d->mConfigInformation->getFlip();

    if(currentFlip != 0)
        CameraInstance::flip(image, currentFlip);

    auto qrStr = CQRCode::deCodeImage(image);

    auto logText =  QString("QR Read : readCount %1 ").arg(qrStr.count());

    qDebug() << logText;


    for(auto& itr : qrStr)
    {
        auto currentQR = QString("QR Read : %1 ").arg(itr);
        qDebug() << currentQR;
        logText.push_back(currentQR);
    }

    CUtil::messageBox(logText);
}

void CameraView::onAngleReset()
{
    CameraInstance::getInstance()->clearRotation();
}

void CameraView::onUpdateFindPcPosX(int idx, int value)
{
    if(d->mRectBandResult.count() == 0)
        return;

    if(idx < d->mRectBandResult.count())
    {
        auto isMultiBand = ConfigInformation::getInstance()->getIsUseMultiBand();
        auto beginX = d->mRectBandResult.first().rectBand.x;

        if(isMultiBand == true)
        {
            auto multiBandBeginIdx = ConfigInformation::getInstance()->getBoxCarAnalysis().boxCarbandCount/2;

            if(multiBandBeginIdx <= idx)
                beginX = d->mRectBandResult[multiBandBeginIdx].rectBand.x;
        }

        beginX = d->mRectBandResult.first().rectBand.x;
        d->mRectBandResult[idx+1].rectBand.x = beginX + value;
        d->mRectBandResult[idx+1].rectDynamicBand.x = beginX +value;
        ui->widgetImage->setBoxCarRect(d->mRectBandResult);
    }
}

void CameraView::onUpdateFindPcPosX(QVector<int> value)
{
    if(d->mRectBandResult.count() == 0)
        return;

    for(int i = 0; i < value.count(); i++)
    {
        if(i < d->mRectBandResult.count())
        {
            auto isMultiBand = ConfigInformation::getInstance()->getIsUseMultiBand();
            auto beginX = d->mRectBandResult.first().rectBand.x;

            if(isMultiBand == true)
            {
                auto multiBandBeginIdx = ConfigInformation::getInstance()->getBoxCarAnalysis().boxCarbandCount/2;

                if(multiBandBeginIdx <= i)
                    beginX = d->mRectBandResult[multiBandBeginIdx].rectBand.x;
            }

            beginX = d->mRectBandResult.first().rectBand.x;
            d->mRectBandResult[i+1].rectBand.x = beginX + value[i];
            d->mRectBandResult[i+1].rectDynamicBand.x = beginX + value[i];
            ui->widgetImage->setBoxCarRect(d->mRectBandResult);
        }
    }
}


void CameraView::onLedOFF()
{
    auto ledOff =  WriteProtocolCameraLedSetting::writeLEDOnOff(WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_OFF).toUtf8();
    this->onQueuedSend(ledOff);
}

void CameraView::onSendXHome()
{
    QString protocolStrX = WriteProtocolControlRead::writeXAxisHome();
    QByteArray sendDataX = protocolStrX.toUtf8();
    this->onQueuedSend(sendDataX);
}

void CameraView::showEvent(QShowEvent *event)
{
    TunnningDataProtocolSendHelper::writeShakeCameraPosMove("CameraView");
}

