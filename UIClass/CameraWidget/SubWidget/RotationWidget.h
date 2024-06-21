#ifndef ROTATIONWIDGET_H
#define ROTATIONWIDGET_H

#include <QWidget>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_RotationWidget;
class RotationWidget : public QWidget, public CConfig
{
    Q_OBJECT

public:
    explicit RotationWidget(QWidget *parent = nullptr);
    ~RotationWidget();

    void readConfigData();
    void writeConfigData();

private:
    void initSignalSlots();

private:
    QScopedPointer<Ui_RotationWidget> ui;
};

#endif // ROTATIONWIDGET_H
