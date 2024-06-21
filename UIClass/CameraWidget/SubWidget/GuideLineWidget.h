#ifndef GUIDELINEWIDGET_H
#define GUIDELINEWIDGET_H

#include <QWidget>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_GuideLineWidget;
class GuideLineWidget : public QWidget, public CConfig
{
    Q_OBJECT

public:
    explicit GuideLineWidget(QWidget *parent = nullptr);
    virtual ~GuideLineWidget();

    void readConfigData();
    void writeConfigData();

private:
    void initSignalSlots();

private slots:
    void onClickedWrite();

private:
    QScopedPointer<Ui_GuideLineWidget> ui;
};

#endif // GUIDELINEWIDGET_H
