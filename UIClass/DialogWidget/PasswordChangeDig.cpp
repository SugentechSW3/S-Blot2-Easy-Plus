#include "PasswordChangeDig.h"
#include "ui_PasswordChangeDig.h"

#include <QLineEdit>

#include "Common/CUtil.h"
#include "Common/Io/INIFileControls/UserLoginDataControls.h"
#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct PasswordChangeDig::privateStruct
{
    bool misFirstProgram;
};

PasswordChangeDig::PasswordChangeDig(QWidget *parent,bool isProgramFirst) :
    CDialogParentWidget(parent),
    ui(new Ui_PasswordChangeDig),d(new privateStruct)
{
    ui->setupUi(this);
    d->misFirstProgram = isProgramFirst;
    this->init();
}

PasswordChangeDig::~PasswordChangeDig()
{

}

void PasswordChangeDig::init()
{
    this->initStyleSheet();
    this->initSignalSlot();
}

void PasswordChangeDig::initStyleSheet()
{
   QLineEdit* line[] = {ui->edtPassword, ui->edtConfirmPassword};
   QIcon passwordIcon(":/Image/Image/Dialog/lock.png");

   for(auto& itr : line)
   {
       itr->setEchoMode(QLineEdit::Password);
       itr->setClearButtonEnabled(true);
       itr->addAction(passwordIcon,QLineEdit::TrailingPosition);
       itr->setAlignment(Qt::AlignRight);
   }

   if(d->misFirstProgram == true)
       ui->btnCancel->setVisible(false);
   else
       ui->btnCancel->setVisible(true);
}

void PasswordChangeDig::initSignalSlot()
{
    connect(ui->btnOKPassword,&QPushButton::clicked,this,&PasswordChangeDig::onClickPasswordSave);
    connect(ui->btnCancel,&QPushButton::clicked,this,&PasswordChangeDig::reject);
}


void PasswordChangeDig::onClickPasswordSave()
{
    QString password = ui->edtPassword->text();
    QString confirmpassword = ui->edtConfirmPassword->text();

    if(password.isEmpty() == true)
    {
       CUtil::messageBox(tr("Write the password"), this);
       return;
    }

    if(password.compare(confirmpassword,Qt::CaseInsensitive) == 0)
    {
        auto sha256_password = CUtil::convertSHA256(password);

        UserLoginDataControls ini;
        ini.writePassword(sha256_password);
        ConfigInformation::getInstance()->setUserPassword(password);
        this->accept();

    }else
    {
        this->lower();
        CUtil::messageBox(tr("Password Is UnMatched"), this);
    }
}
