#ifndef SEQUENCEVIEW_H
#define SEQUENCEVIEW_H


#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include <QTreeWidgetItem>
#include "Analysis/GlobalDataStruct.h"

class Ui_SequenceView;
class SequenceView : public CSerialNetworkWidget
{
private:
    Q_OBJECT

    struct privateStruct;
public:
    explicit SequenceView(QWidget *parent = nullptr);
    virtual ~SequenceView();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void initWidgets();

    void setComboBoxData();
    void setListData();
    QTreeWidgetItem *addStep(const QString& stepName, const uchar stepNumber);
    QTreeWidgetItem *addProcess(QTreeWidgetItem *topLevelWidget, const QString& processName, const GlobalDataStruct::PROCESS_DATA& processData);

    void stackSequenceCommand(QTreeWidget* widget, QVector<QByteArray> &sendData);

    QString findStepName(int stepNumber);
    QString findProcessName(int processNumber);
    void hideAllViews();

    void changePrime1(const QVariantMap &parameter);
    void changePrime2(const QVariantMap &parameter);
    void changePrime3(const QVariantMap &parameter);
    void changeDispense1(const QVariantMap &parameter);
    void changeDispense2(const QVariantMap &parameter);
    void changeDispenseAsp1(const QVariantMap &parameter);
    void changeDispenseAsp2(const QVariantMap &parameter);
    void changeDispenseAsp3(const QVariantMap &parameter);
    void changeIncubation(const QVariantMap &parameter);
    void changeIncubationWS(const QVariantMap &parameter);
    void changeAspration(const QVariantMap &parameter);
    void changeDry(const QVariantMap &parameter);
    void changeRollBack1(const QVariantMap &parameter);
    void changeRollBack2(const QVariantMap &parameter);
    void changeRollBack3(const QVariantMap &parameter);
    void changeDPrime(const QVariantMap &parameter);
    void changeDRollBack(const QVariantMap &parameter);
    void changeDDispense(const QVariantMap &parameter);

    QTreeWidgetItem* getCurrentTopLevelWidgetItem();
    bool isTopLevelTreeItem(QTreeWidgetItem *curItem);

    QVariant getQVariantFromProcessData(const GlobalDataStruct::PROCESS_DATA& data);
    GlobalDataStruct::PROCESS_DATA getProcessDataFromQVariant(const QVariant& data);

    void deviceAcceptAfterSave();

private slots:
    void onClickAddStep();
    void onClickAddProcess();
    void onClickChange();
    void onClickDelete();
    void onClickClear();
    void onClickSaveSequence();
    void onClickLoadSequence();
    void onAccept(bool);
    void onUpdateSelectChanged();
    void onFinishSeqeuence();
    void onContentsChanged(int);

private:
    QScopedPointer<Ui_SequenceView> ui;
    QScopedPointer<privateStruct> d;
};

#endif // SEQUENCEVIEW_H
