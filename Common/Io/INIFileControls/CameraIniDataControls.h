#ifndef CAMERAINIDATACONTROLS_H
#define CAMERAINIDATACONTROLS_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"

class CameraIniDataControls : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CameraIniDataControls(const QString& contentsName, QObject *parent = nullptr);
    ~CameraIniDataControls();

    QString getCurrentDirPath();

    void upLoadData(const QString& contents);

    GlobalDataStruct::HOUSING_ROI readHousingRoi();
    void writeHousingRoi(const GlobalDataStruct::HOUSING_ROI& roi);

    GlobalDataStruct::INTENSITY readIntensityOption();
    void writeIntensityOption(const GlobalDataStruct::INTENSITY& intensity);

    GlobalDataStruct::PATICLE_REMOVAL readPaticleRemoval();
    void writePaticleRemoval(const GlobalDataStruct::PATICLE_REMOVAL& removal);

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

#endif // CAMERAINIDATACONTROLS_H
