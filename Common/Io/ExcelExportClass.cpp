#include "ExcelExportClass.h"

#include <QModelIndex>
#include <QProcess>
#include <QThread>
#include <QDebug>

#include "Analysis/ConfigInformation.h"

#include "Common/CUtil.h"

#include "Report/ReportData.h"


USING_SUGENTECH

struct ExcelExportClass::privateStruct
{
    QVector<GlobalDataStruct::AnalysisReportInfomation> mResult;
    QList<QModelIndex> mSelectedRows;
    QString mDirPath;
    QThread *mThread = nullptr;

};

ExcelExportClass::ExcelExportClass(const QVector<GlobalDataStruct::AnalysisReportInfomation> resultData, const QList<QModelIndex> selectedRows, const QString dirPath, QObject *parent) :
                                                                                                                                                                                          QObject(parent),
                                                                                                                                                                                          d(new privateStruct)
{
    d->mResult = resultData;
    d->mSelectedRows = selectedRows;
    d->mDirPath = dirPath;
}

ExcelExportClass::~ExcelExportClass()
{
    this->stopThread();
}

void ExcelExportClass::startThread()
{
    d->mThread = new QThread(this);
    connect(d->mThread, &QThread::started, this, &ExcelExportClass::onStartExport);
    connect(this, &ExcelExportClass::onStartExport, this, &ExcelExportClass::ExportExcel);
    connect(this, &ExcelExportClass::onFinishedExport, this, &ExcelExportClass::deleteLater);

    this->moveToThread(d->mThread);
    d->mThread->start();
}

void ExcelExportClass::stopThread()
{
    if(d->mThread != nullptr)
    {
        d->mThread->terminate();
        d->mThread->wait();
        d->mThread = nullptr;
    }
}

void ExcelExportClass::ExportExcel()
{
    QStringList filePathList;

    if(d->mSelectedRows.isEmpty())
        filePathList = this->saveAllResultRowData(d->mDirPath);
    else
        filePathList = this->saveSelectedResultRowData(d->mDirPath);

    ReportData data;
    auto defaultPath = data.getDirPath();
    auto password = ConfigInformation::getInstance()->getUserPassword();

    this->compressionExcelFile(filePathList, defaultPath, d->mDirPath, password);

    emit onFinishedExport();
}

QString ExcelExportClass::saveResultRowData(const QString& dirPath, int selectedRow)
{
    if(selectedRow < 0)
        return QString();

    if(d->mResult.count() <= selectedRow)
        return QString();

    auto timeStamp = d->mResult[selectedRow].dateTime.toString("yyyy-MM-dd-hh-mm");

    ReportData data;
    data.openExcelFile();

    CUtil::createDirectory(dirPath);
    QString fileName = QString("%1/(Report-%2-%3)%4.xlsx")
                           .arg(dirPath)
                           .arg(d->mResult[selectedRow].element.patientID)
                           .arg(d->mResult[selectedRow].element.panelName)
                           .arg(timeStamp);

    data.writeExcelDataV5(d->mResult[selectedRow]);
    data.closeExcelFile(fileName);

    return fileName;
}

QStringList ExcelExportClass::saveAllResultRowData(const QString& dirPath)
{
    QStringList savedPathList;

    for(int i = 0 ; i < d->mResult.count(); i++)
        savedPathList << this->saveResultRowData(dirPath, i);

    return savedPathList;
}

QStringList ExcelExportClass::saveSelectedResultRowData(const QString& dirPath)
{
    QStringList savedPathList;
    auto selectedRows = d->mSelectedRows;

    for(auto& itr : selectedRows)
    {
        auto row = itr.row();
        savedPathList << this->saveResultRowData(dirPath, row);
    }

    return savedPathList;
}

void ExcelExportClass::compressionExcelFile(const QStringList &pathList, const QString& defaultFolderPath, const QString& zipSaveDir, const QString &password)
{
    QProcess process;
    QString zipExeFileName = defaultFolderPath + "/" + "7zr.exe"; //7zr이 있는 절대 path;
    QString zipFileName =  zipSaveDir + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".7z";

    QString cmd = QString("\"%1\" a -bb \"%2\"").arg(zipExeFileName).arg(zipFileName);

    for(auto itr : pathList)
        cmd.append(QString(" \"%1\"").arg(itr));

    cmd.append(QString(" -p%1").arg(password));

    process.start(cmd);
    process.waitForFinished(-1);

    for(auto& itr : pathList)
        QFile::remove(itr);
}


