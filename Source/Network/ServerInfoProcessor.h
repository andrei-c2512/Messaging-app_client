
#ifndef USER_H
#define USER_H
#include <QObject>
#include <QTcpSocket>
#include "UserInfo.h"
#include <QTimer>
#include "MediaCache.h"
#include "MediaUploader.h"


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


class ServerInfoProcessor :  public UserInfo{
    Q_OBJECT
public:
    ServerInfoProcessor(QObject* object = nullptr);
    const QTcpSocket& socket() const;
    std::vector<ContactInfo*>& searchedForUsers() noexcept;
public slots:
    //this is all for messaging the server
    void registerIntoDatabase();
    void logIntoDatabase();
    void manageFriendRequest(bool accepted , int userId);
    void searchForPeople(QString criteria);
    void addMessage(const MessageInfo& info , int chatId);
    void sendFriendRequest(int userId);
    void removeFriend(int userId);
    void getInfoForUsers(std::vector<int> list);
    void onReadyRead();
    void createPrivateChatWithFriend(int friendId);
    void createGroupChat(std::vector<int> list);
    void blockUser(int id);
    void unblockUser(int id);
    void updateChatName(int chatId, QString newName);
    void removeFromGroup(int chatId, int userId);
    void leaveFromGroup(int chatId);
    void addPeopleToGroup(int chatId , std::vector<int> idList);
    void sendMedia(QByteArray fileName , QByteArray arr);
    void setName(QString str) override;
    void setPassword(QString pas) override;
    void setEmail(QString email) override;

    //this is for images that are already in the cache
    QImage image(const QString& fileName);
    void addImage(const QUrl& url);
    bool fileExists(const QString& fileName);
private slots:
    void attemptConnection();
signals:
    void sentRegisterRequest();
    void sentLoginRequest();
signals:
    //request failures
    void nameUsedError();
    void nameNotFoundError();
    void incorrectPasswordError();
    void emailUsedError();
signals:
    //info from server
    void signInAccepted();
    void contactInfoLoaded();
    void newChatData();
    void newFriend();
    void chatDataReceived();
    void newFriendRequest();
    void newSearchData();
    void createdNewChat(int id);
    void addedToNewChat(int id);
    void unknownListReceived();
    void allUserInfoReceived();
    void accountDataCleared();
private:
    //returns the position it remained at
    int processCommand(const QString& str , int start);
    int processSignInInfo(const QString& str , int start);
    int processChatListInfo(const QString& str , int start);
    std::pair<ChatInfo*, int> processChatInfo(const QString& str , int start);
    std::pair<MessageInfo*, int> processMessageInfo(const QString& str , int start);
    std::vector<MessageInfo*> extractHistoryFromChat(const QString& str , int start);
    int processNewMessageInfo(const QString& str, int start);
    int processContactInfo(const QString& str , int start);
    std::vector<ContactInfo*>  processContactList(const QString& str , int start , int end);
    int processNewFriendInfo(const QString& str , int start);
    int processSearchedForList(const QString& str , int start);
    int processNewFriendRequest(const QString& str , int start);
    int processNewChat(const QString& str, int start);
    int processFriendRemoval(const QString& str, int start);
    int processGettingBlocked(const QString& str, int start);
    int processPersonBlocked(const QString& str, int start);
    int processListOfStrangers(const QString& str, int start);
    int processUserUnblocked(const QString& str, int start);
    int processUserUnblockedYou(const QString& str, int start);
    int processNewChatName(const QString& str, int start);
    int processGroupMemberRemoval(const QString& str, int start);
    int processNewGroupMembers(const QString& str, int start);
    int processNecessaryContacts(const QString& str, int start);
    int processNewAdmin(const QString& str, int start);
    int processFriendStatus(const QString& str, int start);
    int processNewUploadId(const QString& str, int start);
    int processChunkStatus(const QString& str, int start);

    static std::vector<int> extractIntsFromArr(const QString& str);
    void requestDataOfUnknownUsers();
    void requestDataOfWaitingUsers();
private:
    bool loggedIn;
    bool userInfoReceived;
    QTimer* connectTimer;
    QTcpSocket* _socket;

    MediaCache* pMediaCache;
    MediaUploader* pMediaUploader;
    std::vector<ContactInfo*> _searchedForUsers;
    std::vector<std::pair<int , int>> _awaitingContactInfoList;
};

#endif // USER_H
