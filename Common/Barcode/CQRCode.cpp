#include <QDebug>
#include "CQRCode.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include "zbar.h"
#include "Analysis/ConfigInformation.h"

NS_SUGENTECH_BEGIN
CQRCode::CQRCode(QObject *parent) :
    QObject(parent)
{

}

CQRCode::~CQRCode()
{

}

QVector<QString> CQRCode::deCodeImage(const cv::Mat& imgMat)
{
    cv::Mat imGray;
    QVector<QString> resultList;
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

    if(imgMat.data == nullptr)
        return resultList;

    cv::cvtColor(imgMat, imGray, cv::COLOR_BGR2GRAY);

    zbar::Image image(imGray.cols, imGray.rows, "Y800", (uchar *)imGray.data, imGray.cols * imGray.rows);
    scanner.scan(image);

    for(auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
    {
        QString result;
        result = QString::fromStdString(symbol->get_data());
        resultList.push_back(result);
    }

    return resultList;
}

QString CQRCode::qrFlagParser(const QString& qrString)
{
    auto typeFlag = qrString.left(1);

    return typeFlag;
}

QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> CQRCode::qrAssemblyOfPartsAndParsing(const QVector<QString>& parts, bool& isSuccess)
{
    QMap<QString, GlobalDataStruct::QR_RELATION_DATA_STRUCT> parsedResultMapData;
    QMap<QString, QMap<int,QString>> lotAndIndexedParamaterData;
    isSuccess = true;

    for(auto& itr : parts)
    {
        auto splitedData = itr.split("|");

        if(splitedData.count() != QR_POSITION_FLAG_FUNCTION_END)
        {
            parsedResultMapData.clear();
            isSuccess = false;
            break;
        }

        auto lotNumber = splitedData[QR_POSITION_FLAG_FUNCTION_LOT_NUMBER];
        auto indexNumber = splitedData[QR_POSITION_FLAG_FUNCTION_INDEX_NUMBER];
        auto funcList = splitedData[QR_POSITION_FLAG_FUNCTION_FUNCTION];

        auto indexAndParameterMap = lotAndIndexedParamaterData[lotNumber];
        indexAndParameterMap[indexNumber.toInt()] = funcList;
        lotAndIndexedParamaterData[lotNumber] = indexAndParameterMap;
    }

    CQRCode::sortAndAddBandFormulaParameter(lotAndIndexedParamaterData, parsedResultMapData);

    auto vectorData = CQRCode::mapToVector(parsedResultMapData);
    CQRCode::removeUnCompleteData(vectorData);

    return vectorData;
}

bool CQRCode::isExpQRData(const QString& decodedData)
{
    auto split = decodedData.split('|');

    if(split.last().toUpper() != "NOFUNCADDED")
        return false;

    return true;
}

QVector<QString> CQRCode::qrFunctionRelationParser(const QString& funcStr)
{
    QVector<QString> relList;

    for(auto i = 0; i < funcStr.count(); i += 3)
        relList.push_back(funcStr.mid(i,3));

    return relList;
}

QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> CQRCode::mapToVector(const QMap<QString,GlobalDataStruct::QR_RELATION_DATA_STRUCT>& mapData)
{
    QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> vecData;
    QMapIterator<QString,GlobalDataStruct::QR_RELATION_DATA_STRUCT> itr(mapData);

    while(itr.hasNext())
    {
        itr.next();
        vecData << itr.value();
    }

    return vecData;
}

void CQRCode::removeUnCompleteData(QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT>& data)
{
    auto config = ConfigInformation::getInstance();
    auto bandCount = config->getStatus().stripSettings.boxCarAnalaysis.boxCarbandCount;

    QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> completedData;

    for(auto& itr : data)
    {
        auto currentBandCount = itr.bandFormulaParameter.count();

        if(bandCount == currentBandCount)
            completedData << itr;
    }

    data = completedData;
}

QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> CQRCode::qrFunctionNewFunctionParser(const QVector<QString>& list, bool& isSuccess)
{
    QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> parsingList;
    isSuccess = true;

    for(auto& itr : list)
    {
        GlobalDataStruct::QR_FORMULA_DATA_STRUCT data;
        auto split = itr.split('|');

        if(split.count() != QR_POSITION_FLAG_NEW_FORMULA_END)
        {
            parsingList.clear();
            isSuccess = false;
            break;
        }

        data.newFunctionNumber = split[QR_POSITION_FLAG_NEW_FORMULA_NEW_FUNCTION_NUMBER];
        data.functionDataA = split[QR_POSITION_FLAG_NEW_FORMULA_A].toDouble();
        data.functionDataB = split[QR_POSITION_FLAG_NEW_FORMULA_B].toDouble();
        data.functionDataC = split[QR_POSITION_FLAG_NEW_FORMULA_C].toDouble();
        data.functionDataD = split[QR_POSITION_FLAG_NEW_FORMULA_D].toDouble();
        data.functionDataE = split[QR_POSITION_FLAG_NEW_FORMULA_E].toDouble();
        data.functionDataF = split[QR_POSITION_FLAG_NEW_FORMULA_F].toDouble();
        data.functionDataG = split[QR_POSITION_FLAG_NEW_FORMULA_G].toDouble();

        parsingList.push_back(data);
    }

    return parsingList;
}

QVector<GlobalDataStruct::QR_STRIP_DATA_STRUCT> CQRCode::qrFunctionStripParser(const QVector<QString>& list)
{
    QVector<GlobalDataStruct::QR_STRIP_DATA_STRUCT> parsingList;

    for(auto itr: list)
    {
        GlobalDataStruct::QR_STRIP_DATA_STRUCT parsingData;
        parsingData.contentsName = CQRCode::convertInitialToFullContentsName(itr.left(QR_STRIP_LOT_POSITION_PANEL_INITIAL_NAME - QR_STRIP_LOT_POSITION_CONTENTS_NAME));
        parsingData.panelName =  CQRCode::convertInitialToFullPanelName(itr.mid(QR_STRIP_LOT_POSITION_PANEL_INITIAL_NAME,  QR_STRIP_LOT_POSITION_DEVICE_INITIAL_NAME - QR_STRIP_LOT_POSITION_PANEL_INITIAL_NAME));
        parsingData.deviceType = CQRCode::convertInitialToFullDeviceType(itr.mid(QR_STRIP_LOT_POSITION_DEVICE_INITIAL_NAME, QR_STRIP_LOT_POSITION_YEAR - QR_STRIP_LOT_POSITION_DEVICE_INITIAL_NAME));
        parsingData.yearOfManufacture = itr.mid(QR_STRIP_LOT_POSITION_YEAR, QR_STRIP_LOT_POSITION_NUMBER - QR_STRIP_LOT_POSITION_YEAR);
        parsingData.number = itr.right(QR_STRIP_LOT_POSITION_END - QR_STRIP_LOT_POSITION_NUMBER);
        parsingList.push_back(parsingData);
    }

    return parsingList;
}

QString CQRCode::getPanelName(const QString& lotNumber)
{
    if(lotNumber.isEmpty() == true)
        return QString();

    return CQRCode::convertInitialToFullPanelName(lotNumber.mid(2,1));
}

GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT CQRCode::getLotExpData(const QString &expData)
{
    GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT data;

    if(expData.isEmpty() == true)
        return GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT();

    QStringList splitExpData = expData.split('|');

    if(splitExpData.count() != QR_POSITION_FLAG_EXP_DATA_END)
        return GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT();

    data.lotNumber = splitExpData[QR_POSITION_FLAG_EXP_LOT_NUMBER];
    data.cutoff = splitExpData[QR_POSITION_FLAG_EXP_DATA_CUTOFF].toInt();
    data.expDate = splitExpData[QR_POSITION_FLAG_EXP_DATA_EXP_DATE];

    return data;
}

QString CQRCode::convertInitialToFullDeviceType(const QString& deviceInitial)
{
    QString fullName;
    QString initial = deviceInitial;
    initial = initial.toUpper();

    if(initial == "B")
        fullName = "BLOT";

    else if(initial == "S")
        fullName = "S-SCAN";

    return fullName;
}

QString CQRCode::convertInitialToFullPanelName(const QString& panelInitial)
{
    QString fullName;
    QString initial = panelInitial;
    initial = initial.toUpper();

    if(initial == "P")
        fullName = ConfigInformation::convertQEnumToQString(ConfigInformation::ALLERGY_PANEL_LISTS::ALLERGY_PANEL, "ALLERGY_PANEL_LISTS");

    return fullName;
}

QString CQRCode::convertInitialToFullContentsName(const QString& contentsInitial)
{
    QString fullName ="";
    QString initial = contentsInitial;
    initial = initial.toUpper();

    if(initial == "AL")
        fullName = "ALLERGY";

    return fullName;
}

void CQRCode::sortAndAddBandFormulaParameter(QMap<QString, QMap<int,QString>>& parameterData, QMap<QString, GlobalDataStruct::QR_RELATION_DATA_STRUCT>& returnData)
{
    QMapIterator<QString, QMap<int,QString>> itr(parameterData);

    while(itr.hasNext())
    {
        itr.next();

        QVector<QString> parameter;
        GlobalDataStruct::QR_RELATION_DATA_STRUCT lotAndBandParameter;
        auto lotNumber = itr.key();
        auto indexAndParameter = itr.value();

        if(indexAndParameter.count() != CQRCode::BAND_PARAMETER_QR_SIZE)
            continue;

        for(int i = 1 ; i < 5; i++)
            parameter << CQRCode::qrFunctionRelationParser(indexAndParameter[i]);

        lotAndBandParameter.lotNumber = lotNumber;
        lotAndBandParameter.bandFormulaParameter = parameter;


        returnData[lotNumber] = lotAndBandParameter;
    }
}

NS_SUGENTECH_END
