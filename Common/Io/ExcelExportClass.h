#ifndef EXCELEXPORTCLASS_H
#define EXCELEXPORTCLASS_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"

class ExcelExportClass : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ExcelExportClass(const QVector<GlobalDataStruct::AnalysisReportInfomation> resultData, const QList<QModelIndex> selectedRows, const QString dirPath, QObject *parent = nullptr);
    ~ExcelExportClass();

    void startThread();

signals:
    void onStartExport();
    void onFinishedExport();

private:
    void stopThread();

    QString saveResultRowData(const QString& dirPath, int selectedRow);
    QStringList saveAllResultRowData(const QString& dirPath);
    QStringList saveSelectedResultRowData(const QString& dirPath);
    void compressionExcelFile(const QStringList &pathList, const QString& defaultFolderPath, const QString& zipSaveDir, const QString &password);

private slots:
    void ExportExcel();

private:
    QScopedPointer<privateStruct> d;
};

#endif // EXCELEXPORTCLASS_H
