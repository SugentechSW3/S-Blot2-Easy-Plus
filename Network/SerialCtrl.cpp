#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include "mainwindow.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"

#include "Common/Network/CSerialNetwork.h"
#include "Common/Network/CPacketTimer.h"
#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

#include "PacketWriter/WriteProtocolStatusRead.h"
#include "PacketWriter/WriteProtocolControlRead.h"
#include "PacketWriter/WriteProtocolControlWrite.h"
#include "PacketWriter/WriteProtocolCameraLedSetting.h"
#include "PacketWriter/WriteProtocolSequenceGenrator.h"

#include "PacketReader/PacketReader.h"
#include "SerialCtrl.h"

#include "Analysis/ConfigInformation.h"



// Creater      : sp.Park by SystemDevelopment
// Description  : C++ 11 Based SerialControl Class(SingleTon)
// Created Date : 2018-03-19

USING_SUGENTECH

struct SerialCtrl::privateStruct
{
    CSerialNetwork mNetwork;
    QByteArray mQueue;
    QByteArray mLastMsg;
    CPacketTimer mPacketTimer;
    QMutex mMutex;
    QMutex mQueueMutex;
    QMutex mSendFailCountMutex;

    QVector<QByteArray> mMessageQueue;

    QTimer mResetTimer;

    int mSendFailCount = 0;
    bool mIsConnected = true;
};

SerialCtrl::SerialCtrl(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    this->initSignalSlots();
    emit onDeviceIsConnected();
}

SerialCtrl::~SerialCtrl()
{
    this->closeSerial();
}

bool SerialCtrl::openSerial()
{
    static bool isError = false;
    if(d->mNetwork.isOpen() == false)
    {
        if(d->mNetwork.open(CSerialNetwork::OPEN_MODE_READ_WRITE, QSerialPort::Baud57600) == false)
        {
            if(isError == false)
            {
                CLogWriter::printLog("(SerialCtrl::) Open: Serial Can not Open ", CLogWriter::LOG_TYPE_RELEASE);
                isError = true;
            }
            return false;
        }
        CLogWriter::printLog(QString("(SerialCtrl::) Find Serial Port : %1").arg(CSerialNetwork::getConnectedSerialPortName()), CLogWriter::LOG_TYPE_RELEASE);
    }

    isError = false;

    return true;
}

void SerialCtrl::closeSerial()
{
    d->mNetwork.close();
}

bool SerialCtrl::isConnected()
{
    return d->mIsConnected;
}

void SerialCtrl::initPacketFailCount()
{
    d->mSendFailCountMutex.lock();
    d->mSendFailCount = 0;
    //CLogWriter::printLog("mSendFailCount init!");
    d->mSendFailCountMutex.unlock();
}

void SerialCtrl::initSignalSlots()
{

    connect(&d->mNetwork, &CSerialNetwork::recvData, this, &SerialCtrl::onCallBackFunction);
    connect(this, &SerialCtrl::onRecvDataForSerial, this, &SerialCtrl::onRecvControl,Qt::DirectConnection );

    connect(this, &SerialCtrl::onNetworkErrorMsg, this, [this]
            (QString errorMsg)
            {
                MessageShowDlg* dlg = new MessageShowDlg(errorMsg);
                dlg->open();
            });

    connect(&d->mPacketTimer, &CPacketTimer::packetTimeOut, this, [this]
            {
                d->mPacketTimer.stopPacket();
                d->mSendFailCountMutex.lock();
                d->mSendFailCount++;
                CLogWriter::printLog(QString("[packetTimeOut] : mSendFailCount = %1").arg(d->mSendFailCount), CLogWriter::LOG_TYPE_RELEASE);
                if(SEND_RETRY_COUNT <= d->mSendFailCount)
                    this->onDisConnectCallBack();
                else
                    this->writeQueuedDataForPushFront(d->mLastMsg);
                d->mSendFailCountMutex.unlock();
            });

    connect(this, &SerialCtrl::onDeviceIsConnected, this, [this]
            {
                d->mIsConnected = true;
                this->onClearSerial();
            });

    connect(this, &SerialCtrl::onDeviceIsDisConnected, this, [this]
            {
                this->closeSerial();
                d->mIsConnected = false;
                d->mPacketTimer.stopPacket();
            });

    connect(&d->mResetTimer, &QTimer::timeout, this, [this]
            {
                auto heartBeatProtocol = WriteProtocolStatusRead::writeHeartBeat().toUtf8();
                this->writeQueuedData(heartBeatProtocol);
            });
}

void SerialCtrl::onConnectCallBack()
{
    QTimer::singleShot(3000, this, [this]
                       {
                           if(this->openSerial() == false)
                               return ;

                           emit onDeviceIsConnected();
                       });
}

void SerialCtrl::onDisConnectCallBack()
{
    this->closeSerial();
    d->mIsConnected = false;
    d->mSendFailCount = 0;
    emit onDeviceIsDisConnected();
}

void SerialCtrl::writeDirectDataTansmit(const QByteArray &data)
{
    if(this->openSerial() == false)
    {
        if(d->mIsConnected == true)
            this->onDisConnectCallBack();

        return;
    }
    QByteArray rawData(data);

    if(d->mPacketTimer.isSended())
    {
        emit onDeviceIsBusy();
        emit onNetworkErrorMsg(tr("[ERROR] : Conflict NetWork Packet"));

        return;
    }
    if(d->mNetwork.sendData(rawData) == false)
    {
        emit onDeviceSendError();
        emit onNetworkErrorMsg(tr("Device Send Error"));

        return;
    }
    emit onSendControl(rawData);

}

void SerialCtrl::stackRecvData(QByteArray &queue, const QByteArray recvData)
{
    if(recvData.isEmpty())
        return;

    QString recvLog(recvData);
    CLogWriter::printLog(QString("RecvRaw / %1").arg(recvLog), CLogWriter::LOG_TYPE_RELEASE);
    qDebug() <<"(SerialCtrl::) Recv : " << recvLog;

    d->mMutex.lock();

    for(int i = 0 ; i < recvData.size(); i++)
    {
        char data = recvData[i];

        if(data == ASC_ACK || data == ASC_NAK || data == 0)
            continue;

        queue.push_back(data);
    }

    queue.replace(" ","");
    d->mMutex.unlock();
}

void SerialCtrl::useSetXPacket(QByteArray queue)
{
    QByteArray data;

    for(int i =0; i< queue.size(); i++)
    {
        data.push_back(queue.at(i));

        if(i == PROTOCOL_TOT_SIZE)
            break;
    }

    if(data.size() >= PROTOCOL_TOT_SIZE)
    {
        if(data.at(POSITION_STX) == S_STX)
        {
            if(data.at(POSITION_ETX) == E_ETX)
            {
                if(this->checkSumCheck(data) == true)
                {
                    if(d->mPacketTimer.isActive())
                        d->mPacketTimer.stopPacket();

                    this->successProcessing(data);
                    this->purgeToNextSTX(d->mQueue);
                }
                else
                    this->purgeToNextSTX(d->mQueue);
            }
            else
                this->purgeToNextSTX(d->mQueue);
        }
        else
            this->purgeToNextSTX(d->mQueue);
    }
}

void SerialCtrl::packetBroadCaster(QByteArray &queue)
{
    QByteArray data;

    for(int i = 0; i < PROTOCOL_TOT_SIZE; i++)
        data.push_back(queue.at(i));

    QString recvLog(data);

    data.remove(0, SIZE_STX);
    data.remove(data.size() - SIZE_ETX, SIZE_ETX);
    data.remove(data.size() - SIZE_CHECKSUM, SIZE_CHECKSUM);

    emit onRecvDataForSerial(data);
    emit onResponseFromDevice();
}

void SerialCtrl::successProcessing(QByteArray& queue)
{
    this->packetBroadCaster(queue);
}

void SerialCtrl::purgeToNextSTX(QByteArray &queue)
{
    d->mMutex.lock();

    QString removedData;

    for(int i = 1; i < queue.size(); i++)
    {
        removedData.push_back(queue.at(i-1));

        if(queue.at(i) == S_STX)
        {
            queue.remove(0, removedData.count());
            d->mMutex.unlock();

            return;
        }
    }

    queue.clear();
    d->mMutex.unlock();

    if(queue.isEmpty() == false)
        this->onCallBackFunction(QByteArray());
}

void SerialCtrl::attachStxEtxChecksum(QByteArray& data)
{
    data = data.toUpper();

    auto ckSum = CUtil::makeCheckSum(data);
    ckSum = ckSum.toUpper();

    data.push_front(S_STX);
    data.push_back(ckSum);
    data.push_back(E_ETX);
}

bool SerialCtrl::checkSumCheck(const QByteArray& data)
{
    short checkSum = 0;
    QString rawData;

#pragma omp parallel for schedule(auto) reduction(+:checkSum)
    for(int i = SIZE_STX; i < PROTOCOL_TOT_SIZE - (SIZE_CHECKSUM + SIZE_ETX); i++)
        checkSum += data.at(i);

    int checkSumStart = (PROTOCOL_TOT_SIZE - (SIZE_CHECKSUM + SIZE_ETX));
    int checkSumEnd = (PROTOCOL_TOT_SIZE -  SIZE_ETX);

#pragma omp parallel for ordered
    for(int i = checkSumStart; i < checkSumEnd; i++)
#pragma omp ordered
        rawData.push_back(data.at(i));

    QString hexa = CUtil::hexaToHexaString(checkSum);

    if(hexa.mid(2,2).compare(rawData, Qt::CaseInsensitive) == 0)
        return true;
    else
        return false;
}

bool SerialCtrl::isReadyMove(const QByteArray& queue)
{
    char mdFlag = MD_MODE_CONTROL_WRITE;
    char cmdFlag = CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_REQ;

    if(queue.size() <= PROTOCOL_TOT_SIZE)
    {
        if((queue.mid(SIZE_STX, SIZE_MD_SIZE).toInt(0,16) ==  mdFlag) && (queue.mid(SIZE_STX + SIZE_MD_SIZE, SIZE_CMD_SIZE).toInt(0,16) == cmdFlag))
            return true;
    }
    return false;
}

void SerialCtrl::writeQueuedData(const QByteArray& sendData)
{
    d->mQueueMutex.lock();
    d->mMessageQueue.push_back(sendData);
    d->mQueueMutex.unlock();

    if(d->mPacketTimer.isSended() == false)
        this->onRecvQueueControl();
}

void SerialCtrl::writeQueuedDataForPushFront(const QByteArray& sendData)
{
    d->mQueueMutex.lock();
    d->mMessageQueue.push_front(sendData);
    d->mQueueMutex.unlock();

    if(d->mPacketTimer.isSended() == false)
        this->onRecvQueueControl();
}

void SerialCtrl::writeData(const QByteArray& data, const bool isNeedResp, const bool isNeedGUIDisable)
{
    if(d->mPacketTimer.isSended())
    {
        this->writeQueuedData(data);
        return;
    }
#if ENABLE_SERIAL_MODE
    QMutexLocker locker(&d->mMutex);
    d->mLastMsg = data;

    if(this->openSerial() == false)
    {
        if(d->mIsConnected == true)
            this->onDisConnectCallBack();

        return;
    }

    QByteArray rawData(data);

    if(d->mPacketTimer.isSended())
    {
        emit onDeviceIsBusy();
        emit onNetworkErrorMsg(tr("[ERROR] : Conflict NetWork Packet"));

        return;
    }

    this->attachStxEtxChecksum(rawData);
    CLogWriter::printLog(QString("SendRaw / %1").arg(QString(rawData)), CLogWriter::LOG_TYPE_RELEASE);
    qDebug() << "(SerialCtrl::) Send : " << rawData;

    if(d->mNetwork.sendData(rawData) == false)
    {
        emit onDeviceSendError();
        emit onNetworkErrorMsg(tr("Device Send Error"));

        return;
    }
    emit onSendControl(rawData);

    if(isNeedResp == true)
    {
        int timeOutmSec = 0;

        if(this->isReadyMove(rawData))
            timeOutmSec = (PACKET_TIME_OUT * 2);
        else
            timeOutmSec = PACKET_TIME_OUT;

        d->mPacketTimer.startPacket(timeOutmSec);
    }

    if(isNeedResp == true && isNeedGUIDisable == true)
        emit onSendedData(true);
    else
        emit onSendedData(false);
#else
    emit onSendedData(false);
#endif

    return;
}

void SerialCtrl::closeApplication()
{
    QString protocolStr;
    QByteArray sendData;

    protocolStr = WriteProtocolControlWrite::writeDeviceRESET();
    sendData = protocolStr.toUtf8();
    this->writeQueuedData(sendData);
}

void SerialCtrl::onCallBackFunction(QByteArray recvData)
{    
    this->stackRecvData(d->mQueue,recvData);
    this->useSetXPacket(d->mQueue);

    d->mMutex.lock();

    bool isReCallBack = false;

    bool isQueueHasData = d->mQueue.count() != 0;
    bool isQueueSize = d->mQueue.count()>= PROTOCOL_TOT_SIZE;

    if(isQueueHasData  == true && isQueueSize == true)
    {
        auto isSTX = d->mQueue.at(POSITION_STX) == S_STX;
        auto isETX = d->mQueue.at(POSITION_ETX) == E_ETX;

        if(isSTX == true && isETX == true)
            isReCallBack = true;
    }

    d->mMutex.unlock();


    if(isReCallBack)
        this->onCallBackFunction("");
}

void SerialCtrl::onRecvControl(QByteArray recvData)
{
    auto mode = PacketReader::modeReader(recvData);
    switch(mode)
    {
    case MD_MODE::MD_MODE_STATUS_READ:
        this->thread()->msleep(TRANSFER_SPEED);
        this->onDefaultControl(recvData);
        break;

      case MD_MODE::MD_MODE_SEQUENCE_GENERATOR_SETTING:
        this->thread()->msleep(TRANSFER_SPEED);
        this->onSequenceControl(recvData);
        break;

    case MD_MODE::MD_MODE_ERROR:
        this->writeData(d->mLastMsg);
        break;
    }

    auto command = PacketReader::commandReader(recvData);
    if(mode != -1 || command != -1)
        SerialCtrl::getInstance()->initPacketFailCount();

    this->onRecvQueueControl();
}

void SerialCtrl::onRecvQueueControl()
{
    QByteArray msg;

    d->mQueueMutex.lock();

    if(d->mMessageQueue.count() != 0)
    {
        this->thread()->msleep(TRANSFER_SPEED);

        if(d->mPacketTimer.isSended() == false)
            if(d->mMessageQueue.isEmpty() == false)
                msg = d->mMessageQueue.takeFirst();
    }

    d->mQueueMutex.unlock();

    if(msg.isEmpty() == false)
        this->writeData(msg);

}

void SerialCtrl::onDefaultControl(const QByteArray& recvData)
{
    auto md =  PacketReader::modeReader(recvData);
    auto cmd = PacketReader::commandReader(recvData);
    if(md != -1 || cmd != -1) initPacketFailCount();
    switch(md)
    {
        case MD_MODE::MD_MODE_STATUS_READ:
        {
            switch(cmd)
            {
            case CMD_STATUS_READ::CMD_STATUS_READ_DEVICE_CHECK_LEGACY :
                this->stopReadyProtocol();
                break;

            case CMD_CONTROL_READ::CMD_CONTROL_READ_SEQUENCE_SELECT:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_READ_SEQUENCE_SELECT"));
                break;

            case CMD_CONTROL_READ::CMD_CONTROL_READ_START_STEP_NUM:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_READ_START_STEP_NUM"));
                break;

            case CMD_CONTROL_READ::CMD_CONTROL_READ_SAMPLE_TOTAL_NUM:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_READ_SAMPLE_TOTAL_NUM"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_SEQUENCE_SELECT_RESP:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_SEQUENCE_SELECT_RESP"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_CONTINUOUS_RUN_RESP:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_CONTINUOUS_RUN_RESP"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_STEP_SINGLE_RUN_RESP:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_STEP_SINGLE_RUN_RESP"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_STEP_PROCESS_SINGLE_RUN_RESP:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_STEP_PROCESS_SINGLE_RUN_RESP"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_SEQUENCE_STOP_RESP:
                emit onResponseResetCommand();
                d->mResetTimer.start(5000);
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_SEQUENCE_STOP_RESP"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_PAUSE_RESP:
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_PAUSE_RESP"));
                break;

            case CMD_CONTROL_WRITE::CMD_CONTROL_WRITE_DEVICE_RESET_RESP:
                emit onResponseResetCommand();
                d->mResetTimer.start(5000);
                CLogWriter::printLog(QString(">>> (SerialCtrl::)Recv : CMD_CONTROL_WRITE_DEVICE_RESET_RESP"));
                break;

            }
        }break;
    }
}

void SerialCtrl::onSequenceControl(const QByteArray& data)
{
    switch(PacketReader::commandReader(data))
    {
        case CMD_SEQUENCE_GENERATOR::CMD_SEQUENCE_GENERATOR_SQ_INPUT_MODE :
        {
            auto pMode = PacketReader::dataReader(data).toUInt(nullptr, 16);

            if(pMode == ConfigInformation::SEQUENCE_STATE_END)
            {
                this->purgeToNextSTX(d->mQueue); //clean up dummy data

                emit onSequenceSendFinished();
                emit onResponseResetCommand();

                if(d->mResetTimer.isActive() == false)
                    d->mResetTimer.start(5000);
            }
        }break;
    }
}

void SerialCtrl::onClearSerial()
{
    QMutexLocker locker(&d->mMutex);
    d->mPacketTimer.stopPacket();
    d->mMessageQueue.clear();
    d->mQueue.clear();
    d->mLastMsg.clear();
}

bool SerialCtrl::inputSequence(const QVector<QByteArray> &protocolQueue)
{
    for(auto itr : protocolQueue)
        this->writeQueuedData(itr);

    return true;
}

void SerialCtrl::stopReadyProtocol()
{
    if(d->mResetTimer.isActive() == true)
    {
        d->mResetTimer.stop();
        emit onResponseResetEndCommand();
    }

    emit onRecvHeartBeatCommand();
}
