#ifndef CHECKLISTDLG_H
#define CHECKLISTDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>
#include "Analysis/GlobalDataStruct.h"

class Ui_CheckListDlg;
class CheckListDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CheckListDlg(QWidget *parent = nullptr);
    virtual ~CheckListDlg();

    void setText(const QString& text);
    void setCheckListSingleDataMode(const QVector<GlobalDataStruct::AnalysisReportInfomation>& checkList);
    void setCheckList(const QVector<GlobalDataStruct::AnalysisReportInfomation> &checkList);
    QVector<int> getUncheckedStripNumberList();
    void setFontSize(const int& fontsize);
    void setButtonHeight(const int& buttonHeight);
private:
    void initStyleSheet();
    void addCheckBox(const QString& checkBoxName);
    void clearCheckBox();


    int getUncheckedStripNumberFromCheckBoxText(const QString& text);
    QVector<int> getUncheckedStripForSingle();
    QVector<int> getUncheckedStripForMulti();

private slots:
    void onUpdateWidget();


private:
    QScopedPointer<Ui_CheckListDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CHECKLISTDLG_H
