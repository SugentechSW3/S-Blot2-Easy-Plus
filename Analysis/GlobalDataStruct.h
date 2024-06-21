#ifndef GLOBALDATASTRUCT_H
#define GLOBALDATASTRUCT_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QVariantMap>
#include <QDataStream>

#include <opencv2/core.hpp>


class GlobalDataStruct : public QObject
{
private:
    Q_OBJECT

public:

    struct HOUSING_ROI
    {
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;
    };

    struct GUIDE_LINE
    {
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;
    };

    struct STRIP_FIXED_POSITON
    {
        int stripPosition1 = 0;
        int stripPosition2 = 0;
    };

    struct CAMERA_LIGHT_SETTING
    {
        int expousre = 0;
        int gain = 0;
        int brightness = 0;
        int contrast = 0;
        int saturation = 0;
        int sharpness = 0;
        int whitebalance = 0;
        int hue = 0;
        int gamma = 0;
        int focus = 0;
    };

    struct INTENSITY
    {
        int paticle = 0;
        int band = 0;
        int background = 0;
        int white = 0;
    };

    struct PATICLE_REMOVAL
    {
        int removalPaticle = 0;
        int averagePaticle = 0;
        int threshold = 0;
    };

    struct ROTATION_FLIP
    {
        int flip = 0;
    };

    struct USE_BOXCAR_ANALYSIS
    {
        int width = 0;
        int height = 0;
        int xStart = 0;
        int xRange = 0;
        int pcThres = 0;
        int housingThres = 0;
        int boxCarbandCount = 0;
    };

    struct USE_MULTIPLE_BAND
    {
        int bandBlock2XStart = 0;
        int bandBlock3XStart = 0;
        int bandBlock4XStart = 0;

        int mutiplePcThres = 0;

        int pcXGap = 0;
        int bandBlock2XGap = 0;
        int bandBlock3XGap = 0;
        int bandBlock4XGap = 0;
    };

    struct USE_BOXCAR_BAND
    {
        QVector<int> boxCarBandGap;
    };


    struct CAMERA_STRIP_SETTINGS
    {
        HOUSING_ROI housingROI;
        INTENSITY intensity;
        PATICLE_REMOVAL particleRemoval;
        USE_BOXCAR_BAND boxCarBandGap;
        USE_BOXCAR_ANALYSIS boxCarAnalaysis;
        USE_MULTIPLE_BAND multipleBandData;
    };

    struct CAMERA_OPTION_SETTINGS
    {
        GUIDE_LINE guideLine;
        CAMERA_LIGHT_SETTING cameraLightSetting;
        double rotation = 0.0;
        int flip = 0;
    };

    struct TABLE_ITEM_ELEMENT
    {
        int stripNumber;

        QString patientID;
        QString name;
        QString contentsName;
        QString panelName;

        bool isSuccess;

        QString applicant;
        QString analyst;
        QString reviewer;
        QString sex;
        QString age;
        QString phone;
        QString eMail;
    };

    struct PROCESS_DATA
    {
        int processNumber;
        QVariantMap data;
    };

    struct SEQUENCE_STRUCT
    {
        int stepNumber;
        QVector<PROCESS_DATA> processData;
    };

    struct CONTENTS_STATUS
    {
        QVector<SEQUENCE_STRUCT> sequenceList;
        CAMERA_STRIP_SETTINGS stripSettings;
        QStringList panelList;
        QMap<QString, QVector<QString>> panleStripNameList;
        QMap<QString, QStringList> panleStripCodeList;
        QMap<QString, QVector<QString>> panleStripGroupList;
        QMap<QString, QVector<QString>> panleStripSecondNameList;
        QMap<QString, QString> mContentsNameList;
        QMap<QString, QString> mContentsGroupList;
        QMap<QString, QString> mContentsSecondNameList;

        QMap<QString,QVector<bool>> mIsBeHindCodeList;
        int stripCount = 0;
        bool isMultiBand = false;        
    };
    struct PANLE_STRIP_CODE_STRUCT
    {
        QString pannelName;
        QStringList bandCodeList;
        QStringList behindeCodeList;
    };

    struct FORMULA_PARAMETER
    {
        double a = 0.0;
        double b = 0.0;
        double c = 0.0;
        double d = 0.0;
        double e = 0.0;
        double f = 0.0;
        double g = 0.0;
    };

    struct FUNC_REL_PARAMETER
    {
        QString lotNumber;
        QStringList functionList;
    };

    struct rectBandResult
    {
        cv::Rect rectBand;
        cv::Rect rectDynamicBand;
        bool isFail = false;
    };

    struct AnalysisResult
    {
        bool isSuccess = false;
        int frontPMXPosition = 0;
        int backPMXPosition = 0;
        double tigeConcentrationValue = 0.0;
        cv::Mat image;

        QString errorReason;
        QString resultImagePath;
        QString comment;

        QVector<double> rawIntensityValue; //데이터를 기록하거나 DB에서 읽기쓰기를 실행할때는 rawData를 사용
        QVector<double> bandValue;
        QVector<double> backgroundValue;

        QVector<QString> resultValue;
        QVector<QString> classType;
        QVector<int> pcPoint;
        QVector<GlobalDataStruct::rectBandResult> rectBandResult;
    };

    struct AnalysisReportInfomation
    {
        QString uniqueID;
        QString lotNumber;
        QDateTime dateTime;
        TABLE_ITEM_ELEMENT element;
        AnalysisResult result;
        bool isQRSkipRead = false;
    };

    struct QR_RELATION_DATA_STRUCT
    {
        QString lotNumber;
        QVector<QString> bandFormulaParameter;
    };

    struct QR_LOTOPTIONAL_DATA_STRRUCT
    {
        QString lotNumber;
        int cutoff;
        QString expDate;
    };

    struct QR_FORMULA_DATA_STRUCT
    {
        QString newFunctionNumber;
        double functionDataA;
        double functionDataB;
        double functionDataC;
        double functionDataD;
        double functionDataE;
        double functionDataF;
        double functionDataG;
    };

    struct QR_STRIP_DATA_STRUCT
    {
        QString contentsName;
        QString panelName;
        QString deviceType;
        QString yearOfManufacture;
        QString number;
    };

    struct QR_EXP_DATA_STRUCT
    {
        QString lotNumber;
        QString date;
        int cutoff;
    };

    struct QUERY_CONDITION_DATA
    {
        QDate fromDate;
        QDate toDate;
        QString patientID;
        QString patientName;
        QString patientPanel;
    };

    struct OPINION_DATA
    {
        QString title;
        QString opinion;
    };

    struct HIGH_LIGHT_DATA
    {
        bool class0 = false;
        bool class1 = false;
        bool class2 = false;
        bool class3 = false;
        bool class4 = false;
        bool class5 = false;
        bool class6 = false;
    };

    enum LANGUAGE_LIST
    {
        LANGUAGE_LIST_ENG,
        LANGUAGE_LIST_KOR,
        LANGUAGE_LIST_FR,
        LANGUAGE_LIST_AR,
        LANGUAGE_LIST_RO,
        LANGUAGE_LIST_RU,
        LANGUAGE_LIST_DE,
        LANGUAGE_LIST_BD,
        LANGUAGE_LIST_COUNT
    };


    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::PROCESS_DATA & object)
    {
        arch << object.data;
        arch << object.processNumber;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::PROCESS_DATA & object)
    {
        arch >> object.data;
        arch >> object.processNumber;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::SEQUENCE_STRUCT & object)
    {
        arch << object.stepNumber;
        arch << object.processData;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::SEQUENCE_STRUCT & object)
    {
        arch >> object.stepNumber;
        arch >> object.processData;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::HOUSING_ROI & object)
    {
        arch << object.x1;
        arch << object.x2;
        arch << object.y1;
        arch << object.y2;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::HOUSING_ROI & object)
    {
        arch >> object.x1;
        arch >> object.x2;
        arch >> object.y1;
        arch >> object.y2;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::INTENSITY & object)
    {
        arch << object.band;
        arch << object.white;
        arch << object.paticle;
        arch << object.background;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::INTENSITY & object)
    {
        arch >> object.band;
        arch >> object.white;
        arch >> object.paticle;
        arch >> object.background;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::PATICLE_REMOVAL & object)
    {
        arch << object.threshold;
        arch << object.averagePaticle;
        arch << object.removalPaticle;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::PATICLE_REMOVAL & object)
    {
        arch >> object.threshold;
        arch >> object.averagePaticle;
        arch >> object.removalPaticle;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::USE_BOXCAR_BAND & object)
    {
        arch << object.boxCarBandGap;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::USE_BOXCAR_BAND & object)
    {
        arch >> object.boxCarBandGap;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::USE_BOXCAR_ANALYSIS & object)
    {
        arch << object.width;
        arch << object.height;
        arch << object.xRange;
        arch << object.xStart;
        arch << object.pcThres;
        arch << object.housingThres;
        arch << object.boxCarbandCount;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::USE_BOXCAR_ANALYSIS & object)
    {
        arch >> object.width;
        arch >> object.height;
        arch >> object.xRange;
        arch >> object.xStart;
        arch >> object.pcThres;
        arch >> object.housingThres;
        arch >> object.boxCarbandCount;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::USE_MULTIPLE_BAND & object)
    {
        arch << object.pcXGap;
        arch << object.bandBlock4XStart;
        arch << object.mutiplePcThres;
        arch << object.bandBlock4XGap;
        arch << object.bandBlock2XStart;
        arch << object.bandBlock3XStart;
        arch << object.bandBlock2XGap;
        arch << object.bandBlock3XGap;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::USE_MULTIPLE_BAND & object)
    {
        arch >> object.pcXGap;
        arch >> object.bandBlock4XStart;
        arch >> object.mutiplePcThres;
        arch >> object.bandBlock4XGap;
        arch >> object.bandBlock2XStart;
        arch >> object.bandBlock3XStart;
        arch >> object.bandBlock2XGap;
        arch >> object.bandBlock3XGap;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::GUIDE_LINE & object)
    {
        arch << object.x1;
        arch << object.x2;
        arch << object.y1;
        arch << object.y2;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::GUIDE_LINE & object)
    {
        arch >> object.x1;
        arch >> object.x2;
        arch >> object.y1;
        arch >> object.y2;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::STRIP_FIXED_POSITON & object)
    {
        arch << object.stripPosition1;
        arch << object.stripPosition2;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::STRIP_FIXED_POSITON & object)
    {
        arch >> object.stripPosition1;
        arch >> object.stripPosition2;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::CAMERA_LIGHT_SETTING & object)
    {
        arch << object.hue;
        arch << object.gain;
        arch << object.focus;
        arch << object.gamma;
        arch << object.contrast;
        arch << object.expousre;
        arch << object.sharpness;
        arch << object.brightness;
        arch << object.saturation;
        arch << object.whitebalance;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::CAMERA_LIGHT_SETTING & object)
    {
        arch >> object.hue;
        arch >> object.gain;
        arch >> object.focus;
        arch >> object.gamma;
        arch >> object.contrast;
        arch >> object.expousre;
        arch >> object.sharpness;
        arch >> object.brightness;
        arch >> object.saturation;
        arch >> object.whitebalance;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::CAMERA_STRIP_SETTINGS & object)
    {
        arch << object.intensity;
        arch << object.housingROI;
        arch << object.multipleBandData;
        arch << object.boxCarBandGap;
        arch << object.boxCarAnalaysis;
        arch << object.particleRemoval;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::CAMERA_STRIP_SETTINGS & object)
    {
        arch >> object.intensity;
        arch >> object.housingROI;
        arch >> object.multipleBandData;
        arch >> object.boxCarBandGap;
        arch >> object.boxCarAnalaysis;
        arch >> object.particleRemoval;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::CAMERA_OPTION_SETTINGS & object)
    {
        arch << object.flip;
        arch << object.rotation;
        arch << object.guideLine;
        arch << object.cameraLightSetting;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::CAMERA_OPTION_SETTINGS & object)
    {
        arch >> object.flip;
        arch >> object.rotation;
        arch >> object.guideLine;
        arch >> object.cameraLightSetting;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::CONTENTS_STATUS & object)
    {
        arch << object.panelList;
        arch << object.isMultiBand;        
        arch << object.stripCount;
        arch << object.sequenceList;
        arch << object.stripSettings;


        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::CONTENTS_STATUS & object)
    {
        arch >> object.panelList;
        arch >> object.isMultiBand;
        arch >> object.stripCount;
        arch >> object.sequenceList;
        arch >> object.stripSettings;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::FORMULA_PARAMETER & object)
    {
        arch << object.a;
        arch << object.b;
        arch << object.c;
        arch << object.d;
        arch << object.e;
        arch << object.f;
        arch << object.g;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::FORMULA_PARAMETER & object)
    {
        arch >> object.a;
        arch >> object.b;
        arch >> object.c;
        arch >> object.d;
        arch >> object.e;
        arch >> object.f;
        arch >> object.g;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::TABLE_ITEM_ELEMENT & object)
    {
        arch << object.age;
        arch << object.sex;
        arch << object.name;
        arch << object.eMail;
        arch << object.phone;
        arch << object.analyst;
        arch << object.reviewer;
        arch << object.isSuccess;        
        arch << object.applicant;
        arch << object.panelName;
        arch << object.patientID;
        arch << object.stripNumber;
        arch << object.contentsName;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::TABLE_ITEM_ELEMENT & object)
    {
        arch >> object.age;
        arch >> object.sex;
        arch >> object.name;
        arch >> object.eMail;
        arch >> object.phone;
        arch >> object.analyst;
        arch >> object.reviewer;
        arch >> object.isSuccess;
        arch >> object.applicant;
        arch >> object.panelName;
        arch >> object.patientID;
        arch >> object.stripNumber;
        arch >> object.contentsName;

        return arch;
    }

    friend QDataStream & operator << (QDataStream &arch, const GlobalDataStruct::OPINION_DATA & object)
    {
        arch << object.title;
        arch << object.opinion;

        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, GlobalDataStruct::OPINION_DATA & object)
    {
        arch >> object.title;
        arch >> object.opinion;

        return arch;
    }

};

Q_DECLARE_METATYPE(QVector<GlobalDataStruct::AnalysisReportInfomation>)
Q_DECLARE_METATYPE(GlobalDataStruct::PROCESS_DATA)
Q_DECLARE_METATYPE(GlobalDataStruct::SEQUENCE_STRUCT)
Q_DECLARE_METATYPE(GlobalDataStruct::HOUSING_ROI)
Q_DECLARE_METATYPE(GlobalDataStruct::INTENSITY)
Q_DECLARE_METATYPE(GlobalDataStruct::PATICLE_REMOVAL)
Q_DECLARE_METATYPE(GlobalDataStruct::USE_BOXCAR_BAND)
Q_DECLARE_METATYPE(GlobalDataStruct::USE_BOXCAR_ANALYSIS)
Q_DECLARE_METATYPE(GlobalDataStruct::GUIDE_LINE)
Q_DECLARE_METATYPE(GlobalDataStruct::CAMERA_LIGHT_SETTING)
Q_DECLARE_METATYPE(GlobalDataStruct::CAMERA_STRIP_SETTINGS)
Q_DECLARE_METATYPE(GlobalDataStruct::CAMERA_OPTION_SETTINGS)
Q_DECLARE_METATYPE(GlobalDataStruct::CONTENTS_STATUS)
Q_DECLARE_METATYPE(GlobalDataStruct::STRIP_FIXED_POSITON)
Q_DECLARE_METATYPE(GlobalDataStruct::USE_MULTIPLE_BAND)
Q_DECLARE_METATYPE(GlobalDataStruct::FORMULA_PARAMETER)
Q_DECLARE_METATYPE(GlobalDataStruct::OPINION_DATA)
Q_DECLARE_METATYPE(GlobalDataStruct::QR_LOTOPTIONAL_DATA_STRRUCT)



#endif // GLOBALDATASTRUCT_H
