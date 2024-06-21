#ifndef USERSEQUENCEDIALOG_H
#define USERSEQUENCEDIALOG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_UserSequenceDialog;
class UserSequenceDialog : public CDialogParentWidget
{
    Q_OBJECT
    struct privateStruct;
public:
    explicit UserSequenceDialog(int currentSequence, QWidget *parent = nullptr);
    virtual ~UserSequenceDialog();


    enum USER_SEQUENCE
    {
        USER_SEQUENCE_NONE = 0x00,
        USER_SEQUENCE_DISPENSE_SPECIMEN_INCUBATE,
        USER_SEQUENCE_DISPENSE_SPECIMEN_SAMPLE,
        USER_SEQUENCE_WASHING_1,
        USER_SEQUENCE_DISPENSE_ANTIBODY,
        USER_SEQUENCE_WASHING_2,
        USER_SEQUENCE_DISPENSE_ENZYME,
        USER_SEQUENCE_DW_WASHING_3,
        USER_SEQUENCE_DISPENSE_SUBSTRATE,
        USER_SEQUENCE_DISPENSE_STOP_SOLUTION,
        USER_SEQUENCE_DRY,
        USER_SEQUENCE_ANALYST
    };


    void setCurrentIdx(int idx);
    int getCurrentIdx();

private:
    void initVariable();
    void initSignalSlots();
    void initStyleSheet();
    void initRadioButton();

    void onEnableStep(int begin);
private:
    QScopedPointer<Ui_UserSequenceDialog> ui;
    QScopedPointer<privateStruct> d;
};

#endif // USERSEQUENCEDIALOG_H
