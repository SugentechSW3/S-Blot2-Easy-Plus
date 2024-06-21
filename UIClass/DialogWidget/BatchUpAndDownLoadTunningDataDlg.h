#ifndef BATCHUPANDDOWNLOADTUNNINGDATADLG_H
#define BATCHUPANDDOWNLOADTUNNINGDATADLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_BatchUpAndDownLoadTunningDataDlg;
class BatchUpAndDownLoadTunningDataDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit BatchUpAndDownLoadTunningDataDlg(QWidget *parent = nullptr);
    ~BatchUpAndDownLoadTunningDataDlg();

    QString getDirFolderName();
private:
    void initVariables();
    void initMessage();
    void initSignalSlots();

    void emptyPathErrorMsg();
    void unsupportedDirNameMsg();
    void reconnectedMessage();

private slots:
    void onStartMessage();
    void onStopMessage();

private:
    QScopedPointer<Ui_BatchUpAndDownLoadTunningDataDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // BATCHUPANDDOWNLOADTUNNINGDATADLG_H
