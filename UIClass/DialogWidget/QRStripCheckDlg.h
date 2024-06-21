#ifndef QRSTRIPCHECKDLG_H
#define QRSTRIPCHECKDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>
#include <Analysis/GlobalDataStruct.h>

class QLineEdit;
class Ui_QRStripCheckDlg;
class QRStripCheckDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit QRStripCheckDlg(QWidget *parent = nullptr);
    ~QRStripCheckDlg();

    void setMissingQRList(const QVector<int>& missingStripQRPositionList);
    void setAnalysisData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> &element);

    QMap<int, QString> getInputedLotData();

private:
    void initVariables();
    void initStyleSheet();
    void initSignalSlots();

    bool missingDataCheck();
    bool validDataCheck();
    bool validPanelCheck();
    GlobalDataStruct::TABLE_ITEM_ELEMENT findTableElement(int stripNumber);

private slots:
    void onUpdateLotData();

private:
    QScopedPointer<Ui_QRStripCheckDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // QRSTRIPCHECKDLG_H
