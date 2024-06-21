#ifndef ASPIRATIONWIDGET_H
#define ASPIRATIONWIDGET_H

#include "CommonSequenceWidget.h"
#include <QScopedPointer>

class Ui_AspirationWidget;
class AspirationWidget : public CommonSequenceWidget
{
    Q_OBJECT

public:
    explicit AspirationWidget(QWidget *parent = nullptr);
    virtual ~AspirationWidget();

    QVariantMap getConfigParameters();
    void setConfigParameters(const QVariantMap &parameter);

private:
    QScopedPointer<Ui_AspirationWidget> ui;
};

#endif // ASPIRATIONWIDGET_H
