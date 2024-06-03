
#ifndef USERINFO_H
#define USERINFO_H
#include <QString>
#include <QDebug>
#include <algorithm>
#include "Tools.h"
#include <QUrl>
#include <queue>
#include "AccountDataStructures/ChatInfo.h"


class AccountInfoStorage : public QObject{
    Q_OBJECT
public:
    AccountInfoStorage(QObject* parent = nullptr);
    virtual void setName(QString str);
    virtual void setPassword(QString pas);
    virtual void setEmail(QString email);
    void setId(int id);

    QString name() const;
    QString password() const;
    QString email() const;
    int id() const;
    void setFriendList(std::vector<ContactInfo*> list);
    void setRequestList(std::vector<ContactInfo*> list);
    void setBlockedList(std::vector<ContactInfo*> list);
    void setSearchedForList(std::vector<ContactInfo*> list);

    void addChat(ChatInfo* chat);
    void addFriend(ContactInfo* contact);
    void addToBlockedList(ContactInfo* contact);
    void addToRequestList(ContactInfo* contact);
    void addUserByFlag(ContactInfo* contact);
    void removeChat(QString name);

    void removeFriend(int id);

    void showChatInDebug() const noexcept;
    ChatInfo& getChatById(int id) ;
    const std::vector<ChatInfo*>& chatList() const noexcept;
    const std::vector<ChatInfo*>& requestList() const noexcept;
    std::vector<ContactInfo*>& friendList() noexcept;
    std::vector<ContactInfo*>& requestList() noexcept;
    std::vector<ContactInfo*>& blockedList() noexcept;
    std::vector<ContactInfo*>& searchedForList() noexcept;

    void removeRequest(int id);
    /* When using this app and switching accounts , to make switching faster , I could preserve the ChatInfo objects and ContactInfo objects according to my use
     * so that I don't lose performance by reallocating again memory. I think I will do this towards the end of development */
    void clearAllData() noexcept;
    //clears all the chats , friends etc etc
    void clearAccountData() noexcept;
    void transformChats() noexcept;
    ContactInfo* findUser_KnownLists(int id) const;
    ContactInfo* findUser_StrangerList(int id) const;
    ContactInfo* findUser(int id) const;
    std::vector<ContactInfo*> findUsers(std::vector<int> list);
    //returns true if a move occurred
    void moveUserToStrangerList(ContactInfo* id);
    void moveUserToBlocked(ContactInfo* id);
    ChatInfo* privateChatById(int id);
    ChatInfo* chatById(int id);
    ChatInfo& firstChat() const;

    bool chatListEmpty() const;
    std::vector<QString> namesForContacts(std::vector<int> idList) const;
    void adaptChat(ChatInfo* info);
    void addToStrangerList(ContactInfo* info);
    std::vector<ContactInfo*>& strangerList();
signals:
    void accountDataCleared();
protected:
    QString _name;
    QString _password;
    QString _email;
    int _id;
    std::vector<ChatInfo*>   _chatList;
    std::vector<ChatInfo*>   _requestChatList;
    std::vector<ContactInfo*> _friendList;
    std::vector<ContactInfo*> _requestList;
    std::vector<ContactInfo*> _blockedList;
    //for people encountered on groups that are not your friends
    std::vector<ContactInfo*> _strangerList;

    std::vector<ContactInfo*> _searchedForUsers;

};

#endif // USERINFO_H
