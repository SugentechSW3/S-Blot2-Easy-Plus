#ifndef COMMONSEQUENCEWIDGET_H
#define COMMONSEQUENCEWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <QVariantMap>

class CommonSequenceWidget : public QWidget
{
private:
    Q_OBJECT

public:
    explicit CommonSequenceWidget(QWidget *parent = nullptr);
    virtual ~CommonSequenceWidget();

    virtual QVariantMap getConfigParameters() = 0;
    virtual void setConfigParameters(const QVariantMap& parameter) = 0;

};

#endif // COMMONSEQUENCEWIDGET_H
