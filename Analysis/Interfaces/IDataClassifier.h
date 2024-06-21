#ifndef IDATACLASSIFIER_H
#define IDATACLASSIFIER_H

#include <Analysis/GlobalDataStruct.h>

#include <DataBase/DataBaseCtrl.h>
#include <QObject>

class IDataClassifier : public QObject
{
public:
    explicit IDataClassifier(QObject* parent = nullptr)
        :QObject(parent){}
    virtual ~IDataClassifier() = default;

    virtual bool calcClassData(QVector<GlobalDataStruct::AnalysisReportInfomation>& element,
                                          DataBaseCtrl* codeFuncDatabase) = 0;

};
#endif // IDATACLASSIFIER_H
