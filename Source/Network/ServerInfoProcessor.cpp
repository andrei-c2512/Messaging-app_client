#include "ServerInfoProcessor.h"
#include "MessageHandlingBase.h"
#include "TextParser.h"

using namespace MessageSeparators;
using namespace TextParser;

ConnectionHandler::ConnectionHandler(QObject* object)
    : QObject(object) , _socket(new QTcpSocket(this)) , pMediaCache(new MediaCache(this)) , pMediaUploader(new MediaUploader(this , *_socket))
{
    pRequestSender = new RequestSender(this, *_socket);
    pStorage = new AccountInfoStorage(this);
    pResponseHandler = new ResponseHandler(this, *pStorage, *pMediaUploader, *pRequestSender, *pMediaCache);
    loggedIn = false;
    userInfoReceived = false;

    connect(_socket , &QTcpSocket::readyRead , this , &ConnectionHandler::onReadyRead);
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

    connect(connectTimer , &QTimer::timeout , this , &ConnectionHandler::attemptConnection);
}

void ConnectionHandler::attemptConnection(){ _socket->connectToHost(QHostAddress::LocalHost , 19704);}

void ConnectionHandler::onReadyRead()
{
    QByteArray message = _socket->readAll();
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

void ConnectionHandler::sendAck()
{
}

std::vector<int> ConnectionHandler::extractIntsFromArr(const QString& str)
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


const QTcpSocket& ConnectionHandler::socket() const { return *_socket;}
AccountInfoStorage& ConnectionHandler::storage() noexcept { return *pStorage; }
RequestSender& ConnectionHandler::requestSender() noexcept { return *pRequestSender; }
ResponseHandler& ConnectionHandler::handler() noexcept { return *pResponseHandler; }

const AccountInfoStorage& ConnectionHandler::storage() const noexcept { return *pStorage; }
const RequestSender& ConnectionHandler::requestSender() const noexcept { return *pRequestSender; }
const ResponseHandler& ConnectionHandler::handler() const noexcept { return *pResponseHandler; }

QImage ConnectionHandler::image(const QString& fileName)
{
    return (pMediaCache->provideImage(fileName));
}
void ConnectionHandler::addImage(const QUrl& url, int chatId, std::vector<int> memberList)
{
    pMediaUploader->addMediaToQueue(url, chatId , pStorage->id(), memberList);
}
bool ConnectionHandler::fileExists(const QString& fileName) { return pMediaCache->fileExists(fileName); }