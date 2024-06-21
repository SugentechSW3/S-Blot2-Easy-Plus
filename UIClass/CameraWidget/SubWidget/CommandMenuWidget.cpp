#include <QFileDialog>
#include <QTimer>

#include "CommandMenuWidget.h"
#include "ui_CommandMenuWidget.h"

struct CommandMenuWidget::privateStruct
{
    QTimer mTimer;
};

CommandMenuWidget::CommandMenuWidget(QWidget *parent) :
                                                        QWidget(parent),
                                                        ui(new Ui_CommandMenuWidget),
                                                        d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
}

CommandMenuWidget::~CommandMenuWidget()
{
}

void CommandMenuWidget::init()
{
    d->mTimer.setSingleShot(true);

    connect(ui->btnAnalysis, SIGNAL(clicked()), this, SIGNAL(onSelectedAnalysis()));
    connect(ui->btnFindPC, SIGNAL(clicked()), this, SIGNAL(onSelectedFindPC()));
    connect(ui->btnAngleReset, SIGNAL(clicked()), this, SIGNAL(onSelectedAngleReset()));
    connect(ui->btnQRReadTest, &QPushButton::clicked, this, &CommandMenuWidget::onReadQRTest);
    connect(ui->btnLoadImage, SIGNAL(clicked()), this, SLOT(onClickedLoadImage()));
    connect(ui->btnSaveImage, SIGNAL(clicked()), this, SLOT(onClickedSaveImage()));

    connect(ui->btnAnalysis, &QPushButton::clicked, this, &CommandMenuWidget::onDelayButton);
    connect(ui->btnFindPC, &QPushButton::clicked, this, &CommandMenuWidget::onDelayButton);
    connect(ui->checkGraph, &QCheckBox::clicked, this, &CommandMenuWidget::onGraphHide);
    connect(&d->mTimer, &QTimer::timeout, this, [this]
            {
                ui->btnAnalysis->setEnabled(true);
                ui->btnFindPC->setEnabled(true);
            });
}

void CommandMenuWidget::onClickedLoadImage()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Load Image"), QCoreApplication::applicationDirPath(), "Image Files (*.png *.jpg *.jpeg *.bmp)", nullptr, QFileDialog::DontUseNativeDialog);

    if(fileName.isEmpty() == false)
        emit onSelectedLoadImage(fileName);
}

void CommandMenuWidget::onClickedSaveImage()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QCoreApplication::applicationDirPath(), QString("Image Files (*.png *.jpeg *.jpg *.bmp)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(fileName.isEmpty() == false)
        emit onSelectedSaveImage(fileName);
}

void CommandMenuWidget::onDelayButton(bool)
{
    ui->btnAnalysis->setDisabled(true);
    ui->btnFindPC->setDisabled(true);
    d->mTimer.start(3000);
}
