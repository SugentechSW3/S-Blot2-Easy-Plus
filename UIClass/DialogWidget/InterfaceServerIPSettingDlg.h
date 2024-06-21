#ifndef INTERFACESERVERIPSETTINGDLG_H
#define INTERFACESERVERIPSETTINGDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_InterfaceServerIPSettingDlg;
class InterfaceServerIPSettingDlg : public CDialogParentWidget
{
    Q_OBJECT

public:
    explicit InterfaceServerIPSettingDlg(QWidget *parent = nullptr);
    virtual ~InterfaceServerIPSettingDlg();

    static QString getIPAddress();
    static QString getPort();
private:
    void initSignalSlots();
    void initIPAddressExp();
    void initPortExp();

    void loadData();
    void saveData();
private:
    QScopedPointer<Ui_InterfaceServerIPSettingDlg> ui;
};

#endif // INTERFACESERVERIPSETTINGDLG_H
