#ifndef TIEGBANDDATATUNNINGDATA_H
#define TIEGBANDDATATUNNINGDATA_H

#include <QObject>

#include "Analysis/GlobalDataStruct.h"

class TIeGBandDataTunningData : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit TIeGBandDataTunningData(QObject* parent = nullptr);
    ~TIeGBandDataTunningData();

    QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> readAllSettingData();
    int readSettingMaxData(int index);
    int readSettingMinData(int index);
    double readSettingBandData(int index);

    void writeSettingMaxData(int index,int data);
    void writeSettingMinData(int index,int data);
    void writeSettingBandData(int index, double data);
    void writeAllSetting(const QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA>& data);

private:
    void initIo();
    int readSettingTotalCount();
    void writeSettingTotalCount(int count);
    static bool sortSettingData(GlobalDataStruct::TIGE_BAND_SETTING_DATA& a, GlobalDataStruct::TIGE_BAND_SETTING_DATA& b);
private:
    QScopedPointer<privateStruct> d;

};



#endif // TIEGBANDDATATUNNINGDATA_H
