#include "ReportJsonDataControls.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QProcess>

#include "Common/CUtil.h"
#include "Common/CLogWriter.h"
#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct ReportJsonDataControls::privateSturct
{
    const QString mDefaultDirPath = "DocumentFormat";
    QString mJSonFilePath;
    QString mExcelFilePath;
    QThread* mThread = nullptr;

};
//deprecated class
ReportJsonDataControls::ReportJsonDataControls(QObject *parent):
    QObject(parent),d(new privateSturct)
{
    d->mThread = new QThread(this);
    this->moveToThread(d->mThread);
    d->mThread->start();
}

ReportJsonDataControls::~ReportJsonDataControls()
{
    if(d->mThread != nullptr)
    {
        d->mThread->terminate();
        d->mThread->wait();
        d->mThread = nullptr;
    }
}

void ReportJsonDataControls::convertJsonAnalysisReportImformation(const GlobalDataStruct::AnalysisReportInfomation &result, const QString &filePath)
{
    auto config = ConfigInformation::getInstance();

    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(result.element.panelName);

    if(itemNameList.isEmpty() == true)
        return;

    if(itemCodeList.isEmpty() == true)
        return;

    d->mExcelFilePath = filePath;
    QJsonObject AnalysisObject;
    AnalysisObject.insert("uniqueID",result.uniqueID);
    AnalysisObject.insert("lotNumber",result.lotNumber);
    AnalysisObject.insert("dateTime",result.dateTime.toString("yyyy-MM-dd hh:mm"));
    AnalysisObject.insert("isDualBand",config->getStatus().isMultiBand);
    AnalysisObject.insert("fileName",filePath);


    auto elemetObject = this->getElementObject(result);
    AnalysisObject.insert("element",elemetObject);

    auto AnalysisResultObject = this->getAnalysisResultObject(result);
    AnalysisObject.insert("AnalysisResult",AnalysisResultObject);

    auto bandName = this->getBandNameList(result);
    AnalysisObject.insert("bandName", bandName);

    QJsonDocument doc(AnalysisObject);
    QString jsonFileName = CUtil::getFileName(filePath);
    jsonFileName.replace("xlsx","json");
    this->saveJsonFile(jsonFileName, doc);

    //return QString(doc.toJson());
}

bool ReportJsonDataControls::isExportJson(const QString password)
{
    QString formFileFullPath = QApplication::applicationDirPath() + "/" + d->mDefaultDirPath + "/convertExcel.exe" ;
    QString query = formFileFullPath;
    query.append(" "+d->mExcelFilePath+ " "+password);

    QProcess process;

    process.start(query);
    process.waitForFinished(-1);

    QByteArray out = process.readAllStandardOutput();

    if(QString(out).contains(QString("Success")))
    {
        process.close();
        CLogWriter::printLog(QString("%1 File Export Sucess").arg(d->mExcelFilePath));
        return true;
    }else
    {
        process.close();
        CLogWriter::printLog(QString("%1 File Export Fail").arg(d->mExcelFilePath));
        return false;
    }
}

QJsonObject ReportJsonDataControls::getElementObject(const GlobalDataStruct::AnalysisReportInfomation &result)
{
    QJsonObject elemetObject;

    elemetObject.insert("stripNumber",result.element.stripNumber);
    elemetObject.insert("patientID",result.element.patientID);
    elemetObject.insert("name",result.element.panelName);
    elemetObject.insert("contentsName",result.element.contentsName);
    elemetObject.insert("panelName",result.element.panelName);
    elemetObject.insert("isSuccess",result.element.isSuccess);
    elemetObject.insert("applicant",result.element.applicant);
    elemetObject.insert("analyst",result.element.analyst);
    elemetObject.insert("reviewer",result.element.reviewer);
    elemetObject.insert("sex",result.element.sex);
    elemetObject.insert("age",result.element.age);
    elemetObject.insert("phone",result.element.phone);
    elemetObject.insert("analyst",result.element.analyst);
    elemetObject.insert("eMail",result.element.eMail);


    return elemetObject;
}

QJsonObject ReportJsonDataControls::getAnalysisResultObject(const GlobalDataStruct::AnalysisReportInfomation &result)
{
    QJsonObject AnalysisResultObject;

    AnalysisResultObject.insert("errorReason",result.result.errorReason);
    AnalysisResultObject.insert("comment",result.result.comment);

    QJsonArray intensityArray, bandValueArray,backgroundValueArray,resultValueArray, classTypeArray;

    for(int index = 0; index< result.result.rawIntensityValue.count(); index++)
    {
        if(result.result.rawIntensityValue.size() > index)
            intensityArray.push_back(result.result.rawIntensityValue.at(index));

        if(result.result.bandValue.size() > index)
            bandValueArray.push_back(result.result.bandValue.at(index));

        if(result.result.backgroundValue.size() > index)
             backgroundValueArray.push_back(result.result.backgroundValue.at(index));

        if(result.result.resultValue.size() > index)
            resultValueArray.push_back(result.result.resultValue.at(index));

        if(result.result.classType.size() > index)
            classTypeArray.push_back(result.result.classType.at(index));

    }
    AnalysisResultObject.insert("intensityValue",intensityArray);
    AnalysisResultObject.insert("bandValue",bandValueArray);
    AnalysisResultObject.insert("backgroundValue",backgroundValueArray);
    AnalysisResultObject.insert("resultValue",resultValueArray);
    AnalysisResultObject.insert("classType",classTypeArray);

    return AnalysisResultObject;
}

QJsonObject ReportJsonDataControls::getBandNameList(const GlobalDataStruct::AnalysisReportInfomation &result)
{
    QJsonObject itemBandName;
#if 0
    auto config = ConfigInformation::getInstance();
    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto itemCodeList = config->getCurrentStripCodeList(result.element.panelName);

    auto resultCount = result.result.resultValue.count();
    auto dualBandFrontCount = config->getMultiBandSetting().dualBandFrontBlockCount;

    itemBandName.insert("bandFrontCount",dualBandFrontCount);
    QJsonArray itemName,itemCode;
    for(int index = 0; index < resultCount; index++)
    {
        itemName.push_back(itemNameList.at(index));
        itemCode.push_back(itemCodeList.at(index));
    }
    itemBandName.insert("bandName",itemName);
    itemBandName.insert("bandCode",itemCode);
#endif
    return itemBandName;
}

void ReportJsonDataControls::saveJsonFile(const QString fileName, const QJsonDocument &doc)
{
    d->mJSonFilePath = QApplication::applicationDirPath()+ "/" + d->mDefaultDirPath + "/" + fileName;
    QFile file(d->mJSonFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(doc.toJson());
    file.close();
}
