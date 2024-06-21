#include "CUtil.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2017-08-17
// Modify  Date : 2017-11-07 USE_QT Macro Addtional
// Modify  Date : 2018-07-30 Move to Convert Enum To String
// Modify  Date : 2018-10-19 questionMessageBox Function Bug Fixed / Modify MessageBox Option to Rich Text
// Modify  Date : 2019-11-11 QuestionMessageBox Function Set Up Always On Top

#include <QApplication>
#include <QMessageBox>
#include <QByteArray>
#include <QBitArray>
#include <QDir>
#include <QProcess>
#include <QDateTime>

#include "Common/CSHA256.h"
#include "Common/CLogWriter.h"
#include "opencv2/opencv.hpp"

NS_SUGENTECH_BEGIN

//
QString CUtil::toString(char target)
{
    QString retValue;
    retValue.push_back(target);

    return retValue;
}
QString CUtil::toString(uchar target)
{
    QString retValue;
    retValue.push_back(target);

    return retValue;
}

QString CUtil::toString(short target)
{
    QString retValue;
    retValue = QString::number(target);

    return retValue;
}

QString CUtil::toString(ushort target)
{
    QString retValue;
    retValue = QString::number(target);

    return retValue;
}

QString CUtil::toString(int target)
{
    QString retValue;
    retValue = QString::number(target);

    return retValue;
}

QString CUtil::toString(uint target)
{
    QString retValue;
    retValue = QString::number(target);

    return retValue;
}

QString CUtil::toString(double target)
{
    QString retValue;
    retValue = QString::number(target);

    return retValue;
}

QString CUtil::hexaToHexaString(char target)
{
    QString retValue;
    retValue.sprintf("%02x", target);

    return retValue;
}

QString CUtil::hexaToHexaString(uchar target)
{
    QString retValue;
    retValue.sprintf("%02x", target);

    return retValue;
}

QString CUtil::hexaToHexaString(short target, ENDIAN endian)
{
    QString retValue;
    uchar targetBuf[2] = { 0, };
    uchar* targetPtr = (uchar*)&target;

    if (endian == ENDIAN_BIG)
    {
        targetBuf[0] = targetPtr[1];
        targetBuf[1] = targetPtr[0];
    }
    else
    {
        targetBuf[0] = targetPtr[0];
        targetBuf[1] = targetPtr[1];
    }

    retValue.sprintf("%02x%02x", targetBuf[0], targetBuf[1]);
    return retValue;
}

QString CUtil::hexaToHexaString(ushort target, ENDIAN endian)
{
    QString retValue;
    uchar targetBuf[2] = { 0, };
    uchar* targetPtr = (uchar*)&target;

    if (endian == ENDIAN_BIG)
    {
        targetBuf[0] = targetPtr[1];
        targetBuf[1] = targetPtr[0];
    }
    else
    {
        targetBuf[0] = targetPtr[0];
        targetBuf[1] = targetPtr[1];
    }

    retValue.sprintf("%02x%02x", targetBuf[0], targetBuf[1]);
    return retValue;
}

QString CUtil::hexaToHexaString(int target, ENDIAN endian)
{
    QString retValue;
    uchar targetBuf[4] = { 0, };
    uchar* targetPtr = (uchar*)&target;

    if (endian == ENDIAN_BIG)
    {
        targetBuf[0] = targetPtr[3];
        targetBuf[1] = targetPtr[2];
        targetBuf[2] = targetPtr[1];
        targetBuf[3] = targetPtr[0];
    }
    else
    {
        targetBuf[0] = targetPtr[0];
        targetBuf[1] = targetPtr[1];
        targetBuf[2] = targetPtr[2];
        targetBuf[3] = targetPtr[3];
    }

    retValue.sprintf("%02x%02x%02x%02x", targetBuf[0], targetBuf[1], targetBuf[2], targetBuf[3]);
    return retValue;
}

QString CUtil::hexaToHexaString(uint target, ENDIAN endian)
{
    QString retValue;
    uchar targetBuf[4] = { 0, };
    uchar* targetPtr = (uchar*)&target;

    if (endian == ENDIAN_BIG)
    {
        targetBuf[0] = targetPtr[3];
        targetBuf[1] = targetPtr[2];
        targetBuf[2] = targetPtr[1];
        targetBuf[3] = targetPtr[0];
    }
    else
    {
        targetBuf[0] = targetPtr[0];
        targetBuf[1] = targetPtr[1];
        targetBuf[2] = targetPtr[2];
        targetBuf[3] = targetPtr[3];
    }

    retValue.sprintf("%02x%02x%02x%02x", targetBuf[0], targetBuf[1], targetBuf[2], targetBuf[3]);
    return retValue;
}

QString CUtil::hexaToAsciiString(const uint hexaData, CUtil::ENDIAN endian)
{
    char* ascStr = (char*)&hexaData;

    QString ret;

    if (endian == ENDIAN_BIG)
    {
        ret.append(ascStr[3]);
        ret.append(ascStr[2]);
        ret.append(ascStr[1]);
        ret.append(ascStr[0]);
    }
    else
    {
        ret.append(ascStr[0]);
        ret.append(ascStr[1]);
        ret.append(ascStr[2]);
        ret.append(ascStr[3]);
    }


    return ret;
}

QString CUtil::convertSHA256(const QString &target)
{
    BYTE pong[32];
    BYTE nong[32];
    UINT pong_leng;
    char a[3];
    QString result;

    memcpy(&pong,target.toUtf8().data(),(size_t)target.toUtf8().size());

    pong_leng = target.size();
    SHA256_Encrpyt(pong,pong_leng,nong);

   for(int i = 0; i < sizeof(pong); i++)
   {
       sprintf(a,"%02X",nong[i]);
       result.append(a);
   }

   return result;
}

QString CUtil::uCharArrayToString(uchar* str)
{
    QString retValue((char*)str);

    return retValue;
}

QString CUtil::uCharArrayToString(const QVector<uchar>& str)
{
    QString retValue((char*)str.constData());

    return retValue;
}

QBitArray CUtil::charToBitArray(char bits)
{
    QBitArray array(8);
    return array.fromBits(&bits, 8);
}

void CUtil::messageBox(const char* str)
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(str);
    msgBox.raise();
    msgBox.exec();
}

void CUtil::messageBox(const uchar *str)
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText((char*)str);
    msgBox.raise();
    msgBox.exec();
}

void CUtil::messageBox(const QString& str)
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(str);
    msgBox.raise();
    msgBox.exec();
}

void CUtil::messageBox(const QString& str, QWidget * widget)
{
    if(widget == nullptr)
        return;

    QMessageBox msgBox(widget);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(str);
    msgBox.raise();
    msgBox.exec();
}

int CUtil::questionMessageBox(const QString& msg, const QString& title)
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
    msgBox.raise();

    return msgBox.question(nullptr, title, msg);
}

QByteArray CUtil::makeCheckSum(const QByteArray data)
{
    short checkSum = 0;

#pragma omp parallel for schedule(auto) reduction(+:checkSum)
    for(int i = 0 ; i < data.size(); i++)
        checkSum += data.at(i);

    QString checkSumStr = CUtil::hexaToHexaString(checkSum);


    return  checkSumStr.toUtf8().mid(2,2);
}

QString CUtil::convertQEnumToQString(const int enumIdx, const QString& enumNames, const QMetaObject& metaObject)
{
    int enumIndex = metaObject.indexOfEnumerator(enumNames.toStdString().data());

    if(enumIndex == -1)
        return QString();

    QMetaEnum en = metaObject.enumerator(enumIndex);
    return QString(en.valueToKey(enumIdx));
}

int CUtil::convertQStringToQEnum(const QString& key, const QString& enumNames, const QMetaObject& metaObject)
{
    int enumIndex = metaObject.indexOfEnumerator(enumNames.toStdString().data());
    QMetaEnum _enum = metaObject.enumerator(enumIndex);
    int enumValue = _enum.keyToValue(key.toStdString().data());

    return enumValue;
}

QPixmap CUtil::convertMatToQPixmap(const cv::Mat& mat)
{
    QImage cvImg  = QImage((uchar*)mat.data, mat.size().width, mat.size().height, mat.step, QImage::Format_RGB888);//0xffRRGGBB (24bit)

    auto img =  cvImg.rgbSwapped(); //BGR -> RGB
    return QPixmap::fromImage(img);
}

QString CUtil::createDirectory(const QString& pathName)
{
    QString path = pathName;
    QDir dir;
    dir.mkpath(path);

    return path;
}

QString CUtil::getFileName(const QString &filePath)
{
    QString file = filePath;
    auto splitPath = file.split("/");
    return splitPath[splitPath.size()-1];
}

QString CUtil::getFilePath(const QString &filePath)
{
    QString file = filePath;
    QString filepath;
    auto splitPath = file.split("/");
    for(auto&itr : splitPath)
        filepath+=itr;
    return filepath;
}

void CUtil::compressionExcelFile(const QStringList &pathList, const QString &defaultFolderPath, const QString &zipSaveDir, const QString &password)
{
    QProcess process;

    QString zipExeFileName = defaultFolderPath + "/" + "7zr.exe"; //7zr이 있는 절대 path;
    QString zipFileName =  zipSaveDir + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".7z";

    QString cmd = QString("\"%1\" a \"%2\"").arg(zipExeFileName).arg(zipFileName);

    for(auto itr : pathList)
        cmd.append(QString(" \"%1\"").arg(itr));

    cmd.append(QString(" -p%1").arg(password));

    process.start(cmd);
    process.waitForFinished(-1);

    for(auto& itr : pathList)
        QFile::remove(itr);

}

bool CUtil::logoResizeImage(const QString &fileName)
{
    auto inputImage = cv::imread(fileName.toStdString().data());
    cv::Mat outputImage;

    cv::resize(inputImage,outputImage,cv::Size(200,50),0,0,CV_INTER_LINEAR);

    QString saveImageFile = QApplication::applicationDirPath() + "/Config/logo.png";

    auto isSucess = cv::imwrite(saveImageFile.toStdString(),outputImage);
    return isSucess;
}


NS_SUGENTECH_END //네임스페이스 종료
