#ifndef ADMINBUTTONMENU_H
#define ADMINBUTTONMENU_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_AdminButtonMenu;
class AdminButtonMenu : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit AdminButtonMenu(QWidget *parent = nullptr);
    ~AdminButtonMenu();

signals:
    void onClickedCamera(bool);
    void onClickedETC(bool);
    void onClickedSequence(bool);
    void onClickedSemiAutoView(bool);
    void onClickedConfig(bool);
    void onClickedWorkPlace(bool);
    void onClickedResultView(bool);
    void onClickedServerIPSetting(bool);
    void onClickedLanguageSelect(bool);
    void onClickedDataExchange(bool);
    void onClickedHiddenBand(bool);

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

private:
    QScopedPointer<Ui_AdminButtonMenu> ui;
};

#endif // ADMINBUTTONMENU_H
