#ifndef DDISPENSE_H
#define DDISPENSE_H

#include "CommonSequenceWidget.h"

class Ui_DDispenseWidget;
class DDispenseWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit DDispenseWidget(QWidget *parent = nullptr);
    virtual ~DDispenseWidget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

    int findIndex(int data);
private :
    void init();

private:
    QScopedPointer<Ui_DDispenseWidget> ui;
};

#endif // DDISPENSE_H
