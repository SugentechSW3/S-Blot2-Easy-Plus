#ifndef LANGUAGESELECTDLG_H
#define LANGUAGESELECTDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_LanguageSelectDlg;
class LanguageSelectDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit LanguageSelectDlg(QWidget *parent = nullptr, bool isSave = true);
    virtual ~LanguageSelectDlg();

private:
    void initButton();
    void initDialog();
    void initSignalSlot();

    void infomationMessage();
private:
    QScopedPointer<Ui_LanguageSelectDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // LANGUAGESELECTDLG_H
