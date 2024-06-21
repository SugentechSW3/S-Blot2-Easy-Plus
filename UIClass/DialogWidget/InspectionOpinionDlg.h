#ifndef INSPECTIONOPINIONDLG_H
#define INSPECTIONOPINIONDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>
#include <Analysis/GlobalDataStruct.h>

class Ui_InspectionOpinionDlg;
class InspectionOpinionDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

    enum OPINION_TABLE_COLUMN
    {
        OPINION_TABLE_COLUMN_TITLE,
        OPINION_TABLE_COLUMN_OPINION
    };

public:
    explicit InspectionOpinionDlg(QWidget *parent = nullptr);
    virtual ~InspectionOpinionDlg();

    static QVector<GlobalDataStruct::OPINION_DATA> getOpinionData();
    static QString findOpinionText(const QString &title);

private:
    void initTableWidget();
    void initSignalSlot();
    void initStyleSheet();

    void setOpinionData();
    void dataToTable();
    void dataFromTable();
    void removeSelectedRowData();

private slots:
    void onSaveData();
    void onLoadData();
    void onClickOK();
    void onClickRemove();
    void onClickCancel();


private:
    QScopedPointer<Ui_InspectionOpinionDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // INSPECTIONOPINIONDLG_H
