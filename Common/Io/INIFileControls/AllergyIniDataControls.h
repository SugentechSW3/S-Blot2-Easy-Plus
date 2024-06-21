#ifndef ALLERGYINIDATACONTROLS_H
#define ALLERGYINIDATACONTROLS_H

#include <QObject>
#include"Analysis/GlobalDataStruct.h"

class AllergyIniDataControls : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit AllergyIniDataControls(QObject *parent = nullptr);
    ~AllergyIniDataControls();

    void upLoadData();
    void downLoadData();

    QString getCurrentDirPath();


    QStringList readHiddenBandCodeData(const QString& contentName);
    void writeHiddenBandCodeData(const QString& contentName , const QStringList& bandCodeList);

    QStringList readBandCodeData(const QString& panelName);
    void writeBandCodeData(const QString& panelName, const QStringList& bandCodeList);

    int readSampleCount();
    void writeSampleCount(int sampleCount);

    int readStripCount();
    void writeStripCount(int stripCount);

    GlobalDataStruct::HOUSING_ROI readHousingROI();
    void writeHousingROI(const GlobalDataStruct::HOUSING_ROI& roi);

    GlobalDataStruct::INTENSITY readIntensityOption();
    void writeIntensityOption(const GlobalDataStruct::INTENSITY& intensity);

    GlobalDataStruct::PATICLE_REMOVAL readPaticleRemoval();
    void writeParticleRemoval(const GlobalDataStruct::PATICLE_REMOVAL& paticle);

    GlobalDataStruct::USE_BOXCAR_ANALYSIS readBoxCarAnalysis();
    void writeBoxCarAnalysis(const GlobalDataStruct::USE_BOXCAR_ANALYSIS& boxcarSetting);

    QVector<int> readBoxCarBandGap();
    void writeBoxCarBandGap(const QVector<int>& bandGap);

    GlobalDataStruct::USE_MULTIPLE_BAND readMultiBandData();
    void writeMultiBandData(const GlobalDataStruct::USE_MULTIPLE_BAND& multiBandData);

    bool readUseMultiBand();
    void writeUseMultiBand(bool isUseMultiBand);

private:
    void initIo();

private:
    QScopedPointer<privateStruct> d;
};


#endif // ALLERGYINIDATACONTROLS_H
