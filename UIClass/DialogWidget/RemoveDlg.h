#ifndef REMOVEDLG_H
#define REMOVEDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>
#include "Analysis/GlobalDataStruct.h"

class Ui_RemoveDlg;
class RemoveDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    enum REMOVE_DIALOG_SELECLECTION_MODE
    {
        REMOVE_DIALOG_SELECLECTION_MODE_SELECTED_ROW,
        REMOVE_DIALOG_SELECLECTION_MODE_CONDITION
    };

    explicit RemoveDlg(QWidget *parent = nullptr);
    virtual ~RemoveDlg();

    RemoveDlg::REMOVE_DIALOG_SELECLECTION_MODE getSelectedMode();
    GlobalDataStruct::QUERY_CONDITION_DATA getData() const;

private:
    void initData();
    void initButton();
    bool conditionProcess();

private slots:
    void onClickOK();

private:
    QScopedPointer<Ui_RemoveDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // REMOVEDLG_H
