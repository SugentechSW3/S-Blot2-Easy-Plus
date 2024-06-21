#ifndef U2BIOXMLLISPROTOCOL_H
#define U2BIOXMLLISPROTOCOL_H

#include <QObject>
#include <QtXml>

#include "Analysis/GlobalDataStruct.h"

class DataBaseCtrl;
class U2BioXmlLisProtocol : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    struct U2_BIO_XML_LIS_DATA
    {
        QString patientID;
        QString name;
        QString sex;
        QString panelName;
        QString birthDate;
    };


    explicit U2BioXmlLisProtocol(QObject *parent = nullptr);
    virtual ~U2BioXmlLisProtocol();

    void saveU2BioXmlOutData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& data, DataBaseCtrl *codeFuncDatabase);
    QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA> loadU2BioXmlInData();

private:
    void createSoftWareInfoData(QDomElement& element);
    void createSessionInfoData(QDomElement& element);
    void createInstrumentData(QDomElement& element);
    void createSourcePositionData(QDomElement& element, int stripNumber);
    void createPatientCardData(QDomElement& element, const GlobalDataStruct::AnalysisReportInfomation &result);
    void createAssayData(QDomElement& element, const GlobalDataStruct::AnalysisReportInfomation &result);

    QDomElement createGroup();
    QDomElement createParam();
    QDomText createTextData(const QString& data);
    void createData();

    bool loadXMLFile(const QString& filePath);
    bool saveXMLFile(const QString& filePath);

    QList<U2_BIO_XML_LIS_DATA> parsingU2BioXmlIn();

    QString getBandType(const QString& codeName);

    bool isSpecificData(const QString &code);
    QString calcForSpecificData(const double& intensity);
    double clacCutOFFTIgEData(const double& intensity);
private:
    QScopedPointer<privateStruct> d;
};

#endif // U2BIOXMLLISPROTOCOL_H
