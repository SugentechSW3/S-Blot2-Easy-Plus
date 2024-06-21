#include <QTimer>
#include <QMouseEvent>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

#include "CameraWidget.h"
#include "ui_CameraWidget.h"

#include "CameraControl/CameraInstance.h"
#include "Common/CLogWriter.h"
#include "Common/CUtil.h"
#include "UIClass/DialogWidget/ProgressBarDlg.h"
#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct CameraWidget::privateStruct
{
    QTimer mViewTimer;
    ProgressBarDlg mPrograssDlg;
    cv::Mat mLastLoadedImage;
    QPoint mSaveMousePoint;
    CameraInstance* mCameraInstance = nullptr;
    ConfigInformation* mConfigInstance = nullptr;
};

CameraWidget::CameraWidget(QWidget *parent) :
                                              CSerialNetworkWidget(parent),
                                              ui(new Ui_CameraWidget),
                                              d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
}

CameraWidget::~CameraWidget()
{
    this->stop();
}

void CameraWidget::initVariables()
{
    d->mCameraInstance = CameraInstance::getInstance();
    d->mConfigInstance = ConfigInformation::getInstance();
}

void CameraWidget::initSignalSlots()
{
    connect(&d->mViewTimer, &QTimer::timeout, this, &CameraWidget::onCameraViewTimer);
    connect(d->mCameraInstance, &CameraInstance::onStartResult, this, [this]
            (bool isSuccess)
            {
                if(isSuccess == true)
                {
                    d->mViewTimer.start(CAMERA_TIMER);
                    return;
                }

                d->mLastLoadedImage = cv::Mat();
                emit requestDisableWidget();
            });
}

void CameraWidget::startWidget()
{
    if(d->mCameraInstance == nullptr)
        return;

    ui->scrollAreaWidget->setMinimumWidth(d->mCameraInstance->getWidthResolution());
    ui->labelImage->setMinimumWidth(d->mCameraInstance->getWidthResolution());

    QTimer::singleShot(1, d->mCameraInstance, &CameraInstance::instanceStart);
}

void CameraWidget::stopWidget()
{
    d->mViewTimer.stop();
    QTimer::singleShot(1, d->mCameraInstance, &CameraInstance::instanceStop);
}

cv::Mat CameraWidget::getCurrentImage()
{
    d->mCameraInstance->clearRotation();

    if(d->mLastLoadedImage.empty())
        return d->mCameraInstance->getFullConfigureImage();
    else
        return d->mLastLoadedImage;
}

void CameraWidget::setCurrentImage(cv::Mat image)
{
    if(image.data == nullptr)
        return;

    this->setUpImage(image);
}

bool CameraWidget::isStaticImage()
{
    return (!d->mLastLoadedImage.empty());
}

void CameraWidget::setBoxCarRect(QVector<GlobalDataStruct::rectBandResult> boxCarList)
{
    ui->labelImage->setBoxCarRect(boxCarList);
}

void CameraWidget::setBandGapLine(int front, int back)
{
    ui->labelImage->setBandGapLine(front, back);
}

void CameraWidget::onCameraViewTimer()
{
    this->useMatrixImage();
}

void CameraWidget::onCameraSaveImage(QString path)
{
    d->mViewTimer.stop();
    auto image = d->mCameraInstance->getFullConfigureImage();

    if(image.data == nullptr)
    {
        d->mViewTimer.start(CAMERA_TIMER);
        return;
    }

    path.append("-FULL.png");
    cv::imwrite(path.toStdString().data(), image);

    d->mViewTimer.start(CAMERA_TIMER);
}

void CameraWidget::onCameraLoadImage(QString path)
{
    this->stop();

    auto matrix = cv::imread(path.toStdString().data());

    if(matrix.data == nullptr)
        return;

    this->setUpImage(matrix);
    d->mLastLoadedImage = matrix;
}

void CameraWidget::onAngleReset()
{

}

void CameraWidget::useMatrixImage()
{
    auto matrix = d->mCameraInstance->getFullConfigureImage();

    if(matrix.data == nullptr)
    {
        CLogWriter::printLog(QString("(CameraWidget::) : Camera Image Is Invalid"));
        return;
    }

    this->setUpImage(matrix);
}

void CameraWidget::setUpImage(const cv::Mat& matrix)
{
    cv::Mat cuttingMat;
    auto isXContians = ui->labelImage->size().width() < matrix.size().width;
    auto isYContians = ui->labelImage->size().height() < matrix.size().height;

    if(isXContians && isYContians)
        cuttingMat = matrix(cv::Rect(0,0, ui->labelImage->size().width(), ui->labelImage->size().height()));

    else if(isXContians == false && isYContians == false)
        cuttingMat = matrix(cv::Rect(0,0, matrix.size().width, matrix.size().height));

    else if(isXContians == false)
        cuttingMat = matrix(cv::Rect(0,0, matrix.size().width, ui->labelImage->size().height()));

    else if(isYContians == false)
        cuttingMat = matrix(cv::Rect(0,0, ui->labelImage->size().width(), matrix.size().height));

    if(cuttingMat.data == nullptr)
        return;

    QImage cvImg  = QImage((uchar*)cuttingMat.data, cuttingMat.size().width, cuttingMat.size().height, cuttingMat.step, QImage::Format_RGB888);//0xffRRGGBB (24bit)

    auto img =  cvImg.rgbSwapped(); //BGR -> RGB
    auto pixmap = QPixmap::fromImage(img);
    ui->labelImage->setPixmap(pixmap);

    auto point = this->mapFromGlobal(QCursor::pos());

    //이미지안으로 커서가 진입했는지 체크
    if (this->rect().contains(point) == true)
        this->requestDrawGraph(cuttingMat, point);
    else
        if(d->mSaveMousePoint.isNull() == false)
        emit this->onDrawGraph(d->mSaveMousePoint, matrix);

}

void CameraWidget::requestDrawGraph(const cv::Mat& matrix, const QPoint& pos)
{
    if(pos.x() > matrix.size().width || pos.y() > matrix.size().height)
        return;

    emit this->onDrawGraph(pos, matrix);
}


void CameraWidget::mouseReleaseEvent(QMouseEvent* event)
{
    auto point = event->pos();

    if (this->rect().contains(point) == false)
        return;

    auto pixmap = ui->labelImage->pixmap();

    if(point.x() > pixmap->width() || point.y() > pixmap->height())
        return;

    d->mSaveMousePoint = point;
}


