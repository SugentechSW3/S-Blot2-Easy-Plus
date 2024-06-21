#ifndef GUIDELINEWIDGET_H
#define GUIDELINEWIDGET_H

#include <QWidget>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_GuideLineWidget;
class GuideLineWidget : public QWidget, public CConfig
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit GuideLineWidget(QWidget *parent = nullptr);
    virtual ~GuideLineWidget();

    void readConfigData();
    void writeConfigData();

private:
    void initVariables();
    void initSignalSlots();

private slots:
    void onClickedWrite();
    void onChangedConfig(GlobalDataStruct::GUIDE_LINE);

private:
    QScopedPointer<Ui_GuideLineWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // GUIDELINEWIDGET_H
