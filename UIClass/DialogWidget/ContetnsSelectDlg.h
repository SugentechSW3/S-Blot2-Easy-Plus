#ifndef CONTETNSSELECTDLG_H
#define CONTETNSSELECTDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_ContetnsSelectDlg;
class ContetnsSelectDlg : public CDialogParentWidget
{
    Q_OBJECT

public:
    explicit ContetnsSelectDlg(QWidget *parent = nullptr);
    virtual ~ContetnsSelectDlg();

private:
    void initVariable();
    void initSignalSlots();
    void initStyleSheet();

private:
    QScopedPointer<Ui_ContetnsSelectDlg> ui;
};

#endif // CONTETNSSELECTDLG_H
