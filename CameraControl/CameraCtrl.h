#ifndef CAMERACTRL_H
#define CAMERACTRL_H

#include <QObject>
#include <QMutex>

//Opencv Used Default Color BGR

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"


#ifdef __WIN32
#else
struct v4l2_queryctrl;
struct v4l2_control;

const int ERROR_LOCAL = -1;
const int SUCCESS_LOCAL = 0;

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#endif


const int CAMERA_PARAMETER_DELAY = 200;

class CameraCtrl : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(CameraCtrl)

    struct privateStruct;

    explicit CameraCtrl(QObject *parent = nullptr);
    virtual ~CameraCtrl();

public:
    static CameraCtrl* getInstance() //경고 : Instnace 포인터를 따로 사용하지 않으면 펑션이 종료되는순간 지역변수처럼 증발하게됨.
    {
        static CameraCtrl instance;
        return &instance;
    }

    struct CameraProperty
    {
        int min = 0;
        int max = 0;
        int step = 0;
        int defaultValue = 0;
    };

    bool startCtrl();
    void stopCtrl();

    int getCameraIndex();
    bool setVideoProperty(cv::VideoCaptureProperties prop, int value);
    bool getVideoProperty(cv::VideoCaptureProperties prop, int& value);
    bool getVideoRangeProperty(cv::VideoCaptureProperties prop, CameraCtrl::CameraProperty &property);


private:
#ifdef __WIN32
//    bool SetCameraVideoAutoExposurePriority();
    bool EnumDevices();
    QString GuidToString(unsigned __LONG32 Data1,  unsigned short Data2,  unsigned short Data3,  unsigned char Data4[8]);
#else
    int open_device(const char* dev_name, int& fd);
    int close_device(int &fd);
    int errnoexit(const char *s);
    int xioctl(int fd, int request, v4l2_queryctrl *arg);
    int cioctl(int fd, int request, v4l2_control *arg);
#endif
    int findSelectedIdx();
    QStringList GetCameraIDList();
    void writeCameraReg(QString classGuid, int idx, int selectedIdx);
    void updateLogitechRegistry();

private:
    QScopedPointer<privateStruct> d;
};

#endif // CAMERACTRL_H
