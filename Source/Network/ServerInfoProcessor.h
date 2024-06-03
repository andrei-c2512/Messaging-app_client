
#ifndef USER_H
#define USER_H
#include <QObject>
#include <QTcpSocket>
#include "AccountInfoStorage.h"
#include <QTimer>
#include "MediaCache.h"
#include "MediaUploader.h"
#include "RequestSender.h"
#include "ResponseHandler.h"

/*
 * Request types:
 *  0 - register
 *  1 - login
 *
 *  RequestToServer{
        Register ,
        Login ,
        NewMessage ,
    };
    Starts with character 'R'
    RequestFailure{
        NameUsed,
        NameNotFound,
        IncorrectPassword,
        EmailUsed
    };
    Starts with character 'I'
    InfoToClient{
        SignedIn,  // will inform the user that he has succesfully signed in
        ChatInfo, // will give the user the info of what chats he takes part in and it's messages
        Contacts, // will give the user his friend list , blocked people , people who are offline/online/last seen on yyyy-mm--dd
        NewMessage,
        SearchedUserList,
        NewFriend   // will give the info of the new friend you just made
    };
 *  */


class ServerInfoProcessor : public QObject{
    Q_OBJECT
public:
    ServerInfoProcessor(QObject* object = nullptr);
    const QTcpSocket& socket() const;
public slots:
    //this is all for messaging the server
    void onReadyRead();
    void sendAck();

    //this is for images that are already in the cache
    QImage image(const QString& fileName);
    void addImage(const QUrl& url , int chatId , std::vector<int> memberList);
    bool fileExists(const QString& fileName);

    AccountInfoStorage& storage() noexcept;
    RequestSender& requestSender() noexcept;
    ResponseHandler& handler() noexcept;

    const AccountInfoStorage& storage() const noexcept;
    const RequestSender& requestSender() const noexcept;
    const ResponseHandler& handler() const noexcept;
private slots:
    void attemptConnection();
signals:
    void sentRegisterRequest();
    void sentLoginRequest();
private:
    //returns the position it remained at

    static std::vector<int> extractIntsFromArr(const QString& str);
private:
    bool loggedIn;
    bool userInfoReceived;
    QTimer* connectTimer;
    QTcpSocket* _socket;

    RequestSender* pRequestSender;
    ResponseHandler* pResponseHandler;
    AccountInfoStorage* pStorage;

    MediaCache* pMediaCache;
    MediaUploader* pMediaUploader;

    int auxPos = 0;
};

#endif // USER_H
