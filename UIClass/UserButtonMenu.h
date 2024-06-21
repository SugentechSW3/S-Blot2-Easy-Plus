#ifndef USERBUTTONMENU_H
#define USERBUTTONMENU_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_UserButtonMenu;
class UserButtonMenu : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit UserButtonMenu(QWidget *parent = nullptr);
    ~UserButtonMenu();

    void setOfflineMode();

signals:
    void onClickedWorkPlace(bool);
    void onClickedResultView(bool);
    void onClickedReset(bool);
    void onClickedConfigure(bool);
    void onClickedAutoClean(bool);
    void onClickedContentsChange(bool);
    void onClickedStripScan(bool);
    void onClickedAspiration(bool);

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void initStyleSheet();

private:
    QScopedPointer<Ui_UserButtonMenu> ui;
};

#endif // USERBUTTONMENU_H
