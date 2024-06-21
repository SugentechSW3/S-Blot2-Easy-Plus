#ifndef DUALBANDWIDGET_H
#define DUALBANDWIDGET_H

#include <QWidget>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_DualBandWidget;
class DualBandWidget : public QWidget, public CConfig
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit DualBandWidget(QWidget *parent = nullptr);
    ~DualBandWidget();

    void readConfigData();
    void writeConfigData();

    int getBandBlock2XStart();
    void setBandBlock2XStart(int value);

    int getBandBlock3XStart();
    void setBandBlock3XStart(int value);

    int getBandBlock4XStart();
    void setBandBlock4XStart(int value);

    int getMultiPCThres();
    void setMultiPCThres(int value);

    int getPCXEndPos();
    void setPCXEndPos(int value);

    int getBandBlock2XEndPos();
    void setBandBlock2XEndPos(int value);

    int getBandBlock3XEndPos();
    void setBandBlock3XEndPos(int value);

    int getBandBlock4XEndPos();
    void setBandBlock4XEndPos(int value);

    int getWidth();
    void setWidth(int value);

    int getHeight();
    void setHeight(int value);

    int getBoxCarBandCount();
    void setBoxCarBandCount(int value);

    int getXRange();
    void setXRange(int value);

    int getXStart();
    void setXStart(int value);

    int getPCThres();
    void setPCThres(int value);

    int getHousingThres();
    void setHousingThres(int value);

private:
    void initVariables();
    void initSignalSlots();

private slots:
    void onChangedConfig(GlobalDataStruct::USE_MULTIPLE_BAND);

private:
    QScopedPointer<Ui_DualBandWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // DUALBANDWIDGET_H
