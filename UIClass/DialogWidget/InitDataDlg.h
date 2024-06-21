#ifndef INITDATADLG_H
#define INITDATADLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_InitDataDlg;
class InitDataDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit InitDataDlg(QWidget *parent = nullptr);
    virtual ~InitDataDlg();


    void getData(bool &initContentsData, bool& isSequenceing, QStringList& errorList);

private:
    void initTimerSettings();
    void initStyleSheet();
    void initSignalSlots();

    bool start();
    void sendHeartBeat();

    void stopDeviceErrorStateForStopTimer();

private:
    QScopedPointer<Ui_InitDataDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // INITDATADLG_H
