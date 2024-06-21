#ifndef ConfigInformation_H
#define ConfigInformation_H

#include <QObject>
#include <QScopedPointer>
#include <QDataStream>

#include "Common/CUtilCommon.h"
#include "Common/Io/INIFileControls/GenericIniDataControls.h"

#include "GlobalDataStruct.h"

class ConfigInformation : public QObject
{
private:
    const int MAX_STRIP_COUNT = 12;
    Q_OBJECT
    Q_DISABLE_COPY(ConfigInformation)

    struct privateStruct;
    explicit ConfigInformation(QObject *parent = nullptr);
    virtual ~ConfigInformation();

public:
    static ConfigInformation* getInstance()
    {
        static ConfigInformation instance;
        return &instance;
    }

    enum STEP_LIST
    {
        STEP_LIST_NONE,
        STEP_LIST_WETTING,
        STEP_LIST_DISPENSE,
        STEP_LIST_WASHING1,
        STEP_LIST_ANTIBODY,
        STEP_LIST_WASHING2,
        STEP_LIST_ENZYME,
        STEP_LIST_WASHINGDW,
        STEP_LIST_SUBSTRATE,
        STEP_LIST_DRY,
        STEP_LIST_ANALYSIS, //0x0A
        STEP_LIST_END = 0xFE
    };

    enum PROCESS_LIST
    {
        PROCESS_LIST_NONE,
        PROCESS_LIST_PRIME,
        PROCESS_LIST_ROLL_BACK,
        PROCESS_LIST_DISPENSE,
        PROCESS_LIST_ASPIRATION,
        PROCESS_LIST_INCUBATION,
        PROCESS_LIST_BUZZER,
        PROCESS_LIST_DRY,
        PROCESS_LIST_PAUSE,
        PROCESS_LIST_SAMPLE_DISPENSE,
        PROCESS_LIST_AUTO_CLEAN,
        PROCESS_LIST_PRIME_D,
        PROCESS_LIST_ROLL_BACK_D,
        PROCESS_LIST_DISPENSE_D
    };

    enum SEMI_AUTO_PROCESS_COMMAND_D
    {
        SEMI_AUTO_PROCESS_COMMAND_D_NONE = 0x00,
        SEMI_AUTO_PROCESS_COMMAND_D_SINGLE,
        SEMI_AUTO_PROCESS_COMMAND_D_DUAL,
        SEMI_AUTO_PROCESS_COMMAND_A_DUAL = 0x04,
        SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL = 0x12
    };

    enum CONFIG_FILE_NAMES
    {
        NULL_CONFIG_NAME,
        Config,
        HousingROI,
        CameraLightSetting,
        GuideLine,
        Intensity,
        PaticleRemoval,
        BoxCar
    };

    enum ALLERGY_PANEL_LISTS
    {
        ALLERGY_PANEL_LISTS_NONE,
        ALLERGY_PANEL,
        ALLERGY_END_OF_PANEL
    };

    enum class STANDARD_CONTETNS_LIST
    {
        ALLERGY,
        END_OF_STANDARD_CONTETNS_LIST
    };

    enum class SUGENTECH_CONTENTS_LIST
    {
        ALLERGY = (int)STANDARD_CONTETNS_LIST::ALLERGY,
        SUGENTECH_END_OF_CONTENTS
    };


    enum SEQUENCE_STATE
    {
        SEQUENCE_STATE_END,
        SEQUENCE_STATE_START
    };

    enum STRIP_COLOR
    {
        STRIP_COLOR_NONE,
        STRIP_COLOR_GRAY,
        STRIP_COLOR_PINK,
        STRIP_COLOR_GREEN
    };

    Q_ENUM(STEP_LIST)
    Q_ENUM(PROCESS_LIST)
    Q_ENUM(CONFIG_FILE_NAMES)
    Q_ENUM(ALLERGY_PANEL_LISTS)
    Q_ENUM(SUGENTECH_CONTENTS_LIST)
    Q_ENUM(STANDARD_CONTETNS_LIST)
    Q_ENUM(SEQUENCE_STATE)

    bool start();

    void aginDeclareContentCodeList();
    GlobalDataStruct::HOUSING_ROI getHousingROI();
    void setHousingROI(const GlobalDataStruct::HOUSING_ROI& roi);

    GlobalDataStruct::INTENSITY getIntensity();
    void setIntensity(const GlobalDataStruct::INTENSITY& data);

    GlobalDataStruct::PATICLE_REMOVAL getPaticleRemoval();
    void setPaticleRemoval(const GlobalDataStruct::PATICLE_REMOVAL& data);

    GlobalDataStruct::USE_BOXCAR_ANALYSIS getBoxCarAnalysis();
    void setBoxCarAnalysis(const GlobalDataStruct::USE_BOXCAR_ANALYSIS& data);

    GlobalDataStruct::USE_BOXCAR_BAND getBoxCarBandGap();
    void setBoxCarBandGap(const GlobalDataStruct::USE_BOXCAR_BAND &data);

    GlobalDataStruct::USE_MULTIPLE_BAND getMultiBandSetting();
    void setMultiBandSetting(const GlobalDataStruct::USE_MULTIPLE_BAND& data);

    GlobalDataStruct::CAMERA_LIGHT_SETTING getCameraLightSetting();
    void setCameraLightSetting(const GlobalDataStruct::CAMERA_LIGHT_SETTING& data);

    GlobalDataStruct::GUIDE_LINE getGuideLine();
    void setGuideLine(const GlobalDataStruct::GUIDE_LINE& data);

    void setUserPassword(const QString& password);
    QString getUserPassword();

    int getFlip();
    void setFlip(const int data);

    int getLastSelectedSequence();
    void setLastSelectedSequence(int sequenceNumber);

    double getRotation();
    void setRotation(const double data);

    GlobalDataStruct::CONTENTS_STATUS getStatus();

    bool getIsUseMultiBand();
    void setIsUseMultiBand(bool isUse);

    int getStripCount();

    QStringList getPanelList(QString contentsName);
    QStringList getPanelList();
    QMap<QString,QString> getContentPanelCodes(const QString& contentName);
    int getCurrentContentPanelIndex(const QString& panelName);

    QMap<QString,int> getPanelMapFromEnum(const QString& panelName) const;
    QStringList getPanelListFromContentsName(const QString& contentsName) ;

    QVector<GlobalDataStruct::SEQUENCE_STRUCT> loadSequenceFromFile(const QString& contentsName, const QString& fullPath = (""));
    QVector<GlobalDataStruct::SEQUENCE_STRUCT> saveSequence(const QString& contentsName, const QVector<GlobalDataStruct::SEQUENCE_STRUCT> &writeData, const QString& fullPath =(""));
    QVector<GlobalDataStruct::SEQUENCE_STRUCT> getSequenceList(const QString &contentsName) const;

    int getContentsIdx(QString contentsName);
    int getCurrentContentIdx();
    QString getCurrentContents();
    void setCurrentContents(const QString& contents);

    GlobalDataStruct::CAMERA_STRIP_SETTINGS getStripSettings(const QString& contentsName);

    QSize getBaseResolution();
    void setBaseResolution(QSize size);

    QRect getBaseROI();
    void setBaseROI(QRect roi);

    void saveLastConfigureData();
    void loadLastConfigureData();

    void saveContentsTunningINIData(const QString &contentsName);

    QStringList getAllergyPanelList() const;
    QVector<QString> getStripNameList(const QString &contents, const QString& panelName);    
    QVector<QString> getCurrentStripNameList(const QString& panelName);
    QStringList getCurrentStripCodeList(const QString& panelName);
    QVector<QString> getCurrentStripGroupList(const QString& panelName);
    QVector<QString> getCurrentStripSecondNameList(const QString& panelName);
    QStringList getContentsList() const;
    QVector<bool> getIsStripBehindeList(const QString& panelName);
    QStringList getHiddenBandList(const QString& panelName);
    QString getAllergyNameFromCode(const QString& code) const;

    QString getCompanyName() { return "SUGENTECH";  }
    void sendDataEnd();

    int getCurrentStripPoint();
    void setCurrentStripPoint(int point);

    int getCurrentASPBathPosition();
    void setCurrentASPBathPosition(int position);

    GenericIniDataControls::FIXED_USER_DATA getFixedUser();
    void setFixedUser(const GenericIniDataControls::FIXED_USER_DATA& fixedUser);

    int getQRTriplePosition();
    void setQRTriplePosition(const int& x);

    QRect getQRStripFixedROI();
    QRect getQRTripleFixedROI();

    bool isAdminMode();
    void setAdminMode(bool isAdmin);

    bool isOffLine();
    void setOffLine(bool isOffLine);

    void loadOfflineData();
    void loadAllergyDatas();
    QVector<GlobalDataStruct::AnalysisReportInfomation> getLastAnalysisData();
    void setLastAnalysisData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& lastData);

    ConfigInformation::STRIP_COLOR getAllergyColor(int panelIdx);
    ConfigInformation::STRIP_COLOR getAllergyColor(const QString& panelName);

    GlobalDataStruct::LANGUAGE_LIST changeCurrentLanguage(const GlobalDataStruct::LANGUAGE_LIST& language);

    QString getCurrentLanguagetoString();
    QString getLanguagetoString(const GlobalDataStruct::LANGUAGE_LIST& language);


    void setExportChangeLanguage(GlobalDataStruct::LANGUAGE_LIST language);
    GlobalDataStruct::LANGUAGE_LIST getExportChangeLanguage();

    void setExportChangeSecondLanguage(GlobalDataStruct::LANGUAGE_LIST language);
    GlobalDataStruct::LANGUAGE_LIST getExportChangeSecondLanguage();

    void setSelectSequence(QString seq){mSelectSeq = seq;}
    QString getSelectSequence(){return mSelectSeq;}

    static QString getContentsNameFromEnum(int enumIdx);
    static int getContentsEnumFromName(const QString& contentsName);
    static QString convertQEnumToQString(const int type, const QString enumNames);
    static int convertQStringToQEnum(const QString &key, const QString enumNames);
    static QString convertStepEnumToStepName(int stepNumber);
    static ConfigInformation::STEP_LIST convertStepNameToStepEnum(const QString& stepName);
    static int getPanelIndex(const QString &contentsName, const QString& panelName);
    static GlobalDataStruct::LANGUAGE_LIST getDefaultLanguage();

    static QString getSavedLanguageStr();
    static QString getLanguagetoTranslationFileNameString(const GlobalDataStruct::LANGUAGE_LIST& language);

    static void setCurrentLanguage(GlobalDataStruct::LANGUAGE_LIST language);
    static QByteArray makeSequenceInputMode(bool isStart);
    static QByteArray makeSequenceTotalStep(uchar totalStep);
    static QByteArray makeSequenceSave(uchar smb);
    static QByteArray makeSequenceID(uchar smb, ushort sequenceID);
    static QByteArray makeStepNumber(uchar stepNumber);
    static QByteArray makeProcessNumber(ushort processNumber, ushort processIndex);
signals:
    void onChangedHousingROI(GlobalDataStruct::HOUSING_ROI configData);
    void onChangedIntensity(GlobalDataStruct::INTENSITY configData);
    void onChangedPaticleRemoval(GlobalDataStruct::PATICLE_REMOVAL configData);
    void onChangedCameraLightSetting(GlobalDataStruct::CAMERA_LIGHT_SETTING configData);
    void onChangedGuideLine(GlobalDataStruct::GUIDE_LINE configData);
    void onChangedFlip(int configData);
    void onChangedRotation(double configData);
    void onChangedUseBoxCarAnalaysis(GlobalDataStruct::USE_BOXCAR_ANALYSIS configData);
    void onChangedUseBoxCarBandGap(GlobalDataStruct::USE_BOXCAR_BAND configData);
    void onChangedUseMultiBand(GlobalDataStruct::USE_MULTIPLE_BAND configData);
    void onChangedIsUseMultiBand(bool status);
    void onChangedSelectedContents(QString contentsName);
    void onChangedStripPoint(int point);
    void onChangedStripFixedPostion(GlobalDataStruct::STRIP_FIXED_POSITON position);
    void onChangedCurrentLanguage(GlobalDataStruct::LANGUAGE_LIST language);

    void onEmptySelectedContents();
    void onMissingSequenceFile(QString contentsName);
    void onContentsListMissMatch(QString contentsName);
    void onTuningDataMissing(QString contentsName);

    void startConfigDownLoad();
    void downloadGlobalData();
    void downloadAnalysisData(int current);
    void endConfigProtocol(bool isSequencing);

public slots:
    void onFullSequenceProcess();

private:
    void initString();
    void initConfig();
    void initLanguage();
    void checkReconnection();

    void loadAllergyStripDatas();
    void loadAllergyNames();
    void loadAllergyGroups();
    void loadAllergySecondNames();
    void loadAllConentsData();

    void readHousingROI(QString contentsName);
    void readIntensity(QString contentsName);
    void readPaticleRemoval(QString contentsName);
    void readBoxCarAnalysisSetting(QString contentsName);
    void readMultiBand(const QString& contentsName);
    void readBandGaps(QString contentsName, int count);
    void readCameraLightSetting();
    void readGuideLine();
    void readFlip();
    void setPanelList(QString contentsName);

    void readStripSetting(QString contentsName);
    void readSugentechContentsList();

    void setAllergySampleAndStripCount();

    //contents dependent
    void writeHousingROI(QString contentsName);
    void writeIntensity(QString contentsName);
    void writePaticleRemoval(QString contentsName);
    void writeBoxCarAnalysisSetting(QString contentsName);
    void writeBoxCarBandGapSetting(QString contentsName);
    void writeMultiBandSetting(QString contentsName);
    void writeUseMultiBand(QString contentsName);

    //ini interface
    GlobalDataStruct::HOUSING_ROI readHousingROIIniData(const QString& contentsName);
    GlobalDataStruct::INTENSITY readIntensityIniData(const QString& contentsName);
    GlobalDataStruct::PATICLE_REMOVAL readPaticleRemovalIniData(const QString& contentsName);
    GlobalDataStruct::USE_BOXCAR_ANALYSIS readBoxcarAnalysisIniData(const QString& contentsName);
    QVector<int> readBoxcarBandGapIniData(const QString& contentsName);
    GlobalDataStruct::USE_MULTIPLE_BAND readMultiBandIniData(const QString& contentsName);
    bool readUseMultiBandIniData(const QString& contentsName);

    void writeHousingROIIniData(const QString& contentsName, const GlobalDataStruct::HOUSING_ROI &roi);
    void writeIntensityIniData(const QString& contentsName, const GlobalDataStruct::INTENSITY& intensity);
    void writePaticleRemovalIniData(const QString& contentsName, const GlobalDataStruct::PATICLE_REMOVAL& removal);
    void writeBoxcarAnalysisIniData(const QString& contentsName, const GlobalDataStruct::USE_BOXCAR_ANALYSIS& boxcarSetting);
    void writeBoxcarBandGapIniData(const QString& contentsName, const QVector<int>& bandGap);
    void writeMultiBandIniData(const QString& contentsName, const GlobalDataStruct::USE_MULTIPLE_BAND& multiBand);
    void writeUseMultiBandIniData(const QString& contentsName, bool isUseMultiBand);

    //contents independent
    void writeCameraLightSetting();
    void writeGuideLine();
    void writeFlip();

    void readSeqeunce(const QString& contents);
    void readSeqeunceCount();
    void requestSequenceData(int idx);

    void setContentsList(const QStringList& list);
    void setSequenceList(const QString& contentsName, const QVector<GlobalDataStruct::SEQUENCE_STRUCT>& sequenceList);

    void parsingGlobalData(const QByteArray& recvData);
    void parsingAnalysis(const QByteArray& recvData, const QString& contentsName, int contentsIdx);
    void parsingBoxCar(const QByteArray& recvData, const QString& contentsName);

    QString writeReadProtocol(const QString& contentsName, bool, uchar cmd);
    QString writeWriteProtocol(const QString& contentsName, bool, uchar cmd, uint parameter);
    QString writeEndProtocol();

private slots:
    void onRecvSequenceData(QByteArray recvData);
    void onRecvData(QByteArray recvData);

    void onWriteSeqeuenceQueue();
    void onReadSequenceData();
    void onReadTuningData();

private:
    QScopedPointer<privateStruct> d;
    QString mSelectSeq;
};


#endif // ConfigInformation_H
