#include <QVector>
#include <QDebug>
#include <QThread>
#include <QSettings>
#include "CameraCtrl.h"
#include "Common/CLogWriter.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : C++ 11 Based CameraCtrl Class
// Created Date : 2018-03-19
// Modified Date #1 : 2018-06-20 (Linux API Added)
// Modified Date #2 : 2019-10-23 (Camera Connection Modified)

#ifdef __WIN32
#include <windows.h>
#include <dshow.h>
#include <ksmedia.h>

#else
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

USING_SUGENTECH

struct CameraCtrl::privateStruct
{

    QMutex mMutex;
#ifdef __WIN32
    QSet<QString> mCameraList;
    IBaseFilter* mBaseFilter = nullptr;
    IAMCameraControl* mCameraControl = nullptr;
    IAMVideoProcAmp* mVideoProcAmp = nullptr;
    IKsPropertySet* mPropertySet = nullptr;
    int mCameraIndex = -1;
#else
    int mDeviceHandle = 0;
#endif

};

CameraCtrl::CameraCtrl(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    d->mCameraList << QString("Logitech QuickCam Pro 9000");
    d->mCameraList << QString("Logitech Webcam Pro 9000");
    d->mCameraList << QString("Logitech HD Webcam C615");
    d->mCameraList << QString("Logitech HD Pro Webcam C920");
    d->mCameraList << QString("HD USB Camera");
    d->mCameraList << QString("USB Camera");
#ifndef __WIN32
    this->open_device("/dev/video0", d->mDeviceHandle);
#endif
}

CameraCtrl::~CameraCtrl()
{
    this->stopCtrl();
}

bool CameraCtrl::startCtrl()
{
     CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
     d->mCameraIndex = -1;

     if(this->EnumDevices() == false)
     {
         this->stopCtrl();
         return false;
     }

    if(d->mBaseFilter != nullptr)
    {
        //
        // Get a pointer to the IAMCameraControl interface used to control the camera
        // 노출, 밝기등을 설정하기 위한 구조체
        //
        d->mBaseFilter->QueryInterface(IID_IAMCameraControl, (void **)&d->mCameraControl);

        //
        // Get a pointer to the IAMCameraControl interface used to control the camera
        // 게인을 설정하기 위한 포인터
        //
        d->mBaseFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&d->mVideoProcAmp);

        // 저광증폭
        // 설정값외의 값을 Fix시키는 포인터
        d->mBaseFilter->QueryInterface(IID_IKsPropertySet, (void **)&d->mPropertySet);
    }

    return true;
}

void CameraCtrl::stopCtrl()
{
#ifdef __WIN32
    if(d->mBaseFilter != nullptr)
    {
        d->mBaseFilter->Release();
        d->mBaseFilter = nullptr;
    }

    if(d->mCameraControl != nullptr)
    {
        d->mCameraControl->Release();
        d->mCameraControl = nullptr;
    }

    if(d->mVideoProcAmp != nullptr)
    {
        d->mVideoProcAmp->Release();
        d->mVideoProcAmp = nullptr;
    }

    if(d->mPropertySet != nullptr)
    {
        d->mPropertySet->Release();
        d->mPropertySet = nullptr;
    }

#else
    close(d->mDeviceHandle);

#endif

    CoUninitialize();
}

int CameraCtrl::getCameraIndex()
{
    return d->mCameraIndex;
}

#ifdef __WIN32
bool CameraCtrl::setVideoProperty(cv::VideoCaptureProperties prop, int value)
{
    if(d->mCameraControl == nullptr)
        return false;

    if(d->mVideoProcAmp == nullptr)
        return false;

    QMutexLocker locker(&d->mMutex);

    switch(prop)
    {
    case cv::CAP_PROP_AUTO_EXPOSURE:
    {
        auto nFlags = CameraControl_Flags_Auto;
        d->mCameraControl->Set(CameraControl_Exposure, value, nFlags);
    }   break;

    case cv::CAP_PROP_EXPOSURE:
    {
        auto nFlags = CameraControl_Flags_Manual;
        d->mCameraControl->Set(CameraControl_Exposure, value, nFlags);
    }   break;

    case cv::CAP_PROP_FOCUS:
    {
        auto nFlags = CameraControl_Flags_Manual;
        d->mCameraControl->Set(CameraControl_Focus, value, nFlags);
    }   break;

    case cv::CAP_PROP_BRIGHTNESS:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Brightness, value, nFlags);
    }   break;

    case cv::CAP_PROP_CONTRAST:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Contrast, value, nFlags);
    } break;

    case cv::CAP_PROP_SATURATION:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Saturation, value, nFlags);
    }break;

    case cv::CAP_PROP_SHARPNESS:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Sharpness, value, nFlags);
    }  break;

    case cv::CAP_PROP_GAIN:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Gain, value, nFlags);
    }  break;

    case cv::CAP_PROP_WHITE_BALANCE_RED_V:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_WhiteBalance, value, nFlags);
    }break;

    case cv::CAP_PROP_HUE:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Hue, value, nFlags);
    }  break;

    case cv::CAP_PROP_GAMMA:
    {
        auto nFlags = VideoProcAmp_Flags_Manual;
        d->mVideoProcAmp->Set(VideoProcAmp_Gamma, value, nFlags);
    }break;

    default :
        break;
    }

    this->thread()->msleep(CAMERA_PARAMETER_DELAY);
    return true;
}

#endif

#ifdef __WIN32
bool CameraCtrl::getVideoProperty(cv::VideoCaptureProperties prop, int& value)
{
    if(d->mCameraControl == nullptr)
        return false;

    if(d->mVideoProcAmp == nullptr)
        return false;

    QMutexLocker locker(&d->mMutex);

    LONG _val = 0;
    LONG _flags = 0;

    switch(prop)
    {
    case cv::CAP_PROP_EXPOSURE:
        d->mCameraControl->Get(CameraControl_Exposure, &_val, &_flags);
        break;

    case cv::CAP_PROP_FOCUS:
        d->mCameraControl->Get(CameraControl_Focus, &_val, &_flags);
        break;

    case cv::CAP_PROP_BRIGHTNESS:
        d->mVideoProcAmp->Get(VideoProcAmp_Brightness, &_val, &_flags);
        break;

    case cv::CAP_PROP_CONTRAST:
        d->mVideoProcAmp->Get(VideoProcAmp_Contrast, &_val, &_flags);
        break;

    case cv::CAP_PROP_SATURATION:
        d->mVideoProcAmp->Get(VideoProcAmp_Saturation, &_val, &_flags);
        break;

    case cv::CAP_PROP_SHARPNESS:
        d->mVideoProcAmp->Get(VideoProcAmp_Sharpness, &_val, &_flags);
        break;

    case cv::CAP_PROP_GAIN:
        d->mVideoProcAmp->Get(VideoProcAmp_Gain, &_val, &_flags);
        break;

    case cv::CAP_PROP_WHITE_BALANCE_RED_V:
        d->mVideoProcAmp->Get(VideoProcAmp_WhiteBalance, &_val, &_flags);
        break;

    case cv::CAP_PROP_HUE:
        d->mVideoProcAmp->Get(VideoProcAmp_Hue, &_val, &_flags);
        break;

    case cv::CAP_PROP_GAMMA:
        d->mVideoProcAmp->Get(VideoProcAmp_Gamma, &_val, &_flags);
        break;

    default :
        break;
    }

    value = _val;
    this->thread()->msleep(CAMERA_PARAMETER_DELAY);

    return true;
}

bool CameraCtrl::getVideoRangeProperty(cv::VideoCaptureProperties prop, CameraProperty& property)
{
    long nFlags = CameraControl_Flags_Manual;

    property.max = 0;
    property.min = 0;
    property.step = 0;
    property.defaultValue = 0;

    long _min = 0;
    long _max = 0;
    long  delta = 0;
    long _default = 0;

    if(d->mCameraControl == nullptr)
        return false;

    if(d->mVideoProcAmp == nullptr)
        return false;

    QMutexLocker locker(&d->mMutex);

    switch(prop)
    {
    case cv::CAP_PROP_EXPOSURE:
        d->mCameraControl->GetRange(CameraControl_Exposure, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_FOCUS:
        d->mCameraControl->GetRange(CameraControl_Focus, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_BRIGHTNESS:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Brightness, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_CONTRAST:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Contrast, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_SATURATION:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Saturation, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_SHARPNESS:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Sharpness, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_GAIN:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Gain, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_WHITE_BALANCE_RED_V:
        d->mVideoProcAmp->GetRange(VideoProcAmp_WhiteBalance, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_HUE:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Hue, &_min, &_max, &delta, &_default, &nFlags);
        break;

    case cv::CAP_PROP_GAMMA:
        d->mVideoProcAmp->GetRange(VideoProcAmp_Gamma, &_min, &_max, &delta, &_default, &nFlags);
        break;

    default :
        break;
    }

    property.defaultValue = _default;
    property.step = delta;
    property.max = _max;
    property.min = _min;

    this->thread()->msleep(CAMERA_PARAMETER_DELAY);

    return true;
}


#if 0 //deprecated
bool CameraCtrl::SetCameraVideoAutoExposurePriority()
{

    // Low-light Comp
    HRESULT hr;
    KSPROPERTY_CAMERACONTROL_S aep;

    memset(&aep, 0, sizeof(aep));
    aep.Property.Set = PROPSETID_VIDCAP_CAMERACONTROL;
    aep.Property.Id = KSPROPERTY_CAMERACONTROL_AUTO_EXPOSURE_PRIORITY;
    aep.Value = 0;

    hr = d->mPropertySet->Set(
        aep.Property.Set,
        aep.Property.Id,
        PKSPROPERTY(&aep) + 1,
        sizeof(aep) - sizeof(KSPROPERTY),
        &aep,
        sizeof(aep)
        );

    return true;
}
#endif
bool CameraCtrl::EnumDevices()
{
    HRESULT hr;

    ICreateDevEnum *pSysDevEnum = NULL;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);

    if (FAILED(hr))
        return false;

    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

    if (hr == S_OK)
    {
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        int currentIndex = 0;

        while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
        {
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
            if (SUCCEEDED(hr))
            {

                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);

                if (SUCCEEDED(hr))
                {
                    auto mfcStr = varName.bstrVal;
                    auto qtStr = QString::fromWCharArray(mfcStr);
                    if(d->mCameraList.contains(qtStr) == true)
                    {
                        this->updateLogitechRegistry();
                        pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&d->mBaseFilter);
                        pPropBag->Release();
                        d->mCameraIndex = currentIndex;
                        break;
                    }

                }
            }
            currentIndex++;
            pPropBag->Release();
        }

        pMoniker->Release();
        pEnumCat->Release();
        pSysDevEnum->Release();
    }
    else
    {
        pSysDevEnum->Release();
        return false;	// 카메라드라이버가 하나도 설치되어 있지 않을때
    }

    if(d->mCameraIndex == -1)
        return false;

    return true;
}

#else

int CameraCtrl::open_device(const char* dev_name, int& fd)
{
    struct stat st;

    if(-1 == stat(dev_name, &st))
    {
        CLogWriter::printLog(QString("[CameraCtrl::] : %1 : %2, %3").arg(dev_name)
                             .arg(QString::number(errno).arg(strerror(errno))), CLogWriter::LOG_TYPE_RELEASE);
        return ERROR_LOCAL;
    }

    if(!S_ISCHR(st.st_mode))
    {
        CLogWriter::printLog(QString("[CameraCtrl::] : %1 is not a valid device").arg(dev_name), CLogWriter::LOG_TYPE_RELEASE);
        return ERROR_LOCAL;
    }

    fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
    if(-1 == fd)
    {
        CLogWriter::printLog(QString("[CameraCtrl::] : Cannot open '%1' : %2 %3").arg(dev_name)
                             .arg(QString::number(errno)).arg(strerror(errno)), CLogWriter::LOG_TYPE_RELEASE);

        if(EACCES == errno)
        {
            CLogWriter::printLog(QString("[CameraCtrl::] : Insufficient permissions on '%1' : %2 %3").arg(dev_name)
                                 .arg(QString::number(errno)).arg(strerror(errno)), CLogWriter::LOG_TYPE_RELEASE);
        }
        return ERROR_LOCAL;
    }

    return SUCCESS_LOCAL;
}

int CameraCtrl::errnoexit(const char *s)
{
    CLogWriter::printLog(QString("[CameraCtrl::] : [%1] : error %2, %3").arg(s).arg(QString::number(errno)).arg(strerror(errno)), CLogWriter::LOG_TYPE_RELEASE);
    return ERROR_LOCAL;
}

int CameraCtrl::xioctl(int fd, int request, v4l2_queryctrl *arg)
{
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while(-1 == r && EINTR == errno);

    return r;
}

int CameraCtrl::cioctl(int fd, int request, v4l2_control *arg)
{
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while(-1 == r && EINTR == errno);

    return r;
}

bool CameraCtrl::setVideoProperty(cv::VideoCaptureProperties prop, int value)
{
    v4l2_control control;
    CLEAR(control);

    QString idStr;

    d->mMutex.lock();

    switch(prop)
    {
    case cv::CAP_PROP_EXPOSURE:
        control.id = V4L2_CID_EXPOSURE_ABSOLUTE;
        idStr = "EXPOSURE";
        break;

    case cv::CAP_PROP_FOCUS:
        control.id = V4L2_CID_FOCUS_ABSOLUTE;
        idStr = "FOCUS";
        break;

    case cv::CAP_PROP_BRIGHTNESS:
        control.id = V4L2_CID_BRIGHTNESS;
        idStr = "BRIGHTNESS";
        break;

    case cv::CAP_PROP_CONTRAST:
        control.id = V4L2_CID_CONTRAST;
        idStr = "CONTRAST";
        break;

    case cv::CAP_PROP_SATURATION:
        control.id = V4L2_CID_SATURATION;
        idStr = "SATURATION";
        break;

    case cv::CAP_PROP_SHARPNESS:
        control.id = V4L2_CID_SHARPNESS;
        idStr = "SHARPNESS";
        break;

    case cv::CAP_PROP_GAIN:
        control.id = V4L2_CID_GAIN;
        idStr = "GAIN";
        break;

    case cv::CAP_PROP_WHITE_BALANCE_RED_V:
        control.id = V4L2_CID_AUTO_WHITE_BALANCE;
        idStr = "WHITE BALANCE";
        break;

    case cv::CAP_PROP_HUE:
        control.id = V4L2_CID_HUE;
        idStr = "HUE";
        break;

    case cv::CAP_PROP_GAMMA:
        control.id = V4L2_CID_GAMMA;
        idStr = "GAMMA";
        break;

    default :
        break;
    }

    control.value = value;

    if (cioctl(d->mDeviceHandle, VIDIOC_S_CTRL, &control) == 0)
    {
        d->mMutex.unlock();
        return true;
    }
    else
    {
        CLogWriter::printLog(QString("[CameraCtrl::] : ID [%1] : ErrNo : %2").arg(idStr).arg(errno), CLogWriter::LOG_TYPE_RELEASE);
        d->mMutex.unlock();
        return false;
    }
}

bool CameraCtrl::getVideoProperty(cv::VideoCaptureProperties prop, int& value)
{

}

bool CameraCtrl::getVideoRangeProperty(cv::VideoCaptureProperties prop, CameraProperty& property)
{
    v4l2_queryctrl queryctrl;
    CLEAR(queryctrl);

    QString idStr;

    property.max = 0;
    property.min = 0;
    property.defaultValue = 0;

    d->mMutex.lock();

    switch(prop)
    {
    case cv::CAP_PROP_EXPOSURE:
        queryctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
        idStr = "EXPOSURE";
        break;

    case cv::CAP_PROP_FOCUS:
        queryctrl.id = V4L2_CID_FOCUS_ABSOLUTE;
        idStr = "FOCUS";
        break;

    case cv::CAP_PROP_BRIGHTNESS:
        queryctrl.id = V4L2_CID_BRIGHTNESS;
        idStr = "BRIGHTNESS";
        break;

    case cv::CAP_PROP_CONTRAST:
        queryctrl.id = V4L2_CID_CONTRAST;
        idStr = "CONTRAST";
        break;

    case cv::CAP_PROP_SATURATION:
        queryctrl.id = V4L2_CID_SATURATION;
        idStr = "SATURATION";
        break;

    case cv::CAP_PROP_SHARPNESS:
        queryctrl.id = V4L2_CID_SHARPNESS;
        idStr = "SHARPNESS";
        break;

    case cv::CAP_PROP_GAIN:
        queryctrl.id = V4L2_CID_GAIN;
        idStr = "GAIN";
        break;

    case cv::CAP_PROP_WHITE_BALANCE_RED_V:
        queryctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
        idStr = "WHITE_BALANCE";
        break;

    case cv::CAP_PROP_HUE:
        queryctrl.id = V4L2_CID_HUE;
        idStr = "HUE";
        break;

    case cv::CAP_PROP_GAMMA:
        queryctrl.id = V4L2_CID_GAMMA;
        idStr = "GAMMA";
        break;

    default :
        break;
    }

    if (-1 == xioctl(d->mDeviceHandle, VIDIOC_QUERYCTRL, &queryctrl))
    {
        d->mMutex.unlock();
        CLogWriter::printLog(QString("[CameraCtrl::] : ID [%1] : ErrNo : %2").arg(idStr).arg(errno), CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }

    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
    {
        d->mMutex.unlock();
        CLogWriter::printLog(QString("[CameraCtrl::] : ID [%1] : ErrNo : %2").arg(idStr).arg(errno), CLogWriter::LOG_TYPE_RELEASE);
        return false;
    }


    property.defaultValue = queryctrl.default_value;
    property.max = queryctrl.maximum;
    property.min = queryctrl.minimum;

    d->mMutex.unlock();
    return true;
}


#endif

void CameraCtrl::updateLogitechRegistry()
{
    auto idx = this->findSelectedIdx();
    auto idList =  this->GetCameraIDList();

    for(int i =0 ; i< idList.count(); i++)
    {
        for(int regCount =0; regCount < 5 ; regCount++)
            this->writeCameraReg(idList[i], regCount, idx);
    }
}

int CameraCtrl::findSelectedIdx()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\Select", QSettings::NativeFormat);
    return settings.value("Current").toInt();

}

QStringList CameraCtrl::GetCameraIDList()
{
    QStringList idList;
    QString strID;
    QSettings registry("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class", QSettings::NativeFormat);
    auto classIDList = registry.childGroups();

    for(auto classID : classIDList)
    {
        QSettings regKey(QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\%1").arg(classID), QSettings::NativeFormat);

        if(regKey.value("Class").toString() == QString("Image"))
            idList.push_back(classID);
    }

    return idList;
}

void CameraCtrl::writeCameraReg(QString classGuid, int idx, int selectedIdx)
{
    QString controlDefult =
        QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\%1\\%2\\Settings").arg(classGuid).arg(idx, 4, 10, QChar('0'));
    QString controlIndex  =
        QString("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet%1\\Control\\Class\\%2\\%3\\Settings").arg(selectedIdx,3, 10, QChar('0')).arg(classGuid).arg(idx, 4, 10, QChar('0'));

    QSettings defaultPath(controlDefult, QSettings::NativeFormat);
    QSettings currentPath(controlIndex, QSettings::NativeFormat);

    defaultPath.setValue("LVUVC_AutoExposurePriority", 0);
    defaultPath.setValue("LVUVC_AutoExposureMode", 1);
    defaultPath.setValue("LVUVC_WBTemperatureAuto", 0);

    currentPath.setValue("LVUVC_AutoExposurePriority", 0);
    currentPath.setValue("LVUVC_AutoExposureMode", 1);
    currentPath.setValue("LVUVC_WBTemperatureAuto", 0);
}

QString CameraCtrl::GuidToString(unsigned __LONG32 Data1, unsigned short Data2, unsigned short Data3, unsigned char Data4[8])
{
    QString string;
    string.append(QString("%1-").arg(Data1, 8, 16, QLatin1Char('0')));
    string.append(QString("%1-").arg(Data2, 4, 16, QLatin1Char('0')));
    string.append(QString("%1-").arg(Data3, 4, 16, QLatin1Char('0')));
    string.append(QString("%1%2-%3%4%5%6%7%8")
                      .arg(Data4[0], 2, 16, QLatin1Char('0'))
                      .arg(Data4[1], 2, 16, QLatin1Char('0'))
                      .arg(Data4[2], 2, 16, QLatin1Char('0'))
                      .arg(Data4[3], 2, 16, QLatin1Char('0'))
                      .arg(Data4[4], 2, 16, QLatin1Char('0'))
                      .arg(Data4[5], 2, 16, QLatin1Char('0'))
                      .arg(Data4[6], 2, 16, QLatin1Char('0'))
                      .arg(Data4[7], 2, 16, QLatin1Char('0'))
                  );

    return string;
}



/*
 VARIANT varName;
hr = pPropBag->Read(L"FriendlyName", &varName, NULL);

if (SUCCEEDED(hr))
{
    //
    // 특정 드라이버를 사용할 때 루틴임
    // 사용하고자하는 카메라드라이버의 디스크립션이 empty가 아니면 수행한다
    //
    for (int i = 0; i < d->mCameraList.count(); i++)
    {
        wchar_t *str_ = varName.bstrVal;
        QString name = QString::fromWCharArray(str_);
        if (name == d->mCameraList[i])
        {
            if(name.contains("Logitech", Qt::CaseInsensitive) == true)
                this->updateLogitechRegistry();

            *pnIdxCamera = nloop;
            pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&d->mBaseFilter);
            break;
        }
    }
}
varName = VARIANT();

//Remember to release pFilter later.
pPropBag->Release();
*/
