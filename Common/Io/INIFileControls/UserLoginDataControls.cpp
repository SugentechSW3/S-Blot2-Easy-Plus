#include "UserLoginDataControls.h"

#include "Common/Io/CSettingIo.h"
#include "Common/CUtil.h"

USING_SUGENTECH

struct UserLoginDataControls::privateStruct
{
    QScopedPointer<CSettingIo> mUserLogeinSettingIo;
    const QString mUserLoginSettingFileName = "USER_LOGIN_DATA";
    const QString mDirPath ="USER_DATA";
};

UserLoginDataControls::UserLoginDataControls(QObject *parent):
    QObject(parent),d(new privateStruct)
{
    this->initIo();
}

UserLoginDataControls::~UserLoginDataControls()
{

}

void UserLoginDataControls::writePassword(QString password)
{
    d->mUserLogeinSettingIo->setConfigureData("LOGIN","PASSWORD",password);
}

QString UserLoginDataControls::readPassword()
{
    return d->mUserLogeinSettingIo->getConfigureData("LOGIN","PASSWORD").toString();
}

void UserLoginDataControls::writeISFirstInstall(bool isFirst)
{
    d->mUserLogeinSettingIo->setConfigureData("INSTALL","FIRST",isFirst);
}

bool UserLoginDataControls::isFirstInstall()
{
    return d->mUserLogeinSettingIo->getConfigureData("INSTALL","FIRST").toBool();
}

void UserLoginDataControls::initIo()
{
    d->mUserLogeinSettingIo.reset(new CSettingIo(d->mUserLoginSettingFileName,d->mDirPath));
}
