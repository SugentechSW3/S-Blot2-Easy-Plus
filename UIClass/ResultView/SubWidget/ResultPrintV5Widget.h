#ifndef RESULTPRINTV5WIDGET_H
#define RESULTPRINTV5WIDGET_H

#include <QWidget>
#include <QPicture>

#include "Analysis/GlobalDataStruct.h"

class Ui_ResultPrintV5Widget;
class ResultPrintV5Widget : public QWidget
{
private:
    Q_OBJECT

    struct privateStruct;
    enum TABLE_COLUMN_SPECIFIC
    {
        TABLE_COLUMN_SPECIFIC_VHEADER,
        TABLE_COLUMN_SPECIFIC_CLASS0,
        TABLE_COLUMN_SPECIFIC_CLASS1,
        TABLE_COLUMN_SPECIFIC_CLASS2,
        TABLE_COLUMN_SPECIFIC_CLASS3,
        TABLE_COLUMN_SPECIFIC_CLASS4,
        TABLE_COLUMN_SPECIFIC_CLASS5,
        TABLE_COLUMN_SPECIFIC_CLASS6
    };

    enum TABLE_COLUMN_TOTAL_IGE
    {
        TABLE_COLUMN_TOTAL_VHEADER,
        TABLE_COLUMN_TOTAL_NORMAL,
        TABLE_COLUMN_TOTAL_POSITIVE,
    };

    enum TABLE_COLUMN_MEMO
    {
        TABLE_COLUMN_MEMO_VHEADER,
        TABLE_COLUMN_MEMO_VALUE,
        TABLE_COLUMN_MEMO_ALLERGEN,
    };

    enum TABLE_COLUMN_BANDLIST_HEADER
    {
        TABLE_COLUMN_BANDLIST_HEADER_NO,
        TABLE_COLUMN_BANDLIST_HEADER_GROUP,
        TABLE_COLUMN_BANDLIST_HEADER_ALLERGEN,
        TABLE_COLUMN_BANDLIST_HEADER_CODE,
        TABLE_COLUMN_BANDLIST_HEADER_IUML,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS1,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS2,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS3,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS4,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS5,
        TABLE_COLUMN_BANDLIST_HEADER_CLASS6
    };

    enum TABLE_COLUMN_BANDLIST
    {
        TABLE_COLUMN_BANDLIST_NO,
        TABLE_COLUMN_BANDLIST_GROUP,
        TABLE_COLUMN_BANDLIST_ALLERGEN,
        TABLE_COLUMN_BANDLIST_CODE,
        TABLE_COLUMN_BANDLIST_IUML,
        TABLE_COLUMN_BANDLIST_CLASS,
        TABLE_COLUMN_BANDLIST_CLASS_BAR,
    };

public:
    explicit ResultPrintV5Widget(QWidget *parent = nullptr);
    ~ResultPrintV5Widget();

    QVector<QPicture> printWindow();
    void setData(const GlobalDataStruct::AnalysisReportInfomation &data);

    static QString UpperToLowerForPanelName(const QString& panelName);

private:
    void createPage1();
    void createPage2and3();
    void initPage2TableHeader();
    void initPage2Tablebody();
    void initPage3TableHeader();
    void initPage3Tablebody();
    void initETCTable();
    void initMemoTable();
    void writeClassMemoV5(QString tigeNote, QString ccdNote, QVector<QStringList>& classNoteItem);
    QStringList getAllergyMemoNewLine(QStringList classNoteItems);
    void mergeAllergyGroupV5(int bandCount);

    void setGeneralData(const GlobalDataStruct::AnalysisReportInfomation &data);
    void setRowData(const GlobalDataStruct::AnalysisReportInfomation& data);

private:
    QScopedPointer<Ui_ResultPrintV5Widget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RESULTPRINTV5WIDGET_H
