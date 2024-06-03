#pragma once
#include <QTcpSocket>
#include "AccountDataStructures/MessageInfo.h"
#include "MessageHandlingBase.h"
#include "AccountInfoStorage.h"

class RequestSender : public QObject{
    Q_OBJECT
public:
	RequestSender(QObject* parent , QTcpSocket& socket);
    void setId(int id);
public slots:
    void registerIntoDatabase(const QString& name , const QString& email , const QString& password);
    void logIntoDatabase(const QString& name , const QString& password);
    void manageFriendRequest(bool accepted, int userId);
    void searchForPeople(QString criteria);
    void sendFriendRequest(int userId);
    void removeFriend(int userId);
    void getInfoForUsers(std::vector<int> list);
    void createPrivateChatWithFriend(int friendId);
    void createGroupChat(std::vector<int> list);
    void blockUser(int id);
    void unblockUser(int id);
    void updateChatName(int chatId, QString newName);
    void removeFromGroup(int chatId, int userId);
    void leaveFromGroup(int chatId);
    void addPeopleToGroup(int chatId, std::vector<int> idList);
    void sendMedia(QByteArray fileName, QByteArray arr);
    void sendAck();
    void addMessage(const MessageInfo& info, int chatId);
    void requestDataOfUnknownUsers(AccountInfoStorage& storage);
    void requestDataOfWaitingUsers(std::vector<std::pair<int, int>>& awaitingContactInfoList);
signals:
    void sentRegisterRequest();
    void sentLoginRequest();
    void allUserInfoReceived();
private:
    QTcpSocket& _socket;
    int _id;

};

