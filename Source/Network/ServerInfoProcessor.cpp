#include "ServerInfoProcessor.h"
#include "MessageHandlingBase.h"
#include "TextParser.h"

using namespace MessageSeparators;
using namespace TextParser;

ServerInfoProcessor::ServerInfoProcessor(QObject* object)
    : QObject(object) , _socket(new QTcpSocket(this)) , pMediaCache(new MediaCache(this)) , pMediaUploader(new MediaUploader(this , *_socket))
{
    pRequestSender = new RequestSender(this, *_socket);
    pStorage = new AccountInfoStorage(this);
    pResponseHandler = new ResponseHandler(this, *pStorage, *pMediaUploader, *pRequestSender, *pMediaCache);
    loggedIn = false;
    userInfoReceived = false;

    connect(_socket , &QTcpSocket::readyRead , this , &ServerInfoProcessor::onReadyRead);
    connect(_socket , &QTcpSocket::connected , this , [=](){
        qDebug() << "Connected to server";
        if(loggedIn)
        {
            pRequestSender->logIntoDatabase(pStorage->name() , pStorage->password());
        }
        this->connectTimer->stop();
    });
    connect(_socket , &QTcpSocket::disconnected , this , [=](){
        qDebug() << "Disconnected from server";
        pStorage->clearAccountData();
        connectTimer->start();
        userInfoReceived = false;
        pResponseHandler->reset();
    });
    connectTimer = new QTimer(this);
    connectTimer->setInterval(1000);
    connectTimer->start();

    connect(connectTimer , &QTimer::timeout , this , &ServerInfoProcessor::attemptConnection);
}

void ServerInfoProcessor::attemptConnection(){ _socket->connectToHost(QHostAddress::LocalHost , 19704);}

void ServerInfoProcessor::onReadyRead()
{
    QString message = _socket->readAll();
    qDebug() <<" Message from server: " << message;

    int lastPos = -1;
    int pos = message.indexOf('~');
    while(pos > lastPos && pos < message.length())
    {
        qDebug() << message.mid(pos);
        lastPos = pos;
        pos = pResponseHandler->processCommand(message , pos);
    }
}

void ServerInfoProcessor::sendAck()
{
}

std::vector<int> ServerInfoProcessor::extractIntsFromArr(const QString& str)
{
    std::vector<int> memberList;
    QString nrStr;
    qint16 i = str.indexOf('{') + 1;

    while(str[i] != '}')
    {
        if(str[i] == ',')
        {
            memberList.emplace_back(nrStr.toInt());
            nrStr ="";
        }
        else
            nrStr += str[i];
        i++;
    }
    memberList.emplace_back(nrStr.toInt());
    return memberList;
}


const QTcpSocket& ServerInfoProcessor::socket() const { return *_socket;}
AccountInfoStorage& ServerInfoProcessor::storage() noexcept { return *pStorage; }
RequestSender& ServerInfoProcessor::requestSender() noexcept { return *pRequestSender; }
ResponseHandler& ServerInfoProcessor::handler() noexcept { return *pResponseHandler; }

const AccountInfoStorage& ServerInfoProcessor::storage() const noexcept { return *pStorage; }
const RequestSender& ServerInfoProcessor::requestSender() const noexcept { return *pRequestSender; }
const ResponseHandler& ServerInfoProcessor::handler() const noexcept { return *pResponseHandler; }

QImage ServerInfoProcessor::image(const QString& fileName)
{
    return (pMediaCache->provideImage(fileName));
}
void ServerInfoProcessor::addImage(const QUrl& url, int chatId, std::vector<int> memberList)
{
    pMediaUploader->addMediaToQueue(url, chatId , pStorage->id(), memberList);
}
bool ServerInfoProcessor::fileExists(const QString& fileName) { return pMediaCache->fileExists(fileName); }