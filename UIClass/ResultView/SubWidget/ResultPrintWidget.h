#ifndef RESULTPRINTWIDGET_H
#define RESULTPRINTWIDGET_H

#include <QWidget>
#include <QPicture>

#include "Analysis/GlobalDataStruct.h"

class Ui_ResultPrintWidget;
class ResultPrintWidget : public QWidget
{
private:
    Q_OBJECT

    struct privateStruct;
    enum TABLE_COLUMN_REPORT_RESULT_FRONT
    {
        TABLE_COLUMN_REPORT_RESULT_FRONT_NUMBER,
        TABLE_COLUMN_REPORT_RESULT_FRONT_ALLERGEN,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CODE,
        TABLE_COLUMN_REPORT_RESULT_FRONT_IU,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS1,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS2,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS3,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS4,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS5,
        TABLE_COLUMN_REPORT_RESULT_FRONT_CLASS6
    };


    enum TABLE_COLUMN_REPORT_RESULT_BACK
    {
        TABLE_COLUMN_REPORT_RESULT_BACK_NUMBER,
        TABLE_COLUMN_REPORT_RESULT_BACK_ALLERGEN,
        TABLE_COLUMN_REPORT_RESULT_BACK_CODE,
        TABLE_COLUMN_REPORT_RESULT_BACK_IU,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS1,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS2,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS3,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS4,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS5,
        TABLE_COLUMN_REPORT_RESULT_BACK_CLASS6
    };

    enum TABLE_COLUMN_SPECIFIC
    {
        TABLE_COLUMN_SPECIFIC_IU_ML,
        TABLE_COLUMN_SPECIFIC_CLASS,
        TABLE_COLUMN_SPECIFIC_LEVEL
    };

public:
    explicit ResultPrintWidget(QWidget *parent = nullptr);
    ~ResultPrintWidget();

    QPicture printWindow();
    void setData(const GlobalDataStruct::AnalysisReportInfomation &data);

    static QString UpperToLowerForPanelName(const QString& panelName);

private:
    void initTitleText();
    void createTableHeader();

    void setGeneralData(const GlobalDataStruct::AnalysisReportInfomation &data);
    void setRowData(const GlobalDataStruct::AnalysisReportInfomation& data);

    void loadHighLightData();
    bool isHighLightColor(const QString &classType);

    void setSingleRowData(const GlobalDataStruct::AnalysisReportInfomation& data);
    void setDualBandRowData(const GlobalDataStruct::AnalysisReportInfomation& data);

private:
    QScopedPointer<Ui_ResultPrintWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RESULTPRINTWIDGET_H
