#include "AllergyIniDataControls.h"

#include "Analysis/ConfigInformation.h"

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"

#include "CameraIniDataControls.h"

USING_SUGENTECH

struct AllergyIniDataControls::privateStruct
{
    QScopedPointer<CSettingIo> mGenericIO;
    QScopedPointer<CSettingIo> mBandIO;
    QScopedPointer<CSettingIo> mBeHiddenBandIO;

    const QString mBandIniFileName = "ALLERGY_BAND_DATA";
    const QString mAlllergyIniFileName = "ALLERGY_GENERIC_DATA";
    const QString mBeHiddenBandIniFileName = "LIST_BE_HIDDEN_BAND";
    QString mDirPath;

};

AllergyIniDataControls::AllergyIniDataControls(QObject *parent) :
                                                                  QObject(parent),
                                                                  d(new privateStruct)
{
    this->initIo();
}

AllergyIniDataControls::~AllergyIniDataControls()
{

}

void AllergyIniDataControls::initIo()
{
    auto categoryName = ConfigInformation::getContentsNameFromEnum((int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY);
    d->mDirPath = categoryName;

    d->mGenericIO.reset(new CSettingIo(d->mAlllergyIniFileName, d->mDirPath));
    d->mBandIO.reset(new CSettingIo(d->mBandIniFileName, d->mDirPath));
    d->mBeHiddenBandIO.reset(new CSettingIo(d->mBeHiddenBandIniFileName, d->mDirPath));
}

void AllergyIniDataControls::upLoadData()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
}

void AllergyIniDataControls::downLoadData()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
}

QString AllergyIniDataControls::getCurrentDirPath()
{
    return d->mDirPath;
}

QStringList AllergyIniDataControls::readHiddenBandCodeData(const QString &contentName)
{
    return d->mBeHiddenBandIO->getConfigureData("HIDDEN_BAND", contentName).toStringList();
}

void AllergyIniDataControls::writeHiddenBandCodeData(const QString &contentName, const QStringList &bandCodeList)
{
    d->mBeHiddenBandIO->setConfigureData("HIDDEN_BAND", contentName, bandCodeList);
}

QStringList AllergyIniDataControls::readBandCodeData(const QString& panelName)
{
    return d->mBandIO->getConfigureData(panelName, "BAND_CODE").toStringList();
}

void AllergyIniDataControls::writeBandCodeData(const QString& panelName, const QStringList& bandCodeList)
{
    d->mBandIO->setConfigureData(panelName, "BAND_CODE", bandCodeList);
}

int AllergyIniDataControls::readSampleCount()
{
    return d->mGenericIO->getConfigureData("SAMPLE_COUNT", "COUNT").toInt();
}

void AllergyIniDataControls::writeSampleCount(int sampleCount)
{
    d->mGenericIO->setConfigureData("SAMPLE_COUNT", "COUNT", sampleCount);
}

int AllergyIniDataControls::readStripCount()
{
    return d->mGenericIO->getConfigureData("STRIP_COUNT", "COUNT").toInt();
}

void AllergyIniDataControls::writeStripCount(int stripCount)
{
    d->mGenericIO->setConfigureData("STRIP_COUNT", "COUNT", stripCount);
}

GlobalDataStruct::HOUSING_ROI AllergyIniDataControls::readHousingROI()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readHousingRoi();
}

void AllergyIniDataControls::writeHousingROI(const GlobalDataStruct::HOUSING_ROI& roi)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writeHousingRoi(roi);
}

GlobalDataStruct::INTENSITY AllergyIniDataControls::readIntensityOption()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readIntensityOption();
}

void AllergyIniDataControls::writeIntensityOption(const GlobalDataStruct::INTENSITY& intensity)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writeIntensityOption(intensity);
}

GlobalDataStruct::PATICLE_REMOVAL AllergyIniDataControls::readPaticleRemoval()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readPaticleRemoval();
}

void AllergyIniDataControls::writeParticleRemoval(const GlobalDataStruct::PATICLE_REMOVAL& paticle)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writePaticleRemoval(paticle);
}

GlobalDataStruct::USE_BOXCAR_ANALYSIS AllergyIniDataControls::readBoxCarAnalysis()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readBoxCarAnalysis();
}

void AllergyIniDataControls::writeBoxCarAnalysis(const GlobalDataStruct::USE_BOXCAR_ANALYSIS& boxcarSetting)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writeBoxCarAnalysis(boxcarSetting);
}

QVector<int> AllergyIniDataControls::readBoxCarBandGap()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readBoxCarBandGap();
}

void AllergyIniDataControls::writeBoxCarBandGap(const QVector<int>& bandGap)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writeBoxCarBandGap(bandGap);
}

GlobalDataStruct::USE_MULTIPLE_BAND AllergyIniDataControls::readMultiBandData()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readMultiBandData();
}

void AllergyIniDataControls::writeMultiBandData(const GlobalDataStruct::USE_MULTIPLE_BAND& multiBandData)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writeMultiBandData(multiBandData);
}

bool AllergyIniDataControls::readUseMultiBand()
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    return cameraDataControls.readUseMultiBand();
}

void AllergyIniDataControls::writeUseMultiBand(bool isUseMultiBand)
{
    CameraIniDataControls cameraDataControls(d->mDirPath);
    cameraDataControls.writeUseMultiBand(isUseMultiBand);
}
