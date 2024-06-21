#ifndef CONTETNSSELECTDLG_H
#define CONTETNSSELECTDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_ContetnsSelectDlg;
class ContetnsSelectDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ContetnsSelectDlg(QWidget *parent = nullptr);
    virtual ~ContetnsSelectDlg();

private:
    void initVariable();
    void initSignalSlots();
    void initStyleSheet();

    void sequenceUpdate(const QString& contentsName);

    void disableButton();

private slots:
    void onFinishSeqeuence();

private:
    QScopedPointer<Ui_ContetnsSelectDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CONTETNSSELECTDLG_H
