#ifndef ETCVIEW_H
#define ETCVIEW_H

#include "Common/CommonParentWidget/CSerialNetworkWidget.h"

class Ui_ETCView;
class ETCView : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ETCView(QWidget *parent = nullptr);
    ~ETCView();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void setMaximumStripCount();
    void setInterlock(int ms = 7000);
    void releaseInterlock();

private slots:
    void onClickedFanTestON();
    void onClickedFanTestOFF();

    void onClickedStatusLEDON();
    void onClickedStatusLEDOFF();

    void onClickPrimeFunc();
    void onClickDispFunc();
    void onClickAspFunc();

    void onClickIncubationFuc();

    void onClickTemperature();

    void onclickedTrayHeatingPadON();
    void onclickedTrayHeatingPadOFF();

    void onClickedTrayAdcReverseTemperature();

    void onClickedTrayHeatingPadTempCh1Read();
    void onClickedTrayHeatingPadTempCh1Write();
    void onClickedTrayHeatingPadTempCh2Read();
    void onClickedTrayHeatingPadTempCh2Write();

    void onRecvData(QByteArray recvData);

    void onClickAutoClean();
private:
    QScopedPointer<Ui_ETCView> ui;
    QScopedPointer<privateStruct> d;
    QScopedPointer<class CirclePrograssDialog> prograssCircleDlg;
};

#endif // ETCVIEW_H
