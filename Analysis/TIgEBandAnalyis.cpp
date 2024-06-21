#include "TIgEBandAnalyis.h"

#include "Common/DataBase/CDataBase.h"
#include "Common/Io/INIFileControls/TIeGBandDataTunningData.h"

#include "DataBase/DataBaseCtrl.h"
#include "Analysis/ClassHelper.h"
#include "Analysis/BasicSupremeIAnalysis.h"
#include "Analysis/ConfigInformation.h"

TIgEBandAnalyis::TIgEBandAnalyis(QObject *parent):
    QObject(parent)
{

}

TIgEBandAnalyis::~TIgEBandAnalyis()
{

}

void TIgEBandAnalyis::calcTIgEBandData(int currentTigeidx, GlobalDataStruct::AnalysisReportInfomation &element)
{

    if(currentTigeidx < 0)
        return;

    TIeGBandDataTunningData io;
    QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> settingData;
    settingData = io.readAllSettingData();
    int classSum = TIgEBandAnalyis::bandClassSum(element);

    for(GlobalDataStruct::TIGE_BAND_SETTING_DATA& itr : settingData)
    {
        if((itr.BAND_MIN<= classSum)&&(itr.BAND_MAX >= classSum))
        {
            auto currentIntensity = element.result.intensityValue[currentTigeidx];
            auto calculateTIGE = currentIntensity * itr.TUNNING_DATA;
            auto isValidTIGE = ClassHelper::isValidTIGE(calculateTIGE);

            if(isValidTIGE)
            {
                element.result.classType[currentTigeidx] = "P";
                element.result.resultValue[currentTigeidx] = ">100";
            }
            else
            {
                element.result.classType[currentTigeidx] = "N";
                element.result.resultValue[currentTigeidx] = "≤100";
            }
            break;
        }
        else if((itr.BAND_MIN != 0)&&(itr.BAND_MAX == 0))
        {
            auto currentIntensity = element.result.intensityValue[currentTigeidx];
            auto calculateTIGE = currentIntensity * itr.TUNNING_DATA;
            auto isValidTIGE = ClassHelper::isValidTIGE(calculateTIGE);

            if(isValidTIGE)
            {
                element.result.classType[currentTigeidx] = "P";
                element.result.resultValue[currentTigeidx] = ">100";
            }
            else
            {
                element.result.classType[currentTigeidx] = "N";
                element.result.resultValue[currentTigeidx] = "≤100";
            }
            break;
        }
    }

}

void TIgEBandAnalyis::calcTIgEBandData(GlobalDataStruct::AnalysisReportInfomation &element)
{

    DataBaseCtrl* fucDatabase = new DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::DBCodeFunc);

    if(fucDatabase == nullptr)
        return;

    auto funcMap = DataBaseCtrl::getFuncData(fucDatabase);
    auto stripRelMap = DataBaseCtrl::getStripRelData(fucDatabase);
    auto config = ConfigInformation::getInstance();
    int currentTigeIdx = -1;

    element.result.resultValue.clear();
    auto panelName = element.element.panelName;
    auto codeList = config->getCurrentStripCodeList(panelName);
    QString currentLot = element.lotNumber;
    auto funcRelList = stripRelMap[currentLot];

    for(int i = 0; i < element.result.intensityValue.count(); i++)
    {
         if(funcRelList.count() <= i)
            break;

         QString code;
         auto funcRel = funcRelList[i];

         if(i < codeList.count())
            code = codeList[i];

         if(code.compare("tIgE",Qt::CaseInsensitive)==0)
             currentTigeIdx = i;

         auto intensity = element.result.intensityValue[i];

         auto calcValue = BasicSupremeIAnalysis::calcContentsValue(intensity, funcMap[funcRel]);

         BasicSupremeIAnalysis::appendClassTypeAndResultValue(element, intensity, calcValue, code, funcRel);
    }

    TIgEBandAnalyis::calcTIgEBandData(currentTigeIdx,element);

    delete fucDatabase;
}

int TIgEBandAnalyis::bandClassSum(const GlobalDataStruct::AnalysisReportInfomation &itr)
{
    int sumResult =0 ;

    for(auto i : itr.result.classType)
    {
        auto currentitr = i.toInt();
        if(0 > currentitr)
            continue;
        sumResult+=currentitr;
    }

    return sumResult;
}
