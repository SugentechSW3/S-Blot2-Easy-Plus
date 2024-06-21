#include "CameraIniDataControls.h"

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"

USING_SUGENTECH

struct CameraIniDataControls::privateStruct
{
    QScopedPointer<CSettingIo> mCameraContentsSettingIo;

    QString mContentsName;
    QString mContetnsCameraSettingFileName = "CONTENTS_CAMERA_SETTINGS";
};

CameraIniDataControls::CameraIniDataControls(const QString &contentsName, QObject *parent) :
                                                                QObject(parent),
                                                                d(new privateStruct)
{
    d->mContentsName = contentsName;
    this->initIo();
}

CameraIniDataControls::~CameraIniDataControls()
{

}

void CameraIniDataControls::initIo()
{
     d->mCameraContentsSettingIo.reset(new CSettingIo(d->mContetnsCameraSettingFileName, d->mContentsName));
}

GlobalDataStruct::HOUSING_ROI CameraIniDataControls::readHousingRoi()
{
    GlobalDataStruct::HOUSING_ROI roi;

    roi.x1 = d->mCameraContentsSettingIo->getConfigureData("HOUSING_ROI","X1").toInt();
    roi.x2 = d->mCameraContentsSettingIo->getConfigureData("HOUSING_ROI","X2").toInt();
    roi.y1 = d->mCameraContentsSettingIo->getConfigureData("HOUSING_ROI","Y1").toInt();
    roi.y2 = d->mCameraContentsSettingIo->getConfigureData("HOUSING_ROI","Y2").toInt();

    return roi;
}

void CameraIniDataControls::writeHousingRoi(const GlobalDataStruct::HOUSING_ROI& roi)
{
    d->mCameraContentsSettingIo->setConfigureData("HOUSING_ROI","X1",roi.x1);
    d->mCameraContentsSettingIo->setConfigureData("HOUSING_ROI","X2",roi.x2);
    d->mCameraContentsSettingIo->setConfigureData("HOUSING_ROI","Y1",roi.y1);
    d->mCameraContentsSettingIo->setConfigureData("HOUSING_ROI","Y2",roi.y2);
}

GlobalDataStruct::INTENSITY CameraIniDataControls::readIntensityOption()
{
    GlobalDataStruct::INTENSITY intensityOption;

    intensityOption.paticle = d->mCameraContentsSettingIo->getConfigureData("INTENSITY", "PATICLE").toInt();
    intensityOption.band = d->mCameraContentsSettingIo->getConfigureData("INTENSITY", "BAND").toInt();
    intensityOption.background = d->mCameraContentsSettingIo->getConfigureData("INTENSITY", "BACKGROUND").toInt();
    intensityOption.white = d->mCameraContentsSettingIo->getConfigureData("INTENSITY", "WHITE").toInt();

    return intensityOption;
}

void CameraIniDataControls::writeIntensityOption(const GlobalDataStruct::INTENSITY& intensity)
{
    d->mCameraContentsSettingIo->setConfigureData("INTENSITY", "PATICLE", intensity.paticle);
    d->mCameraContentsSettingIo->setConfigureData("INTENSITY", "BAND", intensity.band);
    d->mCameraContentsSettingIo->setConfigureData("INTENSITY", "BACKGROUND", intensity.background);
    d->mCameraContentsSettingIo->setConfigureData("INTENSITY", "WHITE", intensity.white);
}

GlobalDataStruct::PATICLE_REMOVAL CameraIniDataControls::readPaticleRemoval()
{
    GlobalDataStruct::PATICLE_REMOVAL removal;

    removal.removalPaticle = d->mCameraContentsSettingIo->getConfigureData("PATICLE_REMOVAL", "REMOVAL_PATICLE_COUNT").toInt();
    removal.averagePaticle = d->mCameraContentsSettingIo->getConfigureData("PATICLE_REMOVAL", "AVERAGE_PATICLE").toInt();
    removal.threshold = d->mCameraContentsSettingIo->getConfigureData("PATICLE_REMOVAL", "THRESHOLD").toInt();

    return removal;
}

void CameraIniDataControls::writePaticleRemoval(const GlobalDataStruct::PATICLE_REMOVAL& removal)
{
    d->mCameraContentsSettingIo->setConfigureData("PATICLE_REMOVAL", "REMOVAL_PATICLE_COUNT", removal.removalPaticle);
    d->mCameraContentsSettingIo->setConfigureData("PATICLE_REMOVAL", "AVERAGE_PATICLE", removal.averagePaticle);
    d->mCameraContentsSettingIo->setConfigureData("PATICLE_REMOVAL", "THRESHOLD", removal.threshold);
}

GlobalDataStruct::USE_BOXCAR_ANALYSIS CameraIniDataControls::readBoxCarAnalysis()
{
    GlobalDataStruct::USE_BOXCAR_ANALYSIS boxCar;

    boxCar.width = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "WIDTH").toInt();
    boxCar.height = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "HEIGHT").toInt();
    boxCar.xRange = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "X_RANGE").toInt();
    boxCar.xStart = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "X_START").toInt();
    boxCar.pcThres = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "PC_THRES").toInt();
    boxCar.housingThres = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "HOUSING_THRES").toInt();
    boxCar.boxCarbandCount = d->mCameraContentsSettingIo->getConfigureData("BOXCAR_ANALYSIS_SETTING", "BAND_COUNT").toInt();

    return boxCar;
}

void CameraIniDataControls::writeBoxCarAnalysis(const GlobalDataStruct::USE_BOXCAR_ANALYSIS& boxcarSetting)
{
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "WIDTH", boxcarSetting.width);
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "HEIGHT", boxcarSetting.height);
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "X_RANGE", boxcarSetting.xRange);
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "X_START", boxcarSetting.xStart);
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "PC_THRES", boxcarSetting.pcThres);
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "HOUSING_THRES", boxcarSetting.housingThres);
    d->mCameraContentsSettingIo->setConfigureData("BOXCAR_ANALYSIS_SETTING", "BAND_COUNT", boxcarSetting.boxCarbandCount);
}

QVector<int> CameraIniDataControls::readBoxCarBandGap()
{
    auto boxCarAnalysis = this->readBoxCarAnalysis();
    QVector<int> bandGapData;

    for(auto i = 0 ; i< boxCarAnalysis.boxCarbandCount; i++)
        bandGapData << d->mCameraContentsSettingIo->getConfigureData("BOXCAR_BAND_GAP", QString::number(i)).toInt();

    return bandGapData;
}

void CameraIniDataControls::writeBoxCarBandGap(const QVector<int>& bandGap)
{
    for(auto i = 0; i < bandGap.count(); i++)
        d->mCameraContentsSettingIo->setConfigureData("BOXCAR_BAND_GAP", QString::number(i), bandGap[i]);
}

GlobalDataStruct::USE_MULTIPLE_BAND CameraIniDataControls::readMultiBandData()
{
    GlobalDataStruct::USE_MULTIPLE_BAND multipleBand;

    multipleBand.pcXGap = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_PC_X_GAP").toInt();
    multipleBand.bandBlock2XGap = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_2X_GAP").toInt();
    multipleBand.bandBlock3XGap = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_3X_GAP").toInt();
    multipleBand.bandBlock4XGap = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_4X_GAP").toInt();

    multipleBand.bandBlock2XStart = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_2X_START").toInt();
    multipleBand.bandBlock3XStart = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_3X_START").toInt();
    multipleBand.bandBlock4XStart = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_4X_START").toInt();

    multipleBand.mutiplePcThres = d->mCameraContentsSettingIo->getConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_PC_THRES").toInt();

    return multipleBand;
}

void CameraIniDataControls::writeMultiBandData(const GlobalDataStruct::USE_MULTIPLE_BAND& multiBandData)
{
    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_PC_X_GAP", multiBandData.pcXGap);
    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_2X_GAP", multiBandData.bandBlock2XGap);
    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_3X_GAP", multiBandData.bandBlock3XGap);
    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_4X_GAP", multiBandData.bandBlock4XGap);

    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_2X_START", multiBandData.bandBlock2XStart);
    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_3X_START", multiBandData.bandBlock3XStart);
    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_4X_START", multiBandData.bandBlock4XStart);

    d->mCameraContentsSettingIo->setConfigureData("MULTIPLE_BAND_SETTING", "BAND_BLOCK_PC_THRES", multiBandData.mutiplePcThres);
}

bool CameraIniDataControls::readUseMultiBand()
{
    return d->mCameraContentsSettingIo->getConfigureData("STRIP_USE_MULTI_BAND", "USE_MULTI_BAND").toBool();
}

void CameraIniDataControls::writeUseMultiBand(bool isUseMultiBand)
{
    d->mCameraContentsSettingIo->setConfigureData("STRIP_USE_MULTI_BAND", "USE_MULTI_BAND", isUseMultiBand);
}
