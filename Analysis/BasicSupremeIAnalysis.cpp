#include <QTime>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QThread>
#include <QUuid>

#include "BasicSupremeIAnalysis.h"
#include "ClassHelper.h"
#include "CameraControl/CameraInstance.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketReader/PacketReader.h"

#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/WriteProtocolControlWrite.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"
#include "Common/Io/CImageToExcelData.h"
#include "Common/Io/CSettingIo.h"

#include "DataBase/DataBaseCtrl.h"
#include "Analysis/ConfigInformation.h"


USING_SUGENTECH

using namespace cv;

struct BasicSupremeIAnalysis::privateStruct
{
    QThread* mThread = nullptr;
    QString mImageLogPath;
    ConfigInformation* mConfigInstance;
    CameraInstance* mCameraInstance;
    SerialCtrl* mNetworkInstance;
    QVector<GlobalDataStruct::AnalysisReportInfomation> mAnalysisList;

    int mAnalysisNextIdx = 0;
    int mAnalysisTotalCount = 0;
    int mAnalysisCurrentCount = 0;
    bool mIsFirstStart = false;
    bool mIsCheckBlackScreen = false;

    double mPrevAverageBG = 0;
    double mNextAverageBG = 0;
};

BasicSupremeIAnalysis::BasicSupremeIAnalysis(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    d->mConfigInstance = ConfigInformation::getInstance();
    d->mCameraInstance = CameraInstance::getInstance();
    d->mNetworkInstance = SerialCtrl::getInstance();
    d->mImageLogPath = CUtil::createDirectory("AnalysisImage");

    connect(d->mNetworkInstance, &SerialCtrl::onRecvDataForSerial, this, &BasicSupremeIAnalysis::onRecvAnalysisData);
    connect(this, &BasicSupremeIAnalysis::onSendData, d->mNetworkInstance, &SerialCtrl::writeQueuedData);
}

BasicSupremeIAnalysis::~BasicSupremeIAnalysis()
{
    if(d->mThread != nullptr)
    {
        d->mThread->terminate();
        d->mThread->wait();
        d->mThread = nullptr;
    }
}

bool BasicSupremeIAnalysis::findPCSingleBand(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& contentsRecipe, const cv::Mat& stripRoiImage,
                                             cv::Mat& stripRoiGrayImg, GlobalDataStruct::AnalysisResult& result, int stripPoint)
{
    cv::Point ptPCposA;
    cv::Point ptPCposB;
    int yThreshold = 0;
    auto startPoint = contentsRecipe.boxCarAnalaysis.xStart;
    auto pcThres = contentsRecipe.boxCarAnalaysis.pcThres;

    if (this->FindPcYCenter(contentsRecipe, stripRoiGrayImg, ptPCposA, ptPCposB, yThreshold, stripPoint, startPoint, false, false) == false)
    {
        QString fileName(QString("Fail_%1_Y").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find PC Y Center(First) ");
        return false;
    }

    if (this->FindPcXCenter(contentsRecipe, stripRoiGrayImg, ptPCposA, stripPoint, startPoint, pcThres, false, false) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find PC X Center(First) ");
        return false;
    }

    auto protoBandRect = this->setBandPosition(contentsRecipe, ptPCposA);

    auto rotationData  = 0.0;

    if(yThreshold <= 0)
        yThreshold = contentsRecipe.boxCarAnalaysis.housingThres;

    this->CalRotationAngle(stripRoiGrayImg, rotationData, protoBandRect,
                           stripPoint, yThreshold);
    d->mConfigInstance->setRotation(rotationData);
    CameraInstance::rotation(stripRoiGrayImg, rotationData);

    if (this->FindPcYCenter(contentsRecipe, stripRoiGrayImg, ptPCposA, ptPCposB, yThreshold, stripPoint, startPoint, true, false) == false)
    {
        QString fileName(QString("Fail_%1_Y").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find PC Y Center(Second) ");
        return false;
    }

    if (this->FindPcXCenter(contentsRecipe, stripRoiGrayImg, ptPCposA, stripPoint, startPoint, pcThres, true, false) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find PC X Center(Second) ");
        return false;
    }

    auto findBaseBand =  this->setBandPosition(contentsRecipe, ptPCposA);

    QVector<int> pcPoint;
    pcPoint.push_back(0);
    result.pcPoint = pcPoint;

#pragma omp parallel for ordered
    for(int i = 0; i< findBaseBand.count(); i++)
    {
#pragma omp ordered
        {
            GlobalDataStruct::rectBandResult bandResult;
            bandResult.rectBand = findBaseBand[i];
            result.rectBandResult.push_back(bandResult);
        }
    }

    return true;
}

bool BasicSupremeIAnalysis::findPCMultipleBand(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& contentsRecipe, const cv::Mat& stripRoiImage,
                                           cv::Mat& stripRoiGrayImg, GlobalDataStruct::AnalysisResult& result, int stripPoint)
{
    cv::Point posAFront;
    cv::Point posBFront;
    cv::Point posABack;
    cv::Point posBBack;

    cv::Point posX2Front;
    cv::Point posX2BFront;
    cv::Point posX2Back;

    cv::Point posX3Front;
    cv::Point posX3BFront;
    cv::Point posX3Back;

    cv::Point lastPosAFront;
    cv::Point lastPosBFront;
    cv::Point lastPosABack;
    cv::Point lastPosBBack;

    int yThreshold = 0;
    int yDummyThreshold = 0;

    auto pcXThres = contentsRecipe.boxCarAnalaysis.pcThres;
    auto multiplePCXThres = contentsRecipe.multipleBandData.mutiplePcThres;

    auto boxcarWidth = contentsRecipe.boxCarAnalaysis.width;
    auto boxcarHeight = contentsRecipe.boxCarAnalaysis.height;

    auto frontPCXStart = contentsRecipe.boxCarAnalaysis.xStart;
    auto pcX2Start = contentsRecipe.multipleBandData.bandBlock2XStart;
    auto pcX3Start = contentsRecipe.multipleBandData.bandBlock3XStart;
    auto pcX4Start = contentsRecipe.multipleBandData.bandBlock4XStart;

    auto pcXGap = contentsRecipe.multipleBandData.pcXGap;
    auto pcX2Gap = contentsRecipe.multipleBandData.bandBlock2XGap;
    auto pcX3Gap = contentsRecipe.multipleBandData.bandBlock3XGap;
    auto pcX4Gap = contentsRecipe.multipleBandData.bandBlock4XGap;

    auto housingRoi = this->HousingROItoCVRect(contentsRecipe.housingROI);

    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, posAFront, posBFront, yThreshold, stripPoint, frontPCXStart, false, false, pcXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 1-1(X OR Y) ");
        return false;
    }

    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, lastPosAFront, lastPosBFront, yThreshold, stripPoint, pcX4Start, true, false, pcXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 4-1(X OR Y) ");
        return false;
    }

    auto calcPCX4Start = pcX4Gap + lastPosAFront.x + housingRoi.x;
    lastPosABack.y = lastPosAFront.y;

    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, lastPosABack, lastPosBBack, yDummyThreshold, stripPoint, calcPCX4Start, false, true, pcXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 4-2(X OR Y) ");
        return false;
    }

    QVector<cv::Rect> protoBandRect;
    protoBandRect.push_back(this->calcRect(posAFront.x, posAFront.y, boxcarWidth, boxcarHeight));
    protoBandRect.push_back(this->calcRect(lastPosABack.x, lastPosABack.y, boxcarWidth, boxcarHeight));

    auto rotationData  = 0.0;

    if(yThreshold <= 0)
        yThreshold = contentsRecipe.boxCarAnalaysis.housingThres;

    this->CalRotationAngle(stripRoiGrayImg, rotationData, protoBandRect, stripPoint, yThreshold);
    d->mConfigInstance->setRotation(rotationData);
    CameraInstance::rotation(stripRoiGrayImg, rotationData);

    // ###End angle calculation###

    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, posAFront, posBFront, yThreshold, stripPoint, frontPCXStart, true, false, pcXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 1-1(X OR Y) ");
        return false;
    }

    auto calcBackPCXStart = pcXGap + posAFront.x + housingRoi.x;
    posABack.y = posAFront.y;
    result.frontPMXPosition = calcBackPCXStart;

    if (this->FindPcXCenter(contentsRecipe, stripRoiGrayImg, posABack, stripPoint, calcBackPCXStart, pcXThres, true, true) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);

        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 1-2(XLine) ");
        return false;
    }


    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, posX2Front, posX2BFront, yThreshold, stripPoint, pcX2Start, true, false, multiplePCXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 2-1(X OR Y) ");
        return false;
    }


    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, posX3Front, posX3BFront, yThreshold, stripPoint, pcX3Start, true, false, multiplePCXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 3-1(X OR Y) ");
        return false;
    }

    if(this->FindPCCenter(contentsRecipe, stripRoiGrayImg, lastPosAFront, lastPosBFront, yThreshold, stripPoint, pcX4Start, true, false, multiplePCXThres) == false)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 4-1(X OR Y) ");
        return false;
    }

    posX2Back.y = posX2Front.y;
    posX3Back.y = posX3Front.y;

    auto calcX2Start = posX2Front.x + pcX2Gap + housingRoi.x;
    auto calcX3Start = posX3Front.x + pcX3Gap + housingRoi.x;

    calcPCX4Start = pcX4Gap + lastPosAFront.x + housingRoi.x;
    lastPosABack.y = lastPosAFront.y;
    result.backPMXPosition = calcPCX4Start;

    if (this->FindPcXCenter(contentsRecipe, stripRoiGrayImg, posX2Back, stripPoint, calcX2Start, multiplePCXThres, true, true) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 2-2(XLine) ");
        return false;
    }

    if (this->FindPcXCenter(contentsRecipe, stripRoiGrayImg, posX3Back, stripPoint, calcX3Start, multiplePCXThres, true, true) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 3-2(XLine) ");
        return false;
    }

    if (this->FindPcXCenter(contentsRecipe, stripRoiGrayImg, lastPosABack, stripPoint, calcPCX4Start, multiplePCXThres, true, true) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(stripRoiImage, fileName, stripPoint);
        result.isSuccess = false;
        result.errorReason = tr("[Strip Scan Error] : Can not Find Line 4-2(XLine) ");
        return false;
    }

    auto flagmunt = contentsRecipe.boxCarAnalaysis.boxCarbandCount/4;

    auto frontBandBlock = this->calcBandBlock(contentsRecipe, posAFront, posABack, flagmunt); // 1/4 4블록.....
    auto x2BandBlock = this->calcBandBlock(contentsRecipe, posX2Front, posX2Back, flagmunt); // 2/4 4블록
    auto x3BandBlock = this->calcBandBlock(contentsRecipe, posX3Front, posX3Back, flagmunt); // 3/4 4블록
    auto backBandBlock = this->calcBandBlock(contentsRecipe, lastPosAFront, lastPosABack, flagmunt); // 4/4 4블록

    QVector<int> pcPoint;
    pcPoint.push_back(0);

    QVector<cv::Rect> findBaseBand;
    findBaseBand.append(frontBandBlock);
    pcPoint.push_back(findBaseBand.count()-1);
    pcPoint.push_back(findBaseBand.count());

    findBaseBand.append(x2BandBlock);
    pcPoint.push_back(findBaseBand.count()-1);
    pcPoint.push_back(findBaseBand.count());

    findBaseBand.append(x3BandBlock);
    pcPoint.push_back(findBaseBand.count()-1);
    pcPoint.push_back(findBaseBand.count());

    findBaseBand.append(backBandBlock);
    pcPoint.push_back(findBaseBand.count()-1);

    result.pcPoint = pcPoint;

#pragma omp parallel for ordered
    for(int i = 0; i< findBaseBand.count(); i++)
    {
#pragma omp ordered
        {
            GlobalDataStruct::rectBandResult bandResult;
            bandResult.rectBand = findBaseBand[i];
            result.rectBandResult.push_back(bandResult);
        }
    }

    return true;
}

//find band only 추가.
GlobalDataStruct::AnalysisResult BasicSupremeIAnalysis::analysisSingleStrip(const Mat& matrix, QString contentsName,
                                                                                 int isFindBandOnly, bool isStaticImage, int stripPoint)
{
    GlobalDataStruct::AnalysisResult result;

    if(matrix.data == nullptr)
    {
        result.isSuccess = false;
        result.errorReason = tr("[Image Load Error] : ImageData Is NULL");
        return result;
    }

#ifdef QT_DEBUG
    QTime usedTime;
    usedTime.start();
#endif

    cv::Mat sourceImg;
    cv::Mat stripRoiGrayImg;
    matrix.copyTo(sourceImg);

    if(d->mConfigInstance->getContentsList().contains(contentsName) == false )
    {
        result.isSuccess = false;
        result.errorReason = tr("[Config Error] : Invalid Contents");
        return result;
    }

    auto contentsRecipe =  d->mConfigInstance->getStripSettings(contentsName);
    auto stripRoiRect = this->HousingROItoCVRect(contentsRecipe.housingROI);
    auto stripRoiImage = sourceImg(stripRoiRect);

#ifdef QT_DEBUG
    this->imageSave(stripRoiImage, "Source", stripPoint);
#endif

    cv::cvtColor(stripRoiImage, stripRoiGrayImg, cv::COLOR_BGR2GRAY);
    this->imageSave(stripRoiGrayImg, "LastGrayImage", stripPoint);

    cv::Mat imgBinary;

    auto threSholdValue = d->mConfigInstance->getBoxCarAnalysis().pcThres;
    cv::threshold(stripRoiGrayImg, imgBinary, threSholdValue, 255, cv::THRESH_BINARY);

    this->imageSave(imgBinary, "LastBinaryImage", stripPoint);

    auto isMultiBand = d->mConfigInstance->getIsUseMultiBand();

    if(isMultiBand)
    {
        if(this->findPCMultipleBand(contentsRecipe, stripRoiImage, stripRoiGrayImg, result, stripPoint) == false)
            return result;
    }
    else
    {
        if(this->findPCSingleBand(contentsRecipe, stripRoiImage, stripRoiGrayImg, result, stripPoint) == false)
            return result;
    }

#ifdef _USE_DYNAMIC_BAND
    this->setCorrectBandRect(contentsRecipe, stripRoiGrayImg, result);
    this->updateFailDyanmicBand(result);
    this->drawRectForLogging(result, stripRoiRect, stripPoint);
#endif

    if (isFindBandOnly)
    {
        result.isSuccess = true;
        return result;
    }

    QVector<cv::Mat> imageList;

    if(isStaticImage == false)
        imageList = this->getCaptureROIGrayImage(stripRoiRect, CAPTURE_FRAME_COUNT);
    else
        imageList = this->getStaticROIGrayImage(stripRoiGrayImg, d->mConfigInstance->getRotation(), d->mConfigInstance->getFlip(),CAPTURE_FRAME_COUNT);

    if(imageList.isEmpty())
    {
        result.isSuccess = false;
        result.errorReason = tr("[Image Load Error] : CaptureImageData Fail");
        return result;
    }

    this->calcIntensity(contentsRecipe, result, imageList);
    this->CalibrationOfAbnormalBackgroundData(result);

#ifdef QT_DEBUG
    qDebug() << "총 사용 시간(ms) : " <<  usedTime.elapsed();
#endif

    CameraInstance::rotation(stripRoiImage, d->mConfigInstance->getRotation());
    stripRoiImage.copyTo(result.image);

    if(result.image.data == nullptr)
    {
        result = GlobalDataStruct::AnalysisResult();
        result.isSuccess = false;
        result.errorReason = tr("[Image Save Error] : Result Image Fail");
        return result;
    }

    if(this->isValidBandData(result) == false)
    {
        result = GlobalDataStruct::AnalysisResult();
        result.isSuccess = false;
        result.errorReason = tr("[Band Scan Error] : A Minus band was detected.");
        return result;
    }

    if(this->isValidPCData(result) == false)
    {
        result = GlobalDataStruct::AnalysisResult();
        result.isSuccess = false;
        result.errorReason = tr("[PC Scan Error] : PC Band has a value below the threshold.");
        return result;
    }

    result.isSuccess = true;
    return result;
}

void BasicSupremeIAnalysis::analysisStart(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisStripList)
{
    d->mAnalysisList.clear();
    d->mAnalysisNextIdx = -1;
    d->mAnalysisList = analysisStripList;
    d->mAnalysisTotalCount = 0;
    d->mAnalysisCurrentCount = 0;

    connect(d->mCameraInstance, &CameraInstance::onStartResult, this, [this]
            (bool isSuccess)
            {
                if(d->mIsFirstStart == false)
                {
                    if(isSuccess == true)
                    {
                        d->mIsFirstStart = true;
                        this->onStartAnalysisThread();
                        emit onAnalysisStart(true);
                    }
                }
                else
                {
                    if(isSuccess == true)
                        this->onMovePosition();
                }
            });

    connect(d->mCameraInstance, &CameraInstance::onInitializingFail, this, [this]
            {
                emit onAnalysisStart(false);
            });

    QTimer::singleShot(1, d->mCameraInstance, &CameraInstance::instanceStart);
}

void BasicSupremeIAnalysis::findStripNextIdx(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisStripList, int& nextIdx)
{
    for(int i = nextIdx + 1; i < analysisStripList.count(); i++)
    {
        auto isValidPanel = (analysisStripList[i].element.panelName != "PANEL");
        auto isIDEmpty = analysisStripList[i].element.patientID.isEmpty();

        if(isValidPanel == true && isIDEmpty == false)
        {
            nextIdx = i;
            return;
        }
    }

    nextIdx = d->mAnalysisList.count();
}

void BasicSupremeIAnalysis::calcStripCount(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisStripList, int& count)
{
#pragma omp parallel for reduction(+:count)
    for(int i = 0; i < analysisStripList.count(); i++)
    {
        auto isValidPanel = (analysisStripList[i].element.panelName != "PANEL");
        auto isIDEmpty = analysisStripList[i].element.patientID.isEmpty();

        if(isValidPanel == true && isIDEmpty == false)
            count++;
    }
}

QString BasicSupremeIAnalysis::resultImageSave(const cv::Mat& matrix,  QString fileName)
{
    if(matrix.data == nullptr)
        return QString();

    QString fullPath = QString("ResultImage/%1").arg(QDateTime::currentDateTime().toString("yyyy-MM"));

    QDir dir;
    dir.mkpath(fullPath);

    fullPath.append("/");
    fileName.append(".png");
    fullPath.append(fileName);
    cv::imwrite(fullPath.toStdString().data(), matrix);

    return fullPath;
}




void BasicSupremeIAnalysis::onStop()
{
    d->mCameraInstance->instanceStop();

    if(d->mThread != nullptr)
    {
        d->mThread->terminate();
        d->mThread->wait();
        d->mThread = nullptr;
    }
}

void BasicSupremeIAnalysis::imageSave(const cv::Mat& matrix, QString fileName, int stripNumber, const QString& filePath)
{
    QString fullPath;

    if(filePath.isEmpty() == true)
        fullPath = d->mImageLogPath;
    else
        fullPath = filePath;

    fileName.prepend(QString("[Strip%1]-").arg(stripNumber));
    fileName.append(".png");

    fullPath.append("/");
    fullPath.append(fileName);

    cv::imwrite(fullPath.toStdString().data(), matrix);
}

void BasicSupremeIAnalysis::analysisRawImageSave(const cv::Mat& matrix, int stripNumber)
{
    if(matrix.data == nullptr)
        return;

    QString saveDirPath = QString("%1/%2").arg(d->mImageLogPath).arg(QDateTime::currentDateTime().toString("yyyy-MM"));
    QString fileName(QString("RawImage_%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));

    QDir dir;
    dir.mkpath(saveDirPath);

    this->imageSave(matrix, fileName, stripNumber,saveDirPath);
}

cv::Rect BasicSupremeIAnalysis::HousingROItoCVRect(const GlobalDataStruct::HOUSING_ROI& housingROI)
{
    cv::Rect roiRect;
    roiRect.x = housingROI.x1;
    roiRect.width = housingROI.x2 - housingROI.x1;
    roiRect.y = housingROI.y1;
    roiRect.height = housingROI.y2 - housingROI.y1;

    if(roiRect.x < 0)
        roiRect.x = 0 ;

    if(roiRect.width < 0 )
        roiRect.width = 0;

    if(roiRect.y < 0)
        roiRect.y = 0;

    if(roiRect.height < 0)
        roiRect.height = 0 ;

    return roiRect;
}

cv::Rect BasicSupremeIAnalysis::GuideLineToROItoCVRect(const GlobalDataStruct::GUIDE_LINE & guideLine)
{
    cv::Rect roiRect;
    roiRect.x = guideLine.x1;
    roiRect.width = guideLine.x2 - guideLine.x1;
    roiRect.y = guideLine.y1;
    roiRect.height = guideLine.y2 - guideLine.y1;

    if(roiRect.x < 0)
        roiRect.x = 0 ;

    if(roiRect.width < 0 )
        roiRect.width = 0;

    if(roiRect.y < 0)
        roiRect.y = 0;

    if(roiRect.height < 0)
        roiRect.height = 0 ;

    return roiRect;
}

bool BasicSupremeIAnalysis::FindPcYCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& recipe, const Mat &imgGray, cv::Point& pcPosA, cv::Point& pcPosB,
                                          int& threshold, int stripPoint, int startPoint, bool isRotated, bool isBackToFront)
{
    auto membraneHeightMin =  recipe.boxCarAnalaysis.height*2.0;
    auto membraneHeightMax =  recipe.boxCarAnalaysis.height*4.0;

    cv::Mat imgU8;
    imgGray.copyTo(imgU8);

    int nTempEdge[100] = {0,};
    int nFindEdge[2];

    nFindEdge[0] = -1;
    nFindEdge[1] = -1;

    memset(nTempEdge, -1, sizeof(nTempEdge));

    auto housingRoi = this->HousingROItoCVRect(recipe.housingROI);
    int nStartPoint =  startPoint - housingRoi.x;

    auto orgThreshold = recipe.boxCarAnalaysis.housingThres;
    auto nThresEnd = orgThreshold + 20;

    if (nThresEnd > 240)
        nThresEnd = 240;

    cv::Mat imgU8Binary;
    cv::Mat imgU8Blur;

    if(nStartPoint < 0)
        return false;

    if(recipe.boxCarAnalaysis.xRange < 0)
        return false;

    for (int iThreshold = orgThreshold; iThreshold < nThresEnd; iThreshold += 5)
    {
        cv::threshold(imgU8, imgU8Binary, iThreshold, 255, cv::THRESH_BINARY);
        imgU8Binary.copyTo(imgU8Blur);

#ifdef QT_DEBUG
        if(isRotated == true)
            this->imageSave(imgU8Blur, QString("Rotated-y_%1").arg(QString::number(iThreshold)), stripPoint);
        else
            this->imageSave(imgU8Blur, QString("y_%1").arg(QString::number(iThreshold)), stripPoint);
#endif

        if(isBackToFront == true)
        {
            for (int range = 0; range > (-recipe.boxCarAnalaysis.xRange); range--)
            {
                auto nEdgeCount = 0;
                pcPosA.x = nStartPoint + range;


                for (int y = 0; y <  housingRoi.height - 1; y++)
                {
                    auto tempPixel_1 = imgU8Blur.at<uchar>(y,pcPosA.x);
                    auto tempPixel_2 = imgU8Blur.at<uchar>(y+1,pcPosA.x);


                    if (abs(tempPixel_1 - tempPixel_2) == 255)
                        nTempEdge[nEdgeCount++] = y;
                }

                if (nEdgeCount >= 2)
                {
                    auto isCal1 = (abs(membraneHeightMax - (nTempEdge[1] - nTempEdge[0])) < abs(membraneHeightMax - (nFindEdge[1] - nFindEdge[0])));
                    auto isCal2 = (abs(nTempEdge[1] - nTempEdge[0]) >= membraneHeightMin);
                    auto isCal3 = (abs(nTempEdge[1] - nTempEdge[0]) <= membraneHeightMax);

                    // 찾은 edge point 간격이 높이*2 ~ 높이*4 흰 영역을 찾음(최대높이에 가까운것 우선)
                    if (isCal1 && isCal2 && isCal3)
                    {
                        nFindEdge[0] = nTempEdge[0];
                        nFindEdge[1] = nTempEdge[1];
                    }
                }

                if (nFindEdge[0] > 0)
                {
                    threshold = iThreshold;
                    break;
                }
            }
        }
        else
        {
            for (int range = 0; range < recipe.boxCarAnalaysis.xRange; range++)
            {
                auto nEdgeCount = 0;
                pcPosA.x = nStartPoint + range;


                for (int y = 0; y <  housingRoi.height - 1; y++)
                {
                    auto tempPixel_1 = imgU8Blur.at<uchar>(y,pcPosA.x);
                    auto tempPixel_2 = imgU8Blur.at<uchar>(y+1,pcPosA.x);


                    if (abs(tempPixel_1 - tempPixel_2) == 255)
                        nTempEdge[nEdgeCount++] = y;
                }

                if (nEdgeCount >= 2)
                {
                    auto isCal1 = (abs(membraneHeightMax - (nTempEdge[1] - nTempEdge[0])) < abs(membraneHeightMax - (nFindEdge[1] - nFindEdge[0])));
                    auto isCal2 = (abs(nTempEdge[1] - nTempEdge[0]) >= membraneHeightMin);
                    auto isCal3 = (abs(nTempEdge[1] - nTempEdge[0]) <= membraneHeightMax);

                    // 찾은 edge point 간격이 높이*2 ~ 높이*4 흰 영역을 찾음(최대높이에 가까운것 우선)
                    if (isCal1 && isCal2 && isCal3)
                    {
                        nFindEdge[0] = nTempEdge[0];
                        nFindEdge[1] = nTempEdge[1];
                    }
                }

                if (nFindEdge[0] > 0)
                {
                    threshold = iThreshold;
                    break;
                }
            }
        }



        if (nFindEdge[0] > 0)
        {
            threshold = iThreshold;
            break;
        }
    }

    if (nFindEdge[0] == -1) // Edge point를 찾지 못 했으면...
        return false;

    pcPosA.y = nFindEdge[0] + ((nFindEdge[1] - nFindEdge[0]) / 2) + 1;
    pcPosB.x = pcPosA.x + recipe.boxCarAnalaysis.xRange;
    pcPosB.y = nFindEdge[0] + ((nFindEdge[1] - nFindEdge[0]) / 2) + 1;

    return true;
}

bool BasicSupremeIAnalysis::FindPcXCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const Mat &imgGray, cv::Point& pcPosA,
                                          int stripPoint, int startPoint, int threshold, bool isRotated, bool isBackToFront)
{
    cv::Mat imgU8;
    imgGray.copyTo(imgU8);

    QVector<int> edge;
    int nFindEdge[2] = {-1, };

    int pcThreshold = threshold;
    int nThresEnd = pcThreshold + 20;

    if (nThresEnd > 240)
        nThresEnd = 240;

    cv::Mat imgU8Binary;
    cv::Mat imgU8Blur;

    auto housingRoi = this->HousingROItoCVRect(recipe.housingROI);

    if(pcThreshold < 0)
        return false;

    if(recipe.boxCarAnalaysis.xRange < 0)
        return false;

    for (int iThreshold = pcThreshold; iThreshold < nThresEnd; iThreshold += 5)
    {
        cv::threshold(imgU8, imgU8Binary, iThreshold, 255, cv::THRESH_BINARY);
        imgU8Binary.copyTo(imgU8Blur);

#ifdef QT_DEBUG
        if(isRotated == true)
            this->imageSave(imgU8Blur, QString("Rotated-x_%1").arg(QString::number(iThreshold)), stripPoint);
        else
            this->imageSave(imgU8Blur, QString("x_%1").arg(QString::number(iThreshold)), stripPoint);
#endif
        int nStartPoint = startPoint - housingRoi.x;

        if(nStartPoint < 0)
            nStartPoint = 0;


        if(isBackToFront == true)
        {
            for (int x =  nStartPoint; x >  nStartPoint - recipe.boxCarAnalaysis.xRange; x--)
            {
                unsigned char tempPixel_1 = imgU8Blur.data[pcPosA.y * (imgU8Blur.step) + x];
                unsigned char tempPixel_2 = imgU8Blur.data[pcPosA.y * (imgU8Blur.step) + (x - 1)];

                if (abs(tempPixel_1 - tempPixel_2) == 255)
                    edge.push_back(x);
            }
        }
        else
        {
            for (int x = nStartPoint; x < nStartPoint + recipe.boxCarAnalaysis.xRange - 1; x++)
            {
                unsigned char tempPixel_1 = imgU8Blur.data[pcPosA.y * (imgU8Blur.step) + x];
                unsigned char tempPixel_2 = imgU8Blur.data[pcPosA.y * (imgU8Blur.step) + (x + 1)];

                if (abs(tempPixel_1 - tempPixel_2) == 255)
                    edge.push_back(x);
            }
        }

        if (edge.count() >= 2)
        {
            if(isBackToFront == true)
                std::sort(edge.begin(), edge.end(), std::greater<int>());

            for (int i = 0; i < edge.count() - 1; i++)
            {
                auto edgeSize = abs(edge[i + 1] - edge[i]);
                auto cal1 = edgeSize >= (recipe.boxCarAnalaysis.width * 0.3); //  cal1 >=  4.2 == 4
                auto cal2 = edgeSize <= (recipe.boxCarAnalaysis.width * 0.75); // cal2 <=  10.5 == 10

                if (cal1 && cal2)
                {
                    nFindEdge[0] = edge[i];
                    nFindEdge[1] = edge[i + 1];
                    break;
                }
            }

            if (nFindEdge[0] > 0)
                break;
        }

        edge.clear();
    }

    if (nFindEdge[0] == -1)
        return false;

    pcPosA.x = nFindEdge[0] + ((nFindEdge[1] - nFindEdge[0]) / 2);

    if(isBackToFront == true)
        pcPosA.x -= 1;
    else
        pcPosA.x += 1;

    qDebug() << "pcPosA.x : " << pcPosA.x << " pcPosA.y : " << pcPosA.y;

    return true;
}

bool BasicSupremeIAnalysis::FindPCCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat& imgGray,
                                         cv::Point &pcPosA, cv::Point &pcPosB, int& threshold, int stripPoint, int startPoint, bool isRotated, bool isBackToFront,
                                         int pcXThres)
{
    if (this->FindPcYCenter(recipe, imgGray, pcPosA, pcPosB, threshold, stripPoint, startPoint, isRotated, isBackToFront) == false)
    {
        QString fileName(QString("Fail_%1_Y").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(imgGray, fileName, stripPoint);
        return false;
    }

    if (this->FindPcXCenter(recipe, imgGray, pcPosA, stripPoint, startPoint, pcXThres, isRotated, isBackToFront) == false)
    {
        QString fileName(QString("Fail_%1_X").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm")));
        this->imageSave(imgGray, fileName, stripPoint);
        return false;
    }


    return true;
}

bool BasicSupremeIAnalysis::FindBoxCarCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& recipe, const cv::Mat &imgGray, int x, int& y, int xRange)
{
    auto membraneHeightMin =  recipe.boxCarAnalaysis.height*2;
    auto membraneHeightMax =  recipe.boxCarAnalaysis.height*4;

    cv::Mat imgU8;
    imgGray.copyTo(imgU8);

    int nTempEdge[100] = {0,};
    int nFindEdge[2];

    nFindEdge[0] = -1;
    nFindEdge[1] = -1;

    memset(nTempEdge, -1, sizeof(nTempEdge));

    auto nStartPoint = x;
    auto orgThreshold = recipe.boxCarAnalaysis.housingThres;
    auto nThresEnd = orgThreshold + 20;

    if (nThresEnd > 240)
        nThresEnd = 240;

    cv::Mat imgU8Binary;
    cv::Mat imgU8Blur;

    if(nStartPoint < 0)
        return false;

    if(xRange < 0)
        return false;

    auto housingRoi = this->HousingROItoCVRect(recipe.housingROI);

    for (int iThreshold = orgThreshold; iThreshold < nThresEnd; iThreshold += 5)
    {
        cv::threshold(imgU8, imgU8Binary, iThreshold, 255, cv::THRESH_BINARY);
        imgU8Binary.copyTo(imgU8Blur);

        for (int range = 0; range < xRange; range++)
        {
            auto nEdgeCount = 0;

            for (int y = 0; y <  housingRoi.height - 1; y++)
            {
                auto tmepPixel_1 = imgU8Blur.data[y * (imgU8Blur.step) + nStartPoint + range];
                auto tmepPixel_2 = imgU8Blur.data[(y + 1) * (imgU8Blur.step) + nStartPoint + range];

                if (abs(tmepPixel_1 - tmepPixel_2) == 255)
                    nTempEdge[nEdgeCount++] = y;
            }

            if (nEdgeCount >= 2)
            {
                auto isCal1 = (abs(membraneHeightMax - (nTempEdge[1] - nTempEdge[0])) < abs(membraneHeightMax - (nFindEdge[1] - nFindEdge[0])));
                auto isCal2 = (abs(nTempEdge[1] - nTempEdge[0]) >= membraneHeightMin);
                auto isCal3 = (abs(nTempEdge[1] - nTempEdge[0]) <= membraneHeightMax);

                if (isCal1 && isCal2 && isCal3)
                {
                    nFindEdge[0] = nTempEdge[0];
                    nFindEdge[1] = nTempEdge[1];
                }
            }

            if (nFindEdge[0] > 0)
                break;
        }

        if (nFindEdge[0] > 0)
            break;
    }

    if (nFindEdge[0] == -1)
        return false;

    y = nFindEdge[0] + ((nFindEdge[1] - nFindEdge[0]) / 2) + 1;

    return true;
}

QVector<cv::Rect> BasicSupremeIAnalysis::setBandPosition(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Point& pcPosA)
{
    QVector<cv::Rect> rectBand;

    cv::Rect rect;
    rect.x = pcPosA.x - (recipe.boxCarAnalaysis.width / 2);
    rect.y = pcPosA.y - (recipe.boxCarAnalaysis.height / 2);
    rect.width = recipe.boxCarAnalaysis.width;
    rect.height = recipe.boxCarAnalaysis.height;

    if(rect.x < 0)
        rect.x = 0;

    if(rect.y < 0)
        rect.y = 0;

    if(rect.width < 0)
        rect.width = 0;

    if(rect.height < 0)
        rect.height = 0;

    rectBand.push_back(rect);

    this->GetBandRect(recipe, rectBand);

    return rectBand;
}

void BasicSupremeIAnalysis::GetBandRect(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, QVector<cv::Rect>& rectBand)
{
    auto rect = rectBand.first();

    auto housingRoi = this->HousingROItoCVRect(recipe.housingROI);
    auto boxCarBandGap = ConfigInformation::getInstance()->getBoxCarBandGap().boxCarBandGap;

#pragma omp parallel for ordered
    for (int i = 0; i < boxCarBandGap.count(); i++)
    {
#pragma omp ordered
        {
            cv::Rect rectBandPos;

            if (boxCarBandGap[i] > (housingRoi.width - rect.x))
                rectBandPos.x = rect.x;
            else
                rectBandPos.x = rect.x + boxCarBandGap[i];

            rectBandPos.y = rect.y;
            rectBandPos.width = rect.width;
            rectBandPos.height = rect.height;

            rectBand.push_back(rectBandPos);
        }
    }
}

bool BasicSupremeIAnalysis::rectComp(const cv::Rect& a, const cv::Rect& b)
{
    return (a.x < b.x);
}

bool BasicSupremeIAnalysis::rectCompDesc(const cv::Rect& a, const cv::Rect& b)
{
    return (a.x > b.x);
}

int BasicSupremeIAnalysis::findCenterPosition(QVector<int> posList)
{
    if(posList.isEmpty() == true)
        return 0;

    std::sort(posList.begin(), posList.end());

    return posList[posList.count()/2];
}

void BasicSupremeIAnalysis::setCorrectBandRect(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const Mat& imgGray, GlobalDataStruct::AnalysisResult& result)
{
    auto nWidth  = 0;
    auto nHeight = 0;

    nWidth  = recipe.boxCarAnalaysis.width;
    nHeight = recipe.boxCarAnalaysis.height;

    if(nWidth < 0)
        nWidth = 0;

    if(nHeight < 0)
        nHeight = 0;

    cv::Mat imgU8;
    imgGray.copyTo(imgU8);

#pragma omp parallel for
    for(int i = 0; i < result.rectBandResult.count(); i++)
        result.rectBandResult[i].rectDynamicBand = result.rectBandResult[i].rectBand;

    this->findEdgePoint(imgU8, result, nWidth, nHeight);
    this->copyBoxCar(result);
}

bool BasicSupremeIAnalysis::calcFitedStripSize(const cv::Mat& imgU8, const GlobalDataStruct::AnalysisResult& result, int& width, int& height)
{
    if(result.rectBandResult.count() < 1)
        return false;

    width = result.rectBandResult.last().rectDynamicBand.x + result.rectBandResult.last().rectDynamicBand.width
            - result.rectBandResult.first().rectDynamicBand.x;
    height = result.rectBandResult.first().rectDynamicBand.height;

    if(width < 0)
        width = 0;

    if(height < 0)
        height = 0;

    if ((result.rectBandResult.first().rectDynamicBand.x + width) > imgU8.size().width)
        return false;

    if ((result.rectBandResult.first().rectDynamicBand.y + height) > imgU8.size().height)
        return false;

    return true;
}

void BasicSupremeIAnalysis::GetTwoEdgePoint(const cv::Mat& imgU8,const cv::Rect& dynamicBand, int nWidth, int nHeight,
                                            int &forwardEdgePoint, int &backEdgePoint, int &forwardRisingFalling, int &backRisingFalling)
{
    QVector<int> forwardEdgePointArr(nHeight);
    QVector<int> backEdgePointArr(nHeight);
    QVector<uchar> data(nWidth);

    int	nForwardRisingCount = 0;
    int nBackRisingCount = 0;

    if(imgU8.size().width < (dynamicBand.x + dynamicBand.width))
    {
        Q_ASSERT("[GetTwoEdgePoint] : DynamicBand Width Size OverFlow");
        return ;
    }

    auto imgRoi = imgU8(dynamicBand);
    auto pImgU8 = IplImage(imgRoi);

    for(int y = 0; y < nHeight; y++)
    {
        int nForwardRisingFalling = eNone;
        int nBackRisingFalling = eNone;

        cvSampleLine(&pImgU8, cvPoint(0, y), cvPoint(nWidth, y), data.data()); //need for c -> c++

        this->BandBoxcarLineFind(nWidth, data, forwardEdgePointArr[y], backEdgePointArr[y], nForwardRisingFalling, nBackRisingFalling);

        if((nForwardRisingFalling == eRising) && (forwardEdgePointArr[y] > -1))
            nForwardRisingCount++;

        if((nBackRisingFalling == eRising) && (backEdgePointArr[y] > -1))
            nBackRisingCount++;
    }

    forwardEdgePoint = this->CalcHistogram_Ver2(nHeight, forwardEdgePointArr);
    backEdgePoint = this->CalcHistogram_Ver2(nHeight, backEdgePointArr);

    if(forwardEdgePoint == -1)
        forwardRisingFalling = eNone;

    else if(nForwardRisingCount > ((double)nHeight * 0.49)) // org 0.49
        forwardRisingFalling = eRising;

    else
        forwardRisingFalling = eFalling;

    if(backEdgePoint == -1)
        backRisingFalling = eNone;

    else if(nBackRisingCount > ((double)nHeight * 0.49))// org 0.49
        backRisingFalling = eRising;

    else
        backRisingFalling = eFalling;
}

void BasicSupremeIAnalysis::BandBoxcarLineFind(int width, const QVector<uchar>& lineData, int &nForwardEdge, int &nBackEdge, int &nForwardRisingFalling, int &nBackRisingFalling)
{
    double dMax = DBL_MIN;
    double dMin = DBL_MAX;
    int nFront = -1;
    int nBack = -1;
    int nBlackCount = 0;
    int  nStart = 0;

    for(int i = 0; i < width; i++)
    {
        double dval = lineData[i];

        if(dval > dMax)
            dMax = lineData[i];
        if(dval < dMin)
            dMin = lineData[i];

        //check for is this black column or band?
        if(dval < 40.0) // original threshold value(50.0)
            nBlackCount++;
    }

    const int bandWidthLimit = static_cast<int>(width * 0.7); //band의 최대 범위 70%

    if(nBlackCount > bandWidthLimit) //original value hard coded 10pixel(not percent) -> 80% -> 70%
    {
        nForwardEdge	= -2;
        nBackEdge	= -2;
        return;
    }

    double dRange = abs(dMin - dMax);
    double dCompare = 0.0;

    //edge값의 변화가 거의 없으면
    if(dRange < 18) // 9-> 18
    {
        nForwardEdge	= nFront;
        nBackEdge	= nBack;
        return;
    }

    else if(dRange < 25)
        dCompare = dRange * 0.15;

    else if(dRange < 50)
        dCompare = dRange * 0.175;

    else if(dRange < 100)
        dCompare = dRange * 0.2;
    else
        dCompare = dRange * 0.23;

    for(int i = 1; i < width; i++)		// 앞 라인
    {
        if((lineData[i - 1] - lineData[i]) > dCompare)
        {
            nForwardRisingFalling = eRising;
            nFront = i;
            break;
        }
        else if((lineData[i - 1] - lineData[i]) < -dCompare)
        {
            nForwardRisingFalling = eFalling;
            nFront = i;
            break;
        }
    }

    nStart = width - 1;
    for(int i = nStart; i > nFront + 1; i--)		// 뒤 라인
    {
        if((lineData[i] - lineData[i - 1]) > dCompare)
        {
            nBackRisingFalling = eRising;
            nBack = i - 1;
            break;
        }
        else if((lineData[i] - lineData[i - 1]) < -dCompare)
        {
            nBackRisingFalling = eFalling;
            nBack = i - 1;
            break;
        }
    }

    nForwardEdge  = nFront;
    nBackEdge = nBack;
}


void BasicSupremeIAnalysis::findEdgePoint(cv::Mat imgU8, GlobalDataStruct::AnalysisResult& result, const int width, const int height)
{
    auto bandCount = result.rectBandResult.count();

    for(int i = 0; i < bandCount; i++)
    {
        if(result.pcPoint.contains(i))
        {
            result.rectBandResult[i].rectDynamicBand.x = result.rectBandResult[i].rectBand.x;
            continue;
        }

        int weightBegin = -1;
        int weightEnd = 2;

        auto currentRect = result.rectBandResult[i].rectDynamicBand;

        QVector<int> findEdges;

        for(int posWeight = weightBegin; posWeight < weightEnd; posWeight++)
        {
            auto findRect = currentRect;
            findRect.x = currentRect.x + posWeight;

            auto forwardEdgePoint = -1;
            auto backEdgePoint = -1;
            auto forwardRisingFalling = -1;
            auto backRisingFalling  = -1;
            auto nCenter = 0;

            this->GetTwoEdgePoint(imgU8, findRect, width, height, forwardEdgePoint, backEdgePoint, forwardRisingFalling, backRisingFalling);

            if((forwardEdgePoint == -1) && (backEdgePoint == -1))
                continue;

            if((forwardRisingFalling == eRising) && (backRisingFalling == eRising))
            {
                if((forwardEdgePoint > -1) && (backEdgePoint > -1))
                {
                    nCenter = forwardEdgePoint + (int)(((double)backEdgePoint - (double)forwardEdgePoint) / 2.0);
                    auto findPosX = findRect.x + (int)(nCenter - ((double)width / 2.0));
                    findEdges.push_back(findPosX);
                    continue;
                }
            }
        }

        if(findEdges.isEmpty() == false)
        {
            result.rectBandResult[i].rectDynamicBand.x = this->findCenterPosition(findEdges);
            result.rectBandResult[i].isFail = false;
        }
        else
        {
            result.rectBandResult[i].rectDynamicBand = result.rectBandResult[i].rectBand;
            result.rectBandResult[i].isFail = true;
        }
    }
}

void BasicSupremeIAnalysis::copyBoxCar(GlobalDataStruct::AnalysisResult& result)
{
    auto dynamicBandCount = result.rectBandResult.count();
    auto dynamicRange = 5;

    for(int i = 0; i < dynamicBandCount; i++)
    {
        if (abs(result.rectBandResult[i].rectDynamicBand.x - result.rectBandResult[i].rectBand.x) > dynamicRange)
            result.rectBandResult[i].rectDynamicBand = result.rectBandResult[i].rectBand;
    }
}

int BasicSupremeIAnalysis::CalcHistogram_Ver2(int nData, const QVector<int>& inputData)
{
    int nVal = 0;
    int nNearMaxCount = INT_MIN;
    int nPerfectMaxCount = INT_MIN;
    auto nNearCount = 0;
    auto nNearMaxValue = 0;
    auto nPerfectCount = 0;
    auto nPerfectMaxValue = 0;

    for(int i = 0; i < nData; i++)
    {
        nNearCount = 0;
        nPerfectCount = 0;
        nVal = inputData[i];

        if(nVal > -1)
        {
            for(int j = 0; j < nData; j++)
            {
                if(abs(nVal - inputData[j]) < 2)
                    nNearCount++;

                if(nVal == inputData[j])
                    nPerfectCount++;
            }

            if(nNearMaxCount < nNearCount)
            {
                nNearMaxCount = nNearCount;
                nNearMaxValue = nVal;
            }

            if(nPerfectMaxCount < nPerfectCount)
            {
                nPerfectMaxCount = nPerfectCount;
                nPerfectMaxValue = nVal;
            }
        }
    }

    if(nNearMaxCount >= (nData * 0.5)) // org 0.5
    {
        if(abs(nPerfectMaxValue - nNearMaxValue) < 2)
            return nPerfectMaxValue;
        else
            return nNearMaxValue;
    }
    else
        return -1;
}

void BasicSupremeIAnalysis::calcIntensity(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, GlobalDataStruct::AnalysisResult& result, QVector<cv::Mat>& imageList)
{

#if USE_SORT_INTENSITY

    int bandCount = result.rectBandResult.count();
    int frameCount = imageList.count();

    double allFrameIntensityValue[bandCount][frameCount];
    double allFrameBandValue[bandCount][frameCount];
    double allFrameBackgroundValue[bandCount][frameCount];

    memset(allFrameIntensityValue, 0, sizeof(allFrameIntensityValue));
    memset(allFrameBandValue, 0, sizeof(allFrameBandValue));
    memset(allFrameBackgroundValue, 0, sizeof(allFrameBackgroundValue));

#pragma omp parallel for ordered
    for (int frameIdx = 0; frameIdx < frameCount; frameIdx++)
    {
#pragma omp ordered
        {
            QVector<double> intensityValue;
            QVector<double> bandValue;
            QVector<double> backgroundValue;

            //하나의 프레임에서 intensity 추출
            this->calcBandValue(recipe, result, imageList[frameIdx], intensityValue, bandValue, backgroundValue);

#pragma omp parallel for
            for (int nBandIdx = 0; nBandIdx < bandCount; nBandIdx++)
            {
                allFrameIntensityValue[nBandIdx][frameIdx] = intensityValue[nBandIdx];
                allFrameBandValue[nBandIdx][frameIdx] = bandValue[nBandIdx];
                allFrameBackgroundValue[nBandIdx][frameIdx] = backgroundValue[nBandIdx];
            }

        }
    }

    const auto frameFilterCount = frameCount * 0.3;

#pragma omp parallel for ordered
    for (int nBandIdx = 0; nBandIdx < bandCount; nBandIdx++)
    {
#pragma omp ordered
        {
            std::sort(allFrameIntensityValue[nBandIdx], allFrameIntensityValue[nBandIdx] + frameCount- 1);		// Ascending Sort
            std::sort(allFrameBackgroundValue[nBandIdx], allFrameBackgroundValue[nBandIdx] + frameCount -1);
            std::sort(allFrameBandValue[nBandIdx], allFrameBandValue[nBandIdx] + frameCount -1);

            int nStart = ((frameCount / 2) - (frameFilterCount / 2)) - 1;

            auto intensitySum = 0.0;
            auto backgroundSum = 0.0;
            auto bandValueSum = 0.0;

#pragma omp parallel for schedule(auto) reduction(+:intensitySum, backgroundSum, bandValueSum)
            for (int frameIdx = nStart; frameIdx < nStart + frameFilterCount; frameIdx++)
            {
                intensitySum += allFrameIntensityValue[nBandIdx][frameIdx];
                backgroundSum += allFrameBackgroundValue[nBandIdx][frameIdx];
                bandValueSum += allFrameBandValue[nBandIdx][frameIdx];
            }

            double intensity = intensitySum / (double)frameFilterCount;
            double background = backgroundSum / (double)frameFilterCount;
            double bandValue = bandValueSum / (double)frameFilterCount;

            result.intensityValue.push_back(intensity);
            result.backgroundValue.push_back(background);
            result.bandValue.push_back(bandValue);
        }
    }
#elif USE_AREA_INTENSITY

    int bandCount = result.rectBandResult.count();
    int frameCount = imageList.count();

    double allFrameIntensityValue[bandCount][frameCount];
    double allFrameBackgroundValue[bandCount][frameCount];
    double allFrameBandValue[bandCount][frameCount];

    memset(allFrameIntensityValue, 0, sizeof(allFrameIntensityValue));
    memset(allFrameBackgroundValue, 0, sizeof(allFrameBackgroundValue));
    memset(allFrameBandValue, 0, sizeof(allFrameBandValue));

#if USE_EXPORT_BAND_DATA_TO_EXCEL
    CImageToExcelData excelData;
    excelData.open("Test");
#endif

#pragma omp parallel for ordered
    for (int frameIdx = 0; frameIdx < frameCount; frameIdx++)
    {
#pragma omp ordered
        {
            QVector<double> intensityValue;
            QVector<double> backgroundValue;
            QVector<double> bandValue;

            auto bandDataMatrix =  this->calcBandValue(recipe, result, imageList[frameIdx], intensityValue, bandValue, backgroundValue);

#if USE_EXPORT_BAND_DATA_TO_EXCEL
            excelData.addData(bandDataMatrix);
#endif

#pragma omp parallel for
            for (int nBandIdx = 0; nBandIdx < bandCount; nBandIdx++)
            {
                allFrameIntensityValue[nBandIdx][frameIdx] = intensityValue[nBandIdx];
                allFrameBackgroundValue[nBandIdx][frameIdx] = backgroundValue[nBandIdx];
                allFrameBandValue[nBandIdx][frameIdx] = bandValue[nBandIdx];
            }
        }
    }

//프레임 데이터의 평균을 냄
#pragma omp parallel for ordered
    for (int nBandIdx = 0; nBandIdx < bandCount; nBandIdx++)
    {
#pragma omp ordered
        {
            auto intensitySum = 0.0;
            auto backgroundSum = 0.0;
            auto bandValueSum = 0.0;

#pragma omp parallel for schedule(auto) reduction(+:intensitySum, backgroundSum, bandValueSum)
            for (int frameIdx = 0; frameIdx < frameCount; frameIdx++)
            {
                intensitySum  += allFrameIntensityValue[nBandIdx][frameIdx];
                backgroundSum += allFrameBackgroundValue[nBandIdx][frameIdx];
                bandValueSum  += allFrameBandValue[nBandIdx][frameIdx];
            }

            double intensity = intensitySum / (double)frameCount;
            double background = backgroundSum / (double)frameCount;
            double bandValue = bandValueSum / (double)frameCount;

            result.rawIntensityValue.push_back(intensity);
            result.backgroundValue.push_back(background);
            result.bandValue.push_back(bandValue);
        }
    }
#endif
}

void BasicSupremeIAnalysis::CalibrationOfAbnormalBackgroundData(GlobalDataStruct::AnalysisResult& result)
{
    GenericIniDataControls io;
    auto backgroundCutoff = io.readBackgroundCutoffData();

    if(backgroundCutoff == 0)
        return;

    double bgAvgValue = 0.0;
    int nomalBGCount = 0;

    for(auto& itr : result.backgroundValue)
    {
        auto isNotNomalData = (backgroundCutoff <= itr);

        if(isNotNomalData)
            continue;

        bgAvgValue += itr;
        nomalBGCount++;
    }

    if(qFuzzyIsNull(bgAvgValue))
        return;

    if(nomalBGCount == 0)
        return;

    bgAvgValue = (bgAvgValue/nomalBGCount);

    for(auto i = 0 ; i < result.backgroundValue.count(); i++)
    {
        auto itr = result.backgroundValue[i];
        auto isNomalData = (itr <= backgroundCutoff); //80 <= 110

        if(isNomalData)
            continue;


        result.backgroundValue[i] = bgAvgValue;
        result.rawIntensityValue[i] = result.bandValue[i] - bgAvgValue;
    }
}

QVector<cv::Mat> BasicSupremeIAnalysis::getCaptureROIGrayImage(cv::Rect roi, int captureCount)
{
    QVector<cv::Mat> imageList;

    for (int nFrame = 0; nFrame < captureCount; nFrame++)
    {
        auto camera = CameraInstance::getInstance();
        auto image = camera->getFullConfigureImage();

        if(image.data == nullptr)
            return QVector<cv::Mat>();

        cv::Mat imgRoi;
        cv::Mat stripImgRoiGray;
        imgRoi = image(roi);
        cv::cvtColor(imgRoi, stripImgRoiGray, cv::COLOR_BGR2GRAY);
        imageList.push_back(stripImgRoiGray);
        d->mThread->msleep(FRAME_FRAGMENT_MS_TIME);
    }

    return imageList;
}

QVector<cv::Mat> BasicSupremeIAnalysis::getStaticROIGrayImage(const cv::Mat& grayImg, const double rotation, const int flip, int captureCount)
{
    QVector<cv::Mat> imageList;

    for (int nFrame = 0; nFrame < captureCount; nFrame++)
    {
        cv::Mat image;
        grayImg.copyTo(image);

        if(image.data == nullptr)
            return QVector<cv::Mat>();

        CameraInstance::rotationFlip(image, flip, rotation);
        imageList.push_back(image);
    }

    return imageList;
}


QVector<cv::Mat> BasicSupremeIAnalysis::calcBandValue(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, GlobalDataStruct::AnalysisResult& result, const cv::Mat& roiGrayImage,
                                          QVector<double>& intensityValue, QVector<double>& bandValue, QVector<double>& backgroundValue)
{
    QVector<cv::Mat> clacBandData;
    int bandCount = result.rectBandResult.count();
    this->clacBackGroundAverageData(result, roiGrayImage);

    for(int i = 0; i < bandCount; i++)
    {
        auto rect = result.rectBandResult[i].rectDynamicBand;
        bool isWidthCorrect = (rect.x + rect.width) < (roiGrayImage.size().width + 1);

        if(isWidthCorrect == true)
            clacBandData.append(this->calcBackGroundROIAndIntensity(i, roiGrayImage, result, intensityValue, bandValue, backgroundValue));
        else
            break;
    }

    return clacBandData;
}

void BasicSupremeIAnalysis::clacBackGroundAverageData(const GlobalDataStruct::AnalysisResult& result, const cv::Mat& roiGrayImage)
{
    auto configRemoval = d->mConfigInstance->getPaticleRemoval();
    d->mPrevAverageBG = 0;
    d->mNextAverageBG = 0;

    QVector<cv::Mat> prevBackGroundList;
    QVector<cv::Mat> nextBackGroundList;

    for(auto& itr : result.rectBandResult)
    {
        auto rect = itr.rectDynamicBand;

        if(this->isCorrectBackGroundRange(rect, roiGrayImage))
        {
            auto prevRect = this->getPrevBackGroundRect(rect);
            auto nextRect = this->getNextBackGroundRect(rect);

            cv::Mat prevBackground = roiGrayImage(prevRect);
            cv::Mat nextBackGround = roiGrayImage(nextRect);

            cv::Mat prevBackgroundImage;
            cv::Mat nextBackGroundImage;

            prevBackground.copyTo(prevBackgroundImage);
            nextBackGround.copyTo(nextBackGroundImage);

            this->yLineParticleEraseProc(prevBackgroundImage, configRemoval.removalPaticle, configRemoval.averagePaticle, configRemoval.threshold);
            this->yLineParticleEraseProc(nextBackGroundImage, configRemoval.removalPaticle, configRemoval.averagePaticle, configRemoval.threshold);

            prevBackGroundList.push_back(prevBackgroundImage);
            nextBackGroundList.push_back(nextBackGroundImage);
        }
    }

    auto prevAvg = this->clacAverageImageData(prevBackGroundList);
    auto nextAvg = this->clacAverageImageData(nextBackGroundList);

    d->mPrevAverageBG = prevAvg;
    d->mNextAverageBG = nextAvg;
}

QVector<cv::Mat> BasicSupremeIAnalysis::calcBackGroundROIAndIntensity(const int index, const cv::Mat& roiGrayImage, const GlobalDataStruct::AnalysisResult& result,
                                                                      QVector<double>& intensityValue, QVector<double>& bandValue, QVector<double>& backgroundValue)
{
    QVector<cv::Mat> calcMat;
    auto rect = result.rectBandResult[index].rectDynamicBand;
    auto bandCount = result.rectBandResult.count();

    cv::Mat boxCarRoi = roiGrayImage(rect);

#if USE_SORT_INTENSITY
    if(result.pcPoint.contains(i) == false)
        this->yLineParticleEraseProc(boxCarRoi, configRemoval.removalPaticle, configRemoval.averagePaticle, configRemoval.threshold);

    this->GetSortBandIntensityValue(recipe, boxCarRoi, intensityValue, bandValue, backgroundValue);			//intensity 계산
#elif USE_AREA_INTENSITY

    if(this->isCorrectBackGroundRange(rect, roiGrayImage))
    {
        if(index+1 == bandCount)
            rect = result.rectBandResult[index-1].rectDynamicBand;

#if USE_EXPORT_BAND_DATA_TO_EXCEL
        calcMat.push_back(boxCarRoi);
#endif
        auto backGroundValue = this->getAreaBackGroundValue(rect, roiGrayImage, calcMat);
        this->GetAreaBandIntensityValue(boxCarRoi, backGroundValue, intensityValue, bandValue, backgroundValue);
    }
#endif

    return calcMat;
}

int BasicSupremeIAnalysis::clacAverageImageData(const QVector<cv::Mat> matList)
{
    QVector<double> bgAvgList;

    for(auto& itr : matList)
    {
        auto imageSum = this->GetSectionSum(itr);
        auto averageBg = (double)(imageSum/ double(itr.size().width * itr.size().height));

        bgAvgList << averageBg;
    }

    std::sort(bgAvgList.begin(), bgAvgList.end());
    QVector<double> filteredBgAvgList;

    auto bgAvgBeginIndex = (bgAvgList.count()*0.3);
    auto bgAvgEndIndex = (bgAvgList.count()*0.7);

    for(int i = bgAvgBeginIndex; i < bgAvgEndIndex; i++)
        filteredBgAvgList << bgAvgList[i];

    auto sumBgAvg = 0.0;

#pragma omp parallel for reduction(+:sumBgAvg)
    for(auto i = 0; i < filteredBgAvgList.count(); i++)
        sumBgAvg += filteredBgAvgList[i];

    return (sumBgAvg/filteredBgAvgList.count());
}

bool BasicSupremeIAnalysis::isSmudgedBackground(const cv::Mat& roiGrayImage, bool isPrev)
{
    double imageSum = 0.0;
    cv::Mat backGround;
    roiGrayImage.copyTo(backGround);

    auto clacSmudged = [] (double avgBG, double imageSum) -> bool
    {
        auto baseValue = (avgBG * 0.025);
        auto isSmudged = baseValue < (avgBG - imageSum);

        return isSmudged;
    };

    if(isPrev)
    {
        imageSum = this->getSectionAverage(backGround);
        auto isSmudged = clacSmudged(d->mPrevAverageBG, imageSum);

        if(isSmudged)
            return true;
    }
    else
    {
        imageSum = this->getSectionAverage(backGround);
        auto isSmudged = clacSmudged(d->mNextAverageBG, imageSum);

        if(isSmudged)
            return true;
    }

    return false;
}

bool BasicSupremeIAnalysis::isCorrectBackGroundRange(const cv::Rect& targetRect, const cv::Mat& originalImage)
{
    auto isBackGroundCorrectPrev =  0 < (targetRect.x - 1);
    auto isBackGroundCorrectNext =  (targetRect.x + targetRect.width + 2) < (originalImage.size().width + 1);

    if(isBackGroundCorrectPrev == true && isBackGroundCorrectNext == true)
        return true;

    return false;
}

cv::Rect BasicSupremeIAnalysis::getPrevBackGroundRect(const cv::Rect& rect)
{
    auto prevRect = rect;
    prevRect.x = rect.x;
    prevRect.width = 2;

    return prevRect;
}

cv::Rect BasicSupremeIAnalysis::getNextBackGroundRect(const cv::Rect& rect)
{
    auto nextRect = rect;
    nextRect.x = (rect.x + rect.width - 1);
    nextRect.width = 2;

    return nextRect;
}

void BasicSupremeIAnalysis::GetSortBandIntensityValue(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat& boxcarRoi,
                                                  QVector<double>& intensityValue, QVector<double>& bandValue, QVector<double>& backgroundValue)
{
    int nYLineNoiseCount = 0;
    int nWidth  = boxcarRoi.size().width;
    int nHeight = boxcarRoi.size().height;

    QVector<uchar> sortValue(nWidth);
    QVector<uchar> pData(nHeight);
    auto srcImage = IplImage(boxcarRoi);

    for(int x = 0; x < nWidth; x++)	//Line Intensity 얻기
    {
        cvSampleLine(&srcImage, cvPoint(x, 0), cvPoint(x, nHeight), pData.data());

        std::sort(pData.begin(), pData.end());		// Ascending Sort

        //각 Line Intensity 평균값. (Line내에 각 픽셀 정렬하여 앞/뒤 2픽셀씩 제외하고 Intensity를 구하도록 함)
        sortValue[x] = (uchar)(this->GetSectionSum(pData, nYLineNoiseCount, nHeight - nYLineNoiseCount) / ((nHeight - (nYLineNoiseCount * 2))));
    }

    std::sort(sortValue.begin(), sortValue.end());		//Line Intensity 정렬

    auto dSumBandValue = 0.0;
    auto dSumBgValue = 0.0;

    auto nParticle_Noise_Count =  recipe.intensity.paticle;
    auto nIntensity_Count      = recipe.intensity.band;
    auto nBackground_Count     = recipe.intensity.background;
    auto nWhite_Noise_Count    = recipe.intensity.white;

    dSumBandValue = this->GetSectionSum(sortValue, nParticle_Noise_Count, nParticle_Noise_Count + nIntensity_Count);
    auto dBandValue = 255.0 - (dSumBandValue / (double)nIntensity_Count);

#pragma omp parallel for schedule(auto) reduction(+:dSumBgValue)
    for(int i = nWidth - nWhite_Noise_Count - 1; i > nWidth - nWhite_Noise_Count - nBackground_Count - 1; i--)
        dSumBgValue += sortValue[i];			//BackGround 계산

    auto dBackgroundValue = 0.0;

    if(nBackground_Count > 0)
        dBackgroundValue = 255.0 - (dSumBgValue / (double)nBackground_Count);

    auto intensitiValue = dBandValue - dBackgroundValue;

    if(intensitiValue < 0)
        intensitiValue = 0.0;

    if(intensitiValue > 255)
        intensitiValue = 255.0;

    intensityValue.push_back(intensitiValue);
    backgroundValue.push_back(dBackgroundValue);
    bandValue.push_back(dBandValue);
}

double BasicSupremeIAnalysis::getSectionAverage(const cv::Mat& image)
{
    cv::Mat backGround;
    image.copyTo(backGround);

    auto currentSum = this->GetSectionSum(backGround);

    return (double)(currentSum/ double(backGround.size().width * backGround.size().height));
}

double BasicSupremeIAnalysis::getAreaBackGroundValue(const cv::Rect &rect, const cv::Mat& roiGrayImage, QVector<cv::Mat>& calcMat)
{
    auto configRemoval = d->mConfigInstance->getPaticleRemoval();
    auto prevRect = this->getPrevBackGroundRect(rect);
    auto nextRect = this->getNextBackGroundRect(rect);

    cv::Mat prevBackground = roiGrayImage(prevRect);
    cv::Mat nextBackGround = roiGrayImage(nextRect);

    cv::Mat prevBackgroundImage;
    cv::Mat nextBackGroundImage;

    prevBackground.copyTo(prevBackgroundImage);
    nextBackGround.copyTo(nextBackGroundImage);

    this->yLineParticleEraseProc(prevBackgroundImage, configRemoval.removalPaticle, configRemoval.averagePaticle, configRemoval.threshold);
    this->yLineParticleEraseProc(nextBackGroundImage, configRemoval.removalPaticle, configRemoval.averagePaticle, configRemoval.threshold);

#if USE_EXPORT_BAND_DATA_TO_EXCEL
    calcMat.push_front(prevBackground);
    calcMat.push_back(nextBackGround);
#endif

    double prevBackGroundValue = 0.0;
    double nextBackGroundValue = 0.0;

#if 0 //1.0.4.0
    if(this->isSmudgedBackground(prevBackground, true))
        prevBackGroundValue = d->mPrevAverageBG;
    else
        prevBackGroundValue = this->getSectionAverage(prevBackground);

    if(this->isSmudgedBackground(nextBackGround, false))
        nextBackGroundValue = d->mNextAverageBG;
    else
        nextBackGroundValue = this->getSectionAverage(nextBackGround);
#else //1.0.3.0
    prevBackGroundValue = this->getSectionAverage(prevBackgroundImage);
    nextBackGroundValue = this->getSectionAverage(nextBackGroundImage);
#endif

    prevBackGroundValue = 255.0 - prevBackGroundValue;
    nextBackGroundValue = 255.0 - nextBackGroundValue;

    return (prevBackGroundValue + nextBackGroundValue)/2.0;
}

void BasicSupremeIAnalysis::GetAreaBandIntensityValue(const cv::Mat &boxcarRoi, const double backGroundValue,
                                                      QVector<double> &intensityValue, QVector<double> &bandValue, QVector<double> &backgroundValue)
{
    auto configRemoval = d->mConfigInstance->getPaticleRemoval();
    cv::Mat srcImage;
    boxcarRoi.copyTo(srcImage);

    this->yLineParticleEraseProc(srcImage, configRemoval.removalPaticle, configRemoval.averagePaticle, configRemoval.threshold);

    auto resultBandValue = 255 - this->getSectionAverage(srcImage);
    auto resultIntensityValue = resultBandValue - backGroundValue;

    intensityValue.push_back(resultIntensityValue);
    backgroundValue.push_back(backGroundValue);
    bandValue.push_back(resultBandValue);
}

double BasicSupremeIAnalysis::GetSectionSum(const cv::Mat& data)
{
    double dSum = 0;
    auto dataSize = data.size().width * data.size().height;

#pragma omp parallel for schedule(auto) reduction(+:dSum)
    for(int i = 0; i < dataSize; i++)
        dSum = data.data[i] + dSum;

    return dSum;
}

double BasicSupremeIAnalysis::GetSectionSum(const QVector<uchar>& pData, int nStart, int nEnd)
{
    if(nStart < 0)
        nStart = 0;

    if(pData.count() < nEnd)
        nEnd = pData.count();

    double dSum = 0.0;

#pragma omp parallel for schedule(auto) reduction(+:dSum)
    for(int i = nStart; i < nEnd; i++)
        dSum = pData[i] + dSum;

    return dSum;
}

bool BasicSupremeIAnalysis::CalRotationAngle(const cv::Mat& stripGrayRoiImg, double &rotation, QVector<cv::Rect>& bandRect,
                                             int stripPoint, int yThreshold)
{
    cv::Mat stripRoiBinaryImg;

    if(0 < yThreshold)
        cv::threshold(stripGrayRoiImg, stripRoiBinaryImg, yThreshold, 255, cv::THRESH_BINARY);
    else
        return false;

    rotation = 0.0;

    cv::Point2d posFindA;
    cv::Point2d posFindB;

    posFindA = this->findFrontCeilingEdge(stripRoiBinaryImg, bandRect);
    posFindB = this->findBackCeilingEdge(stripRoiBinaryImg, bandRect);

    this->drawCircleForCeilingEdgeAndSaveLogImage(posFindA, posFindB, stripRoiBinaryImg, stripPoint, bandRect.count());

    if((posFindA.x < 0) || (posFindA.y < 0) || (posFindB.x < 0) || (posFindB.y < 0))
    {
        rotation = 0.0;
        return false;
    }

    this->calcRotationFromRadian(posFindA, posFindB, rotation);

    return true;
}

void BasicSupremeIAnalysis::calcRotationFromRadian(const Point2d& posFindA,const Point2d& posFindB, double& rotation)
{
    rotation = ((atan2(posFindB.y - posFindA.y, posFindB.x - posFindA.x) * DOUBLE_HALF_CIRCLE_DEGREE) / DOUBLE_PI_VALUE);//

    if (rotation != 0.0)
    {
        if((rotation < 0.01) && (rotation > -0.01)) //remove f
            rotation = 0.0;

        if((rotation > 0.5) || (rotation < -0.5))
            rotation = 0.0;
    }

    QString posLogStr;
    posLogStr.sprintf("Angle : %3.2f",
                      rotation);

    qDebug() << posLogStr;
}

void BasicSupremeIAnalysis::drawCircleForCeilingEdgeAndSaveLogImage(const cv::Point2d &posFindA, const cv::Point2d &posFindB, const cv::Mat& stripRoiBinaryImg
                                                                    , int stripPoint, int bandTotalCount)
{
    QString logStr;
    logStr.sprintf("posFindA.x = %03f, posFindA.y = %03f, posFindB.x = %03f, posFindB.y = %03f, Band Count : %d ",
                   posFindA.x, posFindA.y, posFindB.x, posFindB.y, bandTotalCount);
    qDebug() << logStr;

    cv::circle(stripRoiBinaryImg, posFindA, 5, cv::Scalar(0,255,255));
    cv::circle(stripRoiBinaryImg, posFindB, 5, cv::Scalar(0,255,255));
    this->imageSave(stripRoiBinaryImg, "Find-Angle", stripPoint);
}

void BasicSupremeIAnalysis::yLineParticleEraseProc(cv::Mat& srcImage, const int removalPaticle, const int averagePaticle, const int threshold )
{
    int remove		= removalPaticle;
    int avg		= averagePaticle;
    int nThreshold	= threshold;

    if((remove < 1) || (avg < 1))
        return;

    int nWidth		= srcImage.size().width;
    int nHeight		= srcImage.size().height;

    for(int x = 0; x < nWidth; x++)
    {
        QVector<uchar> pData(nHeight);

        for(int y=0 ; y < nHeight; y++)
            pData[y] = srcImage.at<uchar>(cv::Point(x, y));

        std::sort(pData.begin(), pData.end());

        auto start = remove;
        auto end = start+avg;
        auto bgStart = end;
        auto bgEnd = nHeight;
        auto bgAvg = abs(bgEnd-bgStart);

        uchar average = this->GetSectionSum(pData, start, end) / avg; //pData, nHeight - remove - avg, nHeight - remove
        uchar averageBG = this->GetSectionSum(pData, bgStart, bgEnd) / bgAvg; //pData, nHeight - remove, nHeight

        for(int y= 0 ; y < nHeight; y++)
        {
            auto currentValue = srcImage.at<uchar>(cv::Point(x, y));

            if(currentValue < abs(average - nThreshold))
                srcImage.at<uchar>(cv::Point(x, y)) = average;

            if(currentValue > (averageBG) && (remove != 0))
                srcImage.at<uchar>(cv::Point(x, y)) = averageBG;
        }
    }
}

cv::Point2d BasicSupremeIAnalysis::findFrontCeilingEdge(const cv::Mat& stripRoiBinaryImg, QVector<cv::Rect>& bandRect)
{
    cv::Point2d posFindA(-1,-1);
    const int pointXFront = bandRect.first().x - (bandRect.first().width * 0.4);
    const int pointXBack = bandRect.first().x - (bandRect.first().width * 0.3);
    int y1 = 0;
    int y2 = 0;

    int beginY = bandRect.first().y;

     for(int y = beginY; y > 0; y--)
    {
        bool b1 = false;
        bool b2 = false;

        unsigned char tmepPixel_1 = stripRoiBinaryImg.data[y * (stripRoiBinaryImg.step) + pointXFront];
        unsigned char tmepPixel_2 = stripRoiBinaryImg.data[(y + 1) * (stripRoiBinaryImg.step) + pointXFront];

        if(abs(tmepPixel_1 - tmepPixel_2) == 255)
        {
            y1 = y;
            b1 = true;
        }

        tmepPixel_1 = stripRoiBinaryImg.data[y * (stripRoiBinaryImg.step) + pointXBack];
        tmepPixel_2 = stripRoiBinaryImg.data[(y + 1) * (stripRoiBinaryImg.step) + pointXBack];

        if(abs(tmepPixel_1 - tmepPixel_2) == 255)
        {
            y2 = y;
            b2 = true;
        }

        if (b1 && b2)
            break;
    }

    if ((y1 <= 0 ) || (y2 <= 0))
        posFindA.y = 0;
    else
        posFindA.y = (y1 + y2)/2;

    posFindA.x = bandRect.first().x - (bandRect.first().width * 0.4);

    return posFindA;
}

cv::Point2d BasicSupremeIAnalysis::findBackCeilingEdge(const cv::Mat& stripRoiBinaryImg, QVector<cv::Rect>& bandRect)
{
    auto housingRoi =  ConfigInformation::getInstance()->getHousingROI();
    auto housingRoiRect = this->HousingROItoCVRect(housingRoi);
    cv::Point2d posFindB(-1, -1);

    const int pointXFront = bandRect.last().x + (bandRect.last().width*1);
    const int pointXBack = pointXFront + (bandRect.last().width *0.4);
    int y1 = 0;
    int y2 = 0;

    auto beginY = bandRect.last().y;

    for(int y = beginY; y > 0; y--)
    {
        bool b1 = false;
        bool b2 = false;

        if(y > housingRoiRect.height - 1)
            return posFindB;

        unsigned char tmepPixel_1 = stripRoiBinaryImg.data[y * (stripRoiBinaryImg.step) + pointXFront];
        unsigned char tmepPixel_2 = stripRoiBinaryImg.data[(y + 1) * (stripRoiBinaryImg.step) + pointXFront];

        if(abs(tmepPixel_1 - tmepPixel_2) == 255)
        {
            y1 = y;
            b1 = true;
        }

        tmepPixel_1 = stripRoiBinaryImg.data[y * (stripRoiBinaryImg.step) + pointXBack];
        tmepPixel_2 = stripRoiBinaryImg.data[(y + 1) * (stripRoiBinaryImg.step) + pointXBack];

        if(abs(tmepPixel_1 - tmepPixel_2) == 255)
        {
            y2 = y;
            b2 = true;
        }

        if (b1 && b2)
            break;
    }

    if ((y1 <= 0 ) || (y2 <= 0))
        posFindB.y = 0;
    else
        posFindB.y = (y1 + y2)/2;

    posFindB.x = bandRect.last().x + bandRect.last().width;

    return posFindB;
}

void BasicSupremeIAnalysis::onMovePosition()
{
    auto currentIdx = d->mAnalysisList[d->mAnalysisNextIdx].element.stripNumber;
    auto protocol =  WriteProtocolCameraLedSetting::writePostCameraPosMoveFinishedReq(currentIdx).toUtf8();
    emit onSendData(protocol);
}

void BasicSupremeIAnalysis::onSendNextStep()
{
   this->findStripNextIdx(d->mAnalysisList, d->mAnalysisNextIdx);

    if(d->mAnalysisNextIdx >= d->mAnalysisList.count())
    {
        emit onAnalysisFinished(d->mAnalysisList);
        return;
    }

    this->onMovePosition();
}

void BasicSupremeIAnalysis::doAnalysis()
{
    QTimer::singleShot(2500, this, [this]
                       {
                           d->mCameraInstance->clearRotation();
                           d->mCameraInstance->flushBuffer();

                           if(this->isvalidCurrentIndexStrip())
                               this->getCurrentIndexAnalysisResultData();

                           emit onCurrentProcessing(d->mAnalysisNextIdx);
                           this->onSendNextStep();
                       });
}

bool BasicSupremeIAnalysis::isvalidCurrentIndexStrip()
{
    auto currentErrorReason = d->mAnalysisList[d->mAnalysisNextIdx].result.errorReason;

    if(currentErrorReason.isEmpty())
        return true;

    auto errorToken = currentErrorReason.split(":");
    auto errorHeader = errorToken.first();

    if(errorHeader.contains("[ETC-S]"))
        return false;

    return true;
}

bool BasicSupremeIAnalysis::isValidBandData(const GlobalDataStruct::AnalysisResult& result)
{
    CSettingIo io;
    auto bandCutLine = io.getConfigureData("MINUS_BAND","MINUS_BAND_VALUE").toDouble();

    if(qFuzzyIsNull(bandCutLine))
        bandCutLine = -5.0;

    for(auto itr : result.rawIntensityValue)
    {
        if(itr <= bandCutLine)
            return false;
    }

    return true;
}

bool BasicSupremeIAnalysis::isValidPCData(const GlobalDataStruct::AnalysisResult& result)
{
    for(auto itr : result.pcPoint)
    {
        if(result.rawIntensityValue.count() <= itr)
            return false;

        if(ClassHelper::isValidPCLine(result.rawIntensityValue[itr]) == false)
            return false;
    }

    return true;
}

void BasicSupremeIAnalysis::getCurrentIndexAnalysisResultData()
{
    GlobalDataStruct::AnalysisResult result;

    if(d->mAnalysisNextIdx < d->mAnalysisList.count())
    {
        auto currentStripNumber = d->mAnalysisList[d->mAnalysisNextIdx].element.stripNumber;
        auto currentContentsName =  d->mConfigInstance->getCurrentContents();
        cv::Mat lastMatrix;

        for(int retryCount = 0; retryCount < 3 ; retryCount++)
        {
            d->mCameraInstance->clearRotation();
            const auto image = d->mCameraInstance->getFullConfigureImage();
            lastMatrix = image;

            if(d->mCameraInstance->isOpen() == false || image.data == nullptr)
                break;

            result = this->analysisSingleStrip(image, currentContentsName, false, false, currentStripNumber);

            if(result.isSuccess)
                break;

            else
                d->mThread->msleep(1000);
        }

        this->analysisRawImageSave(lastMatrix, currentStripNumber);
        d->mAnalysisList[d->mAnalysisNextIdx].result = result;
    }
}

bool BasicSupremeIAnalysis::sortData(const GlobalDataStruct::TABLE_ITEM_ELEMENT& a, const GlobalDataStruct::TABLE_ITEM_ELEMENT& b)
{
    return a.stripNumber < b.stripNumber;
}

void BasicSupremeIAnalysis::onRecvAnalysisData(QByteArray recvData)
{
    if(PacketReader::modeReader(recvData) != MD_MODE::MD_MODE_CONTROL_WRITE)
        return;

    switch(PacketReader::commandReader(recvData))
    {

    case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_RESP :

         if(d->mAnalysisNextIdx == 0)
             d->mThread->sleep(25);
         else
             d->mThread->sleep(1);

         this->doAnalysis();
        break;

    }
}

void BasicSupremeIAnalysis::onStartAnalysisThread()
{
    d->mThread = new QThread(this);
    this->moveToThread(d->mThread);
    d->mThread->start();

    QTimer::singleShot(1000, this, [this]
            {
                this->calcStripCount(d->mAnalysisList, d->mAnalysisTotalCount);
                this->onSendNextStep();
            });
}

cv::Rect BasicSupremeIAnalysis::calcRect(int pcPosAx, int pcPosAy, int boxcarWidget, int boxcarHeight)
{
    cv::Rect rect;
    rect.x = pcPosAx - (boxcarWidget / 2);
    rect.y = pcPosAy - (boxcarHeight / 2);
    rect.width = boxcarWidget;
    rect.height = boxcarHeight;

    if(rect.x < 0)
        rect.x = 0;

    if(rect.y < 0)
        rect.y = 0;

    if(rect.width < 0)
        rect.width = 0;

    if(rect.height < 0)
        rect.height = 0;

    return rect;
}

QVector<cv::Rect> BasicSupremeIAnalysis::calcBandBlock(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Point& pcPosAFront, const cv::Point& pcPosABack,
                                                       double bandCount)
{
    QVector<cv::Rect> rectBand;

    auto housingRoi = this->HousingROItoCVRect(recipe.housingROI);
    double pcGap = abs(pcPosABack.x - pcPosAFront.x);
    double bandAvg = (pcGap / (bandCount - 1.0));

    auto rectFront = this->calcRect(pcPosAFront.x, pcPosAFront.y, recipe.boxCarAnalaysis.width, recipe.boxCarAnalaysis.height);
    rectBand.push_front(rectFront);

    for(double i = 1 ; i < bandCount-1; i++)
    {
        auto bandData = rectFront;
        bandData.x += ( (bandAvg * (i)));

        if(housingRoi.width < bandData.x)
            bandData.x = rectFront.x;

        rectBand.push_back(bandData);
    }

    auto rectBack = this->calcRect(pcPosABack.x, pcPosABack.y, recipe.boxCarAnalaysis.width, recipe.boxCarAnalaysis.height);
    rectBand.push_back(rectBack);

    return rectBand;
}

void BasicSupremeIAnalysis::updateFailDyanmicBand(GlobalDataStruct::AnalysisResult& result)
{
    double orgFront = 0;
    double dynamicFront = 0;
    double successCountFront = 0;

    double orgBack = 0;
    double dynamicBack = 0;
    double successCountBack = 0;

    for(int i =1; i< (result.rectBandResult.count() /2); i++)
    {
        if(result.rectBandResult[i].isFail == false)
        {
            orgFront += result.rectBandResult[i].rectBand.x;
            dynamicFront += result.rectBandResult[i].rectDynamicBand.x;
            successCountFront++;
        }
    }

    for(int i = (result.rectBandResult.count() /2)+1 ; i< result.rectBandResult.count() - 1; i++)
    {
        if(result.rectBandResult[i].isFail == false)
        {
            orgBack += result.rectBandResult[i].rectBand.x;
            dynamicBack += result.rectBandResult[i].rectDynamicBand.x;
            successCountBack++;
        }
    }

    double frontAvg = (dynamicFront - orgFront) / successCountFront;
    double backAvg =  (dynamicBack - orgBack) / successCountBack;
    frontAvg = qRound(frontAvg);
    backAvg = qRound(backAvg);

    for(int i = 1 ; i< (result.rectBandResult.count() /2) ; i++)
    {
        if(result.rectBandResult[i].isFail == true)
            result.rectBandResult[i].rectDynamicBand.x += frontAvg;
    }

    for(int i = (result.rectBandResult.count() /2) +1 ; i< result.rectBandResult.count() - 1 ; i++)
    {
        if(result.rectBandResult[i].isFail == true)
            result.rectBandResult[i].rectDynamicBand.x += backAvg;
    }

}

void BasicSupremeIAnalysis::drawRectForLogging(const GlobalDataStruct::AnalysisResult& result, const cv::Rect& roi, int stripIdx)
{
    auto camera = CameraInstance::getInstance();
    auto image = camera->getFullConfigureImage();

    if(image.data == nullptr)
        return ;

    cv::Mat imgRoi;
    cv::Mat stripImgRoiGray;
    imgRoi = image(roi);
    cv::cvtColor(imgRoi, stripImgRoiGray, cv::COLOR_BGR2GRAY);

    for(auto&itr : result.rectBandResult)
        cv::rectangle(stripImgRoiGray, itr.rectDynamicBand, cv::Scalar(39, 237, 250));

    this->imageSave(stripImgRoiGray, "RectPoistion", stripIdx);
}
