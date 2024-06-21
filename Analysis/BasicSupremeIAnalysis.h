//주의 : OpenCV의 이미지 기본포맷은 RGB가아니라 BGR이다
//      따라서 RGB를 사용하려면 cvtColor로 변환이 먼저.

#ifndef BASIC_SUPREMEI_ANALYSIS_H
#define BASIC_SUPREMEI_ANALYSIS_H

#include <QObject>
#include <QScopedPointer>
#include <QDateTime>
#include <QVector>

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "GlobalDataStruct.h"


#define _USE_DYNAMIC_BAND
#define USE_EXPORT_BAND_DATA_TO_EXCEL (0)

const double DOUBLE_PI_VALUE = 3.141592;
const double DOUBLE_HALF_CIRCLE_DEGREE = 180.0;
const int FRAME_FRAGMENT_MS_TIME = 100;

class DataBaseCtrl;
class BasicSupremeIAnalysis : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

    const int CAPTURE_FRAME_COUNT = 10;//2m = 30 , 5m = 10

public:

    enum ANALYSIS_RESULT{ eNone, eRising, eFalling };


    explicit BasicSupremeIAnalysis(QObject *parent = nullptr);
    virtual ~BasicSupremeIAnalysis();

    GlobalDataStruct::AnalysisResult analysisSingleStrip(const cv::Mat& matrix, QString contentsName, int isFindBandOnly = false, bool isStaticImage = false, int stripPoint = 0);
    void analysisStart(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisStripList);

    void imageSave(const cv::Mat& matrix, QString fileName, int stripNumber, const QString& filePath = QString(""));
    void analysisRawImageSave(const cv::Mat& matrix, int stripNumber);

    static bool sortData(const GlobalDataStruct::TABLE_ITEM_ELEMENT& a, const GlobalDataStruct::TABLE_ITEM_ELEMENT& b);
    static QString resultImageSave(const cv::Mat& matrix,  QString fileName);

public slots:
    void onStop();

signals:
    void onSendData(QByteArray data);
    void onCurrentProcessing(int current);
    void onAnalysisFinished(QVector<GlobalDataStruct::AnalysisReportInfomation> infomation);
    void onAnalysisStart(bool isSuccess);

private slots:
    void onRecvAnalysisData(QByteArray recvData);
    void onMovePosition();
    void onSendNextStep();
    void onStartAnalysisThread();

private:
    inline int index2D(int x, int y, int size1D) { return x + size1D * y; }

    bool findPCSingleBand(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& contentsRecipe, const cv::Mat& stripRoiImage,
                          cv::Mat& stripRoiGrayImg, GlobalDataStruct::AnalysisResult& result, int stripPoint);

    bool findPCMultipleBand(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& contentsRecipe, const cv::Mat& stripRoiImage,
                        cv::Mat& stripRoiGrayImg, GlobalDataStruct::AnalysisResult& result, int stripPoint);

    cv::Rect HousingROItoCVRect(const GlobalDataStruct::HOUSING_ROI &housingROI);
    cv::Rect GuideLineToROItoCVRect(const GlobalDataStruct::GUIDE_LINE& guideLine);


    bool FindBoxCarCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS& recipe, const cv::Mat &imgGray, int x, int &y, int xRange);

    bool FindPcYCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat& imgGray,
                       cv::Point &pcPosA, cv::Point &pcPosB, int& threshold, int stripPoint, int startPoint, bool isRotated, bool isBackToFront);

    bool FindPcXCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat& imgGray,
                       cv::Point &pcPosA, int stripPoint, int startPoint, int threshold, bool isRotated, bool isBackToFront);

    bool FindPCCenter(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat& imgGray,
                      cv::Point &pcPosA, cv::Point &pcPosB, int& threshold, int stripPoint, int startPoint, bool isRotated, bool isBackToFront,
                      int pcXThres);


    QVector<cv::Rect> setBandPosition(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Point &pcPosA);

    void GetBandRect(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, QVector<cv::Rect>& rectBand);

    void setCorrectBandRect(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat& imgGray, GlobalDataStruct::AnalysisResult& result);
    bool calcFitedStripSize(const cv::Mat& imgU8, const GlobalDataStruct::AnalysisResult &result, int &width, int &height);
    void GetTwoEdgePoint(const cv::Mat &imgU8, const cv::Rect &dynamicBand, int nWidth, int nHeight,
                         int &forwardEdgePoint, int &backEdgePoint, int &forwardRisingFalling, int &backRisingFalling);
    void BandBoxcarLineFind(int width, const QVector<uchar> &lineData, int &nForwardEdge, int &nBackEdge, int &nForwardRisingFalling, int &nBackRisingFalling);
    void findEdgePoint(cv::Mat imgU8, GlobalDataStruct::AnalysisResult& result, const int width, const int height);

    void copyBoxCar(GlobalDataStruct::AnalysisResult& result);

    int CalcHistogram_Ver2(int nData, const QVector<int> &inputData);

    bool rectComp(const cv::Rect& a, const cv::Rect& b);
    bool rectCompDesc(const cv::Rect& a, const cv::Rect& b);
    int  findCenterPosition(QVector<int> posList);

    void calcIntensity(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, GlobalDataStruct::AnalysisResult &result, QVector<cv::Mat> &imageList);
    void CalibrationOfAbnormalBackgroundData(GlobalDataStruct::AnalysisResult& result);

    QVector<cv::Mat> getCaptureROIGrayImage(cv::Rect roi, int captureCount);
    QVector<cv::Mat> getStaticROIGrayImage(const cv::Mat& grayImg, const double rotation, const int flip, int captureCount);

    QVector<cv::Mat> calcBandValue(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, GlobalDataStruct::AnalysisResult& result, const cv::Mat &roiGrayImage,
                       QVector<double>& intensityValue, QVector<double>& bandValue, QVector<double>& backgroundValue);

    void clacBackGroundAverageData(const GlobalDataStruct::AnalysisResult &result, const cv::Mat &roiGrayImage);

    QVector<cv::Mat> calcBackGroundROIAndIntensity(const int index, const cv::Mat &roiGrayImage, const GlobalDataStruct::AnalysisResult &result,
                                                   QVector<double>& intensityValue, QVector<double>& bandValue, QVector<double>& backgroundValue);

    int clacAverageImageData(const QVector<cv::Mat> matList);
    bool isSmudgedBackground(const cv::Mat &roiGrayImage, bool isPrev);
    bool isCorrectBackGroundRange(const cv::Rect& targetRect, const cv::Mat &originalImage);
    cv::Rect getPrevBackGroundRect(const cv::Rect& rect);
    cv::Rect getNextBackGroundRect(const cv::Rect& rect);

    void GetSortBandIntensityValue(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Mat &boxcarRoi,
                               QVector<double> &intensityValue, QVector<double> &bandValue, QVector<double> &backgroundValue);

    double getSectionAverage(const cv::Mat& image);
    double getAreaBackGroundValue(const cv::Rect& rect, const cv::Mat& roiGrayImage, QVector<cv::Mat> &clacMat);
    void GetAreaBandIntensityValue(const cv::Mat &boxcarRoi, const double backGroundValue,
                                   QVector<double> &intensityValue, QVector<double> &bandValue, QVector<double> &backgroundValue);

    double GetSectionSum(const cv::Mat& data);
    double GetSectionSum(const QVector<uchar> &pData, int nStart, int nEnd);

    bool CalRotationAngle(const cv::Mat& stripGrayRoiImg, double& rotation, QVector<cv::Rect> &bandRect, int stripPoint, int yThreshold);

    void calcRotationFromRadian(const cv::Point2d &posFindA, const cv::Point2d &posFindB, double& rotation);
    void drawCircleForCeilingEdgeAndSaveLogImage(const cv::Point2d &posFindA, const cv::Point2d &posFindB, const cv::Mat& stripRoiBinaryImg
                                                 , int stripPoint, int bandTotalCount);

    void yLineParticleEraseProc(cv::Mat& srcImage, const int removalPaticle, const int averagePaticle, const int threshold );

    cv::Point2d findFrontCeilingEdge(const cv::Mat &stripRoiBinaryImg, QVector<cv::Rect> &bandRect);
    cv::Point2d findBackCeilingEdge(const cv::Mat& stripRoiBinaryImg, QVector<cv::Rect>& bandRect);


    void findStripNextIdx(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisStripList, int& nextIdx);
    void calcStripCount(const QVector<GlobalDataStruct::AnalysisReportInfomation>& analysisStripList, int& count);
    void doAnalysis();
    bool isvalidCurrentIndexStrip();
    bool isValidBandData(const GlobalDataStruct::AnalysisResult &result);
    bool isValidPCData(const GlobalDataStruct::AnalysisResult& result);
    void getCurrentIndexAnalysisResultData();

    cv::Rect calcRect(int pcPosAx, int pcPosAy, int boxcarWidget, int boxcarHeight);

    QVector<cv::Rect> calcBandBlock(const GlobalDataStruct::CAMERA_STRIP_SETTINGS &recipe, const cv::Point& pcPosAFront, const cv::Point& pcPosABack, double bandCount);
    void updateFailDyanmicBand(GlobalDataStruct::AnalysisResult& result);

    void drawRectForLogging(const GlobalDataStruct::AnalysisResult &result, const cv::Rect &roi, int stripIdx);
private:
    QScopedPointer<privateStruct> d;
};




#endif // BASICSUPREMEIANALYSIS_H
