#ifndef PASSWORDCHANGEDIG_H
#define PASSWORDCHANGEDIG_H

#include <QWidget>
#include "Common/CommonParentWidget/CDialogParentWidget.h"

class Ui_PasswordChangeDig;

class PasswordChangeDig : public CDialogParentWidget
{
    Q_OBJECT
    struct privateStruct;

public:
    explicit PasswordChangeDig(QWidget *parent = nullptr,bool isProgramFirst = false);
    ~PasswordChangeDig();

private:
    void init();
    void initStyleSheet();
    void initSignalSlot();

private slots:
    void onClickPasswordSave();
private:
    QScopedPointer<Ui_PasswordChangeDig> ui;
    QScopedPointer<privateStruct> d;
};

#endif // PASSWORDCHANGEDIG_H
