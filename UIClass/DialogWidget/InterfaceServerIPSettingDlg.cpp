#include <QRegExpValidator>

#include "InterfaceServerIPSettingDlg.h"
#include "ui_InterfaceServerIPSettingDlg.h"

#include "Common/Io/INIFileControls/GenericIniDataControls.h"


InterfaceServerIPSettingDlg::InterfaceServerIPSettingDlg(QWidget *parent) :
                                                                            CDialogParentWidget(parent),
                                                                            ui(new Ui_InterfaceServerIPSettingDlg)
{
    ui->setupUi(this);
    this->initSignalSlots();
    this->initIPAddressExp();
    this->initPortExp();
    this->loadData();
}

InterfaceServerIPSettingDlg::~InterfaceServerIPSettingDlg()
{

}

QString InterfaceServerIPSettingDlg::getIPAddress()
{
    GenericIniDataControls genericData;
    return genericData.readServerIPAddress();
}

QString InterfaceServerIPSettingDlg::getPort()
{
    GenericIniDataControls genericData;
    return genericData.readServerPortAddress();
}

void InterfaceServerIPSettingDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, [this]
            {
                this->saveData();
                this->accept();
            });

    connect(ui->btnCancel, &QPushButton::clicked, this, [this]
            {
                this->reject();
            });
}

void InterfaceServerIPSettingDlg::initIPAddressExp()
{
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";

    QRegExp ipRegex ("^" + ipRange
                    + "." + ipRange
                    + "." + ipRange
                    + "." + ipRange + "$");

    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->editServerIP->setValidator(ipValidator);
}

void InterfaceServerIPSettingDlg::initPortExp()
{
    ui->editPort->setValidator(new QIntValidator(0, 65535, this));
}

void InterfaceServerIPSettingDlg::loadData()
{
    ui->editServerIP->setText(this->getIPAddress());
    ui->editPort->setText(this->getPort());
}

void InterfaceServerIPSettingDlg::saveData()
{
    auto ipAddress = ui->editServerIP->text();
    auto port = ui->editPort->text();

    GenericIniDataControls genericData;
    genericData.writeServerIPAddress(ipAddress);
    genericData.writeServerPortAddress(port);
}
