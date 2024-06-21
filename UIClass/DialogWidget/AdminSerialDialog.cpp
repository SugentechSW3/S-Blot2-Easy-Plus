#include "AdminSerialDialog.h"
#include "ui_AdminSerialDialog.h"
#include "Network/SerialCtrl.h"

#include <QListWidgetItem>
#include <QListView>

struct AdminSerialDialog::privateStruct
{
    SerialCtrl* mSerialNetwork = nullptr;
};

AdminSerialDialog::AdminSerialDialog(QWidget *parent) :
    QDialog(parent),d(new privateStruct),
    ui(new Ui::AdminSerialDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_ShowWithoutActivating);
    this->initVarialbes();
    this->initSignalSlot();
}

AdminSerialDialog::~AdminSerialDialog()
{
    ui->listWidget->clear();
}

void AdminSerialDialog::initVarialbes()
{
    d->mSerialNetwork = SerialCtrl::getInstance();    
}

void AdminSerialDialog::initSignalSlot()
{
    connect(d->mSerialNetwork, &SerialCtrl::onRecvDataForSerial, this, &AdminSerialDialog::onRecvData);

    connect(d->mSerialNetwork, &SerialCtrl::onSendControl,this,[=]
    (QByteArray sendMessage){
        this->isAddWidgetText(QString("(SerialCtrl::) Send : " + sendMessage));
    });

    connect(ui->btnSerialDataSubmit, &QPushButton::clicked, this, &AdminSerialDialog::onClickedSeiralDataSend);
}

bool AdminSerialDialog::listWidgetScrollControl()
{
    ui->listWidget->scrollToBottom();
}

bool AdminSerialDialog::isAddWidgetText(const QString &message)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText(message);
    ui->listWidget->addItem(item);
    this->listWidgetScrollControl();
}

void AdminSerialDialog::onClickedSeiralDataSend()
{
    auto sendMessage = ui->edtSendMessage->text();

    //this->isAddWidgetText(QString("(SerialCtrl::) Send : " + sendMessage));

    if(d->mSerialNetwork->isConnected())
        d->mSerialNetwork->writeDirectDataTansmit(sendMessage.toUtf8());

}

void AdminSerialDialog::onRecvData(QByteArray recvData)
{
     QString recvLog(recvData);
     QString listWriteData = "(SerialCtrl::) Recv : " + recvLog;
     QListWidgetItem* item = new QListWidgetItem;
     item->setText(QString("(SerialCtrl::) Recv : " + recvLog));
     ui->listWidget->addItem(item);
     this->listWidgetScrollControl();
}
