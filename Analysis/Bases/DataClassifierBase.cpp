#include "DataClassifierBase.h"
#include "TigEBandAnalyzerBase.h"

#include <Analysis/ClassHelper.h>
#include <Analysis/ConfigInformation.h>
#include <Analysis/Interfaces/ITigEBandAnalyzer.h>
#include <DataBase/CodeFuncDatabaseCtrl.h>

DataClassifierBase::DataClassifierBase(QObject *parent)
    :IDataClassifier(parent)
{

}

bool DataClassifierBase::calcClassData(QVector<GlobalDataStruct::AnalysisReportInfomation> &element, DataBaseCtrl *codeFuncDatabase)
{
    if(codeFuncDatabase == nullptr)
        return false;

    auto funcMap = CodeFuncDatabaseCtrl::getFuncData(codeFuncDatabase);
    auto stripRelMap = CodeFuncDatabaseCtrl::getStripRelData(codeFuncDatabase);
    auto lotCutoff = CodeFuncDatabaseCtrl::getLotCutoffData(codeFuncDatabase);
    auto config = ConfigInformation::getInstance();

    int dataBaseCutOff = 0;

    for(auto& itr : element)
    {
        itr.result.resultValue.clear();

        auto panelName = itr.element.panelName;
        auto codeList = config->getCurrentStripCodeList(panelName);

        QString currentLot = itr.lotNumber;
        dataBaseCutOff = ClassHelper::getVaildTigeCutOff(lotCutoff[currentLot].cutoff);

        auto funcRelList = stripRelMap[currentLot];

       for(int i = 0; i < itr.result.rawIntensityValue.count(); i++)
       {
            if(funcRelList.count() <= i)
                break;

            QString code;
            auto funcRel = funcRelList[i];

            if(i < codeList.count())
                code = codeList[i];

            auto intensity = itr.result.rawIntensityValue[i];
            auto calcValue = this->calcContentsValue(intensity, funcMap[funcRel]);

            this->appendClassTypeAndResultValue(itr, intensity, calcValue, code, funcRel,dataBaseCutOff);
       }
    }

    QScopedPointer<ITigEBandAnalyzer> tigEBandAnalysis;    
    tigEBandAnalysis.reset(new TigEBandAnalyzerBase);

    if(tigEBandAnalysis->analyzeTigEBand(element, lotCutoff) == false)
        return false;

    return true;
}

double DataClassifierBase::calcContentsValue(const double &intensity, const GlobalDataStruct::FORMULA_PARAMETER &parameter)
{
    double calcValue = 0.0;
    auto config = ConfigInformation::getInstance();
    auto contentsName = config->getCurrentContents();
    auto contentsEnum = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(contentsEnum)
    {
    case (int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY:
        calcValue = ClassHelper::calcAllergyValue(intensity, parameter);
        break;
    }

    return calcValue;
}

void DataClassifierBase::appendClassTypeAndResultValue(GlobalDataStruct::AnalysisReportInfomation &itr, double intensity, double calcValue, const QString &code, const QString &funcRel, const int &tigECutoff)
{
    if(code.compare("PC", Qt::CaseInsensitive) == 0)
    {
        auto isValidPC = ClassHelper::isValidPCLine(intensity);

        if(isValidPC)
        {
            itr.result.classType.push_back("Valid");
            itr.result.resultValue.push_back(">100");
        }
        else
        {
            itr.result.classType.push_back("InValid");
            itr.result.resultValue.push_back("≤100");
        }
    }
    else if(code.compare("PM", Qt::CaseInsensitive) == 0)
    {
        auto isValidPC = ClassHelper::isValidPCLine(intensity);

        if(isValidPC)
        {
            itr.result.classType.push_back("P");
            itr.result.resultValue.push_back(">100");
        }
        else
        {
            itr.result.classType.push_back("F");
            itr.result.resultValue.push_back("≤100");
        }
    }
    else if(code.compare("tIgE", Qt::CaseInsensitive) == 0)
    {
        auto isValidTIGE = ClassHelper::isValidTIGE(intensity, tigECutoff);

        if(isValidTIGE)
        {
            itr.result.classType.push_back("P");
            itr.result.resultValue.push_back(">100");
        }
        else
        {
            itr.result.classType.push_back("N");
            itr.result.resultValue.push_back("≤100");
        }
    }
    else if(code.compare("null", Qt::CaseInsensitive) == 0)
    {
        itr.result.resultValue.push_back(QString::number(intensity));
        itr.result.classType.push_back("-");
    }
    else if(code.compare("-", Qt::CaseInsensitive) == 0)
    {
        itr.result.resultValue.push_back("-");
        itr.result.classType.push_back("-");
    }
    else
    {
        bool isValidParameter = (funcRel.isEmpty() == false) && (funcRel != "000");
        auto resultValue = QString::number(calcValue, 'f', 2);

        if(isValidParameter == false)
        {
            itr.result.resultValue.push_back("-");
            itr.result.classType.push_back("-");
            return;
        }

        itr.result.classType.push_back(ClassHelper::checkClass(calcValue));

        if(THRESHOLD_IgE_CLASS_6 < calcValue)
            itr.result.resultValue.push_back(">100");

        else if(THRESHOLD_IgE_CLASS_1 <= calcValue)
            itr.result.resultValue.push_back(resultValue);
        else
        {
            GenericIniDataControls io;
            auto isUseRawData = io.readUseLowLevelConfigData();

            if(isUseRawData)
                itr.result.resultValue.push_back(resultValue);
            else
                itr.result.resultValue.push_back("<0.35");
        }
    }
}
