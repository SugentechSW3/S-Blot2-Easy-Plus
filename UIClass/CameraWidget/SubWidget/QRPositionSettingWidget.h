#ifndef QRPOSITIONSETTINGWIDGET_H
#define QRPOSITIONSETTINGWIDGET_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_QRPositionSettingWidget;
class QRPositionSettingWidget : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit QRPositionSettingWidget(QWidget *parent = nullptr);
    ~QRPositionSettingWidget();

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

private slots:
    void onClickedQRTrayPosSet();
    void onClickedQRTrayPosRead();

    void onRecvData(QByteArray recvData);

private:
    QScopedPointer<Ui_QRPositionSettingWidget> ui;
};

#endif // QRPOSITIONSETTINGWIDGET_H
