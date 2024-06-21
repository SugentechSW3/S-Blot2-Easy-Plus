#include "UserButtonMenu.h"
#include "ui_UserButtonMenu.h"

#include "UIClass/CustomWidget/CustomImageButtonWidget.h"

UserButtonMenu::UserButtonMenu(QWidget *parent) :
                                                  CSerialNetworkWidget(parent),
                                                  ui(new Ui_UserButtonMenu)
{
    ui->setupUi(this);
    this->init();
    this->initStyleSheet();
}

UserButtonMenu::~UserButtonMenu()
{
}

void UserButtonMenu::initVariables()
{
}

void UserButtonMenu::initSignalSlots()
{
    connect(ui->btnAutoClean, &QPushButton::clicked, this, &UserButtonMenu::onClickedAutoClean);
    connect(ui->btnConfigure, &QPushButton::clicked, this, &UserButtonMenu::onClickedConfigure);
    connect(ui->btnReset, &QPushButton::clicked, this, &UserButtonMenu::onClickedReset);
    connect(ui->btnWorkPlace, &QPushButton::clicked, this, &UserButtonMenu::onClickedWorkPlace);
    connect(ui->btnResultView, &QPushButton::clicked, this, &UserButtonMenu::onClickedResultView);
    connect(ui->btnStripScan, &QPushButton::clicked, this, &UserButtonMenu::onClickedStripScan);
    connect(ui->btnAspration, &QPushButton::clicked, this, &UserButtonMenu::onClickedAspiration);
    connect(ui->btnContentsChange, &QPushButton::clicked, this, &UserButtonMenu::onClickedContentsChange);
}

void UserButtonMenu::initStyleSheet()
{
    CustomImageButtonWidget* userButton[] = {ui->btnWorkPlace, ui->btnResultView, ui->btnReset, ui->btnStripScan,ui->btnAutoClean,ui->btnConfigure
                                            , ui->btnAspration};
    ui->btnWorkPlace->setStyleSheetImage(":/Image/Image/Home/workplace_normal.png", ":/Image/Image/Home/workplace_over.png",
                                         ":/Image/Image/Home/workplace_focus.png", ":/Image/Image/Home/workplace_disable.png");

    ui->btnResultView->setStyleSheetImage(":/Image/Image/Home/resultview_normal.png", ":/Image/Image/Home/resultview_over.png"
                                          ,":/Image/Image/Home/resultview_focus.png", ":/Image/Image/Home/resultview_disable.png");

    ui->btnReset->setStyleSheetImage(":/Image/Image/Home/reset_normal.png", ":/Image/Image/Home/reset_over.png"
                                     ,":/Image/Image/Home/reset_focus.png", ":/Image/Image/Home/reset_disable.png");

    ui->btnStripScan->setStyleSheetImage(":/Image/Image/Home/stripscan_normal.png", ":/Image/Image/Home/stripscan_over.png"
                                     ,":/Image/Image/Home/stripscan_focus.png", ":/Image/Image/Home/stripscan_disable.png");

    ui->btnAutoClean->setStyleSheetImage(":/Image/Image/Home/autoclean_normal.png", ":/Image/Image/Home/autoclean_over.png"
                                     ,":/Image/Image/Home/autoclean_focus.png", ":/Image/Image/Home/autoclean_disable.png");

    ui->btnConfigure->setStyleSheetImage(":/Image/Image/Home/configure_normal.png", ":/Image/Image/Home/configure_over.png"
                                         ,":/Image/Image/Home/configure_focus.png", ":/Image/Image/Home/configure_disable.png");

    ui->btnAspration->setStyleSheetImage(":/Image/Image/Home/aspiration_normal.png", ":/Image/Image/Home/aspiration_over.png"
                                         ,":/Image/Image/Home/aspiration_focus.png", ":/Image/Image/Home/aspiration_disable.png");

    ui->btnContentsChange->setStyleSheetImage(":/Image/Image/Home/contentschange_normal.png", ":/Image/Image/Home/contentschange_over.png"
                                              ,":/Image/Image/Home/contentschange_focus.png", ":/Image/Image/Home/contentschange_disable.png");


    for(auto& itr:userButton)
    {
        itr->setGraphicsDropShadowEffect(true);
    }
}

void UserButtonMenu::startWidget()
{
}

void UserButtonMenu::stopWidget()
{
}

void UserButtonMenu::setOfflineMode()
{
    ui->btnWorkPlace->setDisabled(true);
    ui->btnReset->setDisabled(true);
    ui->btnConfigure->setDisabled(true);
    ui->btnAutoClean->setDisabled(true);
    ui->btnStripScan->setDisabled(true);
    ui->btnAspration->setDisabled(true);
    ui->btnContentsChange->setDisabled(true);
}
