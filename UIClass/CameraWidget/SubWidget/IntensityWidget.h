#ifndef INTENSITYWIDGET_H
#define INTENSITYWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_IntensityWidget;
class IntensityWidget : public QWidget, public CConfig
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit IntensityWidget(QWidget *parent = nullptr);
    virtual ~IntensityWidget();

    int getParticle();
    void setParticle(int value);

    int getBand();
    void setBand(int value);

    int getBackGround();
    void setBackGround(int value);

    int getWhite();
    void setWhite(int value);

    virtual void readConfigData();
    virtual void writeConfigData();

private:
    void initVariables();
    void initSignalSlots();

private slots:
    void onChangedConfig(GlobalDataStruct::INTENSITY);

private:
    QScopedPointer<Ui_IntensityWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // INTENSITYWIDGET_H
