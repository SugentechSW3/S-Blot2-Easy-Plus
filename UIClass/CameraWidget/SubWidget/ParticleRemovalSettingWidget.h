#ifndef PARTICLEREMOVALSETTINGWIDGET_H
#define PARTICLEREMOVALSETTINGWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_ParticleRemovalSettingWidget;
class ParticleRemovalSettingWidget : public QWidget, public CConfig
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ParticleRemovalSettingWidget(QWidget *parent = nullptr);
    virtual ~ParticleRemovalSettingWidget();

    int getRemovalPaticleCount();
    void setRemovalPaticleCount(int count);

    int getAveragePaticleCount();
    void setAveragePaticleCount(int count);

    int getThreshold();
    void setThreshold(int threshold);

    virtual void readConfigData();
    virtual void writeConfigData();

private:
    void initVariables();
    void initSignalSlots();

private slots:
    void onChangedConfig(GlobalDataStruct::PATICLE_REMOVAL);

private:
    QScopedPointer<Ui_ParticleRemovalSettingWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // PARTICLEREMOVALSETTINGWIDGET_H
