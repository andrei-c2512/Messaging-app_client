#pragma once
#include "MessageInfo.h"
#include "ContactInfo.h"
#include <queue>
#include <QUrl>

class ChatInfo : public QObject {
    Q_OBJECT
public:
    ChatInfo(QObject* parent = nullptr);
    void setMessageHistory(std::vector<MessageInfo*> vec);
    void setName(QString name);
    //also emits a signal that specifies that the name changed
    void setNewName(QString name);
    void setMembers(std::vector<int> members_id);
    void setMembers(const QString& str);
    void setId(int id);
    void setReadOnlyMembers(std::vector<int> members_id);
    void setReadOnlyMembers(const QString& str);
    //used when initializing
    void setAdminId(int id);
    void setNewAdmin(int id);
    void setType(bool isPrivate);
    void removeMember(int id, int userId, bool forcefullyRemoved);
    void addMember(int id);
    //the chat doesn't know who is the user of this app in the chat , so I have to give him the info
    void connectSlotsForPrivateChats(ContactInfo* info);


    void addMessage(MessageInfo* mes);
    MessageInfo* addMessage(QString name, QString message);
    MessageInfo* addMessageToQueue(QString name, QUrl url);
    MessageInfo* addMessageToQueue(QString name, QString message);
    MessageInfo* addWaitingMessage(QString name, QString message);

    void addMessageFromDatabase(QString name, QString message, QString timestamp);
    //returns true if it approved a new message , false otherwise
    bool approveLastInQueue();
    QString name() const;
    bool isPrivate() const;
    const std::vector<int>& members() const;
    const std::vector<int>& readOnlyMembers() const;
    const std::vector<MessageInfo*>& history() const;
    const MessageInfo& back() const;
    int id() const;
    int adminId() const;
    const std::vector<MessageInfo*> lastNMessages(int n) const;
    //start starts from the end: so if start = 5 , it is 5 indexes from the end towards the begining
    const std::vector<MessageInfo*> lastNMessages(int start, int n) const;

    bool operator<(const ChatInfo* rhs) const;
    bool operator<(ChatInfo* rhs);
    bool operator>(const ChatInfo* rhs) const;
    bool operator>(ChatInfo* rhs);
    bool operator==(const ChatInfo* rhs) const;
    bool operator==(ChatInfo* rhs);

    void mediaMessageUploaded(QString fileName);
signals:
    void newMessageAdded(MessageInfo& m);
    //this signal is connected to the function that sends message info to the server
    void newMessageInQueue(const MessageInfo& info, int chatId);
    void nameChanged(const QString& name);
    void gotBlocked(bool b);
    void blockedYou(bool b);
    void memberRemoved(int id);
    void newMember(int id);
    void removed(bool forcefully = true);
    void newAdmin(int id);
private:
    std::vector<MessageInfo*> _history;
    std::vector<MessageInfo*> _queue;
    std::queue<MessageInfo*> _mediaMessageQueue;

    std::vector<int> _members;
    std::vector<int> _readOnlyMembers;
    int _adminId;
    QString _name;
    int _id;
    bool _waitingForResponse;
    bool _private;
};

