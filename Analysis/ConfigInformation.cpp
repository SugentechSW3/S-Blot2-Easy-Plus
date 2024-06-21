#include <QMetaEnum>
#include <QThread>
#include <QSize>
#include <QRect>
#include <QTranslator>
#include <QApplication>
#include "ConfigInformation.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

#include "Common/Barcode/CQRCode.h"

#include "Common/Io/CSettingIo.h"
#include "Common/Io/INIFileControls/AllergyIniDataControls.h"
#include "Common/Io/INIFileControls/GenericIniDataControls.h"
#include "Common/Io/INIFileControls/TunningIniDataControls.h"
#include "Common/Io/INIFileControls/CameraGenericIniDataControls.h"


#include "Network/SerialCtrl.h"
#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolControlWrite.h"
#include "Network/PacketWriter/WriteProtocolStatusRead.h"
#include "Network/PacketWriter/WriteProtocolSequenceGenrator.h"
#include "Network/Protocols/TuningData/ProtocolTuningDataSettingGlobal.h"


USING_SUGENTECH

struct ConfigInformation::privateStruct
{
    QString mUserPassword;
    QString mSelectedContents;
    QStringList mContentsList;
    GlobalDataStruct::LANGUAGE_LIST mDefaultLanguage;
    GlobalDataStruct::LANGUAGE_LIST mChangeLanguage;
    GlobalDataStruct::LANGUAGE_LIST mSecondLanguage;

    QMap<QString, GlobalDataStruct::CONTENTS_STATUS> mContentsStatus;
    GlobalDataStruct::CAMERA_OPTION_SETTINGS mOptionSettings;
    QRect mBaseRoi;
    QSize mBaseResolution;

    QVector<GlobalDataStruct::AnalysisReportInfomation> mLastAnalysisData;
    QVector<QString> mSequenceContentsQueue;
    QTimer mSequenceTimer;

    SerialCtrl* mSerialNetwork = nullptr;
    QScopedPointer<QTranslator> mProgramLanguage;

    bool mIsInitSequence = false;
    bool mIsAdminMode = false;
    bool mIsOffline = false;
    bool mIsSkipDownLoad = false;
    int mStripPoint = 1;
    int mLastSelectedSequence = 1;
    int mTripleQRPosition = 0;
    int mASPBathPosition = 0;
};

ConfigInformation::ConfigInformation(QObject *parent) :
                                                      QObject(parent),
                                                      d(new privateStruct)
{
    this->initConfig();
}

ConfigInformation::~ConfigInformation()
{
}

void ConfigInformation::initConfig()
{
    d->mProgramLanguage.reset(new QTranslator(this));
    d->mSerialNetwork = SerialCtrl::getInstance();
    d->mSequenceTimer.setSingleShot(true);
    d->mChangeLanguage = this->getDefaultLanguage();
    d->mSecondLanguage = GlobalDataStruct::LANGUAGE_LIST_ENG;

    this->initLanguage();

    connect(d->mSerialNetwork, &SerialCtrl::onRecvDataForSerial, this, &ConfigInformation::onRecvData);
    connect(d->mSerialNetwork, &SerialCtrl::onRecvDataForSerial, this, &ConfigInformation::onRecvSequenceData);
    connect(&d->mSequenceTimer, &QTimer::timeout, this, [this]
            {
                this->readSeqeunceCount();
            });

    this->readSugentechContentsList();
    this->loadAllConentsData();

    connect(this, &ConfigInformation::endConfigProtocol, this ,[this]
            (bool isSequencing)
            {
                d->mIsInitSequence = true;

                if(isSequencing == true)
                    return;

                if(d->mSelectedContents.isEmpty() == true)
                {
                    if(0 < d->mContentsList.count())
                        this->setCurrentContents(d->mContentsList.first());

                    emit onEmptySelectedContents();
                }

                for(auto itr : d->mContentsList)
                {
                    auto sequenceList = this->getSequenceList(itr);

                    if(sequenceList.isEmpty() == true)
                        emit onMissingSequenceFile(itr);
                }
    });
}

void ConfigInformation::initLanguage()
{
    auto loadStr = ConfigInformation::getSavedLanguageStr();

    if(d->mProgramLanguage->load(loadStr))
        qApp->installTranslator(d->mProgramLanguage.data());
}

bool ConfigInformation::start()
{
    if(d->mSerialNetwork->openSerial() == true)
    {
        emit startConfigDownLoad();

        this->checkReconnection();
        return true;
    }

    return false;
}

void ConfigInformation::aginDeclareContentCodeList()
{
    this->loadAllergyNames();
}

void ConfigInformation::checkReconnection()
{
    auto currentStepData = WriteProtocolStatusRead::writeCurrentStatusRead().toUtf8();
    d->mSerialNetwork->writeQueuedData(currentStepData);
}

void ConfigInformation::loadAllConentsData()
{
    this->loadAllergyDatas();
}

void ConfigInformation::loadAllergyDatas()
{
    this->loadAllergyNames();
    this->loadAllergyGroups();
    this->loadAllergySecondNames();
    this->loadAllergyStripDatas();
}

void ConfigInformation::loadAllergyNames()
{
    auto contentsName = this->convertQEnumToQString((int)STANDARD_CONTETNS_LIST::ALLERGY, "STANDARD_CONTETNS_LIST");

    d->mContentsStatus[contentsName].mContentsNameList["PC"] = tr("Positive Control");
    d->mContentsStatus[contentsName].mContentsNameList["PM"] = tr("-");
    d->mContentsStatus[contentsName].mContentsNameList["tIgE"]= tr("Total IgE");
    d->mContentsStatus[contentsName].mContentsNameList["-"] = tr("-");
    d->mContentsStatus[contentsName].mContentsNameList["null"] = tr("RawData");

    d->mContentsStatus[contentsName].mContentsNameList["d1"] = tr("House dust mite (Dp)");
    d->mContentsStatus[contentsName].mContentsNameList["d2"] = tr("House dust mite (Df)");
    d->mContentsStatus[contentsName].mContentsNameList["d70"] = tr("Acarus siro");
    d->mContentsStatus[contentsName].mContentsNameList["d72"] = tr("Storage mite (Tp)");
    d->mContentsStatus[contentsName].mContentsNameList["e1"] = tr("Cat");
    d->mContentsStatus[contentsName].mContentsNameList["e3"] = tr("Horse");
    d->mContentsStatus[contentsName].mContentsNameList["e5"] = tr("Dog");
    d->mContentsStatus[contentsName].mContentsNameList["e6"] = tr("Guinea pig");
    d->mContentsStatus[contentsName].mContentsNameList["e71"] = tr("Mouse");
    d->mContentsStatus[contentsName].mContentsNameList["e73"] = tr("Rat");
    d->mContentsStatus[contentsName].mContentsNameList["e81"] = tr("Sheep");
    d->mContentsStatus[contentsName].mContentsNameList["e82"] = tr("Rabbit");
    d->mContentsStatus[contentsName].mContentsNameList["e84"] = tr("Hamster");
    d->mContentsStatus[contentsName].mContentsNameList["g1"] = tr("Sweet vernal grass");
    d->mContentsStatus[contentsName].mContentsNameList["g2"] = tr("Bermuda grass");
    d->mContentsStatus[contentsName].mContentsNameList["g3"] = tr("Orchard grass");
    d->mContentsStatus[contentsName].mContentsNameList["g6"] = tr("Timothy grass");
    d->mContentsStatus[contentsName].mContentsNameList["g7"] = tr("Common reed");
    d->mContentsStatus[contentsName].mContentsNameList["g9"] = tr("Redtop, Bent grass");
    d->mContentsStatus[contentsName].mContentsNameList["g12"] = tr("Rye");
    d->mContentsStatus[contentsName].mContentsNameList["h1"] = tr("House dust");
    d->mContentsStatus[contentsName].mContentsNameList["i1"] = tr("Honey bee");
    d->mContentsStatus[contentsName].mContentsNameList["i3"] = tr("Yellow jacket, wasp");
    d->mContentsStatus[contentsName].mContentsNameList["i6"] = tr("Cockroach");
    d->mContentsStatus[contentsName].mContentsNameList["k82"] = tr("Latex");
    d->mContentsStatus[contentsName].mContentsNameList["m1"] = tr("Penicillium notatum");
    d->mContentsStatus[contentsName].mContentsNameList["m2"] = tr("Cladosporium herbarum");
    d->mContentsStatus[contentsName].mContentsNameList["m3"] = tr("Aspergillus fumigatus");
    d->mContentsStatus[contentsName].mContentsNameList["m5"] = tr("Candida albicans");
    d->mContentsStatus[contentsName].mContentsNameList["m6"] = tr("Alternaria alternata");
    d->mContentsStatus[contentsName].mContentsNameList["m11"] = tr("Rhizopus nigricans");
    d->mContentsStatus[contentsName].mContentsNameList["t2"] = tr("Alder");
    d->mContentsStatus[contentsName].mContentsNameList["t3"] = tr("Birch");
    d->mContentsStatus[contentsName].mContentsNameList["t4"] = tr("Hazel");
    d->mContentsStatus[contentsName].mContentsNameList["t7"] = tr("Oak White");
    d->mContentsStatus[contentsName].mContentsNameList["t8"] = tr("Elm");
    d->mContentsStatus[contentsName].mContentsNameList["t9"] = tr("Olive tree");
    d->mContentsStatus[contentsName].mContentsNameList["t11"] = tr("Sycamore");
    d->mContentsStatus[contentsName].mContentsNameList["t12"] = tr("Goat willow");
    d->mContentsStatus[contentsName].mContentsNameList["t14"] = tr("Cottonwood");
    d->mContentsStatus[contentsName].mContentsNameList["t15"] = tr("Ash tree");
    d->mContentsStatus[contentsName].mContentsNameList["t16"] = tr("Pine");
    d->mContentsStatus[contentsName].mContentsNameList["t17"] = tr("Japanese cedar");
    d->mContentsStatus[contentsName].mContentsNameList["t19"] = tr("Acacia");
    d->mContentsStatus[contentsName].mContentsNameList["t222"] = tr("Cypress");
    d->mContentsStatus[contentsName].mContentsNameList["w1"] = tr("Ragweed, common");
    d->mContentsStatus[contentsName].mContentsNameList["w4"] = tr("Ragweed False");
    d->mContentsStatus[contentsName].mContentsNameList["w6"] = tr("Mugwort");
    d->mContentsStatus[contentsName].mContentsNameList["w7"] = tr("Oxeye daisy");
    d->mContentsStatus[contentsName].mContentsNameList["w8"] = tr("Dandelion");
    d->mContentsStatus[contentsName].mContentsNameList["w9"] = tr("English Plantain");
    d->mContentsStatus[contentsName].mContentsNameList["w10"] = tr("Lamb's quarter");
    d->mContentsStatus[contentsName].mContentsNameList["w11"] = tr("Russian thistle");
    d->mContentsStatus[contentsName].mContentsNameList["w12"] = tr("Goldenrod");
    d->mContentsStatus[contentsName].mContentsNameList["w13"] = tr("Cocklebur");
    d->mContentsStatus[contentsName].mContentsNameList["w14"] = tr("Pigweed");
    d->mContentsStatus[contentsName].mContentsNameList["w22"] = tr("Japanese hop");

    d->mContentsStatus[contentsName].mContentsNameList["f1"] = tr("Egg White");
    d->mContentsStatus[contentsName].mContentsNameList["f2"] = tr("Milk");
    d->mContentsStatus[contentsName].mContentsNameList["f3"] = tr("Codfish");
    d->mContentsStatus[contentsName].mContentsNameList["f4"] = tr("Wheat");
    d->mContentsStatus[contentsName].mContentsNameList["f6"] = tr("Barley");
    d->mContentsStatus[contentsName].mContentsNameList["f8"] = tr("Maize");
    d->mContentsStatus[contentsName].mContentsNameList["f9"] = tr("Rice");
    d->mContentsStatus[contentsName].mContentsNameList["f10"] = tr("Sesame");
    d->mContentsStatus[contentsName].mContentsNameList["f11"] = tr("Buckwheat");
    d->mContentsStatus[contentsName].mContentsNameList["f13"] = tr("Peanut");
    d->mContentsStatus[contentsName].mContentsNameList["f14"] = tr("Soy bean");
    d->mContentsStatus[contentsName].mContentsNameList["f15"] = tr("White bean");
    d->mContentsStatus[contentsName].mContentsNameList["f17"] = tr("Hazelnut");
    d->mContentsStatus[contentsName].mContentsNameList["f18"] = tr("Brazil nut");
    d->mContentsStatus[contentsName].mContentsNameList["f20"] = tr("Almond");
    d->mContentsStatus[contentsName].mContentsNameList["f23"] = tr("Crab");
    d->mContentsStatus[contentsName].mContentsNameList["f24"] = tr("Shrimp");
    d->mContentsStatus[contentsName].mContentsNameList["f25"] = tr("Tomato");
    d->mContentsStatus[contentsName].mContentsNameList["f26"] = tr("Pork");
    d->mContentsStatus[contentsName].mContentsNameList["f27"] = tr("Beef");
    d->mContentsStatus[contentsName].mContentsNameList["f31"] = tr("Carrot");
    d->mContentsStatus[contentsName].mContentsNameList["f33"] = tr("Orange");
    d->mContentsStatus[contentsName].mContentsNameList["f35"] = tr("Potato");
    d->mContentsStatus[contentsName].mContentsNameList["f36"] = tr("Coconut");
    d->mContentsStatus[contentsName].mContentsNameList["f37"] = tr("Blue mussel");
    d->mContentsStatus[contentsName].mContentsNameList["f40"] = tr("Tuna");
    d->mContentsStatus[contentsName].mContentsNameList["f41"] = tr("Salmon");
    d->mContentsStatus[contentsName].mContentsNameList["f44"] = tr("Strawberry");
    d->mContentsStatus[contentsName].mContentsNameList["f45"] = tr("Yeast, baker");
    d->mContentsStatus[contentsName].mContentsNameList["f47"] = tr("Garlic");
    d->mContentsStatus[contentsName].mContentsNameList["f48"] = tr("Onion");
    d->mContentsStatus[contentsName].mContentsNameList["f49"] = tr("Apple");
    d->mContentsStatus[contentsName].mContentsNameList["f206"] = tr("Mackerel");
    d->mContentsStatus[contentsName].mContentsNameList["f75"] = tr("Egg yolk");
    d->mContentsStatus[contentsName].mContentsNameList["f80"] = tr("Lobster");
    d->mContentsStatus[contentsName].mContentsNameList["f81"] = tr("Cheddar cheese");
    d->mContentsStatus[contentsName].mContentsNameList["f83"] = tr("Chicken");
    d->mContentsStatus[contentsName].mContentsNameList["f84"] = tr("Kiwi");
    d->mContentsStatus[contentsName].mContentsNameList["f85"] = tr("Celery");
    d->mContentsStatus[contentsName].mContentsNameList["f88"] = tr("lamb");
    d->mContentsStatus[contentsName].mContentsNameList["f91"] = tr("Mango");
    d->mContentsStatus[contentsName].mContentsNameList["f92"] = tr("Banana");
    d->mContentsStatus[contentsName].mContentsNameList["f93"] = tr("Cacao");
    d->mContentsStatus[contentsName].mContentsNameList["f95"] = tr("Peach");
    d->mContentsStatus[contentsName].mContentsNameList["f202"] = tr("Cashew nut");
    d->mContentsStatus[contentsName].mContentsNameList["f203"] = tr("Pistachio");
    d->mContentsStatus[contentsName].mContentsNameList["f206"] = tr("Mackerel");
    d->mContentsStatus[contentsName].mContentsNameList["f207"] = tr("Clam");
    d->mContentsStatus[contentsName].mContentsNameList["f212"] = tr("Mushroom");
    d->mContentsStatus[contentsName].mContentsNameList["f225"] = tr("Pumpkin");
    d->mContentsStatus[contentsName].mContentsNameList["f244"] = tr("Cucumber");
    d->mContentsStatus[contentsName].mContentsNameList["f253"] = tr("Pine nut");
    d->mContentsStatus[contentsName].mContentsNameList["f254"] = tr("Plaice");
    d->mContentsStatus[contentsName].mContentsNameList["f256"] = tr("Walnut");
    d->mContentsStatus[contentsName].mContentsNameList["f258"] = tr("Squid");
    d->mContentsStatus[contentsName].mContentsNameList["f262"] = tr("Eggplant");
    d->mContentsStatus[contentsName].mContentsNameList["f264"] = tr("Eel");
    d->mContentsStatus[contentsName].mContentsNameList["f290"] = tr("Oyster");
    d->mContentsStatus[contentsName].mContentsNameList["f299"] = tr("Sweet chestnut");
    d->mContentsStatus[contentsName].mContentsNameList["f313"] = tr("Anchovy");
    d->mContentsStatus[contentsName].mContentsNameList["f338"] = tr("Scallop");
    d->mContentsStatus[contentsName].mContentsNameList["f345"] = tr("Macadamia nut");
    d->mContentsStatus[contentsName].mContentsNameList["o214"] = tr("CCD");
    d->mContentsStatus[contentsName].mContentsNameList["k84"] = tr("Sunflower seed");
    d->mContentsStatus[contentsName].mContentsNameList["temp1"] = tr("Pupa, silk cocoon");
    d->mContentsStatus[contentsName].mContentsNameList["f413"] = tr("Pollock");
}

void ConfigInformation::loadAllergyGroups()
{
    auto contentsName = this->convertQEnumToQString((int)STANDARD_CONTETNS_LIST::ALLERGY, "STANDARD_CONTETNS_LIST");

    d->mContentsStatus[contentsName].mContentsGroupList["PC"] = "PC";
    d->mContentsStatus[contentsName].mContentsGroupList["PM"] = "PM";
    d->mContentsStatus[contentsName].mContentsGroupList["tIgE"]= "Total IgE";
    d->mContentsStatus[contentsName].mContentsGroupList["-"] = "-";

    //------------------------INHALANT------------------
    d->mContentsStatus[contentsName].mContentsGroupList["d1"] = "Mites";
    d->mContentsStatus[contentsName].mContentsGroupList["d2"] = "Mites";
    d->mContentsStatus[contentsName].mContentsGroupList["d70"] = "Mites";
    d->mContentsStatus[contentsName].mContentsGroupList["d72"] = "Mites";
    d->mContentsStatus[contentsName].mContentsGroupList["e1"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e3"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e5"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e6"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e71"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e73"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e81"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e82"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["e84"] = "Animal hair, epithelia";
    d->mContentsStatus[contentsName].mContentsGroupList["g1"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["g2"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["g3"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["g6"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["g7"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["g9"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["g12"] = "Grass pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["h1"] = "House Dust";
    d->mContentsStatus[contentsName].mContentsGroupList["i1"] = "Venoms";
    d->mContentsStatus[contentsName].mContentsGroupList["i3"] = "Venoms";
    d->mContentsStatus[contentsName].mContentsGroupList["i6"] = "Insects";
    d->mContentsStatus[contentsName].mContentsGroupList["k82"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["m1"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["m2"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["m3"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["m5"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["m6"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["m11"] = "Microorganisms";
    d->mContentsStatus[contentsName].mContentsGroupList["t2"] = "Tree pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t3"] = "Tree pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t4"] = "Tree pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t7"] = "Tree pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t8"] = "Tree pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t9"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t11"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t12"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t14"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t15"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t16"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t17"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t19"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["t222"] = "Tree Pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w1"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w4"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w6"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w7"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w8"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w9"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w10"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w11"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w12"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w13"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w14"] = "Weed pollens";
    d->mContentsStatus[contentsName].mContentsGroupList["w22"] = "Weed pollens";

    //--------------Food--------------------
    d->mContentsStatus[contentsName].mContentsGroupList["f1"] = "Egg & Dairy";
    d->mContentsStatus[contentsName].mContentsGroupList["f75"] = "Egg & Dairy";
    d->mContentsStatus[contentsName].mContentsGroupList["f2"] = "Egg & Dairy";
    d->mContentsStatus[contentsName].mContentsGroupList["f81"] = "Egg & Dairy";
    d->mContentsStatus[contentsName].mContentsGroupList["f4"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f9"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f8"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f6"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f11"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f10"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f13"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f14"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f15"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f17"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f18"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f20"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f202"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f203"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f253"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f256"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f299"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f345"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["k84"] = "Seed, legumes & nuts";
    d->mContentsStatus[contentsName].mContentsGroupList["f25"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f31"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f35"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f47"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f48"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f85"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f244"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f212"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f262"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["f225"] = "Vegetables";
    d->mContentsStatus[contentsName].mContentsGroupList["o214"] = "-";
    d->mContentsStatus[contentsName].mContentsGroupList["f26"] = "Meats";
    d->mContentsStatus[contentsName].mContentsGroupList["f27"] = "Meats";
    d->mContentsStatus[contentsName].mContentsGroupList["f83"] = "Meats";
    d->mContentsStatus[contentsName].mContentsGroupList["f88"] = "Meats";
    d->mContentsStatus[contentsName].mContentsGroupList["f33"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f36"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f44"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f49"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f84"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f91"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f92"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f93"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f95"] = "Fruits";
    d->mContentsStatus[contentsName].mContentsGroupList["f23"] = "Shellfishes";
    d->mContentsStatus[contentsName].mContentsGroupList["f24"] = "Shellfishes";
    d->mContentsStatus[contentsName].mContentsGroupList["f80"] = "Shellfishes";
    d->mContentsStatus[contentsName].mContentsGroupList["f3"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f37"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f40"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f41"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f206"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f207"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f258"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f254"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f290"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f313"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f338"] = "Fishes & Shells";
    d->mContentsStatus[contentsName].mContentsGroupList["f45"] = "Others";

    d->mContentsStatus[contentsName].mContentsGroupList["temp1"] = "Pupa, silk cocoon";
    d->mContentsStatus[contentsName].mContentsGroupList["f264"] = "Fish & Mollusks (2)";
    d->mContentsStatus[contentsName].mContentsGroupList["f413"] = "Fish & Mollusks (2)";
}

void ConfigInformation::loadAllergySecondNames()
{
    auto contentsName = this->convertQEnumToQString((int)STANDARD_CONTETNS_LIST::ALLERGY, "STANDARD_CONTETNS_LIST");

    QTranslator translator;
    translator.load(getLanguagetoTranslationFileNameString(d->mSecondLanguage));

    d->mContentsStatus[contentsName].mContentsSecondNameList["PC"] = translator.translate("ConfigInformation", "Positive Control");
    d->mContentsStatus[contentsName].mContentsSecondNameList["PM"] = translator.translate("ConfigInformation", "Position Mark");
    d->mContentsStatus[contentsName].mContentsSecondNameList["tIgE"]= translator.translate("ConfigInformation", "Total IgE");
    d->mContentsStatus[contentsName].mContentsSecondNameList["-"] = translator.translate("ConfigInformation", "-");
    d->mContentsStatus[contentsName].mContentsSecondNameList["null"] = translator.translate("ConfigInformation", "RawData");

    //------------------------INHALANT------------------
    d->mContentsStatus[contentsName].mContentsSecondNameList["d1"] = translator.translate("ConfigInformation", "House dust mite (Dp)");
    d->mContentsStatus[contentsName].mContentsSecondNameList["d2"] = translator.translate("ConfigInformation", "House dust mite (Df)");
    d->mContentsStatus[contentsName].mContentsSecondNameList["d70"] = translator.translate("ConfigInformation", "Acarus siro");
    d->mContentsStatus[contentsName].mContentsSecondNameList["d72"] = translator.translate("ConfigInformation", "Storage mite (Tp)");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e1"] = translator.translate("ConfigInformation", "Cat");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e3"] = translator.translate("ConfigInformation", "Horse");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e5"] = translator.translate("ConfigInformation", "Dog");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e6"] = translator.translate("ConfigInformation", "Guinea pig");

    d->mContentsStatus[contentsName].mContentsSecondNameList["e73"] = translator.translate("ConfigInformation", "Rat");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e81"] = translator.translate("ConfigInformation", "Sheep");
    d->mContentsStatus[contentsName].mContentsSecondNameList["m11"] = translator.translate("ConfigInformation", "Rhizopus nigricans");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t8"] = translator.translate("ConfigInformation", "Elm");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g7"] = translator.translate("ConfigInformation", "Common reed");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w4"] = translator.translate("ConfigInformation", "Ragweed False");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w10"] = translator.translate("ConfigInformation", "Lamb's quarter");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w13"] = translator.translate("ConfigInformation", "Cocklebur");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f75"] = translator.translate("ConfigInformation", "Egg yolk");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f15"] = translator.translate("ConfigInformation", "White bean");

    d->mContentsStatus[contentsName].mContentsSecondNameList["f18"] = translator.translate("ConfigInformation", "Brazil nut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f202"] = translator.translate("ConfigInformation", "Cashew nut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f203"] = translator.translate("ConfigInformation", "Pistachio");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f299"] = translator.translate("ConfigInformation", "Sweet chestnut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f345"] = translator.translate("ConfigInformation", "Macadamia nut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f212"] = translator.translate("ConfigInformation", "Mushroom");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f262"] = translator.translate("ConfigInformation", "Eggplant");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f225"] = translator.translate("ConfigInformation", "Pumpkin");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f36"] = translator.translate("ConfigInformation", "Coconut");

    d->mContentsStatus[contentsName].mContentsSecondNameList["e71"] = translator.translate("ConfigInformation", "Mouse");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e82"] = translator.translate("ConfigInformation", "Rabbit");
    d->mContentsStatus[contentsName].mContentsSecondNameList["e84"] = translator.translate("ConfigInformation", "Hamster");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g1"] = translator.translate("ConfigInformation", "Sweet vernal grass");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g2"] = translator.translate("ConfigInformation", "Bermuda grass");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g3"] = translator.translate("ConfigInformation", "Orchard grass");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g6"] = translator.translate("ConfigInformation", "Timothy grass");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g9"] = translator.translate("ConfigInformation", "Redtop, Bent grass");
    d->mContentsStatus[contentsName].mContentsSecondNameList["g12"] = translator.translate("ConfigInformation", "Rye");
    d->mContentsStatus[contentsName].mContentsSecondNameList["h1"] = translator.translate("ConfigInformation", "House dust");
    d->mContentsStatus[contentsName].mContentsSecondNameList["i1"] = translator.translate("ConfigInformation", "Honey bee");
    d->mContentsStatus[contentsName].mContentsSecondNameList["i3"] = translator.translate("ConfigInformation", "Yellow jacket, wasp");
    d->mContentsStatus[contentsName].mContentsSecondNameList["i6"] = translator.translate("ConfigInformation", "Cockroach");
    d->mContentsStatus[contentsName].mContentsSecondNameList["k82"] = translator.translate("ConfigInformation", "Latex");
    d->mContentsStatus[contentsName].mContentsSecondNameList["m1"] = translator.translate("ConfigInformation", "Penicillium notatum");
    d->mContentsStatus[contentsName].mContentsSecondNameList["m2"] = translator.translate("ConfigInformation", "Cladosporium herbarum");
    d->mContentsStatus[contentsName].mContentsSecondNameList["m3"] = translator.translate("ConfigInformation", "Aspergillus fumigatus");
    d->mContentsStatus[contentsName].mContentsSecondNameList["m5"] = translator.translate("ConfigInformation", "Candida albicans");
    d->mContentsStatus[contentsName].mContentsSecondNameList["m6"] = translator.translate("ConfigInformation", "Alternaria alternata");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t2"] = translator.translate("ConfigInformation", "Alder");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t3"] = translator.translate("ConfigInformation", "Birch");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t4"] = translator.translate("ConfigInformation", "Hazel");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t7"] = translator.translate("ConfigInformation", "Oak White");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t9"] = translator.translate("ConfigInformation", "Olive tree");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t11"] = translator.translate("ConfigInformation", "Sycamore");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t12"] = translator.translate("ConfigInformation", "Goat willow");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t14"] = translator.translate("ConfigInformation", "Cottonwood");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t15"] = translator.translate("ConfigInformation", "Ash tree");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t16"] = translator.translate("ConfigInformation", "Pine");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t17"] = translator.translate("ConfigInformation", "Japanese cedar");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t19"] = translator.translate("ConfigInformation", "Acacia");
    d->mContentsStatus[contentsName].mContentsSecondNameList["t222"] = translator.translate("ConfigInformation", "Cypress");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w1"] = translator.translate("ConfigInformation", "Ragweed, common");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w6"] = translator.translate("ConfigInformation", "Mugwort");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w7"] = translator.translate("ConfigInformation", "Oxeye daisy");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w8"] = translator.translate("ConfigInformation", "Dandelion");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w9"] = translator.translate("ConfigInformation", "English Plantain");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w11"] = translator.translate("ConfigInformation", "Russian thistle");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w12"] = translator.translate("ConfigInformation", "Goldenrod");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w14"] = translator.translate("ConfigInformation", "Pigweed");
    d->mContentsStatus[contentsName].mContentsSecondNameList["w22"] = translator.translate("ConfigInformation", "Japanese hop");

    //--------------Food--------------------
    d->mContentsStatus[contentsName].mContentsSecondNameList["f1"] = translator.translate("ConfigInformation", "Egg White");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f2"] = translator.translate("ConfigInformation", "Milk");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f81"] = translator.translate("ConfigInformation", "Cheddar cheese");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f14"] = translator.translate("ConfigInformation", "Soy bean");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f31"] = translator.translate("ConfigInformation", "Carrot");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f25"] = translator.translate("ConfigInformation", "Tomato");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f47"] = translator.translate("ConfigInformation", "Garlic");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f48"] = translator.translate("ConfigInformation", "Onion");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f4"] = translator.translate("ConfigInformation", "Wheat");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f9"] = translator.translate("ConfigInformation", "Rice");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f8"] = translator.translate("ConfigInformation", "Maize");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f35"] = translator.translate("ConfigInformation", "Potato");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f6"] = translator.translate("ConfigInformation", "Barley");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f244"] = translator.translate("ConfigInformation", "Cucumber");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f11"] = translator.translate("ConfigInformation", "Buckwheat");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f10"] = translator.translate("ConfigInformation", "Sesame");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f85"] = translator.translate("ConfigInformation", "Celery");
    d->mContentsStatus[contentsName].mContentsSecondNameList["o214"] = translator.translate("ConfigInformation", "CCD");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f26"] = translator.translate("ConfigInformation", "Pork");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f27"] = translator.translate("ConfigInformation", "Beef");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f83"] = translator.translate("ConfigInformation", "Chicken");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f88"] = translator.translate("ConfigInformation", "lamb");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f93"] = translator.translate("ConfigInformation", "Cacao");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f13"] = translator.translate("ConfigInformation", "Peanut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f20"] = translator.translate("ConfigInformation", "Almond");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f256"] = translator.translate("ConfigInformation", "Walnut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f17"] = translator.translate("ConfigInformation", "Hazelnut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f45"] = translator.translate("ConfigInformation", "Yeast, baker");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f95"] = translator.translate("ConfigInformation", "Peach");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f49"] = translator.translate("ConfigInformation", "Apple");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f44"] = translator.translate("ConfigInformation", "Strawberry");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f33"] = translator.translate("ConfigInformation", "Orange");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f91"] = translator.translate("ConfigInformation", "Mango");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f84"] = translator.translate("ConfigInformation", "Kiwi");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f92"] = translator.translate("ConfigInformation", "Banana");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f24"] = translator.translate("ConfigInformation", "Shrimp");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f80"] = translator.translate("ConfigInformation", "Lobster");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f338"] = translator.translate("ConfigInformation", "Scallop");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f3"] = translator.translate("ConfigInformation", "Codfish");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f23"] = translator.translate("ConfigInformation", "Crab");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f41"] = translator.translate("ConfigInformation", "Salmon");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f40"] = translator.translate("ConfigInformation", "Tuna");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f206"] = translator.translate("ConfigInformation", "Mackerel");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f258"] = translator.translate("ConfigInformation", "Squid");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f37"] = translator.translate("ConfigInformation", "Blue mussel");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f290"] = translator.translate("ConfigInformation", "Oyster");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f207"] = translator.translate("ConfigInformation", "Clam");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f206"] = translator.translate("ConfigInformation", "Mackerel");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f313"] = translator.translate("ConfigInformation", "Anchovy");
    d->mContentsStatus[contentsName].mContentsSecondNameList["temp1"] = translator.translate("ConfigInformation", "Pupa, silk cocoon");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f253"] = translator.translate("ConfigInformation", "Pine nut");
    d->mContentsStatus[contentsName].mContentsSecondNameList["k84"] = translator.translate("ConfigInformation", "Sunflower seed");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f264"] = translator.translate("ConfigInformation", "Eel");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f254"] = translator.translate("ConfigInformation", "Plaice");
    d->mContentsStatus[contentsName].mContentsSecondNameList["f413"] = translator.translate("ConfigInformation", "Pollock");
}

void ConfigInformation::loadAllergyStripDatas()
{
    // TODO : 구조�변경하 좀�한구조
    auto contentsName = this->convertQEnumToQString((int)STANDARD_CONTETNS_LIST::ALLERGY, "STANDARD_CONTETNS_LIST");
    auto panelList = this->getPanelList(contentsName);

    AllergyIniDataControls ini;
//    d->mHiddenBandData = ini.readHiddenBandCodeData(contentsName);

    for(auto&itr : panelList)
    {
        auto rawData = ini.readBandCodeData(itr);
        auto beHindeBandList = ini.readHiddenBandCodeData(contentsName);

        for(auto& itr : rawData)
            itr = itr.trimmed();

        QVector<QString> stripNames;
        QVector<bool> isHideStripBands;
        QVector<QString> stripGroup;
        QVector<QString> stripSecondNames;
        for(auto& stringItr : rawData)
        {
            stripNames.push_back(d->mContentsStatus[contentsName].mContentsNameList[stringItr]);
            stripGroup.push_back(d->mContentsStatus[contentsName].mContentsGroupList[stringItr]);
            stripSecondNames.push_back(d->mContentsStatus[contentsName].mContentsSecondNameList[stringItr]);
            isHideStripBands.push_back(beHindeBandList.contains(stringItr));
        }
        d->mContentsStatus[contentsName].mIsBeHindCodeList[itr] = isHideStripBands;
        d->mContentsStatus[contentsName].panleStripNameList[itr] = stripNames;
        d->mContentsStatus[contentsName].panleStripGroupList[itr] = stripGroup;
        d->mContentsStatus[contentsName].panleStripCodeList[itr] = rawData;
        d->mContentsStatus[contentsName].panleStripSecondNameList[itr] = stripSecondNames;
    }
    //auto isCheck = panelList.count() >0? true: false;
}

QString ConfigInformation::convertQEnumToQString(const int type, const QString enumNames)
{
    return CUtil::convertQEnumToQString(type, enumNames, staticMetaObject);
}

int ConfigInformation::convertQStringToQEnum(const QString &key, const QString enumNames)
{
    return CUtil::convertQStringToQEnum(key, enumNames, staticMetaObject);
}

QString ConfigInformation::convertStepEnumToStepName(int stepNumber)
{
    QString stepName;

    switch(stepNumber)
    {
    case ConfigInformation::STEP_LIST_WETTING:
        stepName = tr("Wetting");
        break;

    case ConfigInformation::STEP_LIST_DISPENSE:
        stepName = tr("Dispense");
        break;

    case ConfigInformation::STEP_LIST_WASHING1:
        stepName = tr("Washing");
        break;

    case ConfigInformation::STEP_LIST_ANTIBODY:
        stepName = tr("Antibody");
        break;

    case ConfigInformation::STEP_LIST_WASHING2:
        stepName = tr("Washing 2");
        break;

    case ConfigInformation::STEP_LIST_ENZYME:
        stepName = tr("Enzyme");
        break;

    case ConfigInformation::STEP_LIST_WASHINGDW:
        stepName = tr("Washing DW");
        break;

    case ConfigInformation::STEP_LIST_SUBSTRATE:
        stepName = tr("Substrate");
        break;

    case ConfigInformation::STEP_LIST_DRY:
        stepName = tr("Dry");
        break;

    case ConfigInformation::STEP_LIST_ANALYSIS:
        stepName = tr("Analysis");
        break;
    }

    return stepName;
}

ConfigInformation::STEP_LIST ConfigInformation::convertStepNameToStepEnum(const QString& stepName)
{
    ConfigInformation::STEP_LIST stepEnum = STEP_LIST_NONE;

    if(stepName.compare(tr("Wetting")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_WETTING;

    else if(stepName.compare(tr("Dispense")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_DISPENSE;

    else if(stepName.compare(tr("Washing")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_WASHING1;

    else if(stepName.compare(tr("Antibody")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_ANTIBODY;

    else if(stepName.compare(tr("Washing 2")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_WASHING2;

    else if(stepName.compare(tr("Enzyme")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_ENZYME;

    else if(stepName.compare(tr("Washing DW")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_WASHINGDW;

    else if(stepName.compare(tr("Substrate")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_SUBSTRATE;

    else if(stepName.compare(tr("Dry")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_DRY;

    else if(stepName.compare(tr("Analysis")) == 0)
        stepEnum = ConfigInformation::STEP_LIST_ANALYSIS;

    return stepEnum;
}

GlobalDataStruct::HOUSING_ROI ConfigInformation::getHousingROI()
{
    return d->mContentsStatus[d->mSelectedContents].stripSettings.housingROI;
}

void ConfigInformation::setHousingROI(const GlobalDataStruct::HOUSING_ROI& data)
{
    d->mContentsStatus[d->mSelectedContents].stripSettings.housingROI = data;

    this->writeHousingROI(d->mSelectedContents);
}

GlobalDataStruct::INTENSITY ConfigInformation::getIntensity()
{
    return d->mContentsStatus[d->mSelectedContents].stripSettings.intensity;
}

void ConfigInformation::setIntensity(const GlobalDataStruct::INTENSITY& data)
{
    d->mContentsStatus[d->mSelectedContents].stripSettings.intensity = data;
    this->writeIntensity(d->mSelectedContents);
}

GlobalDataStruct::PATICLE_REMOVAL ConfigInformation::getPaticleRemoval()
{
    return d->mContentsStatus[d->mSelectedContents].stripSettings.particleRemoval;
}

void ConfigInformation::setPaticleRemoval(const GlobalDataStruct::PATICLE_REMOVAL& data)
{
    d->mContentsStatus[d->mSelectedContents].stripSettings.particleRemoval = data;
    this->writePaticleRemoval(d->mSelectedContents);
}

GlobalDataStruct::USE_BOXCAR_ANALYSIS ConfigInformation::getBoxCarAnalysis()
{
    return d->mContentsStatus[d->mSelectedContents].stripSettings.boxCarAnalaysis;
}

void ConfigInformation::setBoxCarAnalysis(const GlobalDataStruct::USE_BOXCAR_ANALYSIS &data)
{
    d->mContentsStatus[d->mSelectedContents].stripSettings.boxCarAnalaysis = data;
    this->writeBoxCarAnalysisSetting(d->mSelectedContents);
}

GlobalDataStruct::USE_BOXCAR_BAND ConfigInformation::getBoxCarBandGap()
{
    return d->mContentsStatus[d->mSelectedContents].stripSettings.boxCarBandGap;
}

void ConfigInformation::setBoxCarBandGap(const GlobalDataStruct::USE_BOXCAR_BAND &data)
{
    d->mContentsStatus[d->mSelectedContents].stripSettings.boxCarBandGap = data;
    this->writeBoxCarBandGapSetting(d->mSelectedContents);
}

GlobalDataStruct::USE_MULTIPLE_BAND ConfigInformation::getMultiBandSetting()
{
    return d->mContentsStatus[d->mSelectedContents].stripSettings.multipleBandData;
}

void ConfigInformation::setMultiBandSetting(const GlobalDataStruct::USE_MULTIPLE_BAND& data)
{
    d->mContentsStatus[d->mSelectedContents].stripSettings.multipleBandData = data;
    this->writeMultiBandSetting(d->mSelectedContents);
}

GlobalDataStruct::CAMERA_LIGHT_SETTING ConfigInformation::getCameraLightSetting()
{
    return d->mOptionSettings.cameraLightSetting;
}

void ConfigInformation::setCameraLightSetting(const GlobalDataStruct::CAMERA_LIGHT_SETTING& data)
{
    d->mOptionSettings.cameraLightSetting = data;
    this->writeCameraLightSetting();
}

GlobalDataStruct::GUIDE_LINE ConfigInformation::getGuideLine()
{
    return d->mOptionSettings.guideLine;
}

void ConfigInformation::setGuideLine(const GlobalDataStruct::GUIDE_LINE& data)
{
    d->mOptionSettings.guideLine = data;
    this->writeGuideLine();
}

void ConfigInformation::setUserPassword(const QString &password)
{
    d->mUserPassword = password;
}

QString ConfigInformation::getUserPassword()
{
    return d->mUserPassword;
}

int ConfigInformation::getFlip()
{
    return d->mOptionSettings.flip;
}

void ConfigInformation::setFlip(const int data)
{
    d->mOptionSettings.flip = data;
    this->writeFlip();
}

int ConfigInformation::getLastSelectedSequence()
{
    return d->mLastSelectedSequence;
}

void ConfigInformation::setLastSelectedSequence(int sequenceNumber)
{
    d->mLastSelectedSequence = sequenceNumber;
}

double ConfigInformation::getRotation()
{
    return d->mOptionSettings.rotation;
}

void ConfigInformation::setRotation(const double data)
{
    d->mOptionSettings.rotation = data;
    emit onChangedRotation(d->mOptionSettings.rotation);
}

GlobalDataStruct::CONTENTS_STATUS ConfigInformation::getStatus()
{
    return d->mContentsStatus[d->mSelectedContents];
}

bool ConfigInformation::getIsUseMultiBand()
{
    return d->mContentsStatus[d->mSelectedContents].isMultiBand;
}

void ConfigInformation::setIsUseMultiBand(bool isUse)
{
    d->mContentsStatus[d->mSelectedContents].isMultiBand = isUse;
    this->writeUseMultiBand(d->mSelectedContents);
}

int ConfigInformation::getStripCount()
{
    return d->mContentsStatus[d->mSelectedContents].stripCount;
}

QStringList ConfigInformation::getPanelList(QString contentsName)
{
    return d->mContentsStatus[contentsName].panelList;
}

QStringList ConfigInformation::getPanelList()
{
    return d->mContentsStatus[d->mSelectedContents].panelList;
}

QMap<QString,QString> ConfigInformation::getContentPanelCodes(const QString& contentName)
{
    return d->mContentsStatus[contentName].mContentsNameList;
}

int ConfigInformation::getCurrentContentPanelIndex(const QString& panelName)
{
    return ConfigInformation::getPanelIndex(d->mSelectedContents, panelName);
}

int ConfigInformation::getPanelIndex(const QString& contentsName, const QString& panelName)
{
    auto contentsIdx = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(contentsIdx)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        return ConfigInformation::convertQStringToQEnum(panelName, "ALLERGY_PANEL_LISTS");
    }

    return 0;
}

QStringList ConfigInformation::getPanelListFromContentsName(const QString &contentsName)
{
    auto contentsIdx = this->convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");
    QStringList panelList;

    switch(contentsIdx)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        panelList = this->getAllergyPanelList();
        this->loadAllergyDatas();
        break;
    }

    return panelList;
}

QVector<GlobalDataStruct::SEQUENCE_STRUCT> ConfigInformation::loadSequenceFromFile(const QString& contentsName, const QString& fullPath)
{
    QScopedPointer<CSettingIo> io;

    if(fullPath.isEmpty() == true)
    {
        QString path = QString("SEQUENCE_%1").arg(contentsName);
        io.reset(new CSettingIo(path));
    }
    else
        io.reset(new CSettingIo(false, fullPath));

    auto rootCount =  io->getConfigureData("Root", "RootCount").toInt();

    if(rootCount == 0)
        QVector<GlobalDataStruct::SEQUENCE_STRUCT>();

    QVector<GlobalDataStruct::SEQUENCE_STRUCT> dataList;
    for(int stepIdx = 0; stepIdx < rootCount; stepIdx++)
    {
        GlobalDataStruct::SEQUENCE_STRUCT data;
        data.stepNumber = io->getConfigureData(QString::number(stepIdx), "StepNumber").toInt();

        auto childCount = io->getConfigureData(QString::number(stepIdx), "ChildCount").toInt();

        for(int processIdx = 0; processIdx < childCount; processIdx++)
        {
            auto processNumber = io->getConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessNumber").arg(QString::number(processIdx)));
            auto processName = io->getConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessName").arg(QString::number(processIdx)));
            auto processData  = io->getConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessData").arg(QString::number(processIdx)));

            GlobalDataStruct::PROCESS_DATA processDataStruct;
            processDataStruct.processNumber = processNumber.toInt();
            processDataStruct.data = processData.toMap();

            data.processData.push_back(processDataStruct);
        }

        dataList.push_back(data);
    }

    this->setSequenceList(contentsName, dataList);

    return this->getSequenceList(contentsName);
}

QVector<GlobalDataStruct::SEQUENCE_STRUCT> ConfigInformation::saveSequence(const QString& contentsName,  const QVector<GlobalDataStruct::SEQUENCE_STRUCT>& writeData, const QString& fullPath)
{
    QString path = QString("SEQUENCE_%1").arg(contentsName);
    CSettingIo io(path);
    io.setConfigureData("Root", "RootCount", writeData.count());

    for(int stepIdx = 0; stepIdx < writeData.count(); stepIdx++)
    {
        io.setConfigureData(QString::number(stepIdx), "StepNumber", writeData[stepIdx].stepNumber);
        io.setConfigureData(QString::number(stepIdx), "ChildCount", writeData[stepIdx].processData.count());

        for(auto processIdx = 0; processIdx< writeData[stepIdx].processData.count(); processIdx++)
        {
            auto processNumber = writeData[stepIdx].processData[processIdx].processNumber;
            auto processData = writeData[stepIdx].processData[processIdx].data;

            io.setConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessNumber").arg(QString::number(processIdx)), processNumber);
            io.setConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessData").arg(QString::number(processIdx)),  processData);
        }
    }

    if(fullPath.isEmpty() == false)
    {
        CSettingIo copyIo(true, fullPath);
        copyIo.setConfigureData("Root", "RootCount", writeData.count());

        for(int stepIdx = 0; stepIdx < writeData.count(); stepIdx++)
        {
            copyIo.setConfigureData(QString::number(stepIdx), "StepNumber", writeData[stepIdx].stepNumber);
            copyIo.setConfigureData(QString::number(stepIdx), "ChildCount", writeData[stepIdx].processData.count());

            for(auto processIdx = 0; processIdx< writeData[stepIdx].processData.count(); processIdx++)
            {
                auto processNumber = writeData[stepIdx].processData[processIdx].processNumber;
                auto processData = writeData[stepIdx].processData[processIdx].data;

                copyIo.setConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessNumber").arg(QString::number(processIdx)), processNumber);
                copyIo.setConfigureData(QString::number(stepIdx), QString("ID:%1_ProcessData").arg(QString::number(processIdx)),  processData);
            }
        }
    }

    return writeData;
}

QVector<GlobalDataStruct::SEQUENCE_STRUCT> ConfigInformation::getSequenceList(const QString& contentsName) const
{
    return d->mContentsStatus[contentsName].sequenceList;
}

void ConfigInformation::setSequenceList(const QString& contentsName, const QVector<GlobalDataStruct::SEQUENCE_STRUCT>& sequenceList)
{
    d->mContentsStatus[contentsName].sequenceList = sequenceList;
}

int ConfigInformation::getContentsIdx(QString contentsName)
{
    int contentsIdx = 1;

    for(int i = 0; i< d->mContentsList.count(); i++)
    {
        if(d->mContentsList[i] == contentsName)
        {
            contentsIdx = i + 1;
            return contentsIdx;
        }
    }

    return contentsIdx;
}

int ConfigInformation::getCurrentContentIdx()
{
    int contentsIdx = 1;

    for(int i = 0; i< d->mContentsList.count(); i++)
    {
        if(d->mContentsList[i] == d->mSelectedContents)
        {
            contentsIdx = i + 1;
            return contentsIdx;
        }
    }

    return contentsIdx;
}

QString ConfigInformation::getCurrentContents()
{
    return d->mSelectedContents;
}

void ConfigInformation::setCurrentContents(const QString& contents)
{
    if(contents.isEmpty() == true)
    {
        emit onEmptySelectedContents();
        return;
    }

    if(d->mContentsList.contains(contents) == false)
    {
        emit onContentsListMissMatch(contents);
        return;
    }

    d->mSelectedContents = contents;    

#ifndef IS_DEBUG_PC
    if(d->mIsOffline)
        return;
#endif

    auto contentsProtocol = this->writeReadProtocol(contents, false, TUNING_DATA_GLOBAL_LAST_SELECTED_CONTENTS).toUtf8();

    TunningIniDataControls tunningIniDataControls;
    tunningIniDataControls.writeSelectedContents(contents);

    if(contentsProtocol.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(contentsProtocol);


    emit onChangedHousingROI(d->mContentsStatus[d->mSelectedContents].stripSettings.housingROI);
    emit onChangedIntensity(d->mContentsStatus[d->mSelectedContents].stripSettings.intensity);
    emit onChangedPaticleRemoval(d->mContentsStatus[d->mSelectedContents].stripSettings.particleRemoval);
    emit onChangedUseBoxCarAnalaysis(d->mContentsStatus[d->mSelectedContents].stripSettings.boxCarAnalaysis);
    emit onChangedUseBoxCarBandGap(d->mContentsStatus[d->mSelectedContents].stripSettings.boxCarBandGap);
    emit onChangedUseMultiBand(d->mContentsStatus[d->mSelectedContents].stripSettings.multipleBandData);
    emit onChangedIsUseMultiBand(d->mContentsStatus[d->mSelectedContents].isMultiBand);
    emit onChangedSelectedContents(d->mSelectedContents);
}

GlobalDataStruct::CAMERA_STRIP_SETTINGS ConfigInformation::getStripSettings(const QString& contentsName)
{
    return d->mContentsStatus[contentsName].stripSettings;
}

QSize ConfigInformation::getBaseResolution()
{
    return d->mBaseResolution;
}

void ConfigInformation::setBaseResolution(QSize size)
{
    d->mBaseResolution = size;
}

QRect ConfigInformation::getBaseROI()
{
    return d->mBaseRoi;
}

void ConfigInformation::setBaseROI(QRect roi)
{
    d->mBaseRoi = roi;
}

void ConfigInformation::saveLastConfigureData()
{
    QFile saveData("LastData.dat");

    if(saveData.open(QIODevice::WriteOnly) == false)
        return;

    QDataStream stream(&saveData);

    stream << d->mSelectedContents;
    stream << d->mContentsStatus;
    stream << d->mOptionSettings;
    stream << d->mLastSelectedSequence;

    saveData.close();
}

void ConfigInformation::loadLastConfigureData()
{
    QFile loadData("LastData.dat");

    if(loadData.open(QIODevice::ReadOnly) == false)
        return;

    QDataStream stream(&loadData);

    stream >> d->mSelectedContents;
    stream >> d->mContentsStatus;
    stream >> d->mOptionSettings;
    stream >> d->mLastSelectedSequence;

    loadData.close();

    d->mIsInitSequence = true;
}

void ConfigInformation::saveContentsTunningINIData(const QString& contentsName)
{
    auto currentData = d->mContentsStatus[contentsName];
    auto cameraOptions = d->mOptionSettings;

    auto housingRoi = currentData.stripSettings.housingROI;
    auto intensity = currentData.stripSettings.intensity;
    auto paticleRemoval = currentData.stripSettings.particleRemoval;
    auto boxcarAnalysis = currentData.stripSettings.boxCarAnalaysis;
    auto bandgap = currentData.stripSettings.boxCarBandGap;
    auto multibandData = currentData.stripSettings.multipleBandData;
    auto isUseMultiband = currentData.isMultiBand;

    this->writeHousingROIIniData(contentsName, housingRoi);
    this->writeIntensityIniData(contentsName, intensity);
    this->writePaticleRemovalIniData(contentsName, paticleRemoval);
    this->writeBoxcarAnalysisIniData(contentsName, boxcarAnalysis);
    this->writeBoxcarBandGapIniData(contentsName, bandgap.boxCarBandGap);
    this->writeMultiBandIniData(contentsName, multibandData);
    this->writeUseMultiBandIniData(contentsName, isUseMultiband);


    // === Generic Data ===
    CameraGenericIniDataControls cameraINI;
    cameraINI.writeFilpData(cameraOptions.flip);
    cameraINI.writeGuideLine(cameraOptions.guideLine);
    cameraINI.writeCameraLightSettings(cameraOptions.cameraLightSetting);
}

QStringList ConfigInformation::getContentsList() const
{
    return d->mContentsList;
}

QVector<bool> ConfigInformation::getIsStripBehindeList(const QString& panelName)
{
    auto contents = this->getCurrentContents();
    return d->mContentsStatus[contents].mIsBeHindCodeList[panelName];
}

QStringList ConfigInformation::getHiddenBandList(const QString& panelName)
{
    QStringList hiddenBandList;

    auto contents = this->getCurrentContents();

    for(int i=0 ; i<d->mContentsStatus[contents].panleStripCodeList[panelName].count() ; i++)
    {
        QString code = d->mContentsStatus[contents].panleStripCodeList[panelName].at(i);

        if(d->mContentsStatus[contents].mIsBeHindCodeList[panelName].at(i))
        {
            hiddenBandList.append(code);
        }
    }
    return hiddenBandList;
}

QString ConfigInformation::getContentsNameFromEnum(int enumIdx)
{
    return ConfigInformation::convertQEnumToQString(enumIdx, "STANDARD_CONTETNS_LIST");
}

int ConfigInformation::getContentsEnumFromName(const QString& contentsName)
{
    return ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");
}

QString ConfigInformation::getAllergyNameFromCode(const QString& code) const
{
    auto contentsName = this->convertQEnumToQString((int)STANDARD_CONTETNS_LIST::ALLERGY, "STANDARD_CONTETNS_LIST");
    return d->mContentsStatus[contentsName].mContentsNameList[code];
}

void ConfigInformation::setContentsList(const QStringList& list)
{
    d->mContentsList = list;
}

QStringList ConfigInformation::getAllergyPanelList() const
{
    QStringList panelList;

    for(int i = ALLERGY_PANEL_LISTS_NONE + 1 ; i< ALLERGY_END_OF_PANEL ; i++)
    {
        auto panelName = this->convertQEnumToQString(i, QString("ALLERGY_PANEL_LISTS"));
        panelList << panelName;
    }

    return panelList;
}

QVector<QString> ConfigInformation::getStripNameList(const QString& contents, const QString& panelName)
{
    auto selecteContetns = d->mContentsStatus[contents];
    return selecteContetns.panleStripNameList[panelName];
}

QVector<QString> ConfigInformation::getCurrentStripNameList(const QString& panelName)
{
    auto contents = this->getCurrentContents();
    auto selecteContetns = d->mContentsStatus[contents];
    return selecteContetns.panleStripNameList[panelName];
}

QStringList ConfigInformation::getCurrentStripCodeList(const QString& panelName)
{
    auto contents = this->getCurrentContents();
    auto selecteContetns = d->mContentsStatus[contents];
    return selecteContetns.panleStripCodeList[panelName];
}

QVector<QString> ConfigInformation::getCurrentStripGroupList(const QString& panelName)
{
    auto contents = this->getCurrentContents();
    auto selecteContetns = d->mContentsStatus[contents];
    return selecteContetns.panleStripGroupList[panelName];
}

QVector<QString> ConfigInformation::getCurrentStripSecondNameList(const QString& panelName)
{
    auto contents = this->getCurrentContents();
    auto selecteContetns = d->mContentsStatus[contents];
    return selecteContetns.panleStripSecondNameList[panelName];
}


void ConfigInformation::readHousingROI(QString contentsName)
{
    auto x1 = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X1).toUtf8();
    auto x2 = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X2).toUtf8();
    auto y1 = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y1).toUtf8();
    auto y2 = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y2).toUtf8();

    if(x1.isEmpty() == true)
        return;

    if(x2.isEmpty() == true)
        return;

    if(y1.isEmpty() == true)
        return;

    if(y2.isEmpty() == true)
        return;


    d->mSerialNetwork->writeQueuedData(x1);
    d->mSerialNetwork->writeQueuedData(x2);
    d->mSerialNetwork->writeQueuedData(y1);
    d->mSerialNetwork->writeQueuedData(y2);
}

void ConfigInformation::readIntensity(QString contentsName)
{
    auto paticle = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_PATICLE).toUtf8();
    auto band = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BAND).toUtf8();
    auto background = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BACKGROUND).toUtf8();
    auto white = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_WHITE).toUtf8();

    if(paticle.isEmpty() == true)
        return;

    if(band.isEmpty() == true)
        return;

    if(background.isEmpty() == true)
        return;

    if(white.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(paticle);
    d->mSerialNetwork->writeQueuedData(band);
    d->mSerialNetwork->writeQueuedData(background);
    d->mSerialNetwork->writeQueuedData(white);
}

void ConfigInformation::readPaticleRemoval(QString contentsName)
{
    auto paticle = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_REMOVAL_PATICLE).toUtf8();
    auto average = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_AVERAGE_PATICLE).toUtf8();
    auto threshold = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_THRESHOLD).toUtf8();

    if(paticle.isEmpty() == true)
        return;

    if(average.isEmpty() == true)
        return;

    if(threshold.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(paticle);
    d->mSerialNetwork->writeQueuedData(average);
    d->mSerialNetwork->writeQueuedData(threshold);
}

void ConfigInformation::readBoxCarAnalysisSetting(QString contentsName)
{
    auto width = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_WIDTH).toUtf8();
    auto height = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HEIGHT).toUtf8();
    auto xStart = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_START).toUtf8();
    auto xRange = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_RANGE).toUtf8();
    auto pcThres = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_PC_THRES).toUtf8();
    auto housingThres = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HOUSING_THRES).toUtf8();
    auto bandGapCount = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BAND_GAP_COUNT).toUtf8();

    if(width.isEmpty() == true)
        return;

    if(height.isEmpty() == true)
        return;

    if(xStart.isEmpty() == true)
        return;

    if(xRange.isEmpty() == true)
        return;

    if(pcThres.isEmpty() == true)
        return;

    if(housingThres.isEmpty() == true)
        return;

    if(bandGapCount.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(width);
    d->mSerialNetwork->writeQueuedData(height);
    d->mSerialNetwork->writeQueuedData(xStart);
    d->mSerialNetwork->writeQueuedData(xRange);
    d->mSerialNetwork->writeQueuedData(pcThres);
    d->mSerialNetwork->writeQueuedData(housingThres);
    d->mSerialNetwork->writeQueuedData(bandGapCount);
}

void ConfigInformation::readMultiBand(const QString& contentsName)
{
    auto isMultiBand = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_USE_MULTI_BAND).toUtf8();

    auto bandBlock2XStart = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_START).toUtf8();
    auto bandBlock3XStart = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_START).toUtf8();
    auto bandBlock4XStart = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_START).toUtf8();

    auto multiBandPcThres = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTI_BAND_PC_THRES).toUtf8();

    auto pcXEnd = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PC_X_END).toUtf8();
    auto bandBlock2XEnd = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_GAP).toUtf8();
    auto bandBlock3XEnd = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_GAP).toUtf8();
    auto bandBlock4XEnd = this->writeReadProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_GAP).toUtf8();


    if(isMultiBand.isEmpty() == true)
        return;

    if(bandBlock2XStart.isEmpty() == true)
        return;

    if(bandBlock3XStart.isEmpty() == true)
        return;

    if(bandBlock4XStart.isEmpty() == true)
        return;

    if(multiBandPcThres.isEmpty() == true)
        return;

    if(pcXEnd.isEmpty() == true)
        return;

    if(bandBlock2XEnd.isEmpty() == true)
        return;

    if(bandBlock3XEnd.isEmpty() == true)
        return;

    if(bandBlock4XEnd.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(isMultiBand);

    d->mSerialNetwork->writeQueuedData(bandBlock2XStart);
    d->mSerialNetwork->writeQueuedData(bandBlock3XStart);
    d->mSerialNetwork->writeQueuedData(bandBlock4XStart);

    d->mSerialNetwork->writeQueuedData(multiBandPcThres);

    d->mSerialNetwork->writeQueuedData(pcXEnd);
    d->mSerialNetwork->writeQueuedData(bandBlock2XEnd);
    d->mSerialNetwork->writeQueuedData(bandBlock3XEnd);
    d->mSerialNetwork->writeQueuedData(bandBlock4XEnd);
}

void ConfigInformation::readBandGaps(QString contentsName, int count)
{
    for(int i =0; i < count; i++)
    {
        auto idxProtocol = this->writeReadProtocol(contentsName, true, TUNING_DATA_CONTENTS_BOX_CAR_BAND_GAP_IDX + i).toUtf8();
        d->mSerialNetwork->writeQueuedData(idxProtocol);
    }
}

void ConfigInformation::readCameraLightSetting()
{
    QString nullStr;

    auto contents = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_LAST_SELECTED_CONTENTS).toUtf8();
    auto expousre = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_EXPOSURE).toUtf8();
    auto gain = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_GAIN).toUtf8();
    auto brightness = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_BRIGHTNESS).toUtf8();
    auto contrast = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_CONTRAST).toUtf8();
    auto saturation = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_SATURATION).toUtf8();
    auto sharpness = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_SHARPNESS).toUtf8();
    auto whiteblance = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_WHITE_BALANCE).toUtf8();
    auto hue = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_HUE).toUtf8();
    auto gamma = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_GAMMA).toUtf8();
    auto focus = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_FOCUS).toUtf8();

    if(contents.isEmpty() == true)
        return;

    if(expousre.isEmpty() == true)
        return;

    if(gain.isEmpty() == true)
        return;

    if(brightness.isEmpty() == true)
        return;

    if(contrast.isEmpty() == true)
        return;

    if(saturation.isEmpty() == true)
        return;

    if(sharpness.isEmpty() == true)
        return;

    if(whiteblance.isEmpty() == true)
        return;

    if(hue.isEmpty() == true)
        return;

    if(gamma.isEmpty() == true)
        return;

    if(focus.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(contents);
    d->mSerialNetwork->writeQueuedData(expousre);
    d->mSerialNetwork->writeQueuedData(gain);
    d->mSerialNetwork->writeQueuedData(brightness);
    d->mSerialNetwork->writeQueuedData(contrast);
    d->mSerialNetwork->writeQueuedData(saturation);
    d->mSerialNetwork->writeQueuedData(sharpness);
    d->mSerialNetwork->writeQueuedData(whiteblance);
    d->mSerialNetwork->writeQueuedData(hue);
    d->mSerialNetwork->writeQueuedData(gamma);
    d->mSerialNetwork->writeQueuedData(focus);
}

void ConfigInformation::readGuideLine()
{
    QString nullStr;

    auto x1 = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X1).toUtf8();
    auto x2 = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X2).toUtf8();
    auto y1 = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y1).toUtf8();
    auto y2 = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y2).toUtf8();



    if(x1.isEmpty() == true)
        return;

    if(x2.isEmpty() == true)
        return;

    if(y1.isEmpty() == true)
        return;

    if(y2.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(x1);
    d->mSerialNetwork->writeQueuedData(x2);
    d->mSerialNetwork->writeQueuedData(y1);
    d->mSerialNetwork->writeQueuedData(y2);
}

void ConfigInformation::readFlip()
{
    QString nullStr;

    auto flip = this->writeReadProtocol(nullStr, false, TUNING_DATA_GLOBAL_FLIP).toUtf8();

    if(flip.isEmpty() == true)
        return;

    d->mSerialNetwork->writeQueuedData(flip);
}

void ConfigInformation::readSugentechContentsList()
{
    QStringList contentsList;

    for(int i = 0 ; i < (int)SUGENTECH_CONTENTS_LIST::SUGENTECH_END_OF_CONTENTS; i++)
    {
        auto contents = this->convertQEnumToQString(i, QString("SUGENTECH_CONTENTS_LIST"));
        contentsList.push_back(contents);
    }

    this->setContentsList(contentsList);

    for(int i = 0; i < d->mContentsList.count(); i++)
    {
        if(MAXIMUM_CONTENTS_COUNT <= i)
            break;

        QString contentsName = d->mContentsList[i];
        this->readStripSetting(contentsName);
    }
}

void ConfigInformation::setPanelList(QString contentsName)
{
    auto contentsIdx = this->convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(contentsIdx)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        d->mContentsStatus[contentsName].panelList = this->getAllergyPanelList();
        break;
    }
}

void ConfigInformation::readStripSetting(QString contentsName)
{
    auto contentsIdx = this->convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(contentsIdx)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        this->setAllergySampleAndStripCount();
        break;

    default:
        this->setAllergySampleAndStripCount();
        break;
    }

    d->mContentsStatus[contentsName].panelList = this->getPanelListFromContentsName(contentsName);
}

void ConfigInformation::setAllergySampleAndStripCount()
{
    auto contentsName = ConfigInformation::convertQEnumToQString((int)STANDARD_CONTETNS_LIST::ALLERGY, "STANDARD_CONTETNS_LIST");

    d->mContentsStatus[contentsName].stripCount = MAX_STRIP_COUNT;

    AllergyIniDataControls ini;
    ini.writeStripCount(d->mContentsStatus[contentsName].stripCount);
}

void ConfigInformation::writeHousingROI(QString contentsName)
{
    GlobalDataStruct::HOUSING_ROI roi;

    roi.x1 = d->mContentsStatus[contentsName].stripSettings.housingROI.x1;
    roi.x2 = d->mContentsStatus[contentsName].stripSettings.housingROI.x2;
    roi.y1 = d->mContentsStatus[contentsName].stripSettings.housingROI.y1;
    roi.y2 = d->mContentsStatus[contentsName].stripSettings.housingROI.y2;

    auto x1Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X1, roi.x1).toUtf8();
    auto x2Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X2, roi.x2).toUtf8();
    auto y1Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y1, roi.y1).toUtf8();
    auto y2Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y2, roi.y2).toUtf8();

    if(x1Protocol.isEmpty() == true)
        return;

    if(x2Protocol.isEmpty() == true)
        return;

    if(y1Protocol.isEmpty() == true)
        return;

    if(y2Protocol.isEmpty() == true)
        return;

    this->writeHousingROIIniData(contentsName, roi);

    d->mSerialNetwork->writeQueuedData(x1Protocol);
    d->mSerialNetwork->writeQueuedData(x2Protocol);
    d->mSerialNetwork->writeQueuedData(y1Protocol);
    d->mSerialNetwork->writeQueuedData(y2Protocol);

    emit onChangedHousingROI(d->mContentsStatus[contentsName].stripSettings.housingROI);
}

void ConfigInformation::writeIntensity(QString contentsName)
{
    GlobalDataStruct::INTENSITY intensityOption;

    intensityOption.paticle = d->mContentsStatus[contentsName].stripSettings.intensity.paticle;
    intensityOption.band = d->mContentsStatus[contentsName].stripSettings.intensity.band;
    intensityOption.background = d->mContentsStatus[contentsName].stripSettings.intensity.background;
    intensityOption.white = d->mContentsStatus[contentsName].stripSettings.intensity.white;

    auto paticleProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_PATICLE, intensityOption.paticle).toUtf8();
    auto bandProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BAND, intensityOption.band).toUtf8();
    auto backgroundProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BACKGROUND, intensityOption.background).toUtf8();
    auto whiteProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_WHITE, intensityOption.white).toUtf8();

    if(paticleProtocol.isEmpty() == true)
        return;

    if(bandProtocol.isEmpty() == true)
        return;

    if(backgroundProtocol.isEmpty() == true)
        return;

    if(whiteProtocol.isEmpty() == true)
        return;

    this->writeIntensityIniData(contentsName, intensityOption);

    d->mSerialNetwork->writeQueuedData(paticleProtocol);
    d->mSerialNetwork->writeQueuedData(bandProtocol);
    d->mSerialNetwork->writeQueuedData(backgroundProtocol);
    d->mSerialNetwork->writeQueuedData(whiteProtocol);

    emit onChangedIntensity(d->mContentsStatus[contentsName].stripSettings.intensity);
}

void ConfigInformation::writePaticleRemoval(QString contentsName)
{
    GlobalDataStruct::PATICLE_REMOVAL removal;

    removal.removalPaticle = d->mContentsStatus[contentsName].stripSettings.particleRemoval.removalPaticle;
    removal.averagePaticle = d->mContentsStatus[contentsName].stripSettings.particleRemoval.averagePaticle;
    removal.threshold = d->mContentsStatus[contentsName].stripSettings.particleRemoval.threshold;

    auto removalPaticleProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_REMOVAL_PATICLE, removal.removalPaticle).toUtf8();
    auto averagePaticleProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_AVERAGE_PATICLE, removal.averagePaticle).toUtf8();
    auto thresholdProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_THRESHOLD, removal.threshold).toUtf8();

    this->writePaticleRemovalIniData(contentsName, removal);

    d->mSerialNetwork->writeQueuedData(removalPaticleProtocol);
    d->mSerialNetwork->writeQueuedData(averagePaticleProtocol);
    d->mSerialNetwork->writeQueuedData(thresholdProtocol);

    emit onChangedPaticleRemoval(d->mContentsStatus[contentsName].stripSettings.particleRemoval);
}

void ConfigInformation::writeBoxCarAnalysisSetting(QString contentsName)
{
    GlobalDataStruct::USE_BOXCAR_ANALYSIS analysisData;

    analysisData.width  = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.width;
    analysisData.height = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.height;
    analysisData.xStart = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.xStart;
    analysisData.xRange = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.xRange;
    analysisData.pcThres = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.pcThres;
    analysisData.housingThres = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.housingThres;
    analysisData.boxCarbandCount = d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.boxCarbandCount;

    auto widthProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_WIDTH, analysisData.width).toUtf8();
    auto heightProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HEIGHT, analysisData.height).toUtf8();
    auto xStartProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_START, analysisData.xStart).toUtf8();
    auto xRangeProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_RANGE, analysisData.xRange).toUtf8();
    auto pcThresProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_PC_THRES, analysisData.pcThres).toUtf8();
    auto housingThresProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HOUSING_THRES, analysisData.housingThres).toUtf8();
    auto boxCarBandCountProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_BAND_GAP_COUNT, analysisData.boxCarbandCount).toUtf8();

    if(widthProtocol.isEmpty() == true)
        return;

    if(heightProtocol.isEmpty() == true)
        return;

    if(xStartProtocol.isEmpty() == true)
        return;

    if(xRangeProtocol.isEmpty() == true)
        return;

    if(pcThresProtocol.isEmpty() == true)
        return;

    if(housingThresProtocol.isEmpty() == true)
        return;

    if(boxCarBandCountProtocol.isEmpty() == true)
        return;


    this->writeBoxcarAnalysisIniData(contentsName, analysisData);

    d->mSerialNetwork->writeQueuedData(widthProtocol);
    d->mSerialNetwork->writeQueuedData(heightProtocol);
    d->mSerialNetwork->writeQueuedData(xStartProtocol);
    d->mSerialNetwork->writeQueuedData(xRangeProtocol);
    d->mSerialNetwork->writeQueuedData(pcThresProtocol);
    d->mSerialNetwork->writeQueuedData(housingThresProtocol);
    d->mSerialNetwork->writeQueuedData(boxCarBandCountProtocol);

    emit onChangedUseBoxCarAnalaysis(d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis);
}

void ConfigInformation::writeBoxCarBandGapSetting(QString contentsName)
{
    QVector<int> bandgap = d->mContentsStatus[contentsName].stripSettings.boxCarBandGap.boxCarBandGap;
    QVector<QByteArray> bandgapProtocol;

    for(int i = 0; i <  bandgap.count(); i++)
    {
        bandgapProtocol.push_back(this->writeWriteProtocol(contentsName, true, TUNING_DATA_CONTENTS_BOX_CAR_BAND_GAP_IDX + i, bandgap[i]).toUtf8());

        if(bandgapProtocol[i].isEmpty() == true)
            return;
    }

    this->writeBoxcarBandGapIniData(contentsName, bandgap);

    for(int i = 0; i < bandgap.count(); i++)
        d->mSerialNetwork->writeQueuedData(bandgapProtocol[i]);


    emit onChangedUseBoxCarBandGap(d->mContentsStatus[contentsName].stripSettings.boxCarBandGap);
}

void ConfigInformation::writeMultiBandSetting(QString contentsName)
{
    GlobalDataStruct::USE_MULTIPLE_BAND multiBand;

    multiBand.bandBlock2XStart = d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock2XStart;
    multiBand.bandBlock3XStart = d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock3XStart;
    multiBand.bandBlock4XStart = d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock4XStart;

    multiBand.mutiplePcThres = d->mContentsStatus[contentsName].stripSettings.multipleBandData.mutiplePcThres;

    multiBand.pcXGap = d->mContentsStatus[contentsName].stripSettings.multipleBandData.pcXGap;
    multiBand.bandBlock2XGap = d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock2XGap;
    multiBand.bandBlock3XGap = d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock3XGap;
    multiBand.bandBlock4XGap = d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock4XGap;

    auto multipleX2StartProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_START, multiBand.bandBlock2XStart).toUtf8();
    auto multipleX3StartProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_START, multiBand.bandBlock3XStart).toUtf8();
    auto multipleX4StartProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_START, multiBand.bandBlock4XStart).toUtf8();

    auto multiThresProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTI_BAND_PC_THRES, multiBand.mutiplePcThres).toUtf8();

    auto pcXEndPosProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_PC_X_END, multiBand.pcXGap).toUtf8();
    auto multipleX2EndPosProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_GAP, multiBand.bandBlock2XGap).toUtf8();
    auto multipleX3EndPosProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_GAP, multiBand.bandBlock3XGap).toUtf8();
    auto multipleX4EndPosProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_GAP, multiBand.bandBlock4XGap).toUtf8();

    if(multipleX2StartProtocol.isEmpty())
        return;

    if(multipleX3StartProtocol.isEmpty())
        return;

    if(multipleX4StartProtocol.isEmpty())
        return;

    if(multiThresProtocol.isEmpty())
        return;

    if(pcXEndPosProtocol.isEmpty())
        return;

    if(multipleX2EndPosProtocol.isEmpty())
        return;

    if(multipleX3EndPosProtocol.isEmpty())
        return;

    if(multipleX4EndPosProtocol.isEmpty())
        return;

    this->writeMultiBandIniData(contentsName, multiBand);

    d->mSerialNetwork->writeQueuedData(multipleX2StartProtocol);
    d->mSerialNetwork->writeQueuedData(multipleX3StartProtocol);
    d->mSerialNetwork->writeQueuedData(multipleX4StartProtocol);

    d->mSerialNetwork->writeQueuedData(multiThresProtocol);

    d->mSerialNetwork->writeQueuedData(pcXEndPosProtocol);
    d->mSerialNetwork->writeQueuedData(multipleX2EndPosProtocol);
    d->mSerialNetwork->writeQueuedData(multipleX3EndPosProtocol);
    d->mSerialNetwork->writeQueuedData(multipleX4EndPosProtocol);

    emit onChangedUseMultiBand(d->mContentsStatus[contentsName].stripSettings.multipleBandData);
}

void ConfigInformation::writeUseMultiBand(QString contentsName)
{
    auto multiband = d->mContentsStatus[contentsName].isMultiBand;
    auto multiBandProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_CONTENTS_ANALYSIS_USE_MULTI_BAND, multiband).toUtf8();

    if(multiBandProtocol.isEmpty() == true)
        return;

    this->writeUseMultiBandIniData(contentsName, multiband);

    d->mSerialNetwork->writeQueuedData(multiBandProtocol);
    emit onChangedIsUseMultiBand(multiband);
}

GlobalDataStruct::HOUSING_ROI ConfigInformation::readHousingROIIniData(const QString& contentsName)
{
    GlobalDataStruct::HOUSING_ROI roi;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        roi = allergyControls.readHousingROI();
        break;
    }

    return roi;
}

GlobalDataStruct::INTENSITY ConfigInformation::readIntensityIniData(const QString& contentsName)
{
    GlobalDataStruct::INTENSITY intensity;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:

        AllergyIniDataControls allergyControls;
        intensity = allergyControls.readIntensityOption();
        break;

    }

    return intensity;
}

GlobalDataStruct::PATICLE_REMOVAL ConfigInformation::readPaticleRemovalIniData(const QString& contentsName)
{
    GlobalDataStruct::PATICLE_REMOVAL removal;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:

        AllergyIniDataControls allergyControls;
        removal = allergyControls.readPaticleRemoval();
        break;

    }

    return removal;
}

GlobalDataStruct::USE_BOXCAR_ANALYSIS ConfigInformation::readBoxcarAnalysisIniData(const QString& contentsName)
{
    GlobalDataStruct::USE_BOXCAR_ANALYSIS boxCarAnalysis;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:

        AllergyIniDataControls allergyControls;
        boxCarAnalysis = allergyControls.readBoxCarAnalysis();
        break;

    }

    return boxCarAnalysis;
}

QVector<int> ConfigInformation::readBoxcarBandGapIniData(const QString& contentsName)
{
    QVector<int> bandGap;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:

        AllergyIniDataControls allergyControls;
        bandGap = allergyControls.readBoxCarBandGap();
        break;

    }

    return bandGap;
}

GlobalDataStruct::USE_MULTIPLE_BAND ConfigInformation::readMultiBandIniData(const QString& contentsName)
{
    GlobalDataStruct::USE_MULTIPLE_BAND multibandData;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:

        AllergyIniDataControls allergyControls;
        multibandData = allergyControls.readMultiBandData();
        break;

    }

    return multibandData;
}

bool ConfigInformation::readUseMultiBandIniData(const QString& contentsName)
{
    bool useMultiBand = false;

    auto currentContents = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContents)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:

        AllergyIniDataControls allergyControls;
        useMultiBand = allergyControls.readUseMultiBand();
        break;

    }

    return useMultiBand;
}

void ConfigInformation::writeHousingROIIniData(const QString& contentsName, const GlobalDataStruct::HOUSING_ROI& roi)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeHousingROI(roi);
        break;
    }
}

void ConfigInformation::writeIntensityIniData(const QString& contentsName, const GlobalDataStruct::INTENSITY& intensity)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeIntensityOption(intensity);
        break;
    }

}

void ConfigInformation::writePaticleRemovalIniData(const QString& contentsName, const GlobalDataStruct::PATICLE_REMOVAL& removal)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeParticleRemoval(removal);
        break;
    }
}

void ConfigInformation::writeBoxcarAnalysisIniData(const QString& contentsName, const GlobalDataStruct::USE_BOXCAR_ANALYSIS& boxcarSetting)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeBoxCarAnalysis(boxcarSetting);
        break;
    }
}

void ConfigInformation::writeBoxcarBandGapIniData(const QString& contentsName, const QVector<int>& bandGap)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeBoxCarBandGap(bandGap);
        break;
    }
}

void ConfigInformation::writeMultiBandIniData(const QString& contentsName, const GlobalDataStruct::USE_MULTIPLE_BAND& multiBand)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeMultiBandData(multiBand);
        break;
    }
}

void ConfigInformation::writeUseMultiBandIniData(const QString& contentsName, bool isUseMultiBand)
{
    auto currentContetns = ConfigInformation::convertQStringToQEnum(contentsName, "STANDARD_CONTETNS_LIST");

    switch(currentContetns)
    {
    case (int)STANDARD_CONTETNS_LIST::ALLERGY:
        AllergyIniDataControls allergyControls;
        allergyControls.writeUseMultiBand(isUseMultiBand);
        break;
    }
}

void ConfigInformation::writeCameraLightSetting()
{
    QString dummy;

    auto exposureProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_EXPOSURE, d->mOptionSettings.cameraLightSetting.expousre).toUtf8();
    auto gainProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_GAIN, d->mOptionSettings.cameraLightSetting.gain).toUtf8();
    auto brightnessProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_BRIGHTNESS, d->mOptionSettings.cameraLightSetting.brightness).toUtf8();
    auto contrastProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_CONTRAST, d->mOptionSettings.cameraLightSetting.contrast).toUtf8();
    auto saturationProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_SATURATION, d->mOptionSettings.cameraLightSetting.saturation).toUtf8();
    auto sharpnessProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_SHARPNESS, d->mOptionSettings.cameraLightSetting.sharpness).toUtf8();
    auto whiteblanceProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_WHITE_BALANCE, d->mOptionSettings.cameraLightSetting.whitebalance).toUtf8();
    auto hueProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_HUE, d->mOptionSettings.cameraLightSetting.hue).toUtf8();
    auto gammaProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_GAMMA, d->mOptionSettings.cameraLightSetting.gamma).toUtf8();
    auto focusProtocol = this->writeWriteProtocol(dummy, false, TUNING_DATA_GLOBAL_FOCUS, d->mOptionSettings.cameraLightSetting.focus).toUtf8();

    if(exposureProtocol.isEmpty() == true)
        return;

    if(gainProtocol.isEmpty() == true)
        return;

    if(brightnessProtocol.isEmpty() == true)
        return;

    if(contrastProtocol.isEmpty() == true)
        return;

    if(saturationProtocol.isEmpty() == true)
        return;

    if(sharpnessProtocol.isEmpty() == true)
        return;

    if(whiteblanceProtocol.isEmpty() == true)
        return;

    if(hueProtocol.isEmpty() == true)
        return;

    if(gammaProtocol.isEmpty() == true)
        return;

    if(focusProtocol.isEmpty() == true)
        return;

    CameraGenericIniDataControls cameraGeneric;
    cameraGeneric.writeCameraLightSettings(d->mOptionSettings.cameraLightSetting);

    d->mSerialNetwork->writeQueuedData(exposureProtocol);
    d->mSerialNetwork->writeQueuedData(gainProtocol);
    d->mSerialNetwork->writeQueuedData(brightnessProtocol);
    d->mSerialNetwork->writeQueuedData(contrastProtocol);
    d->mSerialNetwork->writeQueuedData(saturationProtocol);
    d->mSerialNetwork->writeQueuedData(sharpnessProtocol);
    d->mSerialNetwork->writeQueuedData(whiteblanceProtocol);
    d->mSerialNetwork->writeQueuedData(hueProtocol);
    d->mSerialNetwork->writeQueuedData(gammaProtocol);
    d->mSerialNetwork->writeQueuedData(focusProtocol);

    emit onChangedCameraLightSetting(d->mOptionSettings.cameraLightSetting);
}

void ConfigInformation::writeGuideLine()
{
    QString contentsName;

    auto x1Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X1, d->mOptionSettings.guideLine.x1).toUtf8();
    auto x2Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X2, d->mOptionSettings.guideLine.x2).toUtf8();
    auto y1Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y1, d->mOptionSettings.guideLine.y1).toUtf8();
    auto y2Protocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y2, d->mOptionSettings.guideLine.y2).toUtf8();

    if(x1Protocol.isEmpty() == true)
        return;

    if(x2Protocol.isEmpty() == true)
        return;

    if(y1Protocol.isEmpty() == true)
        return;

    if(y2Protocol.isEmpty() == true)
        return;

    CameraGenericIniDataControls cameraGeneric;
    cameraGeneric.writeGuideLine(d->mOptionSettings.guideLine);

    d->mSerialNetwork->writeQueuedData(x1Protocol);
    d->mSerialNetwork->writeQueuedData(x2Protocol);
    d->mSerialNetwork->writeQueuedData(y1Protocol);
    d->mSerialNetwork->writeQueuedData(y2Protocol);

    emit onChangedGuideLine(d->mOptionSettings.guideLine);
}

void ConfigInformation::writeFlip()
{
    QString contentsName;

    auto flipProtocol = this->writeWriteProtocol(contentsName, false, TUNING_DATA_GLOBAL_FLIP, d->mOptionSettings.flip).toUtf8();

    if(flipProtocol.isEmpty() == true)
        return;

    CameraGenericIniDataControls cameraGeneric;
    cameraGeneric.writeFilpData(d->mOptionSettings.flip);

    d->mSerialNetwork->writeQueuedData(flipProtocol);

    emit onChangedFlip(d->mOptionSettings.flip);
}

void ConfigInformation::readSeqeunce(const QString& contents)
{
    //send sequence select
     auto seqeunceIdx = 1; //sblot2 only use 1
     auto sequenceSelectProtocol = WriteProtocolControlWrite::writeSequenceSelect(seqeunceIdx).toUtf8();
     d->mSerialNetwork->writeQueuedData(sequenceSelectProtocol);
}

void ConfigInformation::readSeqeunceCount()
{
    auto seqeunceReadCount = WriteProtocolStatusRead::writeTotalStepNumber().toUtf8();
    d->mSerialNetwork->writeQueuedData(seqeunceReadCount);
}

void ConfigInformation::requestSequenceData(int idx)
{
    auto sequenceStepData = WriteProtocolStatusRead::writeReadStepID(idx).toUtf8();
    d->mSerialNetwork->writeQueuedData(sequenceStepData);
}

void ConfigInformation::onWriteSeqeuenceQueue()
{
    if(d->mSequenceContentsQueue.count() == 0)
    {
        this->onReadTuningData();
        return;
    }

    this->readSeqeunce(d->mSequenceContentsQueue.takeFirst());
}

void ConfigInformation::onReadSequenceData()
{
    for(auto contents : d->mContentsList)
        d->mSequenceContentsQueue.push_back(contents);

    this->onWriteSeqeuenceQueue();
}

void ConfigInformation::onReadTuningData()
{
    this->readCameraLightSetting();
    this->readGuideLine();
    this->readFlip();

    for(auto& contentsName : d->mContentsList)
    {
        this->readHousingROI(contentsName);
        this->readIntensity(contentsName);
        this->readPaticleRemoval(contentsName);
        this->readBoxCarAnalysisSetting(contentsName);
        this->readMultiBand(contentsName);
    }
}

void ConfigInformation::onFullSequenceProcess()
{
    d->mIsSkipDownLoad = true;

    this->loadLastConfigureData();
    CLogWriter::printLog("[Detected] : FullSequence Reconnect Status");
    emit endConfigProtocol(true);
}

void ConfigInformation::onRecvSequenceData(QByteArray recvData)
{
    if(d->mIsInitSequence == true)
        return;

    static int sequenceCount = 0;
    static int contentsIdx = 0;
    static QVector<GlobalDataStruct::SEQUENCE_STRUCT> sequenceList;
    auto md = PacketReader::modeReader(recvData);

    switch(md)
    {
        case MD_MODE::MD_MODE_STATUS_READ:
        {
            switch(PacketReader::commandReader(recvData))
            {
                case CMD_STATUS_READ::CMD_STATUS_READ_TOTAL_STEP_NUMBER:
                {
                    sequenceCount = PacketReader::dataReader(recvData).right(2).toInt(nullptr, 16);

                    if(0xFA < sequenceCount)
                        sequenceCount = 0;

                    for(int i = 0 ; i < sequenceCount; i++)
                        this->requestSequenceData(i+1);

                    if(sequenceCount == 0)
                        this->onWriteSeqeuenceQueue();
                } break;

                case CMD_STATUS_READ::CMD_STATUS_READ_STEP_ID:
                {
                    GlobalDataStruct::SEQUENCE_STRUCT stepData;
                    stepData.stepNumber =  PacketReader::dataReader(recvData).right(2).toInt(nullptr, 16);
                    sequenceList.push_back(stepData);

                    if(sequenceCount == sequenceList.count())
                    {
                        this->setSequenceList(this->getContentsNameFromEnum(contentsIdx-1) ,sequenceList);//contentsname + seqeuenceData
                        this->onWriteSeqeuenceQueue();
                    }
                } break;
            }
        }break;
    }


    if( MD_MODE::MD_MODE_CONTROL_WRITE == md )
    {
        switch(PacketReader::commandReader(recvData))
        {
            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_SEQUENCE_SELECT_RESP:
            {
                sequenceCount = 0;
                contentsIdx  = PacketReader::dataReader(recvData).right(2).toInt(nullptr, 16);

                sequenceList = QVector<GlobalDataStruct::SEQUENCE_STRUCT> ();
                d->mSequenceTimer.start(2500);
            }break;
        }
    }
}

void ConfigInformation::onRecvData(QByteArray recvData)
{
//C0 ~ CC
//D0 ~ DC
//00~ 0x78

    if(d->mIsSkipDownLoad == false)
    {
        if(PacketReader::modeReader(recvData) == MD_MODE::MD_MODE_STATUS_READ)
        {
            if(PacketReader::commandReader(recvData) == CMD_STATUS_READ::CMD_STATUS_READ_CURRENT_STATUS_READ)
            {
                d->mIsSkipDownLoad = true;

                auto value = PacketReader::dataReader(recvData);
                int stepNumber = value.mid(2,2).toInt(nullptr, 16);
                int processNumber = value.mid(4,2).toInt(nullptr, 16);

                if(stepNumber == STEP_LIST::STEP_LIST_NONE && processNumber == PROCESS_LIST_NONE)
                    this->onReadSequenceData();
                else
                    this->onFullSequenceProcess();

                return;
            }
        }
    }

    auto isParaMode = recvData.left(1).toInt(nullptr, 16);

    if(isParaMode != MD_MODE_CAMERA_PARAMETER_READ)
        return;

    auto categoryIdx = recvData.mid(1,1).toInt(nullptr, 16); //0x0A 0x0B

    switch(categoryIdx)
    {
    case TUNING_CONTENTS_IDX_GLOBAL:
        this->parsingGlobalData(recvData);
        break;

    case TUNING_CONTENTS_IDX_ANALYSIS_0:
    {
        if(0 < d->mContentsList.count())
            this->parsingAnalysis(recvData, d->mContentsList[0], categoryIdx);
    }   break;

    case TUNING_CONTENTS_IDX_END:
        emit endConfigProtocol(false);
        CLogWriter::printLog("[Detected] : Nomal Init Status");
        break;
    }
}

void ConfigInformation::parsingGlobalData(const QByteArray& recvData)
{
    auto command = recvData.mid(2,2).toInt(nullptr, 16);
    auto data = PacketReader::dataReader(recvData).toUInt(nullptr, 16);

    if(data == 0xFFFFFFFF)
        data = 0;

    switch(command)
    {
    case TUNING_DATA_GLOBAL_LAST_SELECTED_CONTENTS:
    {
        d->mSelectedContents = ConfigInformation::getContentsNameFromEnum(data);

        if(d->mSelectedContents.isEmpty() == true)
            if(d->mContentsList.count() != 0)
                d->mSelectedContents = d->mContentsList.first();

        CLogWriter::printLog("TUNING_DATA_GLOBAL_LAST_SELECTED_CONTENTS");

        emit this->downloadGlobalData();
    }   break;

    case TUNING_DATA_GLOBAL_EXPOSURE:
        d->mOptionSettings.cameraLightSetting.expousre = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_EXPOSURE");
        break;

    case TUNING_DATA_GLOBAL_GAIN:
        d->mOptionSettings.cameraLightSetting.gain = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_GAIN");
        break;

    case TUNING_DATA_GLOBAL_BRIGHTNESS:
        d->mOptionSettings.cameraLightSetting.brightness = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_BRIGHTNESS");
        break;

    case TUNING_DATA_GLOBAL_CONTRAST:
        d->mOptionSettings.cameraLightSetting.contrast = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_CONTRAST");
        break;

    case TUNING_DATA_GLOBAL_SATURATION:
        d->mOptionSettings.cameraLightSetting.saturation = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_SATURATION");
        break;

    case TUNING_DATA_GLOBAL_SHARPNESS:
        d->mOptionSettings.cameraLightSetting.sharpness = data;
        break;

    case TUNING_DATA_GLOBAL_WHITE_BALANCE:
        d->mOptionSettings.cameraLightSetting.whitebalance = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_SHARPNESS");
        break;

    case TUNING_DATA_GLOBAL_HUE:
        d->mOptionSettings.cameraLightSetting.hue = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_HUE");
        break;

    case TUNING_DATA_GLOBAL_GAMMA:
        d->mOptionSettings.cameraLightSetting.gamma = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_GAMMA");
        break;

    case TUNING_DATA_GLOBAL_FOCUS:
        d->mOptionSettings.cameraLightSetting.focus = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_FOCUS");
        break;

    case TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X1:
        d->mOptionSettings.guideLine.x1 = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X1");
        break;

    case TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X2:
        d->mOptionSettings.guideLine.x2 = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X2");
        break;

    case TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y1:
        d->mOptionSettings.guideLine.y1 = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y1");
        break;

    case TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y2:
        d->mOptionSettings.guideLine.y2 = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y2");
        break;

    case TUNING_DATA_GLOBAL_FLIP:
        d->mOptionSettings.flip = data;
        CLogWriter::printLog("TUNING_DATA_GLOBAL_FLIP");
        break;
    }

}

void ConfigInformation::parsingAnalysis(const QByteArray& recvData, const QString& contentsName, int contentsIdx)
{
    if(contentsName.isEmpty() == true)
        return;

    auto command = recvData.mid(2,2).toInt(nullptr, 16);
    auto data = PacketReader::dataReader(recvData).toUInt(nullptr, 16);

    if(data == 0xFFFFFFFF)
        data = 0;

    switch(command)
    {
    case TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X1:
        d->mContentsStatus[contentsName].stripSettings.housingROI.x1 = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X1");
        emit downloadAnalysisData(contentsIdx - TUNING_CONTENTS_IDX_GLOBAL);
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X2:
        d->mContentsStatus[contentsName].stripSettings.housingROI.x2 = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X2");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y1:
        d->mContentsStatus[contentsName].stripSettings.housingROI.y1 = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y1");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y2:
        d->mContentsStatus[contentsName].stripSettings.housingROI.y2 = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y2");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_PATICLE:
        d->mContentsStatus[contentsName].stripSettings.intensity.paticle = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_PATICLE");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BAND:
        d->mContentsStatus[contentsName].stripSettings.intensity.band = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BAND");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BACKGROUND:
        d->mContentsStatus[contentsName].stripSettings.intensity.background = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BACKGROUND");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_WHITE:
        d->mContentsStatus[contentsName].stripSettings.intensity.white = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_WHITE");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_REMOVAL_PATICLE:
        d->mContentsStatus[contentsName].stripSettings.particleRemoval.removalPaticle = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_REMOVAL_PATICLE");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_AVERAGE_PATICLE:
        d->mContentsStatus[contentsName].stripSettings.particleRemoval.averagePaticle = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_AVERAGE_PATICLE");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_THRESHOLD:
        d->mContentsStatus[contentsName].stripSettings.particleRemoval.threshold = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_THRESHOLD");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_WIDTH:
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.width = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_WIDTH");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HEIGHT:
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.height = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HEIGHT");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_START:
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.xStart = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_START");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_RANGE:
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.xRange = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_RANGE");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_PC_THRES:
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.pcThres = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_PC_THRES");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HOUSING_THRES:
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.housingThres = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HOUSING_THRES");
        break;

    case TUNING_DATA_CONTENTS_ANALYSIS_BAND_GAP_COUNT:
    {
        d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.boxCarbandCount = data;

        if(d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.boxCarbandCount == 0)
            emit onTuningDataMissing(contentsName);

        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_BAND_GAP_COUNT");
    }   break;

    case TUNING_DATA_CONTENTS_ANALYSIS_USE_BLOB_BAND:
    {
       //deprecated
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_USE_BLOB_BAND");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_USE_MULTI_BAND:
    {
        d->mContentsStatus[contentsName].isMultiBand = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_USE_MULTI_BAND");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_DEPRECATED:
    {
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_DEPRECATED(Do Not Use)");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_START:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock4XStart = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_START");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTI_BAND_PC_THRES:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.mutiplePcThres = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTI_BAND_PC_THRES");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_PC_X_END:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.pcXGap = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_PC_X_END");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_START:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock2XStart = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_START");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_START:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock3XStart = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_START");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_GAP:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock2XGap = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X2_START");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_GAP:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock3XGap = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X3_GAP");
    }break;

    case TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_GAP:
    {
        d->mContentsStatus[contentsName].stripSettings.multipleBandData.bandBlock4XGap = data;
        CLogWriter::printLog("TUNING_DATA_CONTENTS_ANALYSIS_MULTIPLE_BAND_X4_GAP");

        //this->readBandGaps(contentsName, d->mContentsStatus[contentsName].stripSettings.boxCarAnalaysis.boxCarbandCount);

        if(d->mContentsList.count() <= (contentsIdx-TUNING_CONTENTS_IDX_GLOBAL))
            this->sendDataEnd();
    }break;

    default :
        this->parsingBoxCar(recvData, d->mContentsList[0]);
        break;
    }
}

void ConfigInformation::parsingBoxCar(const QByteArray &recvData, const QString& contentsName)
{
    if(contentsName.isEmpty() == true)
        return;

    auto data = PacketReader::dataReader(recvData).toUInt(nullptr, 16);

    if(data == 0xFFFFFFFF)
        data = 0;

    if(d->mContentsStatus[contentsName].isMultiBand == true)
        data = 0;

    d->mContentsStatus[contentsName].stripSettings.boxCarBandGap.boxCarBandGap.push_back(data);
}

QString ConfigInformation::writeReadProtocol(const QString& contentsName, bool, uchar cmd)
{
    uchar contentsIdx = TUNING_CONTENTS_IDX_GLOBAL;

    if(contentsName.isEmpty() == true)
        contentsIdx = TUNING_CONTENTS_IDX_GLOBAL;
    else
    {
        for(int i = 0; i< d->mContentsList.count(); i++)
        {
            if(d->mContentsList[i].compare(contentsName) == 0)
            {
                contentsIdx = i + 1 + TUNING_CONTENTS_IDX_GLOBAL;
                break;
            }
        }

        if(contentsIdx == TUNING_CONTENTS_IDX_GLOBAL)
            return QString();
    }

    QString command;
    command.sprintf("%01X%01X%02X", MD_MODE_CAMERA_PARAMETER_READ, contentsIdx, cmd);
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));
    return command;
}

QString ConfigInformation::writeWriteProtocol(const QString& contentsName, bool , uchar cmd, uint parameter)
{
    uchar contentsIdx = TUNING_CONTENTS_IDX_GLOBAL;

    if(contentsName.isEmpty() == true)
        contentsIdx = TUNING_CONTENTS_IDX_GLOBAL;
    else
    {
        for(int i = 0; i< d->mContentsList.count(); i++)
        {
            if(d->mContentsList[i].compare(contentsName) == 0)
            {
                contentsIdx = i + 1 + TUNING_CONTENTS_IDX_GLOBAL;
                break;
            }
        }

        if(contentsIdx == TUNING_CONTENTS_IDX_GLOBAL)
            return QString();
    }

    QString command;
    command.sprintf("%01X%01X%02X", MD_MODE_CAMERA_PARAMETER_WRITE, contentsIdx, cmd);
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(parameter));

    return command;
}

QString ConfigInformation::writeEndProtocol()
{
    QString command;
    command.sprintf("%01X%01X%02X", MD_MODE_CAMERA_PARAMETER_READ, TUNING_CONTENTS_IDX_END, 0);
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

void ConfigInformation::sendDataEnd()
{
   auto endProtocol = this->writeEndProtocol().toUtf8();

   if(endProtocol.isEmpty() == true)
       return;

   d->mSerialNetwork->writeQueuedData(endProtocol);
   CLogWriter::printLog("WRITE_END_PROTOCOL");
}

void ConfigInformation::setCurrentStripPoint(int point)
{
    d->mStripPoint = point;
    emit onChangedStripPoint(d->mStripPoint);
}

int ConfigInformation::getCurrentASPBathPosition()
{
    return d->mASPBathPosition;
}

void ConfigInformation::setCurrentASPBathPosition(int position)
{
    d->mASPBathPosition = position;
}

int ConfigInformation::getCurrentStripPoint()
{
    return d->mStripPoint;
}

GenericIniDataControls::FIXED_USER_DATA ConfigInformation::getFixedUser()
{
    GenericIniDataControls genericData;
    return genericData.readFixedUserData();
}

void ConfigInformation::setFixedUser(const GenericIniDataControls::FIXED_USER_DATA& fixedUser)
{
    GenericIniDataControls genericData;
    genericData.writeFixedUserData(fixedUser);
}

int ConfigInformation::getQRTriplePosition()
{
    return d->mTripleQRPosition;
}

void ConfigInformation::setQRTriplePosition(const int& x)
{
    d->mTripleQRPosition = x;
}

QRect ConfigInformation::getQRStripFixedROI()
{
    auto guideLine = this->getGuideLine();
    auto baseResolution = this->getBaseResolution();

    auto startX = guideLine.x2;
    auto startY = guideLine.y1;

    auto maximumWidth = baseResolution.width();
    auto maximumHeight = baseResolution.height();

    auto width = abs(guideLine.x2 - maximumWidth);
    auto height = abs(guideLine.y2 - guideLine.y1);

    if(maximumWidth < startX + width)
        startX = 0;

    if(maximumHeight < startY + height)
        startY = 0;

    QRect rect;
    rect.setX(startX);
    rect.setY(startY);
    rect.setWidth(width);
    rect.setHeight(height);

    return rect;
}

QRect ConfigInformation::getQRTripleFixedROI()
{
    auto baseROI = this->getBaseROI();
    return baseROI;
}

bool ConfigInformation::isAdminMode()
{
    return d->mIsAdminMode;
}

void ConfigInformation::setAdminMode(bool isAdmin)
{
    d->mIsAdminMode = isAdmin;
}

bool ConfigInformation::isOffLine()
{
#ifdef IS_DEBUG_PC
    return false;
#endif
    return d->mIsOffline;
}

void ConfigInformation::setOffLine(bool isOffLine)
{
    d->mIsOffline = isOffLine;

    if(d->mIsOffline)
        this->loadOfflineData();
}

void ConfigInformation::loadOfflineData()
{
    if(d->mContentsList.isEmpty() == false)
    {
        TunningIniDataControls tunningData;
        auto loadContents = tunningData.readSelectedContents();

        if(loadContents.isEmpty())
            d->mSelectedContents = d->mContentsList.first();
        else
            d->mSelectedContents = loadContents;
    }

    for(auto& itr : d->mContentsList)
    {
        auto boxcarAnalysis = this->readBoxcarAnalysisIniData(itr);
        auto multibandData = this->readMultiBandIniData(itr);

        d->mContentsStatus[itr].stripSettings.boxCarAnalaysis = boxcarAnalysis;
        d->mContentsStatus[itr].stripSettings.multipleBandData = multibandData;
        d->mContentsStatus[itr].isMultiBand = this->readUseMultiBandIniData(itr);
    }
}

QVector<GlobalDataStruct::AnalysisReportInfomation> ConfigInformation::getLastAnalysisData()
{
    return d->mLastAnalysisData;
}

void ConfigInformation::setLastAnalysisData(const QVector<GlobalDataStruct::AnalysisReportInfomation>& lastData)
{
    d->mLastAnalysisData = lastData;
}

ConfigInformation::STRIP_COLOR ConfigInformation::getAllergyColor(int panelIdx)
{
    switch(panelIdx)
    {
    case ConfigInformation::ALLERGY_PANEL_LISTS::ALLERGY_PANEL:
        return ConfigInformation::STRIP_COLOR_PINK;

    default:
        return ConfigInformation::STRIP_COLOR_NONE;
    }
}
ConfigInformation::STRIP_COLOR ConfigInformation::getAllergyColor(const QString& panelName)
{
    auto panelIdx = ConfigInformation::getPanelIndex("ALLERGY", panelName);
    return this->getAllergyColor(panelIdx);
}

GlobalDataStruct::LANGUAGE_LIST ConfigInformation::getDefaultLanguage()
{
    GenericIniDataControls generalData;
    auto language = generalData.readCurrentLanguage();

    GlobalDataStruct::LANGUAGE_LIST lang = GlobalDataStruct::LANGUAGE_LIST_ENG;

    switch(language)
    {
    case GlobalDataStruct::LANGUAGE_LIST_KOR:
        lang = GlobalDataStruct::LANGUAGE_LIST_KOR;
        break;
    case GlobalDataStruct::LANGUAGE_LIST_FR:
        lang = GlobalDataStruct::LANGUAGE_LIST_FR;
        break;
    case GlobalDataStruct::LANGUAGE_LIST_AR:
        lang = GlobalDataStruct::LANGUAGE_LIST_AR;
        break;
    case GlobalDataStruct::LANGUAGE_LIST_RO:
        lang = GlobalDataStruct::LANGUAGE_LIST_RO;
        break;
    case GlobalDataStruct::LANGUAGE_LIST_RU:
        lang = GlobalDataStruct::LANGUAGE_LIST_RU;
        break;
    case GlobalDataStruct::LANGUAGE_LIST_DE:
        lang = GlobalDataStruct::LANGUAGE_LIST_DE;
        break;
    case GlobalDataStruct::LANGUAGE_LIST_BD:
        lang = GlobalDataStruct::LANGUAGE_LIST_BD;
        break;
    default:
        lang =GlobalDataStruct::LANGUAGE_LIST_ENG;
        break;
    }

    return lang;
}

QString ConfigInformation::getSavedLanguageStr()
{
    auto idx = ConfigInformation::getDefaultLanguage();
    return ConfigInformation::getLanguagetoTranslationFileNameString(idx);
}

GlobalDataStruct::LANGUAGE_LIST ConfigInformation::changeCurrentLanguage(const GlobalDataStruct::LANGUAGE_LIST &language)
{
    d->mDefaultLanguage = language;

    QString lang = this->getLanguagetoTranslationFileNameString(d->mChangeLanguage);
    QString convertLang = this->getLanguagetoTranslationFileNameString(language);

    qApp->removeTranslator(d->mProgramLanguage.data());

    if(d->mProgramLanguage->load(convertLang))
        qApp->installTranslator(d->mProgramLanguage.data());

    this->loadAllConentsData();

    return d->mDefaultLanguage;
}

QString ConfigInformation::getCurrentLanguagetoString()
{
    d->mDefaultLanguage = this->getDefaultLanguage();
    return this->getLanguagetoTranslationFileNameString(d->mDefaultLanguage);;
}

QString ConfigInformation::getLanguagetoString(const GlobalDataStruct::LANGUAGE_LIST &language)
{
    QString lang;

    switch(language)
    {
    case GlobalDataStruct::LANGUAGE_LIST_KOR:
        lang = "한국어";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_FR:
        lang = "français";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_AR:
        lang = "اعربة";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_RO:
        lang = "română";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_RU:
        lang = "кий";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_DE:
        lang = "Deutsch";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_BD:
        lang = "বাংলা";
        break;
    default:
        lang = "English";
        break;
    }

    return lang;
}

QString ConfigInformation::getLanguagetoTranslationFileNameString(const GlobalDataStruct::LANGUAGE_LIST &language)
{
    QString lang;

    switch(language)
    {
    case GlobalDataStruct::LANGUAGE_LIST_KOR:
        lang = "lang_ko.qm";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_FR:
        lang = "lang_fr.qm";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_AR:
        lang ="lang_ar.qm";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_RO:
        lang ="lang_ro.qm";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_RU:
        lang ="lang_ru.qm";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_DE:
        lang = "lang_de.qm";
        break;
    case GlobalDataStruct::LANGUAGE_LIST_BD:
        lang = "lang_bd.qm";
        break;
    default:
        lang = "lang_en.qm";
        break;
    }
    return lang;
}

GlobalDataStruct::LANGUAGE_LIST ConfigInformation::getExportChangeLanguage()
{
    return d->mChangeLanguage;
}

void ConfigInformation::setExportChangeLanguage(GlobalDataStruct::LANGUAGE_LIST language)
{
    d->mChangeLanguage = language;
}

void ConfigInformation::setCurrentLanguage(GlobalDataStruct::LANGUAGE_LIST language)
{
    GenericIniDataControls generalData;
    generalData.writeCurrentLanguage(language);
}

GlobalDataStruct::LANGUAGE_LIST ConfigInformation::getExportChangeSecondLanguage()
{
    return d->mSecondLanguage;
}

void ConfigInformation::setExportChangeSecondLanguage(GlobalDataStruct::LANGUAGE_LIST language)
{
    d->mSecondLanguage = language;
}

QByteArray ConfigInformation::makeSequenceInputMode(bool isStart)
{
    uchar pMode;

    if(isStart == true)
        pMode = ConfigInformation::SEQUENCE_STATE_START;
    else
        pMode = ConfigInformation::SEQUENCE_STATE_END;

    QString protocolStr = WriteProtocolSequenceGenrator::writeSQInputMode(pMode);
    QByteArray sendData = protocolStr.toUtf8();

    return sendData;
}

QByteArray ConfigInformation::makeSequenceTotalStep(uchar totalStep)
{
    QString protocolStr = WriteProtocolSequenceGenrator::writeSQTotalStep(totalStep);
    QByteArray sendData = protocolStr.toUtf8();

    return sendData;
}

QByteArray ConfigInformation::makeSequenceSave(uchar smb)
{
    QString protocolStr = WriteProtocolSequenceGenrator::writeSQSave(smb);
    QByteArray sendData = protocolStr.toUtf8();

    return sendData;
}

QByteArray ConfigInformation::makeSequenceID(uchar smb, ushort sequenceID)
{
    QString protocolStr = WriteProtocolSequenceGenrator::writeSQID(smb, sequenceID);
    QByteArray sendData = protocolStr.toUtf8();

    return sendData;
}

QByteArray ConfigInformation::makeStepNumber(uchar stepNumber)
{
    QString protocolStr = WriteProtocolSequenceGenrator::writeSQStepNumber(stepNumber);
    QByteArray sendData = protocolStr.toUtf8();

    return sendData;
}

QByteArray ConfigInformation::makeProcessNumber(ushort processNumber, ushort processIndex)
{
    QString protocolStr = WriteProtocolSequenceGenrator::writeSQProcessNumber(processNumber, processIndex);
    QByteArray sendData = protocolStr.toUtf8();

    return sendData;
}
