#include "CodeFuncDatabaseCtrl.h"

CodeFuncDatabaseCtrl::CodeFuncDatabaseCtrl(QObject *parent)
    :DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::DBCodeFunc,parent)
{

}
QMap<QString, GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT> CodeFuncDatabaseCtrl::getLotCutoffData(DataBaseCtrl *codeFuncDatabase)
{
    QMap<QString, GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT> map;
    QString tableName("LOT_OPTIONAL_TABLE");

    QString funcQuery = QString("SELECT * FROM %1").arg(tableName);
    auto lotOptionList = codeFuncDatabase->select(funcQuery);

    auto lotNumber = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::LOT_OPTIONAL_TABLE::LOT_NUMBER, tableName);
    auto lotCutoff = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::LOT_OPTIONAL_TABLE::CUT_OFF, tableName);
    auto expDate = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::LOT_OPTIONAL_TABLE::EXP_DATE, tableName);

    foreach (auto lotOption, lotOptionList)
    {
        GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT data;
        QString lotName = lotOption[lotNumber].toString();

        data.lotNumber = lotName;
        data.cutoff =  lotOption[lotCutoff].toInt();
        data.expDate =  lotOption[expDate].toString();
        map[lotName] = data;
    }
    return map;
}

QMap<QString, GlobalDataStruct::FORMULA_PARAMETER> CodeFuncDatabaseCtrl::getFuncData(DataBaseCtrl *codeFuncDatabase)
{
    QMap<QString, GlobalDataStruct::FORMULA_PARAMETER> map;

    if(codeFuncDatabase == nullptr)
        return map;

    QString funcQuery = QString("SELECT * FROM %1").arg("FUNC_TABLE");
    auto funcResult = codeFuncDatabase->select(funcQuery);


    auto funcStr = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FUNC, "FUNC_TABLE");
    auto fValueStrA = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_A, "FUNC_TABLE");
    auto fValueStrB = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_B, "FUNC_TABLE");
    auto fValueStrC = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_C, "FUNC_TABLE");
    auto fValueStrD = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_D, "FUNC_TABLE");
    auto fValueStrE = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_E, "FUNC_TABLE");
    auto fValueStrF = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_F, "FUNC_TABLE");
    auto fValueStrG = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_G, "FUNC_TABLE");

    for(int i =0; i< funcResult.count(); i++)
    {
        auto currentColumns = funcResult[i];
        auto funcName = currentColumns[funcStr].toString();

        GlobalDataStruct::FORMULA_PARAMETER parameter;
        parameter.a = currentColumns[fValueStrA].toDouble();
        parameter.b = currentColumns[fValueStrB].toDouble();
        parameter.c = currentColumns[fValueStrC].toDouble();
        parameter.d = currentColumns[fValueStrD].toDouble();
        parameter.e = currentColumns[fValueStrE].toDouble();
        parameter.f = currentColumns[fValueStrF].toDouble();
        parameter.g = currentColumns[fValueStrG].toDouble();

        map[funcName] = parameter;
    }

    return map;
}

QMap<QString, QStringList> CodeFuncDatabaseCtrl::getStripRelData(DataBaseCtrl *codeFuncDatabase)
{
    QMap<QString,QStringList> relList;

    if(codeFuncDatabase == nullptr)
        return relList;

    QString stripRelQuery = QString("SELECT * FROM %1").arg("STRIP_REL_TABLE");
    auto stripRelResult = codeFuncDatabase->select(stripRelQuery);

    QString tableName("STRIP_REL_TABLE");
    QString columnNameLotNumber = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::LOT_NUMBER, tableName);
    QString columnNameFuncList = codeFuncDatabase->convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::FUNC_LIST, tableName);

    for(auto& itr : stripRelResult)
    {
        auto tokenizedFuncList = itr[columnNameFuncList].toString();
        relList[itr[columnNameLotNumber].toString()] = tokenizedFuncList.split(',');
    }

    return relList;
}

// TDOD : 테스트 진행 해봐야함
QVector<QVariantMap> CodeFuncDatabaseCtrl::makeFuncData(const QVector<GlobalDataStruct::QR_FORMULA_DATA_STRUCT> &list)
{
    QString tableName("FUNC_TABLE");
    QVector<QVariantMap> dataMap;
    CodeFuncDatabaseCtrl dataBaseCtrl;

    QString columnNameFunc = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FUNC, tableName);
    QString columnNameFVALUE_A = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_A, tableName);
    QString columnNameFVALUE_B = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_B, tableName);
    QString columnNameFVALUE_C = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_C, tableName);
    QString columnNameFVALUE_D = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_D, tableName);
    QString columnNameFVALUE_E = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_E, tableName);
    QString columnNameFVALUE_F = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_F, tableName);
    QString columnNameFVALUE_G = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::FUNC_TABLE::FVALUE_G, tableName);

    for(auto& itr : list)
    {
        QVariantMap param;
        param[columnNameFunc] = itr.newFunctionNumber;
        param[columnNameFVALUE_A] = itr.functionDataA;
        param[columnNameFVALUE_B] = itr.functionDataB;
        param[columnNameFVALUE_C] = itr.functionDataC;
        param[columnNameFVALUE_D] = itr.functionDataD;
        param[columnNameFVALUE_E] = itr.functionDataE;
        param[columnNameFVALUE_F] = itr.functionDataF;
        param[columnNameFVALUE_G] = itr.functionDataG;

        dataMap << param;
    }

    return dataMap;
}
// TDOD : 테스트 진행 해봐야함
QVariantMap CodeFuncDatabaseCtrl::makeStripRelData(const GlobalDataStruct::QR_RELATION_DATA_STRUCT &data)
{
    QString tableName("STRIP_REL_TABLE");
    QVariantMap dataMap;
    CodeFuncDatabaseCtrl dataBaseCtrl;

    QString columnNameLotNumber = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::LOT_NUMBER, tableName);
    QString columnNameFuncList = dataBaseCtrl.convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::FUNC_LIST, tableName);

    QString parameterList;

    for(auto itr : data.bandFormulaParameter)
    {
        if(parameterList.isEmpty() == false)
            parameterList.push_back(",");

        parameterList.push_back(itr);
    }

    dataMap[columnNameLotNumber] = data.lotNumber;
    dataMap[columnNameFuncList] = parameterList;

    return dataMap;
}
