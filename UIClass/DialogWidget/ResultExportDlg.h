#ifndef RESULTEXPORTDLG_H
#define RESULTEXPORTDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_ResultExportDlg;
class ResultExportDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ResultExportDlg(QWidget *parent = nullptr);
    virtual ~ResultExportDlg();

    QString getPath();

private:
    void initData();
    void initVariables();
    void initStyleSheet();
    void initSignalSlots();

    void loadData();
    void saveData();

    void setupPathForPathLabel();
    void emptyPathErrorMsg();
private:
    QScopedPointer<Ui_ResultExportDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RESULTEXPORTDLG_H
