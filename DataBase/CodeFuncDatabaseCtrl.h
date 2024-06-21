#ifndef CODEFUNCDATABASECTRL_H
#define CODEFUNCDATABASECTRL_H

#include "DataBase/DataBaseCtrl.h"
class CodeFuncDatabaseCtrl : public DataBaseCtrl
{
public:
    explicit CodeFuncDatabaseCtrl(QObject* parent = nullptr);
    virtual ~CodeFuncDatabaseCtrl() = default;

    static QMap<QString,GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT> getLotCutoffData(DataBaseCtrl* codeFuncDatabase);

    static QMap<QString, GlobalDataStruct::FORMULA_PARAMETER> getFuncData(DataBaseCtrl* codeFuncDatabase);
    static QMap<QString,QStringList> getStripRelData(DataBaseCtrl* codeFuncDatabase);

    static QVector<QVariantMap> makeFuncData(const QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT>& list);
    static QVariantMap makeStripRelData(const GlobalDataStruct::QR_RELATION_DATA_STRUCT &data);
};

#endif // CODEFUNCDATABASECTRL_H
