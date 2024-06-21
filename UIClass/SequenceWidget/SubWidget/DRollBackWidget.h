#ifndef DROLLBACK_H
#define DROLLBACK_H

#include "CommonSequenceWidget.h"

class Ui_DRollBackWidget;
class DRollBackWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    enum SEQUENCE_D_ROLL_BACK
    {
        SEQUENCE_D_ROLL_BACK_CHANNEL,
        SEQUENCE_D_ROLL_BACK_OPTION,
        SEQUENCE_D_ROLL_BACK_DURATION
    };


    explicit DRollBackWidget(QWidget *parent = nullptr);
    virtual ~DRollBackWidget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

    int findIndex(int data);
private :
    void init();

private:
    QScopedPointer<Ui_DRollBackWidget> ui;
};

#endif // DROLLBACK_H
