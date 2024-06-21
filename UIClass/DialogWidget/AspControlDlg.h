#ifndef ASPCONTROLDLG_H
#define ASPCONTROLDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_AspControlDlg;
class AspControlDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit AspControlDlg(QWidget *parent = nullptr);
    virtual ~AspControlDlg();

    int getStripCount();
private:
    void init();
    void initStyleSheet();
private slots:
    void onClickAspFunc();

private:
    QScopedPointer<Ui_AspControlDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif
