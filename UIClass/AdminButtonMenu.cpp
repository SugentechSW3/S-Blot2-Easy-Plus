#include "AdminButtonMenu.h"
#include "ui_AdminButtonMenu.h"

AdminButtonMenu::AdminButtonMenu(QWidget *parent) :
                                                    CSerialNetworkWidget(parent),
                                                    ui(new Ui::AdminButtonMenu)
{
    ui->setupUi(this);
    this->init();
}

AdminButtonMenu::~AdminButtonMenu()
{
}

void AdminButtonMenu::initVariables()
{
}

void AdminButtonMenu::initSignalSlots()
{
    connect(ui->btnCamera, &QPushButton::clicked, this, &AdminButtonMenu::onClickedCamera);
    connect(ui->btnETC, &QPushButton::clicked, this, &AdminButtonMenu::onClickedETC);
    connect(ui->btnSequenceView, &QPushButton::clicked, this, &AdminButtonMenu::onClickedSequence);
    connect(ui->btnSemiAutoView, &QPushButton::clicked, this, &AdminButtonMenu::onClickedSemiAutoView);
    connect(ui->btnConfig, &QPushButton::clicked, this, &AdminButtonMenu::onClickedConfig);
    connect(ui->btnWorkPlace, &QPushButton::clicked, this, &AdminButtonMenu::onClickedWorkPlace);
    connect(ui->btnResultView, &QPushButton::clicked, this, &AdminButtonMenu::onClickedResultView);
    connect(ui->btnIPSetting, &QPushButton::clicked, this, &AdminButtonMenu::onClickedServerIPSetting);
    connect(ui->btnLanguage, &QPushButton::clicked, this, &AdminButtonMenu::onClickedLanguageSelect);
    connect(ui->btnDataExchange, &QPushButton::clicked, this, &AdminButtonMenu::onClickedDataExchange);
    connect(ui->btnHiddenBand, &QPushButton::clicked, this, &AdminButtonMenu::onClickedHiddenBand);
}

void AdminButtonMenu::startWidget()
{

}

void AdminButtonMenu::stopWidget()
{

}
