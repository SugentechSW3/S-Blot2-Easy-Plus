#ifndef CIMAGETOEXCELDATA_H
#define CIMAGETOEXCELDATA_H

#include "Common/CUtilCommon.h"
#include "opencv2/core.hpp"

#ifdef USE_QT

#include <QObject>

NS_SUGENTECH_BEGIN
class CImageToExcelData : public QObject
{
private:
    Q_OBJECT
    struct privateSturct;

public:
    explicit CImageToExcelData(QObject *parent = nullptr);
    ~CImageToExcelData();

    void open(const QString& fileName);
    void close();

    void addData(const QVector<cv::Mat>& image);

private:
    QScopedPointer<privateSturct> d;
};

NS_SUGENTECH_END
#else

#endif /* END OF USE_QT */

#endif // CIMAGETOEXCELDATA_H
