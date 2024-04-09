
#ifndef USERINFO_H
#define USERINFO_H
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include <algorithm>
#include "StyleBase/Tools.h"

class MessageInfo : public QObject{
    Q_OBJECT
public:
    MessageInfo(QObject* parent = nullptr);
    MessageInfo(QObject* parent , QString name , QString text , QString timestamp);
    QString name() const;
    QString text() const;
    QDateTime timestamp() const;
    void setName(QString str);
    void setText(QString text);
    void setAt(int i , QString str);
    void setTimestamp(QString str);
    void setTimestamp(QDateTime str);
signals:
    void outOfQueue();
private:
    QString _name;
    QString _text;
    QDateTime _timestamp;
};

class ContactInfo : public QObject{
    Q_OBJECT
public:
    enum class Status {
        Online = 1, 
        HasRequest = 2,
        IsBlocked = 4,
        HasBlockedYou = 8,
        Friend = 16
    };
public:
    ContactInfo(QObject* parent = nullptr);
    ContactInfo(QObject* parent, QString name , char flags , QString lastSeen);
    void setFlags(char f);
    void addFlags(char f);
    void removeFlags(char f);
    void setName(QString name);
    void setLastSeen(QString lastSeen);
    void setFriendList(std::vector<int> friendList0);
    void setId(int id);


    QString name() const;
    QString lastSeen() const;
    const std::vector<int>& friendList() const;
    int id() const;
    char flags() const;


    bool operator<(const ContactInfo& info);
    bool operator>(const ContactInfo& info);
    bool operator==(const ContactInfo& info);
signals:
    void removed();
    //true for blocking , false for unblocking
    void gotBlocked(bool b);
    void blockedYou(bool b);
private:
    int _id;
    QString _name;
    QString _lastSeen;
    char _flags;

    std::vector<int> _friendList;
};


class ChatInfo : public QObject{
    Q_OBJECT
public:
    ChatInfo(QObject* parent = nullptr);
    void setMessageHistory(std::vector<MessageInfo*> vec);
    void setName(QString name);
    void setMembers(std::vector<int> members_id);
    void setMembers(const QString& str);
    void setId(int id);
    void setType(bool isPrivate);
    //the chat doesn't know who is the user of this app in the chat , so I have to give him the info
    void connectSlotsForPrivateChats(ContactInfo* info);


    void addMember(int id);
    void addMessage(MessageInfo* mes);
    MessageInfo* addMessage( QString name , QString message);
    MessageInfo* addMessageToQueue( QString name , QString message);
    MessageInfo* addWaitingMessage( QString name , QString message);

    void addMessageFromDatabase( QString name , QString message, QString timestamp);
    //returns true if it approved a new message , false otherwise
    bool approveLastInQueue();
    QString name() const;
    bool isPrivate() const;
    const std::vector<int>& members() const;
    const std::vector<MessageInfo*>& history() const;
    const MessageInfo& back() const;
    int id() const;
    const std::vector<MessageInfo*> lastNMessages(int n) const;
    const std::vector<MessageInfo*> lastNMessages(int start , int n) const;

    bool operator<(const ChatInfo* rhs) const;
    bool operator<(ChatInfo* rhs);
    bool operator>(const ChatInfo* rhs) const;
    bool operator>(ChatInfo* rhs);
    bool operator==(const ChatInfo* rhs) const;
    bool operator==(ChatInfo* rhs);
signals:
    void newMessageAdded( MessageInfo& m );
    void newMessageInQueue(const MessageInfo& info , int chatId);
    void nameChanged(const QString& name);
    void gotBlocked(bool b);
    void blockedYou(bool b);
private:
    std::vector<MessageInfo*> _history;
    std::vector<MessageInfo*> _queue;
    std::vector<int> _members;
    QString _name;
    int _id;
    bool _waitingForResponse;
    bool _private;
};


class NullInfo{
public:
    static NullInfo& instance();
    ~NullInfo();
    NullInfo(const NullInfo&) = delete;
    NullInfo& operator=(const NullInfo&) = delete;

    MessageInfo& nullMessage() const noexcept;
    ChatInfo& nullChat() const noexcept;
    ContactInfo& nullContact() const noexcept;
private:
    NullInfo();
private:
    MessageInfo* pNullMessage;
    ChatInfo* pNullChat;
    ContactInfo* pNullContact;
};

class UserInfo : public QObject{
public:
    UserInfo(QObject* parent = nullptr);
    virtual void setName(QString str);
    virtual void setPassword(QString pas);
    virtual void setEmail(QString email);

    QString name() const;
    QString password() const;
    QString email() const;
    int id() const;

    void addChat(ChatInfo* chat);
    void addFriend(ContactInfo* contact);
    void addToBlockedList(ContactInfo* contact);
    void addToRequestList(ContactInfo* contact);
    void removeChat(QString name);

    void removeFriend(int id);

    void showChatInDebug() const noexcept;
    ChatInfo& getChatById(int id) ;
    const std::vector<ChatInfo*>& chatList() const noexcept;
    const std::vector<ChatInfo*>& requestList() const noexcept;
    std::vector<ContactInfo*>& friendList() noexcept;
    std::vector<ContactInfo*>& requestList() noexcept;
    std::vector<ContactInfo*>& blockedList() noexcept;

    void removeRequest(int id);
    /* When using this app and switching accounts , to make switching faster , I could preserve the ChatInfo objects and ContactInfo objects according to my use
     * so that I don't lose performance by reallocating again memory. I think I will do this towards the end of development */
    void clearAllData() noexcept;
    //clears all the chats , friends etc etc
    void clearAccountData() noexcept;
    void transformChats() noexcept;
    ContactInfo* findUser_KnownLists(int id);
    ContactInfo* findUser_UnknownList(int id);
    ContactInfo* findUser(int id);
    //returns true if a move occurred
    void moveUserToUnknownList(ContactInfo* id);
    void moveUserToBlocked(ContactInfo* id);
    ChatInfo* privateChatById(int id);
protected:
    void adaptChat(ChatInfo* info);
    void addToStrangerList(ContactInfo* info);
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
};

#endif // USERINFO_H
