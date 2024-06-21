#ifndef DPRIME_H
#define DPRIME_H

#include "CommonSequenceWidget.h"

class Ui_DPrimeWidget;
class DPrimeWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    enum SEQUENCE_D_PRIME
    {
        SEQUENCE_D_PRIME_CHANNEL,
        SEQUENCE_D_OPTION,
        SEQUENCE_D_VOLUME
    };

    explicit DPrimeWidget(QWidget *parent = nullptr);
    virtual ~DPrimeWidget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

    int findIndex(int data);
private :
    void init();

private:
    QScopedPointer<Ui_DPrimeWidget> ui;
};

#endif // DPRIME_H
