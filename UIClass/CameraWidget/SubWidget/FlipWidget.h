#ifndef FLIPWIDGET_H
#define FLIPWIDGET_H

#include <QWidget>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_FlipWidget;
class FlipWidget : public QWidget, public CConfig
{
    Q_OBJECT

public:
    explicit FlipWidget(QWidget *parent = nullptr);
    ~FlipWidget();

    void readConfigData();
    void writeConfigData();

private:
    void initSignalSlots();

private:
    QScopedPointer<Ui_FlipWidget> ui;
};

#endif // FLIPWIDGET_H
