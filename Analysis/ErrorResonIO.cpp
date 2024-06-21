#include <QDataStream>
#include <QDebug>
#include <QFile>

#include "ErrorResonIO.h"

ErrorResonIO::ErrorResonIO(QObject *parent) :
                                              QObject(parent)
{

}

ErrorResonIO::~ErrorResonIO()
{

}

void ErrorResonIO::loadErrorReason(QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    auto dataList = ErrorResonIO::errorReasonFromFile();
    ErrorResonIO::setDataList(dataList, result);
}

void ErrorResonIO::saveErrorReason(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    auto dataList = ErrorResonIO::getDataList(result);
    ErrorResonIO::errorReasonToFile(dataList);
}

QVector<ErrorResonIO::ERROR_REASON> ErrorResonIO::getDataList(const QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    QVector<ErrorResonIO::ERROR_REASON> dataList;

    for(auto& itr : result)
    {
        ErrorResonIO::ERROR_REASON data;
        data.reason = itr.result.errorReason;
        data.timestamp = itr.dateTime;
        data.stripNumber = itr.element.stripNumber;

        dataList << data;
    }

    return dataList;
}

void ErrorResonIO::setDataList(const QVector<ErrorResonIO::ERROR_REASON>& dataList, QVector<GlobalDataStruct::AnalysisReportInfomation>& result)
{
    for(auto& reasonItr : dataList)
    {
        for(auto& resultItr : result)
        {
            auto reasonTimeStr = reasonItr.timestamp.toString("yyyy-MM-dd hh:mm:ss");
            auto resultTimeStr = resultItr.dateTime.toString("yyyy-MM-dd hh:mm:ss");

            auto isEqualStripNumber = (reasonItr.stripNumber == resultItr.element.stripNumber);
            auto isEqualTimeStamp = (reasonTimeStr == resultTimeStr);

            if(isEqualStripNumber && isEqualTimeStamp)
                resultItr.result.errorReason = reasonItr.reason;

            qDebug() << resultItr.result.errorReason;
            qDebug() << reasonItr.timestamp;
            qDebug() << resultItr.dateTime;
        }
    }
}

void ErrorResonIO::errorReasonToFile(const QVector<ErrorResonIO::ERROR_REASON>& dataList)
{
    QFile saveData("LastData2.dat");

    if(saveData.open(QIODevice::WriteOnly) == false)
        return;

    QDataStream stream(&saveData);
    stream << dataList;
}

QVector<ErrorResonIO::ERROR_REASON> ErrorResonIO::errorReasonFromFile()
{
    QVector<ErrorResonIO::ERROR_REASON> dataList;
    QFile loadData("LastData2.dat");

    if(loadData.open(QIODevice::ReadOnly) == false)
        return dataList;

    QDataStream stream(&loadData);
    stream >> dataList;

    return dataList;
}
