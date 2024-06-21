#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_LoginDialog;
class LoginDialog : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:

    enum LOGIN_MODE
    {
        LOGIN_MODE_USER,
        LOGIN_MODE_ADMIN
    };

    explicit LoginDialog(bool isContentsValid, bool isSerialConnection, bool isSequencing,
                         QStringList errorList, QWidget *parent = nullptr);
    virtual ~LoginDialog();

    bool isAdministator();
    bool isSuccessPassword();

    LoginDialog::LOGIN_MODE getMode();


private:
    void initVariable();
    void initSignalSlot();
    void initStyleSheet();

    void createdPasswordInputWidget();
    void removePasswordInputWidget();

    bool isFirstStartProgram();
    bool isUserSuccessPassword();

private slots:
    void onClickedOK();
    void onClickedCancel();

    void onCurrentComboIndexChanged(int idx);

private:
    QScopedPointer<Ui_LoginDialog> ui;
    QScopedPointer<privateStruct> d;
};

#endif // LOGINDIALOG_H
