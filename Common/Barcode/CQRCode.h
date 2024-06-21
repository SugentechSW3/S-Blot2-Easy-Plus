#ifndef CQRCODE_H
#define CQRCODE_H

#include "Common/CUtilCommon.h"
#include <QObject>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "Analysis/GlobalDataStruct.h"

NS_SUGENTECH_BEGIN

class CQRCode : public QObject
{
private:
    Q_OBJECT


    enum QR_POSITION_FLAG_FUNCTION_FUNCTION
    {
        QR_POSITION_FLAG_FUNCTION_TYPE,
        QR_POSITION_FLAG_FUNCTION_LOT_NUMBER,
        QR_POSITION_FLAG_FUNCTION_INDEX_NUMBER,
        QR_POSITION_FLAG_FUNCTION_FUNCTION,
        QR_POSITION_FLAG_FUNCTION_END
    };

    enum QR_POSITION_FLAG_NEW_FORMULA
    {
        QR_POSITION_FLAG_NEW_FORMULA_TYPE,
        QR_POSITION_FLAG_NEW_FORMULA_NEW_FUNCTION_NUMBER,
        QR_POSITION_FLAG_NEW_FORMULA_A,
        QR_POSITION_FLAG_NEW_FORMULA_B,
        QR_POSITION_FLAG_NEW_FORMULA_C,
        QR_POSITION_FLAG_NEW_FORMULA_D,
        QR_POSITION_FLAG_NEW_FORMULA_E,
        QR_POSITION_FLAG_NEW_FORMULA_F,
        QR_POSITION_FLAG_NEW_FORMULA_G,
        QR_POSITION_FLAG_NEW_FORMULA_END
    };

    enum QR_POSITION_FLAG_EXP_DATA
    {
        QR_POSITION_FLAG_EXP_DATA_TYPE,
        QR_POSITION_FLAG_EXP_LOT_NUMBER,
        QR_POSITION_FLAG_EXP_DATA_EXP_CONST_STR,
        QR_POSITION_FLAG_EXP_DATA_EXP_DATE,
        QR_POSITION_FLAG_EXP_DATA_CUTOFF,
        QR_POSITION_FLAG_EXP_DATA_NO_FUNCADDED,
        QR_POSITION_FLAG_EXP_DATA_END
    };

    enum QR_STRIP_LOT_POSITION
    {
        QR_STRIP_LOT_POSITION_CONTENTS_NAME,
        QR_STRIP_LOT_POSITION_PANEL_INITIAL_NAME = 2,
        QR_STRIP_LOT_POSITION_DEVICE_INITIAL_NAME,
        QR_STRIP_LOT_POSITION_YEAR,
        QR_STRIP_LOT_POSITION_NUMBER = 6,
        QR_STRIP_LOT_POSITION_END = 9
    };

public:

    explicit CQRCode(QObject *parent = nullptr);
    virtual ~CQRCode();

    static const int BAND_PARAMETER_QR_SIZE = 4;

    static QVector<QString> deCodeImage(const cv::Mat& imgMat);
    static QString qrFlagParser(const QString &qrString);
    static QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> qrAssemblyOfPartsAndParsing(const QVector<QString> &parts, bool &isSuccess);
    static bool isExpQRData(const QString& decodedData);
    static QVector<QString> qrFunctionRelationParser(const QString& funcStr);
    static QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> qrFunctionNewFunctionParser(const QVector<QString>& list, bool &isSuccess);
    static QVector<GlobalDataStruct::QR_STRIP_DATA_STRUCT> qrFunctionStripParser(const QVector<QString>& list);

    static QString getPanelName(const QString& lotNumber);

    static GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT getLotExpData(const QString& expData);
private:
    static QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT> mapToVector(const QMap<QString,GlobalDataStruct::QR_RELATION_DATA_STRUCT>& mapData);
    static void removeUnCompleteData(QVector<GlobalDataStruct::QR_RELATION_DATA_STRUCT>& data);

    static QString convertInitialToFullDeviceType(const QString& deviceInitial);
    static QString convertInitialToFullPanelName(const QString& panelInitial);
    static QString convertInitialToFullContentsName(const QString& contentsInitial);
    static void sortAndAddBandFormulaParameter(QMap<QString, QMap<int, QString> > &parameterData, QMap<QString, GlobalDataStruct::QR_RELATION_DATA_STRUCT> &returnData);

};
NS_SUGENTECH_END

#endif // CQRCODE_H
