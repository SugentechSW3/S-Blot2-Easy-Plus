#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include <opencv2/core/mat.hpp>
#include <Analysis/BasicSupremeIAnalysis.h>

class Ui_CameraWidget;
const int CAMERA_TIMER = (1000/10);
class CameraWidget : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CameraWidget(QWidget *parent = nullptr);
    virtual ~CameraWidget();

    cv::Mat getCurrentImage();
    void setCurrentImage(cv::Mat image);

    bool isStaticImage();
    void setBoxCarRect(QVector<GlobalDataStruct::rectBandResult> boxCarList);
    void setBandGapLine(int front, int back);

signals:
   void onDrawGraph(QPoint data, cv::Mat image);
   void requestDisableWidget();
   void requestEnableWidget();

public slots:
   void onCameraSaveImage(QString path);
   void onCameraLoadImage(QString path);
   void onAngleReset();

private:
   void initVariables();
   void initSignalSlots();
   void startWidget();
   void stopWidget();

   void useMatrixImage();
   void setUpImage(const cv::Mat& matrix);
   void requestDrawGraph(const cv::Mat &matrix, const QPoint &pos);

private:
   void mouseReleaseEvent(QMouseEvent* event);

private slots:
    void onCameraViewTimer();

private:
    QScopedPointer<Ui_CameraWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CAMERAWIDGET_H
