#include <QStandardItem>
#include <QApplication>
#include <QFileDialog>
#include <QThread>

#include "SequenceView.h"
#include "ui_SequenceView.h"

#include "Common/Io/CSettingIo.h"
#include "Common/CUtil.h"

#include "Network/PacketReader/PacketReader.h"

#include "Network/PacketWriter/WriteProtocolSequenceGenrator.h"

#include "SubWidget/PrimeWidget.h"
#include "SubWidget/Prime2Widget.h"
#include "SubWidget/Prime3Widget.h"
#include "SubWidget/DispenseWidget.h"
#include "SubWidget/Dispense2Widget.h"
#include "SubWidget/DispAsp1Widget.h"
#include "SubWidget/DispAsp2Widget.h"
#include "SubWidget/DispAsp3Widget.h"
#include "SubWidget/IncubationWidget.h"
#include "SubWidget/IncubationWSWidget.h"
#include "SubWidget/AspirationWidget.h"
#include "SubWidget/DryWidget.h"
#include "SubWidget/RollBackWidget.h"
#include "SubWidget/RollBack2Widget.h"
#include "SubWidget/RollBack3Widget.h"
#include "SubWidget/DPrimeWidget.h"
#include "SubWidget/DRollBackWidget.h"
#include "SubWidget/DDispenseWidget.h"

#include "UIClass/DialogWidget/CirclePrograssDialog.h"

#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct SequenceView::privateStruct
{
    QScopedPointer<PrimeWidget> mPrimeWidget;
    QScopedPointer<Prime2Widget> mPrime2Widget;
    QScopedPointer<Prime3Widget> mPrime3Widget;
    QScopedPointer<DispenseWidget> mDispenseWidget;
    QScopedPointer<Dispense2Widget> mDispense2Widget;
    QScopedPointer<DispAsp1Widget> mDispAsp1Widget;
    QScopedPointer<DispAsp2Widget> mDispAsp2Widget;
    QScopedPointer<DispAsp3Widget> mDispAsp3Widget;
    QScopedPointer<IncubationWidget> mIncubationWidget;
    QScopedPointer<IncubationWSWidget> mIncubationWSWidget;
    QScopedPointer<AspirationWidget> mAspriationWidget;
    QScopedPointer<DryWidget> mDryWidget;
    QScopedPointer<RollBackWidget> mRollBackWidget;
    QScopedPointer<RollBack2Widget> mRollBack2Widget;
    QScopedPointer<RollBack3Widget> mRollBack3Widget;
    QScopedPointer<DPrimeWidget> mDPrimeWidget;
    QScopedPointer<DRollBackWidget> mDRollBackWidget;
    QScopedPointer<DDispenseWidget> mDDispenseWidget;
    QScopedPointer<CirclePrograssDialog> mPrograssCircleDlg;

    CommonSequenceWidget* mCurrentProcessWidget = nullptr;
    SerialCtrl* mSerialInstance = nullptr;
};

SequenceView::SequenceView(QWidget *parent) :
                                              CSerialNetworkWidget(parent),
                                              ui(new Ui_SequenceView),
                                              d(new privateStruct)
{
    qRegisterMetaTypeStreamOperators<GlobalDataStruct::PROCESS_DATA>();
    ui->setupUi(this);
    this->init();
    ui->treeWidgetSequence->setDragEnabled(true);
    ui->treeWidgetSequence->setAcceptDrops(true);
    ui->treeWidgetSequence->setDropIndicatorShown(true);
    ui->treeWidgetSequence->setDragDropMode(QAbstractItemView::InternalMove);

    auto rooItem = ui->treeWidgetSequence->invisibleRootItem();
    rooItem->setFlags( rooItem->flags() ^ Qt::ItemIsDropEnabled );
}

SequenceView::~SequenceView()
{

}

void SequenceView::initVariables()
{
    this->initWidgets();
    this->setComboBoxData();
    this->setListData();

    d->mSerialInstance = this->getSerialInstance();
}

void SequenceView::initSignalSlots()
{
    connect(ui->btnAddStep, SIGNAL(clicked()), SLOT(onClickAddStep()));
    connect(ui->btnAddProcess, SIGNAL(clicked()), SLOT(onClickAddProcess()));
    connect(ui->btnChangeConfiguration, SIGNAL(clicked()), SLOT(onClickChange()));
    connect(ui->btnDeleteItem, SIGNAL(clicked()), SLOT(onClickDelete()));
    connect(ui->btnClearSequence, SIGNAL(clicked()), SLOT(onClickClear()));
    connect(ui->btnSaveSequence, SIGNAL(clicked()), SLOT(onClickSaveSequence()));
    connect(ui->btnLoadSequence, SIGNAL(clicked()), SLOT(onClickLoadSequence()));
    connect(ui->btnAccept, &QPushButton::clicked, this, &SequenceView::onAccept);
    connect(ui->treeWidgetSequence, SIGNAL(itemSelectionChanged()), SLOT(onUpdateSelectChanged()));
    connect(ui->comboContents, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SequenceView::onContentsChanged);//
    connect(d->mSerialInstance, SIGNAL(onSequenceSendFinished()), SLOT(onFinishSeqeuence()));
}

void SequenceView::startWidget()
{

}

void SequenceView::stopWidget()
{
}

void SequenceView::initWidgets()
{
    d->mPrimeWidget.reset(new PrimeWidget(ui->widgetConfiguration));
    d->mPrime2Widget.reset(new Prime2Widget(ui->widgetConfiguration));
    d->mPrime3Widget.reset(new Prime3Widget(ui->widgetConfiguration));
    d->mDispenseWidget.reset(new DispenseWidget(ui->widgetConfiguration));
    d->mDispense2Widget.reset(new Dispense2Widget(ui->widgetConfiguration));
    d->mDispAsp1Widget.reset(new DispAsp1Widget(ui->widgetConfiguration));
    d->mDispAsp2Widget.reset(new DispAsp2Widget(ui->widgetConfiguration));
    d->mDispAsp3Widget.reset(new DispAsp3Widget(ui->widgetConfiguration));
    d->mIncubationWidget.reset(new IncubationWidget(ui->widgetConfiguration));
    d->mIncubationWSWidget.reset(new IncubationWSWidget(ui->widgetConfiguration));
    d->mAspriationWidget.reset(new AspirationWidget(ui->widgetConfiguration));
    d->mDryWidget.reset(new DryWidget(ui->widgetConfiguration));
    d->mRollBackWidget.reset(new RollBackWidget(ui->widgetConfiguration));
    d->mRollBack2Widget.reset(new RollBack2Widget(ui->widgetConfiguration));
    d->mRollBack3Widget.reset(new RollBack3Widget(ui->widgetConfiguration));

    d->mDRollBackWidget.reset(new DRollBackWidget(ui->widgetConfiguration));
    d->mDPrimeWidget.reset(new DPrimeWidget(ui->widgetConfiguration));
    d->mDDispenseWidget.reset(new DDispenseWidget(ui->widgetConfiguration));

    d->mPrograssCircleDlg.reset(new CirclePrograssDialog(this));

    ui->groupBoxLayout->addWidget(d->mPrimeWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mPrime2Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mPrime3Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDispenseWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDispense2Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDispAsp1Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDispAsp2Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDispAsp3Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mAspriationWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mIncubationWidget.data(),0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mIncubationWSWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDryWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mRollBackWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mRollBack2Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mRollBack3Widget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDRollBackWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDPrimeWidget.data(), 0,0,1,1);
    ui->groupBoxLayout->addWidget(d->mDDispenseWidget.data(), 0,0,1,1);

    this->hideAllViews();

    auto config = ConfigInformation::getInstance();
    auto contentsList = config->getContentsList();

    for(int i = 0; i < contentsList.count(); i++)
        ui->comboContents->addItem(contentsList[i]);
}

void SequenceView::setComboBoxData()
{
    ui->comboStep->insertItems(0, QStringList()
                                   << QApplication::translate("SequenceView", "Wetting", Q_NULLPTR)
                                   << QApplication::translate("SequenceView", "Dispense", Q_NULLPTR) //분주
                                   << QApplication::translate("SequenceView", "Washing", Q_NULLPTR)
                                   << QApplication::translate("SequenceView", "Antibody", Q_NULLPTR) //항체
                                   << QApplication::translate("SequenceView", "Washing 2", Q_NULLPTR)
                                   << QApplication::translate("SequenceView", "Enzyme", Q_NULLPTR)  //효소
                                   << QApplication::translate("SequenceView", "Washing DW", Q_NULLPTR)
                                   << QApplication::translate("SequenceView", "Substrate", Q_NULLPTR)
                                   << QApplication::translate("SequenceView", "Drying", Q_NULLPTR)
                                   << QApplication::translate("SequenceView", "Analysis", Q_NULLPTR)

                               );

    ui->comboStep->setItemData(0, ConfigInformation::STEP_LIST_WETTING);
    ui->comboStep->setItemData(1, ConfigInformation::STEP_LIST_DISPENSE);
    ui->comboStep->setItemData(2, ConfigInformation::STEP_LIST_WASHING1);
    ui->comboStep->setItemData(3, ConfigInformation::STEP_LIST_ANTIBODY);
    ui->comboStep->setItemData(4, ConfigInformation::STEP_LIST_WASHING2);
    ui->comboStep->setItemData(5, ConfigInformation::STEP_LIST_ENZYME);
    ui->comboStep->setItemData(6, ConfigInformation::STEP_LIST_WASHINGDW);
    ui->comboStep->setItemData(7, ConfigInformation::STEP_LIST_SUBSTRATE);
    ui->comboStep->setItemData(8, ConfigInformation::STEP_LIST_DRY);
    ui->comboStep->setItemData(9, ConfigInformation::STEP_LIST_ANALYSIS);
}

void SequenceView::setListData()
{
    QListWidgetItem* item = nullptr;

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Prime", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_PRIME);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Roll Back", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_ROLL_BACK);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Dispense", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_DISPENSE);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Aspiration", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_ASPIRATION);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Incubation", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_INCUBATION);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Buzzer", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_BUZZER);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Dry", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_DRY);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Pause", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_PAUSE);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "Sample Dispense", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_SAMPLE_DISPENSE);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "D_Prime", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_PRIME_D);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "D_Roll Back", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_ROLL_BACK_D);
    ui->listProcess->addItem(item);

    item = new QListWidgetItem(QApplication::translate("SequenceView", "D_Dispense", Q_NULLPTR));
    item->setData(Qt::UserRole, ConfigInformation::PROCESS_LIST_DISPENSE_D);
    ui->listProcess->addItem(item);
}

QTreeWidgetItem* SequenceView::addStep(const QString &stepName, const uchar stepNumber)
{
    auto item = new QTreeWidgetItem(ui->treeWidgetSequence);
    item->setText(0, stepName);
    item->setData(0, Qt::UserRole, stepNumber);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled);
    item->setExpanded(true);

    return item;
}

QTreeWidgetItem* SequenceView::addProcess(QTreeWidgetItem* topLevelWidget, const QString& processName, const GlobalDataStruct::PROCESS_DATA& processData)
{
    auto processItem = new QTreeWidgetItem(topLevelWidget);

    processItem->setText(0, processName);
    processItem->setData(0, Qt::UserRole, this->getQVariantFromProcessData(processData));
    processItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

    return processItem;
}

void SequenceView::onClickAddStep()
{
    auto stepName = ui->comboStep->currentText();
    auto stepNumber = ui->comboStep->currentData().toInt();

    this->addStep(stepName, stepNumber);
}

void SequenceView::onClickAddProcess()
{
    if(ui->listProcess->currentRow() == -1)
        return;

    auto topLevelWidget = this->getCurrentTopLevelWidgetItem();
    auto processName = ui->listProcess->currentItem()->text();
    auto processNumber = ui->listProcess->currentItem()->data(Qt::UserRole).toInt();

    GlobalDataStruct::PROCESS_DATA data;
    data.processNumber = processNumber;

    this->addProcess(topLevelWidget, processName, data);
}

void SequenceView::onClickChange()
{
    auto currentItem = ui->treeWidgetSequence->currentItem();

    if(currentItem == nullptr)
        return;

    if(this->isTopLevelTreeItem(currentItem) == true)
        return;

    if(d->mCurrentProcessWidget == nullptr)
        return;

    auto dataStruct = this->getProcessDataFromQVariant(currentItem->data(0, Qt::UserRole));
    dataStruct.data =  d->mCurrentProcessWidget->getConfigParameters();

    currentItem->setData(0, Qt::UserRole, this->getQVariantFromProcessData(dataStruct));
}

void SequenceView::onClickDelete()
{
    auto current = ui->treeWidgetSequence->currentItem();

    if(current == nullptr)
        return;

    delete ui->treeWidgetSequence->currentItem();
}

void SequenceView::onClickClear()
{
    ui->treeWidgetSequence->clear();
}

void SequenceView::onClickSaveSequence()
{
    auto filePath = QFileDialog::getSaveFileName(this, tr("Save Sequence"), QApplication::applicationDirPath(), tr("INI File (*.ini)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(filePath.isEmpty())
        return;

    QVector<GlobalDataStruct::SEQUENCE_STRUCT> data;

    for(int stepIdx = 0; stepIdx < ui->treeWidgetSequence->topLevelItemCount(); stepIdx++)
    {
        GlobalDataStruct::SEQUENCE_STRUCT stepData;
        auto topItem = ui->treeWidgetSequence->topLevelItem(stepIdx);
        auto stepNumber = topItem->data(0, Qt::UserRole).toInt();

        stepData.stepNumber = stepNumber;

        for(auto processIdx = 0; processIdx< topItem->childCount(); processIdx++)
        {
            auto childItem = topItem->child(processIdx);
            auto dataStruct = this->getProcessDataFromQVariant(childItem->data(0, Qt::UserRole));

            stepData.processData.push_back(dataStruct);
        }
        data.push_back(stepData);
    }

    ConfigInformation::getInstance()->saveSequence(ui->comboContents->currentText(),data,filePath);
}

void SequenceView::onClickLoadSequence()
{
    ui->treeWidgetSequence->clear();
    auto filePath = QFileDialog::getOpenFileName(this, tr("Load Sequence"), QApplication::applicationDirPath(), tr("INI File (*.ini)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(filePath.isEmpty() == true)
        return;

    auto seqeunceList = ConfigInformation::getInstance()->loadSequenceFromFile(ui->comboContents->currentText(), filePath);

    if(seqeunceList.isEmpty() == true)
        return;

    for(int stepIdx = 0; stepIdx < seqeunceList.count(); stepIdx++)
    {
        auto stepNumber = seqeunceList[stepIdx].stepNumber;
        auto childCount = seqeunceList[stepIdx].processData.count();

        auto treeStepItem =  this->addStep(this->findStepName(stepNumber), stepNumber);

        for(int processIdx = 0; processIdx < childCount; processIdx++)
        {
            auto processNumber = seqeunceList[stepIdx].processData[processIdx].processNumber;
            this->addProcess(treeStepItem, this->findProcessName(processNumber), seqeunceList[stepIdx].processData[processIdx]);
        }
    }
}

void SequenceView::onAccept(bool)
{
    if(ui->treeWidgetSequence->topLevelItemCount() == 0)
        return;

    this->deviceAcceptAfterSave();

    QVector<QByteArray> protocolQueue;
    this->stackSequenceCommand(ui->treeWidgetSequence, protocolQueue);
    protocolQueue.push_front(ConfigInformation::makeSequenceInputMode(true));
    protocolQueue.push_back(ConfigInformation::makeSequenceInputMode(false));

    SerialCtrl::getInstance()->inputSequence(protocolQueue);
    d->mPrograssCircleDlg->start();
}

void SequenceView::onUpdateSelectChanged()
{
    this->hideAllViews();//
    auto item = ui->treeWidgetSequence->currentItem();
    ui->btnChangeConfiguration->setDisabled(true);
    d->mCurrentProcessWidget = nullptr;

    if(item == nullptr)
        return;

    if(this->isTopLevelTreeItem(item) == true)
        return;

    auto val =  item->data(0, Qt::UserRole).value<GlobalDataStruct::PROCESS_DATA>();
    ui->btnChangeConfiguration->setEnabled(true);

    switch(val.processNumber)
    {
    case ConfigInformation::PROCESS_LIST_PRIME:
        this->changePrime1(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_ROLL_BACK:
        this->changeRollBack1(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_DISPENSE:
        this->changeDispense1(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_ASPIRATION:
        this->changeAspration(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_INCUBATION:
        this->changeIncubation(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_BUZZER:
        break;

    case ConfigInformation::PROCESS_LIST_DRY:
        this->changeDry(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_PAUSE:
        break;

    case ConfigInformation::PROCESS_LIST_SAMPLE_DISPENSE:
        break;

    case ConfigInformation::PROCESS_LIST_PRIME_D:
        this->changeDPrime(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_ROLL_BACK_D:
        this->changeDRollBack(val.data);
        break;

    case ConfigInformation::PROCESS_LIST_DISPENSE_D:
        this->changeDDispense(val.data);
        break;
    }
}

void SequenceView::stackSequenceCommand(QTreeWidget* widget, QVector<QByteArray>& sendData)
{
    sendData.clear();

    if(widget == nullptr)
        return;

    auto config = ConfigInformation::getInstance();
    auto rootItemCount = widget->topLevelItemCount();
    auto contentsIdx = config->getContentsList().indexOf(config->getCurrentContents());

    sendData.push_back(ConfigInformation::makeSequenceTotalStep(rootItemCount));
    sendData.push_back(ConfigInformation::makeSequenceID(1, contentsIdx));

    for(int i = 0; i < rootItemCount; i++)
    {
        auto currentItem = widget->topLevelItem(i);
        auto stepNumber = currentItem->data(0, Qt::UserRole).toInt();
        auto childCount = currentItem->childCount();

        sendData.push_back(ConfigInformation::makeStepNumber(stepNumber));

        for(int childIdx = 0; childIdx < childCount ; childIdx++)
        {
            auto childItem = currentItem->child(childIdx);
            auto dataStruct = this->getProcessDataFromQVariant(childItem->data(0, Qt::UserRole));

            sendData.push_back(ConfigInformation::makeProcessNumber(dataStruct.processNumber, childIdx));

            QString protocolStr;

            switch(dataStruct.processNumber)
            {
            case ConfigInformation::PROCESS_LIST_PRIME:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordPrime1(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_PRIME_PUMP_CHANNEL)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_PRIME_PUMP_TIME)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_ROLL_BACK:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordRollBack1(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_ROLL_BACK_PUMP_CHANNEL)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_ROLL_BACK_TIME)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_DISPENSE:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDispense1(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_DISPENSE_PUMP_CHANNEL)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_DISPENSE_PUMP_VOLUME)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_ASPIRATION:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordAspiration(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_ASPIRATION_TIME)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_ASPIRATION_COUNT)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_INCUBATION:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordIncubation(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_INCUBATION_TIME)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_BUZZER:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordBuzzer(3);
                break;

            case ConfigInformation::PROCESS_LIST_DRY:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDry(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_DRY_SPEED)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_HEATING_TIME)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_DRY_TIME)].toUInt());
                break;

            case ConfigInformation::PROCESS_LIST_PAUSE:
                break;

            case ConfigInformation::PROCESS_LIST_SAMPLE_DISPENSE:
                break;

            case ConfigInformation::PROCESS_LIST_PRIME_D:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDwordDPrime(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_PRIME_CHANNEL)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_OPTION)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_VOLUME)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_ROLL_BACK_D:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDwordDPrime(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_CHANNEL)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_OPTION)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_DURATION)].toUInt()
                    );  break;

            case ConfigInformation::PROCESS_LIST_DISPENSE_D:
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessParameterDwordDPrime(
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_CHANNEL)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_OPTION)].toUInt(),
                    dataStruct.data[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_VOLUME)].toUInt()
                    );  break;
            }

            if(protocolStr.isEmpty() == true)
                protocolStr = WriteProtocolSequenceGenrator::writeSQProcessNULLData();


            sendData.push_back(protocolStr.toUtf8());
        }
    }

    sendData.push_back(ConfigInformation::makeStepNumber(ConfigInformation::STEP_LIST_END));
    sendData.push_back(ConfigInformation::makeSequenceSave(1));
}

QString SequenceView::findStepName(int stepNumber)
{
    auto index = ui->comboStep->findData(stepNumber);

    if(index == -1)
        return QString::number(stepNumber);

    return ui->comboStep->itemText(index);
}

QString SequenceView::findProcessName(int processNumber)
{
    int index = -1;

    for(int i = 0; i < ui->listProcess->count(); i++)
    {
        auto currentData = ui->listProcess->item(i);

        if(currentData->data(Qt::UserRole).toInt() == processNumber)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
        return QString::number(processNumber);

    return ui->listProcess->item(index)->text();
}

void SequenceView::hideAllViews()
{
    if(d->mPrimeWidget.data() != nullptr)
        d->mPrimeWidget->hide();

    if(d->mPrime2Widget.data() != nullptr)
        d->mPrime2Widget->hide();

    if(d->mPrime3Widget.data() != nullptr)
        d->mPrime3Widget->hide();

    if(d->mDispenseWidget.data() != nullptr)
        d->mDispenseWidget->hide();

    if(d->mDispense2Widget.data() != nullptr)
        d->mDispense2Widget->hide();

    if(d->mDispAsp1Widget.data() != nullptr)
        d->mDispAsp1Widget->hide();

    if(d->mDispAsp2Widget.data() != nullptr)
        d->mDispAsp2Widget->hide();

    if(d->mDispAsp3Widget.data() != nullptr)
        d->mDispAsp3Widget->hide();

    if(d->mIncubationWidget.data() != nullptr)
        d->mIncubationWidget->hide();

    if(d->mIncubationWSWidget.data() != nullptr)
        d->mIncubationWSWidget->hide();

    if(d->mAspriationWidget.data() != nullptr)
        d->mAspriationWidget->hide();

    if(d->mDryWidget.data() != nullptr)
        d->mDryWidget->hide();

    if(d->mRollBackWidget.data() != nullptr)
        d->mRollBackWidget->hide();

    if(d->mRollBack2Widget.data() != nullptr)
        d->mRollBack2Widget->hide();

    if(d->mRollBack3Widget.data() != nullptr)
        d->mRollBack3Widget->hide();

    if(d->mDPrimeWidget.isNull() == false)
        d->mDPrimeWidget->hide();

    if(d->mDRollBackWidget.isNull() == false)
        d->mDRollBackWidget->hide();

    if(d->mDDispenseWidget.isNull() == false)
        d->mDDispenseWidget->hide();
}

void SequenceView::changePrime1(const QVariantMap &parameter)
{
    if(d->mPrimeWidget.data() == nullptr)
        return;

    d->mPrimeWidget->setConfigParameters(parameter);
    d->mPrimeWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mPrimeWidget.data());
}

void SequenceView::changePrime2(const QVariantMap &parameter)
{
    if(d->mPrime2Widget.data() == nullptr)
        return;

    d->mPrime2Widget->setConfigParameters(parameter);
    d->mPrime2Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mPrime2Widget.data());
}

void SequenceView::changePrime3(const QVariantMap &parameter)
{
    if(d->mPrime3Widget.data() == nullptr)
        return;

    d->mPrime3Widget->setConfigParameters(parameter);
    d->mPrime3Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mPrime3Widget.data());
}

void SequenceView::changeDispense1(const QVariantMap &parameter)
{
    if(d->mDispenseWidget.data() == nullptr)
        return;

    d->mDispenseWidget->setConfigParameters(parameter);
    d->mDispenseWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDispenseWidget.data());
}

void SequenceView::changeDispense2(const QVariantMap &parameter)
{
    if(d->mDispense2Widget.data() == nullptr)
        return;

    d->mDispense2Widget->setConfigParameters(parameter);
    d->mDispense2Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDispense2Widget.data());
}

void SequenceView::changeDispenseAsp1(const QVariantMap &parameter)
{
    if(d->mDispAsp1Widget.data() == nullptr)
        return;

    d->mDispAsp1Widget->setConfigParameters(parameter);
    d->mDispAsp1Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDispAsp1Widget.data());
}

void SequenceView::changeDispenseAsp2(const QVariantMap &parameter)
{
    if(d->mDispAsp2Widget.data() == nullptr)
        return;

    d->mDispAsp2Widget->setConfigParameters(parameter);
    d->mDispAsp2Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDispAsp2Widget.data());
}

void SequenceView::changeDispenseAsp3(const QVariantMap &parameter)
{
    if(d->mDispAsp3Widget.data() == nullptr)
        return;

    d->mDispAsp3Widget->setConfigParameters(parameter);
    d->mDispAsp3Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDispAsp3Widget.data());
}

void SequenceView::changeIncubation(const QVariantMap &parameter)
{
    if(d->mIncubationWidget.data() == nullptr)
        return;

    d->mIncubationWidget->setConfigParameters(parameter);
    d->mIncubationWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mIncubationWidget.data());
}

void SequenceView::changeIncubationWS(const QVariantMap &parameter)
{
    if(d->mIncubationWSWidget.data() == nullptr)
        return;

    d->mIncubationWSWidget->setConfigParameters(parameter);
    d->mIncubationWSWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mIncubationWSWidget.data());
}

void SequenceView::changeAspration(const QVariantMap &parameter)
{
    if(d->mAspriationWidget.data() == nullptr)
        return;

    d->mAspriationWidget->setConfigParameters(parameter);
    d->mAspriationWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mAspriationWidget.data());
}

void SequenceView::changeDry(const QVariantMap &parameter)
{
    if(d->mDryWidget.data() == nullptr)
        return;

    d->mDryWidget->setConfigParameters(parameter);
    d->mDryWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDryWidget.data());
}

void SequenceView::changeRollBack1(const QVariantMap &parameter)
{
    if(d->mRollBackWidget.data() == nullptr)
        return;

    d->mRollBackWidget->setConfigParameters(parameter);
    d->mRollBackWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mRollBackWidget.data());
}

void SequenceView::changeRollBack2(const QVariantMap &parameter)
{
    if(d->mRollBack2Widget.data() == nullptr)
        return;

    d->mRollBack2Widget->setConfigParameters(parameter);
    d->mRollBack2Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mRollBack2Widget.data());
}

void SequenceView::changeRollBack3(const QVariantMap &parameter)
{
    if(d->mRollBack3Widget.data() == nullptr)
        return;

    d->mRollBack3Widget->setConfigParameters(parameter);
    d->mRollBack3Widget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mRollBack3Widget.data());
}

void SequenceView::changeDPrime(const QVariantMap &parameter)
{
    if(d->mDPrimeWidget.data() == nullptr)
        return;

    d->mDPrimeWidget->setConfigParameters(parameter);
    d->mDPrimeWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDPrimeWidget.data());
}

void SequenceView::changeDRollBack(const QVariantMap &parameter)
{
    if(d->mDRollBackWidget.data() == nullptr)
        return;

    d->mDRollBackWidget->setConfigParameters(parameter);
    d->mDRollBackWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDRollBackWidget.data());
}

void SequenceView::changeDDispense(const QVariantMap &parameter)
{
    if(d->mDDispenseWidget.data() == nullptr)
        return;

    d->mDDispenseWidget->setConfigParameters(parameter);
    d->mDDispenseWidget->show();
    d->mCurrentProcessWidget = qobject_cast<CommonSequenceWidget*> (d->mDDispenseWidget.data());
}

QTreeWidgetItem* SequenceView::getCurrentTopLevelWidgetItem()
{
    auto curItem =  ui->treeWidgetSequence->currentItem();

    QTreeWidgetItem* topLevelWidget = nullptr;

    if(curItem == nullptr)
        return nullptr;

    if(this->isTopLevelTreeItem(curItem) == true)
        topLevelWidget = curItem;
    else
        topLevelWidget = curItem->parent();

    return topLevelWidget;
}

bool SequenceView::isTopLevelTreeItem(QTreeWidgetItem* curItem)
{
    if(curItem == nullptr)
        return false;

    if(curItem->parent() == nullptr)
        return true;
    else
        return false;
}

QVariant SequenceView::getQVariantFromProcessData(const GlobalDataStruct::PROCESS_DATA& data)
{
    QVariant variantData;
    variantData.setValue(data);

    return variantData;
}

GlobalDataStruct::PROCESS_DATA SequenceView::getProcessDataFromQVariant(const QVariant& data)
{
    return data.value<GlobalDataStruct::PROCESS_DATA>();
}

void SequenceView::deviceAcceptAfterSave()
{
    QString filePath(QDir::currentPath()+"/Config/TUNNING_DATA/DEVICE_SEQUENCE.ini");

       if(filePath.isEmpty())
           return;

       QVector<GlobalDataStruct::SEQUENCE_STRUCT> data;

       for(int stepIdx = 0; stepIdx < ui->treeWidgetSequence->topLevelItemCount(); stepIdx++)
       {
           GlobalDataStruct::SEQUENCE_STRUCT stepData;
           auto topItem = ui->treeWidgetSequence->topLevelItem(stepIdx);
           auto stepNumber = topItem->data(0, Qt::UserRole).toInt();

           stepData.stepNumber = stepNumber;

           for(auto processIdx = 0; processIdx< topItem->childCount(); processIdx++)
           {
               auto childItem = topItem->child(processIdx);
               auto dataStruct = this->getProcessDataFromQVariant(childItem->data(0, Qt::UserRole));

               stepData.processData.push_back(dataStruct);
           }
           data.push_back(stepData);
       }

       ConfigInformation::getInstance()->saveSequence(ui->comboContents->currentText(),data,filePath);
}

void SequenceView::onFinishSeqeuence()
{
    d->mPrograssCircleDlg->stop();
    CUtil::messageBox(tr("The sequence has been updated. Be sure to reboot SW."));
}

void SequenceView::onContentsChanged(int)//
{
    this->onClickClear();
    auto seqeunceList = ConfigInformation::getInstance()->loadSequenceFromFile(ui->comboContents->currentText());

    if(seqeunceList.isEmpty() == true)
        return;

    for(int stepIdx = 0; stepIdx < seqeunceList.count(); stepIdx++)
    {
        auto stepNumber = seqeunceList[stepIdx].stepNumber;
        auto childCount = seqeunceList[stepIdx].processData.count();

        auto treeStepItem =  this->addStep(this->findStepName(stepNumber), stepNumber);

        for(int processIdx = 0; processIdx < childCount; processIdx++)
        {
            auto processNumber = seqeunceList[stepIdx].processData[processIdx].processNumber;
            this->addProcess(treeStepItem, this->findProcessName(processNumber), seqeunceList[stepIdx].processData[processIdx]);
        }
    }
}
