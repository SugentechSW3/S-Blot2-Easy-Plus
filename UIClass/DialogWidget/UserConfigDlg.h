#ifndef USERCONFIGDLG_H
#define USERCONFIGDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_UserConfigDlg;
class UserConfigDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    struct UserConfigData
    {
        QString hwVersion;
        QString fwVersion;
        QString swVersion;

    };

    explicit UserConfigDlg(UserConfigData initData, QWidget *parent = nullptr);
    virtual ~UserConfigDlg();

private:
    void initVariable();
    void initSignalSlots();
    void initVersion(const UserConfigData &initData);
    void initAuthority();

    void saveConfigData();
    void loadConfigData();

    void setCheckBoxEnable(int startPos);
    void allCheckBoxDisable();

    bool isUnCheckLastClass();

    void saveHighLightClass();
    void loadHighLightClass();

    void saveUseLowLevelDataConfig();
    void loadUseLowLevelDataConfig();

    void savePCCutOff();
    void loadPCCutOff();

    void saveTigeCutOff();
    void loadTigeCutOff();

    void readASPBathPosition();

    void savetIgEZeroConditionSetting();
    void loadtIgEZeroConditionSetting();

    void saveBackgroundCutoff();
    void loadBackgroundCutoff();

    void saveUnmarkIUML();
    void loadUnmarkIUML();

    void saveUseXML();
    void loadUseXML();

    void saveDisableDBTigECutoff();
    void loadDisableDBTigECutoff();

private slots:
    void onSaveConfigData();

    void onClickHighlightClass0();
    void onClickHighlightClass1();
    void onClickHighlightClass2();
    void onClickHighlightClass3();
    void onClickHighlightClass4();
    void onClickHighlightClass5();
    void onClickHighlightClass6();

    void onAboutPopupMenu(const QPoint& point);
    void onShowAboutDlg();

    void onClickASPHome();
    void onClickASPBath();

    void onClickLogoFileOpen();

    void onRecvData(QByteArray data);

private:
    QScopedPointer<Ui_UserConfigDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // USERCONFIGDLG_H
