#ifndef CAMERASTRIPSETTINGS_H
#define CAMERASTRIPSETTINGS_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_CameraStripSettings;
class QGridLayout;
class QVBoxLayout;

class CameraStripSettings : public CSerialNetworkWidget
{
private:
    Q_OBJECT

    struct privateStruct;
    enum CONTENTS_LIST
    {
        CONTENTS_LIST_00 = 0,
        CONTENTS_LIST_END = 0x9999
    };


    enum TABLE_COLUMN
    {
        INDEX,
        BAND,
        BACKGROUND,
        INTENSITY
    };

public:
    explicit CameraStripSettings(QWidget *parent = nullptr);
    virtual ~CameraStripSettings();

    void setResultValue(const QVector<double>& intensity, const QVector<double>& background, const QVector<double>& band);
signals:
  void onSelectedSaveImage(QString path);
  void onSelectedLoadImage(QString path);
  void onSelectedFindPC();
  void onSelectedAnalysis();
  void onSelectedAngleReset();
  void onReadQRTest();
  void onGraphHide(bool isHide);

  void onConfigDataSendingStart();

  void setBoxCarRect(int idx, int value);
  void setBoxCarRect(QVector<int> value);

private slots:
    void readConfigure();
    void writeConfigure();
    void writeDualBand(bool isChecked);
    void onContentsChanged(int idx);
    void onUseChanged();

    void setUpBoxCarWidget(QWidget* part2nd, QGridLayout* layout);
    void setUpDualBandWidget(QWidget* part2nd, QGridLayout* layout);

private:
    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void readDefaultSetting();
    void writeDefaultSetting();

    void initSugentechDefaultGUI();
    void writeSugentechDefaultGUI();
    void initDefaultGUI();
    void writeDefaultGUI();

    void readContentsWidget();
    void writeContentsWidget();


    void initContentsWidget00(QWidget *part2nd, QGridLayout *layout);
    void writeContentsWidget00();

private:
    QScopedPointer<Ui_CameraStripSettings> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CAMERASTRIPSETTINGS_H
