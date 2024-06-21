#ifndef BOXCARWIDGET_H
#define BOXCARWIDGET_H

#include <QWidget>
#include <Common/CommonParentWidget/CConfig.h>
#include "Analysis/GlobalDataStruct.h"

class Ui_BoxCarWidget;
class BoxCarWidget : public QWidget, public CConfig
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit BoxCarWidget(QWidget *parent = nullptr);
    virtual ~BoxCarWidget();

    void readConfigData();
    void writeConfigData();

    int getBandCount();
    void setBandCount(int count);

    int getWidth();
    void setWidth(int width);

    int getHeight();
    void setHeight(int height);

signals:
    void setBoxCarRect(int idx, int value);
    void setBoxCarRect(QVector<int> value);

private slots:
    void onConfigChangedAnalysis(GlobalDataStruct::USE_BOXCAR_ANALYSIS configData);
    void onConfigChangedBandGap(GlobalDataStruct::USE_BOXCAR_BAND);


    void onChangedBoxCarGapSize(int value);
    void onChangedBoxCarData(int idx, int value);

private:
    void initWidget();
    void initSignalSlots();

    int calcBandGap(int idx, int bandGapSize);
    int validRoiPos(int targetPos);
    int getMaximumRoiXPos();
    bool checkValidTuingData();
    void setEnableBoxCar(bool isEnable);

private:
    QScopedPointer<Ui_BoxCarWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // BOXCARWIDGET_H
