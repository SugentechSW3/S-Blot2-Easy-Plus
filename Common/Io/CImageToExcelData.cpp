#include <QApplication>
#include "CImageToExcelData.h"

#include "Common/CUtil.h"
#include "Common/CLogWriter.h"

#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include "xlsxchart.h"
#include "xlsxcellrange.h"

NS_SUGENTECH_BEGIN


struct CImageToExcelData::privateSturct
{
    QScopedPointer<QXlsx::Document> mDocument;
    int mCurrentRowPositon = 1;
    int mCurrentColumnPosition = 1;
};


CImageToExcelData::CImageToExcelData(QObject *parent) :
                                                        QObject(parent),
                                                        d(new privateSturct)
{
}

CImageToExcelData::~CImageToExcelData()
{
    this->close();
}

void CImageToExcelData::open(const QString& fileName)
{
    this->close();
    QString folderPath = QApplication::applicationDirPath() + "/" + "ImageData";
    auto filePath =  CUtil::createDirectory(folderPath) + "/" + fileName + ".xlsx";

    d->mDocument.reset(new QXlsx::Document(filePath));
}

void CImageToExcelData::close()
{
    d->mCurrentRowPositon = 1;
    d->mCurrentColumnPosition = 1;

    if(d->mDocument.isNull() == false)
        d->mDocument->save();
}

void CImageToExcelData::addData(const QVector<cv::Mat>& image)
{
    if(d->mDocument.isNull() == true)
        return;

    d->mCurrentColumnPosition = 1;

    for(int i = 0; i < image.size(); i++)
    {
        cv::Mat sourceImg;
        image[i].copyTo(sourceImg);

        for(int y = 0;  y < sourceImg.size().height; y++)
        {
            for(int x = 0; x < sourceImg.size().width; x++)
                d->mDocument->write(d->mCurrentRowPositon+y, d->mCurrentColumnPosition+x, sourceImg.at<uchar>(y,x));
        }

        d->mCurrentColumnPosition = d->mCurrentColumnPosition + sourceImg.size().width + 2;
    }

    if(image.isEmpty() == false)
        d->mCurrentRowPositon = d->mCurrentRowPositon + 3 + image.first().size().height;
}


NS_SUGENTECH_END
#if 0 //deprecated
void CUtil::matrixToExcelData(const QVector<cv::Mat>& mat, const QString& fileName)
{
    QString folderPath = QApplication::applicationDirPath() + "/" + "ImageData";
    QString filePath = folderPath + "/" + fileName + ".xlsx";
    QDir dir;
    dir.mkdir(folderPath);

    QXlsx::Document xlsx(filePath);
    //    d->mXlsx->write(CellReference("E10"), result.element.age);


    for(int i = 0 ; i < mat.count(); i++)
    {
        cv::Mat sourceImg;
        mat[i].copyTo(sourceImg);

        for(int y = 0;  y < sourceImg.size().height; y++)
        {
            for(int x = 0; x < sourceImg.size().width; x++)
            {
               xlsx.write( sourceImg.at<uchar>(y,x);
            }
        }
    }

    xlsx.save();
}
#endif
