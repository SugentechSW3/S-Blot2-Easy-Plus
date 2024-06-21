#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QModelIndexList>
#include "Analysis/GlobalDataStruct.h"
#include "Common/CommonParentWidget/CSerialNetworkWidget.h"
#include "Common/Network/HL7InterFaceHeader.h"

class QTableWidgetItem;
class Ui_ResultView;
class ResultView : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ResultView(QWidget *parent = nullptr);
    virtual ~ResultView();

    enum RESULT_TABLE_RESULT_COLUMN
    {
        RESULT_TABLE_RESULT_COLUMN_ITEM_NAME,
        RESULT_TABLE_RESULT_COLUMN_INTENSITY,
        RESULT_TABLE_RESULT_COLUMN_CLASS
    };


    enum PATIENT_TABLE_RESULT_COLUMN
    {
        PATIENT_TABLE_RESULT_COLUMN_STRIP_NUMBER,
        PATIENT_TABLE_RESULT_COLUMN_ID,
        PATIENT_TABLE_RESULT_COLUMN_NAME,
        PATIENT_TABLE_RESULT_COLUMN_PANEL,
        PATIENT_TABLE_RESULT_COLUMN_TEST_DATE
    };

signals:
    void onResultViewStarted();

public slots:
    void onResultShow(QVector<GlobalDataStruct::AnalysisReportInfomation> resultInfomation);

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void initStyleSheet();
    void initWaitMessageDialog();
    void initAction();

    void setupTableWidgetHeader();
    void setupAlignRule();
    void cleanWidgets();
    void loadHighLightData();

    void loadUseXML();

    QVector<GlobalDataStruct::AnalysisReportInfomation> setUpElementData(const QString &query);
    QString makeSearchQuery(const GlobalDataStruct::QUERY_CONDITION_DATA &dataStruct);
    QString makeRemoveQuery(const GlobalDataStruct::QUERY_CONDITION_DATA& dataStruct);
    QString makeLastDataQuery();

    QString saveResultRowData(const QString& dirPath, int selectedRow);
    QStringList saveAllResultRowData(const QString& dirPath);
    QStringList saveSelectedResultRowData(const QString& dirPath);

    bool isHighLightColor(const QString &classType);
    bool isSampleMissing(const QString& errorReason);

    void deletingDBDataYearAgo();
    void deleteConditionDBData(const GlobalDataStruct::QUERY_CONDITION_DATA &conditionData);

    bool removeSelectedRows();
    void removeDBDataForSelectedOption();

    void removeImageDataFromFullPath(const QVector<QString>& pathList);
    void removeSelectedDBAndUpdateResultVector(const QModelIndexList &selectedRows);

    void loadLastQueryData();

    void clearStripResultTable();
    void setupResultStripDataForSingleRow(int row);
    void setSingleBandData(int row);
    void setMultiBandData(int row);

    QVector<QString> findFromDBForImageDataPathForRemove(const GlobalDataStruct::QUERY_CONDITION_DATA &conditionData);
    QVector<QString> findFromDBForImageDataPathForRemove(const QModelIndexList &selectedRows);

    void saveExportDataForUserSelectPath();
    void updateCommentData(const int &row);

    void setBackGroundYellowColor(QTableWidgetItem* item);
    void setBackGroundOrangeColor(QTableWidgetItem* item);
    void setToolTipMessage(QTableWidgetItem* item, const QString &message);

    void sendResultToLIS();
    void saveXmlOutData();
    void showLISWaitMessage();
    void hideLISWaitMessage();

    void showExportWaitMessage();
    void hideExportWaitMessage();

    void showAdminUI();
    void hideAdminUI();

    QByteArray getResultToLISProtocol();
    HL7_MSH_DATA getResultMSHData();
    HL7_PID_DATA getResultPIDData(int idx);
    HL7_OBR_DATA getResultOBRData(int idx);
    HL7_OBX_DATA getResultOBXData(int idx);

    QString getWaitTitleMessageString();
    QString getExportWaitMessageString();
    QString getPrintWaitMessageString();

    static bool sortStripNumberData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b);
    static bool sortIDData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b);
    static bool sortNameData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b);
    static bool sortPanelData(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b);
    static bool sortTestDate(GlobalDataStruct::AnalysisReportInfomation& a, GlobalDataStruct::AnalysisReportInfomation& b);

    void changeEvent(QEvent* event);

private slots:
    void onUpdateComment();

    void onClickedSearch();
    void onClickedRemove();
    void onClickClean();
    void onClickedExport();
    void onClickedPrint();
    void onClickTest();
    void onClickTest2();
    void selectedTest1ReportOut();
    void selectedTest2ReportOut();
    void onClickLISInterLocking();

    void onUpdateSort(int column);
    void onUpdatePatientList();
    void onChangedPatientRow();

    void onLoadRecentData();

    void onDisableTopLevelWidget();
    void onEnableTopLevelWidget();

    void onPrintProcessing();

private:
    QScopedPointer<Ui_ResultView> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RESULTVIEW_H
