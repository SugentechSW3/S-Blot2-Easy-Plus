#ifndef LEDSETTING_H
#define LEDSETTING_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_LEDSetting;
class LEDSetting : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit LEDSetting(QWidget *parent = nullptr);
    virtual ~LEDSetting();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

private slots:
    void onClickedLEDRead();
    void onClickedLEDWrite();
    void onClickedLEDON();
    void onClickedLEDOFF();

    void onRecvData(QByteArray recvData);

private:
    QScopedPointer<Ui_LEDSetting> ui;
};

#endif // LEDSETTING_H
