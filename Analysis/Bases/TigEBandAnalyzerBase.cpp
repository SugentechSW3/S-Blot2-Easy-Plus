#include "TigEBandAnalyzerBase.h"

#include <Common/Io/INIFileControls/GenericIniDataControls.h>

#include <Analysis/ClassHelper.h>
#include <Analysis/ConfigInformation.h>

#include <DataBase/CodeFuncDatabaseCtrl.h>

TigEBandAnalyzerBase::TigEBandAnalyzerBase(QObject *parent)
    :ITigEBandAnalyzer(parent)
{
}

bool TigEBandAnalyzerBase::analyzeTigEBand(QVector<GlobalDataStruct::AnalysisReportInfomation> &element, QMap<QString, GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT> lotDataMap)
{
    auto config = ConfigInformation::getInstance();    

    #pragma omp parallel for ordered
        for(auto idx = 0 ; idx < element.count(); idx++)
        {
    #pragma omp ordered
            {
                auto& itr = element[idx];
                auto currentTigeIdx = -1;

                auto panelName = itr.element.panelName;
                auto codeList = config->getCurrentStripCodeList(panelName);
                //auto cutoff = lotDataMap[itr.lotNumber].cutoff;
                auto cutoff = ClassHelper::getVaildTigeCutOff(lotDataMap[itr.lotNumber].cutoff);
                QString code;

                for(int i = 0; i < itr.result.rawIntensityValue.count(); i++)
                {

                    if(i < codeList.count())
                        code = codeList[i];

                    if(code.compare("tIgE", Qt::CaseInsensitive) == 0)
                        currentTigeIdx = i;
                }

                auto combinedData = this->findForSameIDData(itr, element);                
                this->tIgEMagnificationCalc(currentTigeIdx, itr, combinedData, cutoff);
            }
        }

    return true;
}

GlobalDataStruct::AnalysisReportInfomation TigEBandAnalyzerBase::findForSameIDData(const GlobalDataStruct::AnalysisReportInfomation &target,
                                                                                   const QVector<GlobalDataStruct::AnalysisReportInfomation> &element)
{
    auto targetPanel = target.element.panelName;
    auto targetID = target.element.patientID;
    auto targetDate = target.dateTime;

    for(auto& itr : element)
    {
        auto isDiffPanel = (targetPanel != itr.element.panelName);
        auto isSameID = (targetID == itr.element.patientID);
        auto isSameDateTime = (targetDate == itr.dateTime);

        if(isDiffPanel && isSameID && isSameDateTime)
            return itr;
    }


    return GlobalDataStruct::AnalysisReportInfomation();
}

void TigEBandAnalyzerBase::tIgEMagnificationCalc(int currentTigeIdx, GlobalDataStruct::AnalysisReportInfomation &itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData,
                                                 int cutoff)
{
    if(currentTigeIdx < 0)
        return;

    if(itr.result.classType.count() == 0)
        return;

    auto tIgEType = itr.result.classType[currentTigeIdx];
    auto istIgETypeN = (tIgEType == "N");
    auto istIgETypeP = (tIgEType == "P");
    auto isHasNorP = (istIgETypeN || istIgETypeP);

    if(!isHasNorP)
        return;

    if(istIgETypeP)
        this->tIgEAllZeroCondition(currentTigeIdx, itr, combinedData, cutoff);

    else if(istIgETypeN)
        this->sIgECalc(currentTigeIdx, itr, combinedData, cutoff);
}

bool TigEBandAnalyzerBase::tIgEAllZeroCondition(int currentTigeIdx, GlobalDataStruct::AnalysisReportInfomation &itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData,
                                                int cutoff)
{
    GenericIniDataControls io;
    auto tIgEMagnification = io.readtIgEZeroConditionSetting();
    auto isHasNonZeroClass = this->isHasNonZeroClass(itr, combinedData);

    if(currentTigeIdx < 0)
        return false;

    QString resultOrgText;
    auto currentIntensity = itr.result.rawIntensityValue[currentTigeIdx];
    auto calculatedTIGE = currentIntensity * tIgEMagnification;
    auto isValidTIGE = ClassHelper::isValidTIGE(calculatedTIGE, cutoff);

    if(isHasNonZeroClass)
    {
        itr.result.classType[currentTigeIdx] = "P";
        resultOrgText = ">100";
        itr.result.tigeConcentrationValue = getTigeConcentrationValue(currentIntensity, cutoff, isValidTIGE);
        itr.result.resultValue[currentTigeIdx] = getTigeResultText(resultOrgText, itr.result.tigeConcentrationValue);
        return false;
    }


    if(isValidTIGE)
    {
        itr.result.classType[currentTigeIdx] = "P";
        resultOrgText = ">100";
    }
    else
    {
        itr.result.classType[currentTigeIdx] = "N";
        resultOrgText = "≤100";
    }

    itr.result.tigeConcentrationValue = getTigeConcentrationValue(calculatedTIGE, cutoff, isValidTIGE);
    itr.result.resultValue[currentTigeIdx] = getTigeResultText(resultOrgText, itr.result.tigeConcentrationValue);

    return true;
}

void TigEBandAnalyzerBase::sIgECalc(int currentTigeIdx, GlobalDataStruct::AnalysisReportInfomation &itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData,
                                    int cutoff)
{
    auto classNumber = this->getsIgEConditionClass(itr, combinedData);
    auto sIgEMagnification = this->findsIgEMagnification(classNumber);

    if(currentTigeIdx < 0)
        return;

    QString resultOrgText;
    auto currentIntensity = itr.result.rawIntensityValue[currentTigeIdx];
    auto calculatedTIGE = currentIntensity * sIgEMagnification;
    auto isValidTIGE = ClassHelper::isValidTIGE(calculatedTIGE, cutoff);

    if(isValidTIGE)
    {
        itr.result.classType[currentTigeIdx] = "P";
        resultOrgText = ">100";
    }
    else
    {
        itr.result.classType[currentTigeIdx] = "N";
        resultOrgText = "≤100";
    }

    itr.result.tigeConcentrationValue = getTigeConcentrationValue(calculatedTIGE, cutoff, isValidTIGE);
    itr.result.resultValue[currentTigeIdx] = getTigeResultText(resultOrgText, itr.result.tigeConcentrationValue);
}

double TigEBandAnalyzerBase::getTigeConcentrationValue(const double& calculatedTIGE, const int cutoff, const bool isPValueTige)
{
    double concentrationValue = 0.0;

    if(calculatedTIGE <= 0.00000001)
        return ClassHelper::rounding(0.01, 2);

    if(isPValueTige)
        concentrationValue = 100 + ( (calculatedTIGE - cutoff) * (cutoff/5) );
    else
        concentrationValue = (99.99/cutoff)* calculatedTIGE;

    if(concentrationValue <= 0.00000001)
        return ClassHelper::rounding(0.01, 2);

    else
        return ClassHelper::rounding(concentrationValue,2);
}

QString TigEBandAnalyzerBase::getTigeResultText(const QString &orgText, const double &concentrationValue)
{
    GenericIniDataControls io;
    auto isUnmarktIgEIUMLValue = io.readUnmarkIUML();

    if(isUnmarktIgEIUMLValue)
        return QString::number(concentrationValue, 'f', 2);
    else
        return orgText;
}

bool TigEBandAnalyzerBase::isHasNonZeroClass(const GlobalDataStruct::AnalysisReportInfomation &itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData)
{
    for(auto i : itr.result.classType)
    {
       auto currentItr = i.toInt();

       if(0 < currentItr)
           return true;
    }

    for(auto i : combinedData.result.classType)
    {
        auto currentItr = i.toInt();

        if(0 < currentItr)
            return true;
    }

    return false;
}

int TigEBandAnalyzerBase::getsIgEConditionClass(const GlobalDataStruct::AnalysisReportInfomation &itr, const GlobalDataStruct::AnalysisReportInfomation &combinedData)
{
    GenericIniDataControls io;
    auto targetClass = io.readsIgESelectedRadioButtonClass();

    if(targetClass == 0)
        return 0;

    for(auto i : itr.result.classType)
    {
        auto currentItr = i.toInt();

        if(targetClass <= currentItr)
            return targetClass;
    }


    for(auto i : combinedData.result.classType)
    {
        auto currentItr = i.toInt();

        if(targetClass <= currentItr)
            return targetClass;
    }

    return 0;
}

double TigEBandAnalyzerBase::findsIgEMagnification(int classNumber)
{
    GenericIniDataControls io;
    auto magnificaition = 0.0;

    switch(classNumber)
    {
    case 1 :
        magnificaition = io.readsIgEData1();
        break;

    case 2 :
        magnificaition = io.readsIgEData2();
        break;

    case 3 :
        magnificaition = io.readsIgEData3();
        break;

    case 4 :
        magnificaition = io.readsIgEData4();
        break;

    case 5 :
        magnificaition = io.readsIgEData5();
        break;

    case 6 :
        magnificaition = io.readsIgEData6();
        break;

    default :
        break;
    }

    auto isHasNotMagnification = qFuzzyIsNull(magnificaition);

    if(isHasNotMagnification)
        magnificaition = 1;

    return magnificaition;
}

