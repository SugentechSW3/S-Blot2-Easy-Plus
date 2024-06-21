#include "TIeGBandDataTunningData.h"

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"

USING_SUGENTECH

struct TIeGBandDataTunningData::privateStruct
{
    QScopedPointer<CSettingIo> mTIgESettingIo;
    const QString mDirPath ="GENERIC_DATA";
    const QString mTIgEBandSettingFileName = "TIGE_BAND_SETTING_DATA";
};

TIeGBandDataTunningData::TIeGBandDataTunningData(QObject *parent) :
    QObject(parent),d(new privateStruct)
{
    this->initIo();
}

TIeGBandDataTunningData::~TIeGBandDataTunningData()
{

}

void TIeGBandDataTunningData::initIo()
{
    d->mTIgESettingIo.reset(new CSettingIo(d->mTIgEBandSettingFileName,d->mDirPath));
}

QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> TIeGBandDataTunningData::readAllSettingData()
{
    QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> result;

    int totalCount = this->readSettingTotalCount();

    if (totalCount <=0)
        return result;

    for(int i =0; i< totalCount;i++)
    {
       GlobalDataStruct::TIGE_BAND_SETTING_DATA data;
       data.BAND_MAX = this->readSettingMaxData(i);
       data.BAND_MIN = this->readSettingMinData(i);
       data.TUNNING_DATA = this->readSettingBandData(i);

       result.push_back(data);
    }
    std::sort(result.begin(),result.end(),TIeGBandDataTunningData::sortSettingData);
    return result;
}

int TIeGBandDataTunningData::readSettingMaxData(int index)
{
    QString key = QString("%1_MAX_DATA").arg(QString::number(index));
    return d->mTIgESettingIo->getConfigureData("TOAL_SETTING_MAX",key).toInt();
}

int TIeGBandDataTunningData::readSettingMinData(int index)
{
    QString key = QString("%1_MIN_DATA").arg(QString::number(index));
    return d->mTIgESettingIo->getConfigureData("TOAL_SETTING_MIN",key).toInt();
}

double TIeGBandDataTunningData::readSettingBandData(int index)
{
    QString key = QString("%1_TIGE_DATA").arg(QString::number(index));
    return d->mTIgESettingIo->getConfigureData("TOAL_SETTING_DATA",key).toDouble();
}

void TIeGBandDataTunningData::writeSettingMaxData(int index, int data)
{
    QString key = QString("%1_MAX_DATA").arg(QString::number(index));
    d->mTIgESettingIo->setConfigureData("TOAL_SETTING_MAX", key, data);
}

void TIeGBandDataTunningData::writeSettingMinData(int index, int data)
{
    QString key = QString("%1_MIN_DATA").arg(QString::number(index));
    d->mTIgESettingIo->setConfigureData("TOAL_SETTING_MIN", key, data);
}

void TIeGBandDataTunningData::writeSettingBandData(int index, double data)
{
    QString key = QString("%1_TIGE_DATA").arg(QString::number(index));
    d->mTIgESettingIo->setConfigureData("TOAL_SETTING_DATA", key, data);
}

void TIeGBandDataTunningData::writeAllSetting(const QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> &data)
{
    this->writeSettingTotalCount(data.count());

    for(int i =0; i < data.count(); i++)
    {
        this->writeSettingMaxData(i,data[i].BAND_MAX);
        this->writeSettingMinData(i,data[i].BAND_MIN);
        this->writeSettingBandData(i,data[i].TUNNING_DATA);
    }
}

int TIeGBandDataTunningData::readSettingTotalCount()
{
    return d->mTIgESettingIo->getConfigureData("TOAL_SETTING_COUNT", "SETTING_COUNT").toInt();
}

void TIeGBandDataTunningData::writeSettingTotalCount(int count)
{
    d->mTIgESettingIo->setConfigureData("TOAL_SETTING_COUNT", "SETTING_COUNT", count);
}

bool TIeGBandDataTunningData::sortSettingData(GlobalDataStruct::TIGE_BAND_SETTING_DATA &a, GlobalDataStruct::TIGE_BAND_SETTING_DATA &b)
{
    return a.BAND_MIN < b.BAND_MIN;
}
