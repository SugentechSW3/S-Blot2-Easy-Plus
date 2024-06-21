#include <QCoreApplication>
#include <QVariant>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include "CameraInstance.h"
#include "CameraCtrl.h"

#include "Common/CLogWriter.h"
#include "Analysis/ConfigInformation.h"

#include "Network/SerialCtrl.h"
#include "Network/PacketReader/PacketReader.h"
#if 1
#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#endif


#ifndef __WIN32
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <libv4l2.h>
#endif

// Creater      : sp.Park by SystemDevelopment
// Description  : C++ 11 Based CameraInstance Class(SingleTon)
// Created Date : 2018-03-19
using namespace cv;
USING_SUGENTECH

struct CameraInstance::privateStruct
{
    QMap<int, CameraCtrl::CameraProperty> mProperty;
#ifdef __WIN32
    cv::VideoCapture mCap;
#else
    bool mIsV4l2Started = false;
    int  mDeviceHandle = 0;
    int mCameraWidth = 0;
    int mCameraHeight = 0;
    int mCameraFPS = 0;

    void* mMMAPBuffer = nullptr;

#endif
    QThread* mThread = nullptr;
    QMutex mMutex;
    QTimer* mCameraTimer = nullptr;
    SerialCtrl* mNetworkInstance = nullptr;
    CameraCtrl* mCameraCtrlInstance = nullptr;

    int mRetryCount = 0;
    bool mIsInit = false;
    bool mIsOpen = false;
    bool mIsParameterInit = false;
    bool mIsEnableInitSignal = true;

    // 1600, 1200, 30 = 2m
    // 2592, 1944, 30 = 5m
    const int mWidth = 2592; // 2592
    const int mHeight = 1944; // 1944
    const int mFPS = 10;
};

CameraInstance::CameraInstance(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    d->mThread = new QThread(this);
    d->mCameraTimer = new QTimer(this);
    d->mNetworkInstance = SerialCtrl::getInstance();
    d->mCameraCtrlInstance = CameraCtrl::getInstance();

    this->moveToThread(d->mThread);
    d->mCameraTimer->moveToThread(d->mThread);
    this->initSignalSlots();

    d->mThread->start();
}

CameraInstance::~CameraInstance()
{
    if(d->mThread != nullptr)
    {
        this->instanceStop();
        d->mThread->terminate();
        d->mThread = nullptr;
    }
}

void CameraInstance::initSignalSlots()
{
    connect(d->mThread, &QThread::started, this, [this]
            {
                d->mIsInit = true;
            });

    connect(d->mCameraTimer, &QTimer::timeout, this, [this]
            {
                if(d->mIsInit == false)
                    return;

                if(RETRY_COUNT < d->mRetryCount)
                {
                    d->mCameraTimer->stop();
                    d->mRetryCount = 0;
                    emit onInitializingFail();
                    return ;
                }

                if(this->initStart() == true)
                {
                    auto matrix = this->getCaptureMatImage();

                    if(matrix.data != nullptr)
                    {
                        d->mCameraTimer->stop();
                        d->mRetryCount = 0;

                        QTimer::singleShot(1000, this, [this]
                                           {
                                               emit onStartResult(true);
                                           });
                        return;
                    }
                }

                emit onStartResult(false);
                d->mRetryCount++;
            });

    connect(d->mNetworkInstance, &SerialCtrl::onDeviceIsConnected, this, [this]
            {
                if(this->isOpen() == true)
                    this->restart();
            });

    connect(d->mNetworkInstance, &SerialCtrl::onDeviceIsDisConnected, this, [this]
            {
                d->mIsOpen = false;
                d->mCameraTimer->stop();
            });

    connect(this, &CameraInstance::onSendData, d->mNetworkInstance, &SerialCtrl::writeQueuedData);
}

cv::Mat CameraInstance::getCaptureMatImage()
{
    QMutexLocker locker(&d->mMutex);

#ifdef __WIN32
    if(this->isOpen() == false)
        return cv::Mat();

    cv::Mat matrix;
    d->mCap >> matrix;
#else

    if(d->mIsV4l2Started == false)
        return cv::Mat();

    auto matrix = this->captureQueryImage(d->mDeviceHandle, d->mMMAPBuffer, d->mCameraWidth, d->mCameraHeight);
#endif

    if(matrix.data == nullptr)
        return cv::Mat();

    return matrix;
}

cv::Mat CameraInstance::getCaptureMatImage(cv::Rect& roi)
{
    QMutexLocker locker(&d->mMutex);

#ifdef __WIN32
    if(this->isOpen() == false)
        return cv::Mat();

    cv::Mat matrix;
    d->mCap >> matrix;
#else

    if(this->isStarted()== false)
        return cv::Mat();

    auto matrix =  this->captureQueryImage(d->mDeviceHandle, d->mMMAPBuffer, d->mCameraWidth, d->mCameraHeight);

#endif

    if(matrix.data == nullptr)
        return cv::Mat();

    bool isWidth = matrix.size().width >=  roi.x  + roi.width;
    bool isHeight = matrix.size().height >= roi.y + roi.height;


    if(isWidth == false || isHeight == false)
        return matrix;

    cv::Mat roiMat = matrix(roi);
    return roiMat;
}

void CameraInstance::clearRotation()
{
    QMutexLocker locker(&d->mMutex);

    double rotation = 0;
    ConfigInformation::getInstance()->setRotation(rotation);
}

cv::Mat CameraInstance::getFullConfigureImage()
{
    if(this->isOpen() == false)
        return cv::Mat();

    auto currentConfigure = ConfigInformation::getInstance();
    auto defaultRect = currentConfigure->getBaseROI();

    bool isX = defaultRect.x() < 0;
    bool isY = defaultRect.y() < 0;
    bool isWidth = defaultRect.width() < 0;
    bool isHeight = defaultRect.height() < 0;

    if(isX || isY || isWidth || isHeight)
        return cv::Mat();

    cv::Rect rect;
    rect.x = defaultRect.x();
    rect.y = defaultRect.y();
    rect.width = defaultRect.width();
    rect.height = defaultRect.height();

    cv::Mat imgMatrix = this->getCaptureMatImage(rect);

    if(imgMatrix.data == nullptr)
        return cv::Mat();


    double calMat[] = { 21067.056, 0, 1193.959, 0, 21139.831, 145.445, 0, 0, 1};
    double calDistorition[] = { -2.628488,  -46.475697,  -0.001542,  -0.007855 };


    cv::Mat undistortedMatrix;
    cv::Mat cameraMatrix(3,3, CV_64FC1, (void*)calMat) ;
    cv::Mat disCoeffs(1, 4, CV_64FC1, (void*)calDistorition);


    cv::undistort(imgMatrix, undistortedMatrix, cameraMatrix, disCoeffs);


    if(undistortedMatrix.data == nullptr)
        return cv::Mat();


    auto rotation = currentConfigure->getRotation();
    auto flip = currentConfigure->getFlip();
    CameraInstance::rotationFlip(undistortedMatrix, flip, rotation);


    return undistortedMatrix;
}

cv::Mat CameraInstance::getFullConfigureImage(const cv::Rect& roi)
{
    auto configImage = this->getFullConfigureImage();

    if(configImage.data == nullptr)
        return cv::Mat();

    cv::Mat imgMatrix = configImage(roi);
    return imgMatrix;
}

void CameraInstance::flushBuffer()
{
    this->getFullConfigureImage();
}

bool CameraInstance::ImageRotation(Mat& srcImage, double angle)
{
    Mat imageSource = srcImage;
    Mat rotMat;

    double dScale = 1.0;

    if(imageSource.data == nullptr)
        return false;

    auto center = cv::Point2d(imageSource.size().width / 2.0, imageSource.size().height / 2.0);

    rotMat = cv::getRotationMatrix2D(center, angle, dScale);

    if(srcImage.data == nullptr)
        return false;

    cv::warpAffine(imageSource, srcImage, rotMat, imageSource.size());

    return true;
}

bool CameraInstance::rotation(cv::Mat& sourceImg, double dAngle)
{
    if (dAngle != 0.0)
        CameraInstance::ImageRotation(sourceImg, dAngle);

    return true;
}

bool CameraInstance::flip(cv::Mat &sourceImg, int nImageFlip)
{
    cv::Mat img;
    sourceImg.copyTo(img);

    if (img.data == nullptr)
        return true;

    if (sourceImg.data == nullptr)
        return false;

    if (nImageFlip == 1)
        cv::flip(img, sourceImg, 1);


    if (nImageFlip != 0)
    {
        if (nImageFlip == 1)
            nImageFlip = 0; // x

        else if (nImageFlip == 2)
            nImageFlip = 1; // x

        else if (nImageFlip == 3)
            nImageFlip = -1; // x

        cv::flip(img, sourceImg, nImageFlip);
    }

    else
        sourceImg = img;

    return true;
}

bool CameraInstance::rotationFlip(cv::Mat& sourceImg, int nImageFlip, double dAngle)
{
    cv::Mat img;
    sourceImg.copyTo(img);

    if (img.data == nullptr)
        return true;

    CameraInstance::flip(sourceImg, nImageFlip);
    CameraInstance::rotation(sourceImg, dAngle);

    return true;
}

QImage CameraInstance::getCaptureImage()
{
    QMutexLocker locker(&d->mMutex);

#ifdef __WIN32
    if(this->isOpen() == false)
        return QImage();

    cv::Mat matrix;
    d->mCap >> matrix;
#else
    auto matrix =  this->captureQueryImage(d->mDeviceHandle, d->mMMAPBuffer, d->mCameraWidth, d->mCameraHeight);
#endif

    if(matrix.data == nullptr)
        return QImage();

    QImage cvImg  = QImage((uchar*)matrix.data, matrix.size().width, matrix.size().height, matrix.step, QImage::Format_RGB888);//0xffRRGGBB (24bit)

    emit onRespImage(cvImg.rgbSwapped());
    return cvImg.rgbSwapped();
}

QImage CameraInstance::getCaptureImage(QRect roi)
{
    QMutexLocker locker(&d->mMutex);

#ifdef __WIN32

    if(this->isOpen() == false)
        return QImage();

    cv::Mat matrix;
    d->mCap >> matrix;
#else
    auto matrix =  this->captureQueryImage(d->mDeviceHandle, d->mMMAPBuffer, d->mCameraWidth, d->mCameraHeight);
#endif
    if(matrix.data == nullptr)
        return QImage();

    bool isWidth = matrix.size().width >=  roi.x()  + roi.width();
    bool isHeight = matrix.size().height >= roi.y() + roi.height();


    if(isWidth == false || isHeight == false)
        return QImage();

    cv::Rect rect;
    rect.x = roi.x();
    rect.y = roi.y();
    rect.width = roi.width();
    rect.height = roi.height();

    cv::Mat roiMat = matrix(rect);

    if(roiMat.data == nullptr)
        return QImage();

    QImage cvImg  = QImage((uchar*)roiMat.data, roiMat.size().width, roiMat.size().height, roiMat.step, QImage::Format_RGB888);//0xffRRGGBB (24bit)

    emit onRespImage(cvImg.rgbSwapped());
    return cvImg.rgbSwapped();
}

QMap<int, CameraCtrl::CameraProperty> CameraInstance::getCameraPropertyList()
{
    return d->mProperty;
}

int CameraInstance::getCameraProperty(cv::VideoCaptureProperties property)
{
#ifdef __WIN32
    if(this->isOpen()== false)
        return 0;
#else
    if(d->mIsV4l2Started == false)
        return 0;
#endif

    QMutexLocker locker(&d->mMutex);

    int value = 0;
    d->mCameraCtrlInstance->getVideoProperty(property, value);

    return value;
}

bool CameraInstance::setCameraProperty(cv::VideoCaptureProperties property, int value)
{
#ifdef __WIN32
    if(this->isOpen() == false)
        return false;
#else
    if(d->mIsV4l2Started == false)
        return false;
#endif
    d->mCameraCtrlInstance->setVideoProperty(property, value);

    return true;
}


void CameraInstance::readFromCameraProperty(QMap<int, CameraCtrl::CameraProperty>& property)
{
    QMutexLocker locker(&d->mMutex);

    CameraCtrl::CameraProperty propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_BRIGHTNESS, propertyData);
    property[cv::CAP_PROP_BRIGHTNESS] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_CONTRAST, propertyData);
    property[cv::CAP_PROP_CONTRAST] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_SATURATION, propertyData);
    property[cv::CAP_PROP_SATURATION] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_SHARPNESS, propertyData);
    property[cv::CAP_PROP_SHARPNESS] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_WHITE_BALANCE_RED_V, propertyData);
    property[cv::CAP_PROP_WHITE_BALANCE_RED_V] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_HUE, propertyData);
    property[cv::CAP_PROP_HUE] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_GAIN, propertyData);
    property[cv::CAP_PROP_GAIN] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_EXPOSURE, propertyData);
    property[cv::CAP_PROP_EXPOSURE] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_BACKLIGHT, propertyData);
    property[cv::CAP_PROP_BACKLIGHT] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_GAMMA, propertyData);
    property[cv::CAP_PROP_GAMMA] = propertyData;

    d->mCameraCtrlInstance->getVideoRangeProperty(cv::CAP_PROP_FOCUS, propertyData);
    property[cv::CAP_PROP_FOCUS] = propertyData;
}

//start stop
bool CameraInstance::initStart()
{
    if(d->mIsInit == false)
        return false;

#ifdef __WIN32
    if(this->isOpen() == true)
        return true;

    if(d->mCameraCtrlInstance->startCtrl() == false)
        return false;

    if(d->mCap.open(d->mCameraCtrlInstance->getCameraIndex()) == false)
        return false;

    if(d->mIsEnableInitSignal)
        emit onStartInitalizing();

    d->mThread->sleep(1);

#else

    if(d->mIsV4l2Started == true)
        return true;

    d->mIsV4l2Started = (this->open_device("/dev/video0", d->mDeviceHandle) == SUCCESS_LOCAL);

    if(d->mIsV4l2Started == false)
        return false;

#endif
    this->initResolution(d->mWidth, d->mHeight, d->mFPS);

#ifndef __WIN32
    //   this->queryImage(d->mDeviceHandle);
    d->mIsV4l2Started = this->initMMAP(d->mDeviceHandle, d->mMMAPBuffer);


    if(d->mIsV4l2Started == false)
        return false;
#endif

    this->readFromCameraProperty(d->mProperty);

    d->mIsOpen = true;

    QTimer::singleShot(1000, this, [this]
                       {
                           this->initParameter();
                           this->analysisLedON();

                           qDebug() << "CameraStart";

                           emit onFinishedInitalizing();
                       });
    return true;
}

bool CameraInstance::isOpen()
{
    return d->mIsOpen;
}

void CameraInstance::restart()
{
    this->analysisLedOFF();

    QTimer::singleShot(1000, this, [this]
                       {
                           this->instanceStop();
                           this->instanceStart();
                       });
}

void CameraInstance::instanceStart()
{
#ifdef IS_DEBUG_PC
    return;
#endif

    if(this->isOpen() == true)
    {
        QTimer::singleShot(1000, this, [this]
                           {
                               emit onStartResult(true);
                           });
        return;
    }

    if(d->mCameraTimer->isActive() == false)
    {
        this->analysisLedOFF();

        QTimer::singleShot(1, this, [this]
                           {
                               d->mCameraTimer->start(RETRY_DELAY);
                           });
    }
}

void CameraInstance::instanceStop()
{
    QMutexLocker locker(&d->mMutex);

#ifdef __WIN32
    d->mCap.release();
#else
    d->mIsV4l2Started =false;
    close(d->mDeviceHandle);
#endif
    d->mIsParameterInit = false;
    d->mIsOpen = false;
    d->mIsEnableInitSignal = true;
}

void CameraInstance::onCallBackReconnection()
{
    if(this->isOpen() == true)
    {
        d->mCameraCtrlInstance->stopCtrl();
        d->mCameraCtrlInstance->startCtrl();
        this->restart();
    }
}

void CameraInstance::initResolution(int width, int height, int fps)
{
    QMutexLocker locker(&d->mMutex);

#ifdef __WIN32
    d->mCap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    d->mCap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    d->mCap.set(cv::CAP_PROP_FPS, fps);
#else
    d->mCameraWidth = width;
    d->mCameraHeight = height;
    d->mCameraFPS = fps;

    v4l2_format fmt;

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width  = d->mCameraWidth;
    fmt.fmt.pix.height = d->mCameraHeight;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;        // V4L2_PIX_FMT_MJPEG;  // V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;            // V4L2_FIELD_INTERLACED;
    this->xioctl(d->mDeviceHandle, VIDIOC_S_FMT, &fmt);

#endif

    int guiWidth = width;
    int guiHeight = 300;

    int startX = 0;
    int startY = (height - guiHeight) /2;

    auto config = ConfigInformation::getInstance();
    config->setBaseROI(QRect(startX, startY, guiWidth, guiHeight));
    config->setBaseResolution(QSize(width,height));
}

void CameraInstance::initParameter()
{
    auto configData = ConfigInformation::getInstance()->getCameraLightSetting();

    this->setCameraProperty(cv::CAP_PROP_AUTO_EXPOSURE, 0);

    if(d->mIsParameterInit == false)
        d->mThread->sleep(12);

    d->mIsParameterInit = true;

    this->setCameraProperty(cv::CAP_PROP_EXPOSURE, configData.expousre);
    this->setCameraProperty(cv::CAP_PROP_WHITE_BALANCE_RED_V, configData.whitebalance);
    this->setCameraProperty(cv::CAP_PROP_GAIN, configData.gain);
    this->setCameraProperty(cv::CAP_PROP_BRIGHTNESS, configData.brightness);
    this->setCameraProperty(cv::CAP_PROP_CONTRAST, configData.contrast);
    this->setCameraProperty(cv::CAP_PROP_SATURATION, configData.saturation);
    this->setCameraProperty(cv::CAP_PROP_SHARPNESS, configData.sharpness);
    this->setCameraProperty(cv::CAP_PROP_HUE, configData.hue);
    this->setCameraProperty(cv::CAP_PROP_GAMMA, configData.gamma);
    this->setCameraProperty(cv::CAP_PROP_FOCUS, configData.focus);
}

void CameraInstance::analysisLedON()
{
    auto ledOn =  WriteProtocolCameraLedSetting::writeLEDOnOff(WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_ON).toUtf8();
    emit onSendData(ledOn);
}

void CameraInstance::analysisLedOFF()
{
    auto ledOff =  WriteProtocolCameraLedSetting::writeLEDOnOff(WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_OFF).toUtf8();
    emit onSendData(ledOff);
}

void CameraInstance::setEnableStartInitSignal(bool isEnable)
{
    d->mIsEnableInitSignal = isEnable;
}

int CameraInstance::getWidthResolution()
{
    return d->mWidth;
}

int CameraInstance::getHeigthResolution()
{
    return d->mHeight;
}

int CameraInstance::getFPS()
{
    return d->mFPS;
}

#ifndef __WIN32

bool CameraInstance::queryCapture(int handle)
{
    v4l2_capability caps;
    CLEAR(caps);

    if (-1 == this->xioctl(handle, VIDIOC_QUERYCAP, &caps))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Query Caputre Fail"), CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    return true;
}

bool CameraInstance::queryImage(int handle)
{
    v4l2_format fmt;
    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = d->mCameraWidth;
    fmt.fmt.pix.height = d->mCameraHeight;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (-1 == this->xioctl(handle, VIDIOC_S_FMT, &fmt))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Setting Query Image"), CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    return true;
}


bool CameraInstance::initMMAP(int handle, void*& mmapBuffer)
{
    v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == this->xioctl(handle, VIDIOC_REQBUFS, &req))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Requesting Buffer"), CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    v4l2_buffer buf ;
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    if(-1 == this->xioctl(handle, VIDIOC_QUERYBUF, &buf))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Quering Buffer"), CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    mmapBuffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, handle, buf.m.offset);

    //   printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    //   printf("Image Length: %d\n", buf.bytesused);

    return true;
}

cv::Mat CameraInstance::captureQueryImage(int handle, void* mmapBuffer, int width, int height)
{
    v4l2_buffer buf;
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    if(-1 == this->xioctl(handle, VIDIOC_QBUF, &buf))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : CaptureQuery Buffer"), CLogWriter::LOG_TYPE_RELEASE);
        return cv::Mat();
    }

    if(-1 == this->xioctl(handle, VIDIOC_STREAMON, &buf.type))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Capture Error"), CLogWriter::LOG_TYPE_RELEASE);
        return cv::Mat();
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(handle, &fds);

    timeval tv;
    CLEAR(tv);

    tv.tv_sec = 2;
    int r = select(handle+1, &fds, NULL, NULL, &tv);

    if(-1 == r)
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Waiting for Frame"), CLogWriter::LOG_TYPE_RELEASE);
        return cv::Mat();
    }

    if(-1 == this->xioctl(handle, VIDIOC_DQBUF, &buf))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Retrieving Frame"), CLogWriter::LOG_TYPE_RELEASE);
        return cv::Mat();
    }

    cv::Mat matrix(height, width, CV_8UC3, (uchar*)mmapBuffer);
    cv::Mat decodedMatrix = cv::imdecode(matrix, CV_LOAD_IMAGE_UNCHANGED );

    return decodedMatrix;
}


int CameraInstance::open_device(const char* dev_name, int& fd)
{
    struct stat st;

    if(-1 == stat(dev_name, &st))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : %1 : %2, %3").arg(dev_name)
                             .arg(QString::number(errno).arg(strerror(errno))), CLogWriter::LOG_TYPE_RELEASE);
        return ERROR_LOCAL;
    }

    if(!S_ISCHR(st.st_mode))
    {
        CLogWriter::printLog(QString("[CameraInstance::] : %1 is not a valid device").arg(dev_name), CLogWriter::LOG_TYPE_RELEASE);
        return ERROR_LOCAL;
    }

    fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
    if(-1 == fd)
    {
        CLogWriter::printLog(QString("[CameraInstance::] : Cannot open '%1' : %2 %3").arg(dev_name)
                             .arg(QString::number(errno)).arg(strerror(errno)), CLogWriter::LOG_TYPE_RELEASE);

        if(EACCES == errno)
        {
            CLogWriter::printLog(QString("[CameraInstance::] : Insufficient permissions on '%1' : %2 %3").arg(dev_name)
                                 .arg(QString::number(errno)).arg(strerror(errno)), CLogWriter::LOG_TYPE_RELEASE);
        }
        return ERROR_LOCAL;
    }

    return SUCCESS_LOCAL;
}

int CameraInstance::errnoexit(const char *s)
{
    CLogWriter::printLog(QString("[CameraInstance::] : [%1] : error %2, %3").arg(s).arg(QString::number(errno)).arg(strerror(errno)), CLogWriter::LOG_TYPE_RELEASE);
    return ERROR_LOCAL;
}

int CameraInstance::xioctl(int fd, int request, void *arg)
{
    int r;

    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

int CameraInstance::xioctl(int fd, int request, v4l2_queryctrl *arg)
{
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while(-1 == r && EINTR == errno);

    return r;
}

int CameraInstance::cioctl(int fd, int request, v4l2_control *arg)
{
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while(-1 == r && EINTR == errno);

    return r;
}

#endif


