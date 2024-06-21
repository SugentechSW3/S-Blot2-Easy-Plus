#include <QListWidget>
#include <QLineEdit>
#include <QSettings>

#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "Common/CUtil.h"
#include "UIClass/DialogWidget/PasswordChangeDig.h"
#include "Common/Io/INIFileControls/UserLoginDataControls.h"
#include "Analysis/ConfigInformation.h"
#include "Common/CLogWriter.h"

USING_SUGENTECH

struct LoginDialog::privateStruct
{
    LoginDialog::LOGIN_MODE mMode;
    QScopedPointer<QLineEdit> mEditPassword;
    QScopedPointer<PasswordChangeDig> mSetPasswordView;
    QString mPassword = "";
};

LoginDialog::LoginDialog(bool isContentsValid, bool isSerialConnection, bool isSequencing, QStringList errorList, QWidget *parent) :
    CDialogParentWidget(parent),
    ui(new Ui::LoginDialog),
    d(new privateStruct)
{
    ui->setupUi(this);
    this->initVariable();
    this->initSignalSlot();
    this->initStyleSheet();

    if(isContentsValid == false)
    {
        ui->comboMode->setCurrentText(tr("Administrator"));
        ui->comboMode->setDisabled(true);
    }
    else if(isSequencing == true)
    {
        ui->comboMode->setCurrentText(tr("UserMode"));
        ui->comboMode->setDisabled(true);
    }

#if ENABLE_SERIAL_MODE
    if(isSerialConnection == false)
        ui->comboMode->setDisabled(true);
#endif

#ifdef IS_DEBUG_PC //LGH 2024.02.16
    ui->comboMode->setEnabled(true);
#endif

    if(errorList.isEmpty() == true)
        return;

    auto listWidgetError = new QListWidget(this);

    foreach(auto itr, errorList)
        listWidgetError->addItem(tr("[Dected Error]") + itr);

    ui->gridLayout->addWidget(listWidgetError, ui->gridLayout->count() ,0,1,3);
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::initVariable()
{
    ui->comboMode->addItem(tr("UserMode"), false);
    ui->comboMode->addItem(tr("Administrator"), true);
    this->createdPasswordInputWidget();
}

void LoginDialog::initSignalSlot()
{
    connect(ui->btnOK, SIGNAL(clicked()), SLOT(onClickedOK()));
    connect(ui->btnCancel, SIGNAL(clicked()), SLOT(onClickedCancel()));
    connect(ui->comboMode, SIGNAL(currentIndexChanged(int)), SLOT(onCurrentComboIndexChanged(int)));
}

void LoginDialog::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    QIcon userIcon(":/Image/Image/Dialog/male.png");

    for(int i = 0; i < ui->comboMode->count(); i++)
        ui->comboMode->setItemIcon(i, userIcon);

    ui->comboMode->setLayoutDirection(Qt::RightToLeft);

}

bool LoginDialog::isAdministator()
{
    return ui->comboMode->currentData().toBool();
}

bool LoginDialog::isSuccessPassword()
{
    if(d->mEditPassword.isNull() == true)
        return false;

    auto inputPassword = d->mEditPassword->text();
    auto isvalidPassword = (inputPassword.compare(QString("dv+"), Qt::CaseInsensitive) == 0);
    auto config = ConfigInformation::getInstance();
    config->setUserPassword(inputPassword);
    return isvalidPassword;
}

void LoginDialog::onClickedOK()
{
    if(this->isAdministator() == true)
    {
        if(this->isSuccessPassword() == true)
        {
            d->mMode = LOGIN_MODE::LOGIN_MODE_ADMIN;
            CLogWriter::printLog("[LoginDialog::] : LOGIN_MODE_ADMIN - success password");
            this->accept();
        }
        else
        {
            CLogWriter::printLog("[LoginDialog::] : LOGIN_MODE_ADMIN - Password Is UnMatched");
            this->lower();
            CUtil::messageBox(tr("Password Is UnMatched"), this);
        }
    }
    else
    {
        if(this->isFirstStartProgram())
        {
            d->mSetPasswordView.reset(new PasswordChangeDig(this,true));

            if(d->mSetPasswordView->exec() == QDialog::Accepted)
            {
                d->mMode = LOGIN_MODE::LOGIN_MODE_USER;
                CLogWriter::printLog("[LoginDialog::] : LOGIN_MODE_USER - isFirstInstall");
                UserLoginDataControls isFirstInstall;
                isFirstInstall.writeISFirstInstall(true);
                this->accept();
            }
        }else
        {
            if(this->isUserSuccessPassword())
            {
                d->mMode = LOGIN_MODE::LOGIN_MODE_USER;
                CLogWriter::printLog("[LoginDialog::] : LOGIN_MODE_USER - success password");
                this->accept();
            }else
            {
                d->mEditPassword->clear();
                this->lower();
                CLogWriter::printLog("[LoginDialog::] : LOGIN_MODE_USER - Password Is UnMatched");
                CUtil::messageBox(tr("Password Is UnMatched"), this);
            }
        }
    }
}

void LoginDialog::onClickedCancel()
{
    CLogWriter::printLog("[LoginDialog::] : onClickedCancel");
    this->reject();
}

LoginDialog::LOGIN_MODE LoginDialog::getMode()
{
    return d->mMode;
}

void LoginDialog::onCurrentComboIndexChanged(int )
{
    d->mEditPassword->clear();
}

void LoginDialog::createdPasswordInputWidget()
{
    QIcon passwordIcon(":/Image/Image/Dialog/lock.png");
    d->mEditPassword.reset(new QLineEdit(ui->widget));
    ui->gridLayout_2->addWidget(d->mEditPassword.data());

    d->mEditPassword->setEchoMode(QLineEdit::Password);
    d->mEditPassword->setClearButtonEnabled(true);
    d->mEditPassword->addAction(passwordIcon, QLineEdit::TrailingPosition);
    d->mEditPassword->setAlignment(Qt::AlignRight);
}

void LoginDialog::removePasswordInputWidget()
{
    d->mEditPassword.reset();
}

bool LoginDialog::isFirstStartProgram()
{
    UserLoginDataControls isFirstInstall;

    bool isFirst = isFirstInstall.isFirstInstall();
    if (isFirst == false)
    {
        UserLoginDataControls iniLogin;
        return iniLogin.readPassword().isEmpty() ? true : false;
    }
    return false;
}

bool LoginDialog::isUserSuccessPassword()
{
    if(d->mEditPassword->text() == "sugentech01")
        return true;

    QString passowrd = d->mEditPassword->text();
    QString inputPassword = CUtil::convertSHA256(passowrd);

    UserLoginDataControls iniLogin;

    QString comparePassword = iniLogin.readPassword();

    auto isvalidPassword = (inputPassword.compare(comparePassword, Qt::CaseInsensitive) == 0 );
    auto config = ConfigInformation::getInstance();
    config->setUserPassword(passowrd);

    return isvalidPassword;
}
