#ifndef CAMERAINSTANCE_H
#define CAMERAINSTANCE_H

#include <QObject>
#include <QScopedPointer>
#include <QMap>
#include <QImage>
#include <opencv2/core/mat.hpp>


#include "CameraCtrl.h"


class CameraInstance : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(CameraInstance)

    struct privateStruct;

    explicit CameraInstance(QObject *parent = nullptr);
    virtual ~CameraInstance();

    const int RETRY_DELAY = 5000;
    const int RETRY_COUNT = 3;

public:
    static CameraInstance* getInstance()
    {
        static CameraInstance instance;
        return &instance;
    }

    bool isOpen();

    QMap<int, CameraCtrl::CameraProperty> getCameraPropertyList();
    int getCameraProperty(cv::VideoCaptureProperties property);
    bool setCameraProperty(cv::VideoCaptureProperties property, int value);

    static bool ImageRotation(cv::Mat& srcImage, double angle);
    static bool rotation(cv::Mat& sourceImg, double dAngle);
    static bool flip(cv::Mat& sourceImg, int nImageFlip);
    static bool rotationFlip(cv::Mat& sourceImg, int nImageFlip, double dAngle);
    void clearRotation();
    void readFromCameraProperty(QMap<int, CameraCtrl::CameraProperty>& property);

    void analysisLedON();
    void analysisLedOFF();

    void setEnableStartInitSignal(bool isEnable);

    int getWidthResolution();
    int getHeigthResolution();
    int getFPS();

public slots:
    void instanceStart();
    void instanceStop();
    void restart();
    void onCallBackReconnection();

    cv::Mat getCaptureMatImage();
    cv::Mat getCaptureMatImage(cv::Rect& roi);
    cv::Mat getFullConfigureImage();
    cv::Mat getFullConfigureImage(const cv::Rect &roi);
    void flushBuffer();

    QImage getCaptureImage();
    QImage getCaptureImage(QRect roi);

signals:
    void onStartedThread();
    void onRespImage(QImage img);
    void onStartInitalizing();
    void onFinishedInitalizing();
    void onInitializingFail();
    void onStartResult(bool isSuccess);
    void onSendData(QByteArray data);


private:
   void initResolution(int width, int height, int fps);
   void initParameter();
   bool initStart();
   void initSignalSlots();



#ifndef __WIN32
   bool queryCapture(int handle);
   bool queryImage(int handle);
   bool initMMAP(int handle, void*& mmapBuffer);
   cv::Mat captureQueryImage(int handle, void *mmapBuffer, int width, int height);


   int open_device(const char* dev_name, int& fd);
   int errnoexit(const char *s);
   int xioctl(int fd, int request, void *arg);
   int xioctl(int fd, int request, v4l2_queryctrl *arg);
   int cioctl(int fd, int request, v4l2_control *arg);

#endif

private:
    QScopedPointer<privateStruct> d;
};

#endif // CAMERAINSTANCE_H
