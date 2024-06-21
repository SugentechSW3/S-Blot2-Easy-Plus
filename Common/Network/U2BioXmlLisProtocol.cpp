#include <QFileDialog>
#include <QDebug>
#include "U2BioXmlLisProtocol.h"

#include "Analysis/ConfigInformation.h"
#include "Analysis/ClassHelper.h"

#include "DataBase/DataBaseCtrl.h"

#include "Common/CUtil.h"

USING_SUGENTECH

struct U2BioXmlLisProtocol::privateStruct
{
    QDomDocument mDoc;
    QVector<GlobalDataStruct::AnalysisReportInfomation> mOutData;
    DataBaseCtrl* mDatabase = nullptr;
};

U2BioXmlLisProtocol::U2BioXmlLisProtocol(QObject *parent) :
                                                            QObject(parent),
                                                            d(new privateStruct)
{

}

U2BioXmlLisProtocol::~U2BioXmlLisProtocol()
{

}

QDomElement U2BioXmlLisProtocol::createGroup()
{
    return d->mDoc.createElement("GROUP");
}

QDomElement U2BioXmlLisProtocol::createParam()
{
    return d->mDoc.createElement("PARAM");
}

QDomText U2BioXmlLisProtocol::createTextData(const QString& data)
{
    return d->mDoc.createTextNode(data);
}

void U2BioXmlLisProtocol::createData()
{
    QDomElement root = this->createGroup();
    root.setAttribute("ID", "HostOut");

    QDomElement softwareInfo = this->createGroup();
    softwareInfo.setAttribute("ID", "SoftwareInfo");
    this->createSoftWareInfoData(softwareInfo);

    QDomElement sessionInfo = this->createGroup();
    sessionInfo.setAttribute("ID", "SessionInfo");
    sessionInfo.setAttribute("ORDER", "");
    sessionInfo.setAttribute("FILTER", "txxxr");
    this->createSessionInfoData(sessionInfo);

    QDomElement instrument = this->createGroup();
    instrument.setAttribute("TYPE", "Instrument");
    instrument.setAttribute("ID", "S-Blot 2 PLUS");
    this->createInstrumentData(instrument);

    root.appendChild(softwareInfo);
    root.appendChild(sessionInfo);
    root.appendChild(instrument);
    d->mDoc.appendChild(root);
}

void U2BioXmlLisProtocol::createSoftWareInfoData(QDomElement& element)
{
    auto nameElement = this->createParam();
    auto name = this->createTextData("S-Blot 2 PLUS");
    nameElement.setAttribute("TYPE", "String");
    nameElement.setAttribute("ID", "Name");
    nameElement.appendChild(name);

    auto versionElement = this->createParam();
    auto version = this->createTextData("1.0");
    versionElement.setAttribute("TYPE", "String");
    versionElement.setAttribute("ID", "Version");
    versionElement.appendChild(version);

    element.appendChild(nameElement);
    element.appendChild(versionElement);
}

void U2BioXmlLisProtocol::createSessionInfoData(QDomElement& element)
{
    auto idElement = this->createParam();
    auto id = this->createTextData("---");
    idElement.setAttribute("TYPE", "String");
    idElement.setAttribute("ID", "ID");
    idElement.setAttribute("FILTER", "-wwwr");
    idElement.appendChild(id);

    auto userElement = this->createParam();
    auto admin = this->createTextData("admin");
    userElement.setAttribute("TYPE", "String");
    userElement.setAttribute("ID", "User");
    userElement.setAttribute("FILTER", "-rrrr");
    userElement.appendChild(admin);

    auto creationDate = QDateTime::currentDateTime();

    auto lastChangeDateTimeElement = this->createParam();
    auto changeDateTime = this->createTextData(creationDate.toString("yyyy-MM-dd-hh:mm"));
    lastChangeDateTimeElement.setAttribute("TYPE", "DateTime");
    lastChangeDateTimeElement.setAttribute("ID", "LastChangeDateTime");
    lastChangeDateTimeElement.setAttribute("FILTER", "-rrrr");
    lastChangeDateTimeElement.appendChild(changeDateTime);

    auto executionElement = this->createParam();
    auto executionDateTime = this->createTextData(creationDate.toString("yyyy-MM-dd-hh:mm"));
    executionElement.setAttribute("TYPE", "DateTime");
    executionElement.setAttribute("ID", "Execution");
    executionElement.setAttribute("FILTER", "-rrrr");
    executionElement.appendChild(executionDateTime);

    auto phaseSelectionElement = this->createParam();
    auto phase = this->createTextData("FullSession");
    phaseSelectionElement.setAttribute("TYPE", "String");
    phaseSelectionElement.setAttribute("ID", "PhaseSelection");
    phaseSelectionElement.setAttribute("FILTER", "-----");
    phaseSelectionElement.appendChild(phase);

    auto doctorHeaderElement = this->createParam();
    doctorHeaderElement.setAttribute("TYPE", "String");
    doctorHeaderElement.setAttribute("ID", "DoctorHeader");

    auto laboratoryHeaderElement = this->createParam();
    laboratoryHeaderElement.setAttribute("TYPE", "String");
    laboratoryHeaderElement.setAttribute("ID", "LaboratoryHeader");

    element.appendChild(idElement);
    element.appendChild(userElement);
    element.appendChild(lastChangeDateTimeElement);
    element.appendChild(executionElement);
    element.appendChild(phaseSelectionElement);
    element.appendChild(doctorHeaderElement);
    element.appendChild(laboratoryHeaderElement);
}

void U2BioXmlLisProtocol::createSourcePositionData(QDomElement& element, int stripNumber)
{
    QString positonText;
    positonText.sprintf("%02d", stripNumber);
    auto sourcePositionElement = this->createTextData(positonText);
    element.setAttribute("TYPE", "String");
    element.setAttribute("ID", "SourcePosition");
    element.appendChild(sourcePositionElement);
}

void U2BioXmlLisProtocol::createPatientCardData(QDomElement& element, const GlobalDataStruct::AnalysisReportInfomation& result)
{
    auto groupElement = this->createParam();
    groupElement.setAttribute("TYPE","String[32]");
    groupElement.setAttribute("ID","Group");
    groupElement.setAttribute("FILTER","-rrrr");

    auto nameElement = this->createParam();
    auto name = this->createTextData(result.element.name);
    nameElement.setAttribute("TYPE","String[32]");
    nameElement.setAttribute("ID","Name");
    nameElement.setAttribute("FILTER","-rrrr");
    nameElement.appendChild(name);

    auto surNameElement = this->createParam();
    surNameElement.setAttribute("TYPE","String[32]");
    surNameElement.setAttribute("ID","Surname");
    surNameElement.setAttribute("FILTER","-rrrr");

    auto birthDateElement = this->createParam();
    //    auto birthDate = this->createTextData(); //deprecated
    birthDateElement.setAttribute("TYPE","Date");
    birthDateElement.setAttribute("ID","BirthDate");
    birthDateElement.setAttribute("FILTER","-rrrr");
    //    birthDateElement.appendChild(birthDate);

    auto sexElement = this->createParam();
    sexElement.setAttribute("TYPE","SexType");
    sexElement.setAttribute("ID","Sex");
    sexElement.setAttribute("FILTER","-rrrr");

    auto noteElement = this->createParam();
    noteElement.setAttribute("TYPE","String[50]");
    noteElement.setAttribute("ID","Note");
    noteElement.setAttribute("FILTER","-rrrr");

    auto codeElement = this->createParam();
    codeElement.setAttribute("TYPE","String[25]");
    codeElement.setAttribute("ID","Code");
    codeElement.setAttribute("FILTER","-rrrr");

    element.appendChild(groupElement);
    element.appendChild(nameElement);
    element.appendChild(surNameElement);
    element.appendChild(birthDateElement);
    element.appendChild(sexElement);
    element.appendChild(noteElement);
    element.appendChild(codeElement);
}

void U2BioXmlLisProtocol::createAssayData(QDomElement& element, const GlobalDataStruct::AnalysisReportInfomation &result)
{
    auto config = ConfigInformation::getInstance();

    auto itemNameList = config->getCurrentStripNameList(result.element.panelName);
    auto codeList = config->getCurrentStripCodeList(result.element.panelName);

    auto resultElement = this->createGroup();
    resultElement.setAttribute("TYPE", "Results");
    resultElement.setAttribute("ID", "1");

    auto stripDataElement = this->createGroup();

    auto positionElement = this->createParam();
    auto position = this->createTextData(QString::number(result.element.stripNumber));
    positionElement.setAttribute("TYPE", "Long");
    positionElement.setAttribute("ID", "Position");
    positionElement.appendChild(position);

    auto stripLengthElement = this->createParam();
    stripLengthElement.setAttribute("TYPE", "Long");
    stripLengthElement.setAttribute("ID", "StripLength");

    auto imagePathElement = this->createParam();
    auto imagePath = this->createTextData(result.result.resultImagePath);
    imagePathElement.setAttribute("TYPE", "String");
    imagePathElement.setAttribute("ID", "ImgBlotStripPath");
    imagePathElement.appendChild(imagePath);

    auto remarksElement = this->createParam();
    remarksElement.setAttribute("TYPE", "String");
    remarksElement.setAttribute("ID", "Remarks");

    stripDataElement.appendChild(positionElement);
    stripDataElement.appendChild(stripLengthElement);
    stripDataElement.appendChild(imagePathElement);
    stripDataElement.appendChild(remarksElement);

    for(auto i = 0; i < result.result.resultValue.count() ; i++)
    {
        QString itemName;
        QString codeName;

        if(i < itemNameList.count())
            itemName = itemNameList[i];

        if(i < codeList.count())
            codeName = codeList[i];

        auto blotDataElement = this->createGroup();
        blotDataElement.setAttribute("TYPE", "Blot");
        blotDataElement.setAttribute("ID", i+1); //band position
        blotDataElement.setAttribute("STATE", "Ok");

        auto axisElement = this->createParam(); //deprecated
        axisElement.setAttribute("TYPE", "String");
        axisElement.setAttribute("ID", "Axis");

        auto bandTypeElement = this->createParam();
        auto bandTypeString = this->getBandType(codeName);
        auto bandType = this->createTextData(bandTypeString);
        bandTypeElement.setAttribute("TYPE", "String");
        bandTypeElement.setAttribute("ID", "Type");
        bandTypeElement.appendChild(bandType);

        auto bandNameElement = this->createParam();
        auto bandNameStr = itemName;
        auto bandName = this->createTextData(bandNameStr);
        bandNameElement.setAttribute("TYPE", "String");
        bandNameElement.setAttribute("ID", "Name");
        bandNameElement.appendChild(bandName);

        auto codeElement = this->createParam();
        auto codeStr = codeName;
        auto code = this->createTextData(codeStr);
        codeElement.setAttribute("TYPE", "String");
        codeElement.setAttribute("ID", "Code");
        codeElement.appendChild(code);

        auto curveElement = this->createParam(); //deprecated
        curveElement.setAttribute("TYPE", "String");
        curveElement.setAttribute("ID", "Curve");

        auto intensityElement = this->createParam(); //deprecated
        intensityElement.setAttribute("TYPE", "String");
        intensityElement.setAttribute("ID", "Intensity");

        auto iumlElement = this->createParam(); // iu ml
        auto iumlStr = QString();

        if(this->isSpecificData(codeName))
            iumlStr = QString::number(result.result.tigeConcentrationValue, 'f', 2);
        else
            iumlStr = result.result.resultValue[i];

        auto iuml = this->createTextData(iumlStr);

        iumlElement.setAttribute("TYPE", "String");
        iumlElement.setAttribute("ID", "QntResult");
        iumlElement.appendChild(iuml);

        auto classElement = this->createParam(); // class
        auto classStr = result.result.classType[i];
        auto classLevel = this->createTextData(classStr);
        classElement.setAttribute("TYPE", "String");
        classElement.setAttribute("ID", "Result");
        classElement.appendChild(classLevel);

        blotDataElement.appendChild(axisElement);
        blotDataElement.appendChild(bandTypeElement);
        blotDataElement.appendChild(bandNameElement);
        blotDataElement.appendChild(codeElement);
        blotDataElement.appendChild(curveElement);
        blotDataElement.appendChild(intensityElement);
        blotDataElement.appendChild(iumlElement);
        blotDataElement.appendChild(classElement);

        stripDataElement.appendChild(blotDataElement);
    }

    resultElement.appendChild(stripDataElement);
    element.appendChild(resultElement);
}

void U2BioXmlLisProtocol::createInstrumentData(QDomElement& element)
{
    for(auto&itr : d->mOutData)
    {
        auto patientElement = this->createGroup();
        patientElement.setAttribute("TYPE", "Patient");
        patientElement.setAttribute("ID", itr.element.patientID);

        auto paramSourcePosition = this->createParam();
        this->createSourcePositionData(paramSourcePosition, itr.element.stripNumber);

        //create patientCard
        auto patientCardElement = this->createGroup();
        patientCardElement.setAttribute("ID", "PatientCard");
        this->createPatientCardData(patientCardElement, itr);

        //create assay
        auto assayElement = this->createGroup();
        assayElement.setAttribute("TYPE", "Assay");
        assayElement.setAttribute("ID", itr.element.panelName);
        this->createAssayData(assayElement, itr);

        patientElement.appendChild(paramSourcePosition);
        patientElement.appendChild(patientCardElement);
        patientElement.appendChild(assayElement);

        element.appendChild(patientElement);
    }
}

void U2BioXmlLisProtocol::saveU2BioXmlOutData(const QVector<GlobalDataStruct::AnalysisReportInfomation> &data, DataBaseCtrl* codeFuncDatabase)
{
    QString filePath = QString("C:/Blot-2/Out");
    CUtil::createDirectory(filePath);

    QString fullPath = QString("%1/HostOut%2.xml")
                           .arg(filePath)
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));
    d->mOutData = data;
    d->mDatabase = codeFuncDatabase;

    if(d->mDatabase == nullptr)
        return;

    this->createData();
    this->saveXMLFile(fullPath);
}

QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA> U2BioXmlLisProtocol::loadU2BioXmlInData()
{
    QString defaultDir = QString("C:/Blot-2/In");
    CUtil::createDirectory(defaultDir);

    auto fileName = QFileDialog::getOpenFileName(nullptr, tr("Load XML File"), defaultDir, "XML Files (*.xml)", nullptr, QFileDialog::DontUseNativeDialog);

    if(fileName.isEmpty())
        return QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA>();

    if(this->loadXMLFile(fileName) == false)
        return QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA>();

    return this->parsingU2BioXmlIn();
}

bool U2BioXmlLisProtocol::loadXMLFile(const QString& filePath)
{
    QFile xmlFile(filePath);

    if(xmlFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        return false;

    d->mDoc.setContent(&xmlFile);

    return true;
}

bool U2BioXmlLisProtocol::saveXMLFile(const QString& filePath)
{
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text) == false)
        return false;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << d->mDoc.toByteArray();
    file.close();

    return true;
}

QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA> U2BioXmlLisProtocol::parsingU2BioXmlIn()
{
    QList<U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA> data;

    QDomElement  workList = d->mDoc.firstChildElement("GROUP");             // WORKLIST
    QDomNodeList sampleList = workList.childNodes();                    // SampleList

    int sampleCount = sampleList.count();
    qDebug() << "Sample - count : " << sampleCount;

    for(auto sampleIdx = 0; sampleIdx < sampleCount; sampleIdx++)
    {
        QDomElement sample = sampleList.at(sampleIdx).toElement();      // Sample

        qDebug() << "Sample Index : " << sampleIdx;

        U2BioXmlLisProtocol::U2_BIO_XML_LIS_DATA sampleData;

        for(auto sampleChildIdx = 0; sampleChildIdx < sample.childNodes().count(); sampleChildIdx++)
        {
            QDomElement sampleChild = sample.childNodes().at(sampleChildIdx).toElement();   // Patient, Assays

            QString patientID = "";
            QString patientName = "";
            QString patientBirthDate = "";
            QString patientSex = "";

            QString assaysID = "";
            QString assaysPanelName = "";

            QString attribute = sampleChild.attributeNode("TYPE").value();
            if(attribute == "Patient")                                  //////////////// Patient ID
            {
                patientID = sampleChild.attribute("ID");
                sampleData.patientID = patientID;

                qDebug() << "\t Patient ID : " << patientID;

                for(auto patientChildIdx = 0 ; patientChildIdx < sampleChild.childNodes().count() ; patientChildIdx++)
                {
                    QDomElement patientChild = sampleChild.childNodes().at(patientChildIdx).toElement();
                    QString attr = patientChild.attributeNode("ID").value();

                    if(attr == "Group"){}                                       //////////////// PARAM ID Group
                    else if(attr == "Name")                                     //////////////// PARAM ID Name
                    {
                        patientName = patientChild.text();
                        sampleData.name = patientName;
                        qDebug() << "\t Patient Name : " << patientID;
                    }
                    else if(attr == "Surname"){}                                //////////////// PARAM ID Surname
                    else if(attr == "BirthDate")                                //////////////// PARAM ID BirthDate
                    {
                        patientBirthDate = patientChild.text();
                        sampleData.birthDate = patientBirthDate;
                        qDebug() << "\t Patient BirthDate : " << patientBirthDate;
                    }
                    else if(attr == "Sex")                                      //////////////// PARAM ID Sex
                    {
                        patientSex = patientChild.text();
                        sampleData.sex = patientSex;
                        qDebug() << "\t Patient sex : " << patientSex;
                    }
                    else if(attr == "Code"){}                                   //////////////// PARAM ID Code
                }
            }
            else if(attribute == "Assays")
            {
                assaysID = sampleChild.attribute("ID");
                qDebug() << "\t assays ID : " << assaysID;

                for(auto assaysChildIdx = 0 ; assaysChildIdx < sampleChild.childNodes().count() ; assaysChildIdx++)
                {
                    QDomElement patientChild = sampleChild.childNodes().at(assaysChildIdx).toElement();
                    QString attr = patientChild.attributeNode("ID").value();

                    // assays ID == panelName ?????????????????????????????????
                    assaysPanelName = attr;
                    sampleData.panelName = assaysPanelName;
                    qDebug() << "\t Assays panelName : " << assaysPanelName;   //////////////// Assays panelName
                }
            }
        }

        data.append(sampleData);
    }

    return data;
}

QString U2BioXmlLisProtocol::getBandType(const QString& codeName)
{
    if(codeName.compare("PC", Qt::CaseInsensitive) == 0)
        return codeName;

    if(codeName.compare("PM", Qt::CaseInsensitive) == 0)
        return codeName;

    if(codeName.compare("tIgE", Qt::CaseInsensitive) == 0)
        return codeName;

    if(codeName.compare("null", Qt::CaseInsensitive) == 0)
        return codeName;

    if(codeName.compare("-", Qt::CaseInsensitive) == 0)
        return codeName;

    return "Allergen";
}

bool U2BioXmlLisProtocol::isSpecificData(const QString& code)
{
    if(code.compare("tIgE", Qt::CaseInsensitive) == 0)
        return true;

    return false;
}

QString U2BioXmlLisProtocol::calcForSpecificData(const double& intensity)
{
    auto resultData = this->clacCutOFFTIgEData(intensity);
    return QString::number(resultData, 'f', 2);
}

double U2BioXmlLisProtocol::clacCutOFFTIgEData(const double& intensity)
{
    if(intensity <= 0.00000001)
        return ClassHelper::rounding(0.01, 2);

    auto ly = 0.1;
    auto tIgECutOff = ClassHelper::getTigeCutOff();
    auto isValidtIgE = ClassHelper::isValidTIGE(intensity, tIgECutOff);

    if(isValidtIgE) //P
        ly = 100 + ( (intensity - tIgECutOff) * (tIgECutOff/5) );
    else //N
        ly = (99.99/tIgECutOff)* intensity;

    if(ly <= 0.00000001)
        ly = 0.01;

    return ClassHelper::rounding(ly, 2);
}
