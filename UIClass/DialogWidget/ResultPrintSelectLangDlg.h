#ifndef RESULTPRINTSELECTLANGDLG_H
#define RESULTPRINTSELECTLANGDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>
#include "Analysis/GlobalDataStruct.h"

class Ui_ResultPrintSelectLangDlg;
class ResultPrintSelectLangDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ResultPrintSelectLangDlg(QWidget *parent = nullptr);
    ~ResultPrintSelectLangDlg();

private:
    void initData();
    void initStyleSheet();
    void initSignalSlots();

private:
    QScopedPointer<Ui_ResultPrintSelectLangDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RESULTPRINTSELECTLANGDLG_H
