#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_CameraSettings;
class CameraSettings : public CSerialNetworkWidget
{
    Q_OBJECT

public:
    explicit CameraSettings(QWidget *parent = nullptr);
    virtual ~CameraSettings(); 

    void setResultValue(const QVector<double>& intensity, const QVector<double>& background, const QVector<double>& band);

signals:
  void onSelectedSaveImage(QString path);
  void onSelectedLoadImage(QString path);
  void onSelectedFindPC();
  void onSelectedAnalysis();
  void onSelectedAngleReset();
  void onReadQRTest();
  void onGraphHide(bool isHide);

  void setBoxCarRect(int idx, int value);
  void setBoxCarRect(QVector<int> value);

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();


private:
    QScopedPointer<Ui_CameraSettings> ui;
};

#endif // CAMERASETTINGS_H
