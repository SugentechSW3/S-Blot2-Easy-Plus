#ifndef LISTTOHIDDENBANDDLG_H
#define LISTTOHIDDENBANDDLG_H


#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_ListToHiddenBandDlg;
class ListToHiddenBandDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ListToHiddenBandDlg(QWidget *parent = nullptr);
    ~ListToHiddenBandDlg();

    void initData();
    void initSignalSlots();

private slots:
    void loadData(const QString &contentsName);
    void saveData();

    void onClickedAdd();
    void onClickedRemove();

private:
    void allergyDataLoad();

private:
    QScopedPointer<Ui_ListToHiddenBandDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // LISTTOHIDDENBANDDLG_H
