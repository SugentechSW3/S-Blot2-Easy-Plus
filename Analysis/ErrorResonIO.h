#ifndef ERRORRESONIO_H
#define ERRORRESONIO_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"

class ErrorResonIO : public QObject
{
private:
    Q_OBJECT

public:

    struct ERROR_REASON
    {
        QDateTime timestamp;
        QString reason;
        int stripNumber;
    };

    explicit ErrorResonIO(QObject *parent = nullptr);
    ~ ErrorResonIO();

    static void loadErrorReason(QVector<GlobalDataStruct::AnalysisReportInfomation> &result);
    static void saveErrorReason(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result);

    friend QDataStream & operator << (QDataStream &arch, const ErrorResonIO::ERROR_REASON& object)
    {
        arch << object.timestamp;
        arch << object.reason;
        arch << object.stripNumber;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, ErrorResonIO::ERROR_REASON& object)
    {
        arch >> object.timestamp;
        arch >> object.reason;
        arch >> object.stripNumber;

        return arch;
    }

private:
    static QVector<ErrorResonIO::ERROR_REASON> getDataList(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result);
    static void setDataList(const QVector<ErrorResonIO::ERROR_REASON>& dataList, QVector<GlobalDataStruct::AnalysisReportInfomation>& result);

    static void errorReasonToFile(const QVector<ErrorResonIO::ERROR_REASON>& dataList);
    static QVector<ErrorResonIO::ERROR_REASON> errorReasonFromFile();
};

Q_DECLARE_METATYPE(QVector<ErrorResonIO::ERROR_REASON>)

#endif // ERRORRESONIO_H


