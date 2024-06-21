#ifndef ROLLBACKWIDGET_H
#define ROLLBACKWIDGET_H

#include "CommonSequenceWidget.h"

class Ui_RollBackWidget;
class RollBackWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit RollBackWidget(QWidget* parent = nullptr);
    virtual ~RollBackWidget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);
private:
    QScopedPointer<Ui_RollBackWidget> ui;
};

#endif // ROLLBACKWIDGET_H
