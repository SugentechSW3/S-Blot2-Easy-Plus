#ifndef PARTICLEREMOVALSETTINGWIDGET_H
#define PARTICLEREMOVALSETTINGWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_ParticleRemovalSettingWidget;
class ParticleRemovalSettingWidget : public QWidget, public CConfig
{
    Q_OBJECT

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
    QScopedPointer<Ui_ParticleRemovalSettingWidget> ui;
};

#endif // PARTICLEREMOVALSETTINGWIDGET_H
