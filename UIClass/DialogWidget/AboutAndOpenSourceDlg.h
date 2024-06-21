#ifndef ABOUTANDOPENSOURCEDLG_H
#define ABOUTANDOPENSOURCEDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_AboutAndOpenSourceDlg;
class AboutAndOpenSourceDlg : public CDialogParentWidget
{
    Q_OBJECT

public:
    explicit AboutAndOpenSourceDlg(QWidget *parent = nullptr);
    virtual ~AboutAndOpenSourceDlg();

private:
    void init();
    void initStyleSheet();
    void initSignalSlots();

private:
    QScopedPointer<Ui_AboutAndOpenSourceDlg> ui;
};

#endif // ABOUTANDOPENSOURCEDLG_H
