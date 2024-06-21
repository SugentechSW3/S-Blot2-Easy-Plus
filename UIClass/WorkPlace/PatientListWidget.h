#ifndef PATIENTLISTWIDGET_H
#define PATIENTLISTWIDGET_H

#include <QDateTime>
#include <QModelIndexList>

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include "Common/Network/HL7InterFaceHeader.h"
#include "Common/Network/U2BioXmlLisProtocol.h"
#include "Analysis/GlobalDataStruct.h"

class Ui_PatientListWidget;
class PatientListWidget : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

    enum PATIENT_TABLE_COLUMN
    {
        PATIENT_TABLE_COLUMN_ID,
        PATIENT_TABLE_COLUMN_PATIENT_NAME,
        PATIENT_TABLE_COLUMN_PANEL
    };

public:

    explicit PatientListWidget(QWidget *parent = nullptr);
    virtual ~PatientListWidget();

    bool isSelectedStripScan();
    void savePatientListData();
    void setForceStripScanRadioButton();
    QString getStartMode();

public slots:
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> getData();
    void setData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data);

private:
    void initVariables() override;
    void initSignalSlots() override;
    void startWidget() override;
    void stopWidget() override;
    void initStyleSheet();
    void initEditItem();
    void initTableItem();
    void patientTableOrganization();
    void initValidator();
    void initVerticalHeader();

    void moveNextTableWidgetItem(int row, int column);

    void copyToTableItem(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data);

    void reAnalyzeLastFailureStripProcess();
    bool checkLastFailureStripData();
    void confirmFailureStripData();

    void drawingColorForAnalysisFailItems();
    void disableUIButtonsForRescanning();

    bool changePanelDataForSingle(int row, const QString &panel);
    bool changePanelDataForBatch(const QString &panel);
    void changePanelForSingleControlling(int row, const QString& panel);
    void changePanelForBatchControlling(const QString &panel);

    int findNextEmptyRow();

    void copyInspectorData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> &data);

    void updateLISData(const HL7_ORDER_DATA& data);

    void getSelectedData();
    void setSelectedData();

    int getEmptyRowCount();
    int getSelectedPanelCount();

    void setPatientTableHeaderSize();

    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> createCopiedData();
    QMap<int, QString> createPatientIDList();

    bool isEmptyID(int currentRow);
    bool isEmptyIDAndUnEmptyName(int currentRow);
    bool isEmptyIDAndEmptyName(int currentRow);
    bool isAllEmptyData();
    bool isAllEmptyID();

    void createUUID(int currentRow);

    void showLISWaitMessage();
    void hideLISWaitMessage();

    int findRowHasPatientID(const QString& patientID);
    void updateLISData(const QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA> &data);

private slots:
    void onClickedSave();
    void onClickedLoad();
    void onClickedReset();
    void onRemoveRow(int row);
    void onClickRemoveRows();
    void onClickedInterLockingLIS();
    void onClickedAutoID();

    void onSyncItem();
    void onItemUpdate();
    void onClickedSelectSequence(bool);
    void onClearEdit();
    void onItemCellChanged(int row, int column);
    void onKeyPressedPatientTableWidgetItem(int row, int column, int qtKey);
    void onSpinBoxActivated(int row, int column);
    void onSpinMoveHelper(int qtKey);
    void onComboBoxItemActivated(int row);

    void onEditNameChanged(QString text);
    void onEditSexChanged();
    void onEditAgeChanged(QString text);
    void onEditPhoneChanged(QString text);
    void onEditEmailChanged(QString text);

    void onCopyShortCutData();
    void onPasteShortCutData();

    void onCheckedFixedApplicant(bool isChecked);
    void onCheckedFixedAnalyst(bool isChecked);
    void onCheckedFixedReviewer(bool isChecked);

    void onBatchChangeForPanel();

    void onTableWidgetPopupMenu(const QPoint &point);

    void onDisableTopLevelWidget();
    void onEnableTopLevelWidget();

    virtual bool event(QEvent *event) override;


private:
    QScopedPointer<Ui_PatientListWidget> ui;
    QScopedPointer<privateStruct> d;
    int mUserSeq;
};

#endif // PATIENTLISTWIDGET_H
