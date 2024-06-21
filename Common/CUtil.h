#ifndef CUTIL_H
#define CUTIL_H

#include "Common/CUtilCommon.h"

#include <QObject>
#include <QMetaObject>
#include <QMetaEnum>
#include <QString>


#include "opencv2/core.hpp"

NS_SUGENTECH_BEGIN

class CUtil
{

public:
    CUtil();
    virtual ~CUtil();

    enum ENDIAN
    {
        ENDIAN_BIG,
        ENDIAN_LITTLE
    };

    //toString overRide
    static QString toString(char target);
    static QString toString(uchar target);
    static QString toString(short target);
    static QString toString(ushort target);
    static QString toString(int target);
    static QString toString(uint target);
    static QString toString(double target);

    static QString hexaToHexaString(char target);
    static QString hexaToHexaString(uchar target);
    static QString hexaToHexaString(short target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static QString hexaToHexaString(ushort target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static QString hexaToHexaString(int target,  CUtil::ENDIAN endian = ENDIAN_BIG);
    static QString hexaToHexaString(uint target, CUtil::ENDIAN endian = ENDIAN_BIG);

    static QString hexaToAsciiString(const uint hexaData, CUtil::ENDIAN endian = ENDIAN_BIG);

    static QString convertSHA256(const QString& target);

    //typecast
    static QString uCharArrayToString(uchar* str);
    static QString uCharArrayToString(const QVector<uchar> &stream);

    static QBitArray charToBitArray(char bits);

    static void messageBox(const char *str);
    static void messageBox(const uchar *str);
    static void messageBox(const QString& str);
    static void messageBox(const QString& str, QWidget *widget);
    static int questionMessageBox(const QString& msg, const QString& title);

    static QByteArray makeCheckSum(const QByteArray data);

    static QString convertQEnumToQString(const int enumIdx, const QString& enumNames, const QMetaObject& metaObject);
    static int convertQStringToQEnum(const QString &key, const QString& enumNames, const QMetaObject& metaObject);

    static QPixmap convertMatToQPixmap(const cv::Mat& mat);

    static QString createDirectory(const QString &pathName);

    static void matrixToExcelData(const QVector<cv::Mat> &mat, const QString& fileName);

    static QString getFileName(const QString& filePath);
    static QString getFilePath(const QString& filePath);

    static void compressionExcelFile(const QStringList &pathList, const QString &defaultFolderPath, const QString &zipSaveDir, const QString &password);
    static bool logoResizeImage(const QString& fileName);
};

NS_SUGENTECH_END

#else

NS_SUGENTECH_BEGIN

class CUtil
{

public:
    CUtil();
    virtual ~CUtil();

    enum ENDIAN
    {
        ENDIAN_BIG,
        ENDIAN_LITTLE
    };

    //toString overRide
    static std::string toString(char target);
    static std::string toString(uchar target);
    static std::string toString(short target);
    static std::string toString(ushort target);
    static std::string toString(int target);
    static std::string toString(uint target);
    static std::string toString(double target);

    static std::string hexaToString(char target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static std::string hexaToString(uchar target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static std::string hexaToString(short target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static std::string hexaToString(ushort target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static std::string hexaToString(int target, CUtil::ENDIAN endian = ENDIAN_BIG);
    static std::string hexaToString(uint target, CUtil::ENDIAN endian = ENDIAN_BIG);

    //typecast
    static std::string ucharArrayToString(uchar* str);
    static std::string ucharArrayToString(uCharArrayStream& stream);
};

NS_SUGENTECH_END

#endif // CUTIL_H
