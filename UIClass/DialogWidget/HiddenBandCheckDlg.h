#ifndef HIDDENBANDCHECKDLG_H
#define HIDDENBANDCHECKDLG_H

#include <QWidget>

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_HiddenBandCheckDlg;

class HiddenBandCheckDlg : public CDialogParentWidget
{
    Q_OBJECT
    struct privateStruct;
public:
    explicit HiddenBandCheckDlg(QWidget *parent = nullptr);
    ~HiddenBandCheckDlg();

private:
    void initVariables();
    void initSignalSlots();
    void initDrawUI();
    void initLoadData();
    void init();

    bool isCodeCheckHiddenCode(const QString& code);
private slots:
    void onClickHiddenBandSettingSave();
private:
    QScopedPointer<Ui_HiddenBandCheckDlg> ui;
    QScopedPointer<privateStruct>d;
};

#endif // HIDDENBANDCHECKDLG_H
