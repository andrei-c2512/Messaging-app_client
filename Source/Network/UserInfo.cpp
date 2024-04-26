#include "UserInfo.h"

//for MessageInfo
MessageInfo::MessageInfo(QObject* parent) : QObject(parent) {}
MessageInfo::MessageInfo(QObject* parent , QString name0 , QString text0 , QString timestamp0)
    :QObject(parent) , _name(std::move(name0)) , _text(std::move(text0))
{
    setTimestamp(std::move(timestamp0));
}

QString MessageInfo::name() const { return _name;}
QString MessageInfo::text() const { return _text;}
QDateTime MessageInfo::timestamp() const { return _timestamp;}
void MessageInfo::setAt(int i , QString str) {
    switch(i)
    {
    case 0:
        _name = std::move(str);
        return;
    case 1:
        _text = std::move(str);
        return;
    case 2:
        setTimestamp(std::move(str));
        return;
    default:
        return;
    }
}
void MessageInfo::setTimestamp(QString str)
{
    _timestamp = Tools::stringToDateTime(std::move(str));
}
void MessageInfo::setTimestamp(QDateTime t) { _timestamp = std::move(t);}
void MessageInfo::setName(QString str) { _name = std::move(str);}
void MessageInfo::setText(QString text) { _text = std::move(text);}

//for ChatInfo
ChatInfo::ChatInfo(QObject* parent) : QObject(parent) { _private = false; }
void ChatInfo::setMessageHistory(std::vector<MessageInfo*> vec) { _history = vec;}
void ChatInfo::setName(QString name) { _name = std::move(name); emit nameChanged(_name); }
void ChatInfo::setNewName(QString name0)
{
    _name = name0;
    emit nameChanged(_name);
}
void ChatInfo::setMembers(std::vector<int> members_id){ _members = std::move(members_id);}
void ChatInfo::setMembers(const QString& str) { _members = Tools::extractIntsFromArr(str); }

void ChatInfo::setReadOnlyMembers(std::vector<int> members_id) { _readOnlyMembers = std::move(members_id); }
void ChatInfo::setReadOnlyMembers(const QString& str) { _readOnlyMembers = Tools::extractIntsFromArr(str); }
void ChatInfo::setAdminId(int id) { _adminId = id; }
void ChatInfo::setNewAdmin(int id) {
    _adminId = id;
    emit newAdmin(id);
}
void ChatInfo::setId(int id) { _id = std::move(id);}
void ChatInfo::setType(bool isPrivate) { _private = isPrivate; }

void ChatInfo::addMessage(MessageInfo* mes) { _history.emplace_back(mes);}

QString ChatInfo::name() const { return _name;}
int ChatInfo::id() const { return _id;}
bool ChatInfo::isPrivate() const { return _private; }

const std::vector<int>& ChatInfo::members() const { return _members;}
const std::vector<int>& ChatInfo::readOnlyMembers() const { return _readOnlyMembers;  }
int ChatInfo::adminId() const { return _adminId; }
const std::vector<MessageInfo*>& ChatInfo::history() const { return _history;}
const MessageInfo& ChatInfo::back() const { return *_history.back();}
MessageInfo* ChatInfo::addMessage(QString name , QString message)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(std::move(message));
    return _history.emplace_back(m);
}
MessageInfo* ChatInfo::addMessageToQueue( QString name , QString message)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(std::move(message));

    emit newMessageInQueue(*m  , _id);
    return _queue.emplace_back(m);
}


const std::vector<MessageInfo*> ChatInfo::lastNMessages(int start , int n) const
{
    std::vector<MessageInfo*> list;
    n = std::min(n , (int)_history.size() - start);

    for(int i = start ; i < start + n ; i++)
        list.emplace_back(_history[i]);
    return list;
}

const std::vector<MessageInfo*> ChatInfo::lastNMessages(int n) const
{
    if(_history.size() <= n)
        return _history;
    else
    {
        std::vector<MessageInfo*> list;
        for(int i = 0 ; i <  n ; i++)
            list.emplace_back(_history[i]);
        return list;
    }
}
MessageInfo* ChatInfo::addWaitingMessage( QString name , QString message)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(std::move(message));
    return _queue.emplace_back(m);
}
bool  ChatInfo::approveLastInQueue()
{
    if(_queue.empty() == false)
    {
        _history.emplace_back(_queue.back());
        _queue.pop_back();
        return true;
    }
    else
        return false;
}

void ChatInfo::addMessageFromDatabase( QString name , QString message, QString timestamp)
{
    if(_queue.empty() == false)
    {
        auto* m = _queue.back();
        m->outOfQueue();
        _history.emplace_back(m);
        _queue.pop_back();
    }
    else
    {
        MessageInfo* m = new MessageInfo(this , std::move(name) , std::move(message) , std::move(timestamp));
        emit newMessageAdded(*m);
        _history.emplace_back(m);
    }
}

void ChatInfo::removeMember(int id , int userId, bool forcefullyRemoved)
{
    for (int i = 0; i < _members.size(); i++)
    {
        if (_members[i] == id)
        {
            if (id == userId)
            {
                emit removed(forcefullyRemoved);
                deleteLater();
            }
            else
            {
                _members.erase(_members.begin() + i);
                emit memberRemoved(id);
            }
        }
    }
}
void ChatInfo::addMember(int id)
{
    _members.emplace_back(id);
    emit newMember(id);
}

void ChatInfo::connectSlotsForPrivateChats(ContactInfo* info)
{
    connect(info, &ContactInfo::gotBlocked, this, [=](bool blocked) {
        emit gotBlocked(blocked);
    });
    connect(info, &ContactInfo::blockedYou, this, [=](bool blocked) {
        emit blockedYou(blocked);
        });
}



bool ChatInfo::operator<(const ChatInfo* rhs) const { return _id < rhs->id();}
bool ChatInfo::operator<(ChatInfo* rhs) {return _id < rhs->id(); }
bool ChatInfo::operator>(const ChatInfo* rhs) const { return _id > rhs->id(); }
bool ChatInfo::operator>(ChatInfo* rhs) { return _id > rhs->id(); }
bool ChatInfo::operator==(const ChatInfo* rhs) const { return _id == rhs->id(); }
bool ChatInfo::operator==(ChatInfo* rhs) { return _id == rhs->id(); }

// for contact info
ContactInfo::ContactInfo(QObject* parent) : QObject(parent)
{}

ContactInfo::ContactInfo(QObject* parent , QString name , ContactStatus flags0 , QString lastSeen )
    : QObject(parent) , _flags(flags0) , _name(std::move(name)), _lastSeen(std::move(lastSeen))
{}
void ContactInfo::setFlags(ContactStatus f) { _flags = f; }
void ContactInfo::addFlags(ContactStatus f) { 
    if (f & Status::Online)
        emit statusChanged(true);
    _flags = _flags | f;  
}
void ContactInfo::removeFlags(ContactStatus f) { 
    if (f & Status::Online)
        emit statusChanged(false);
    _flags = _flags & ~f; 
}

void ContactInfo::setName(QString name) { _name = std::move(name);}
void ContactInfo::setLastSeen(QString lastSeen) { _lastSeen = std::move(lastSeen);}
void ContactInfo::setFriendList(std::vector<int> friendList0) { _friendList = std::move(friendList0);}
void ContactInfo::setId(int id0) { _id = id0;}

QString ContactInfo::name() const { return _name;}
QString ContactInfo::lastSeen() const { return _lastSeen;}
const std::vector<int>& ContactInfo::friendList() const  {return _friendList;}
int ContactInfo::id() const { return _id; }
ContactInfo::ContactStatus ContactInfo::flags() const { return _flags; }



bool ContactInfo::operator<(const ContactInfo& info) { return _id < info._id; }
bool ContactInfo::operator>(const ContactInfo& info) { return _id > info._id; }
bool ContactInfo::operator==(const ContactInfo& info) { return _id == info._id; }

std::vector<ContactInfo*> ContactInfo::subtractFromList(std::vector<ContactInfo*> list1, std::vector<ContactInfo*> list2)
{
    Tools::sort<ContactInfo*>(list1, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    Tools::sort<ContactInfo*>(list2, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });


    std::vector<ContactInfo*> newVec;
    for (ContactInfo* c1 : list1)
    {
        bool subtracted = false;
        for (ContactInfo* c2 : list2)
        {
            if (c1->id() == c2->id())
                subtracted = true;
        }
        if (subtracted == false)
            newVec.emplace_back(c1);
    }


    return newVec;
}


//for null info

NullInfo::NullInfo()
{
    pNullChat = new ChatInfo;
    pNullChat->setId(-1);
    pNullChat->setName("THIS SHOULD NOT SHOW UP");
    pNullMessage = new MessageInfo;
    pNullMessage->setText("DEFAULT MESSAGE: THIS SHOULD NOT SHOW UP");
    pNullMessage->setTimestamp(QDateTime(QDate(33 , 4 , 3) , QTime(15 , 33 , 33)));
    pNullChat->setMessageHistory({ pNullMessage });

    pNullContact = new ContactInfo;
}
NullInfo::~NullInfo()
{
    pNullMessage->deleteLater();
    pNullChat->deleteLater();
    pNullContact->deleteLater();
}

MessageInfo& NullInfo::nullMessage() const noexcept{   return *pNullMessage; }
ChatInfo&    NullInfo::nullChat() const noexcept{   return *pNullChat; }
ContactInfo& NullInfo::nullContact() const noexcept{   return *pNullContact; }

NullInfo& NullInfo::instance()
{
    static NullInfo info;
    return info;
}


//for user info
UserInfo::UserInfo(QObject* parent ) : QObject(parent){
}


void UserInfo::setName(QString str) { _name = std::move(str);}
void UserInfo::setPassword(QString pas) { _password = std::move(pas);}
void UserInfo::setEmail(QString email) { _email = std::move(email);}

QString UserInfo::name() const { return _name;}
QString UserInfo::password() const { return _password;}
QString UserInfo::email() const{ return _email;}
int UserInfo::id() const { return _id; }

const std::vector<ChatInfo*>& UserInfo::chatList() const noexcept { return _chatList;}
const std::vector<ChatInfo*>& UserInfo::requestList() const noexcept{ return _requestChatList;}
std::vector<ContactInfo*>& UserInfo::friendList() noexcept { return _friendList;}
std::vector<ContactInfo*>& UserInfo::requestList() noexcept { return _requestList;}
std::vector<ContactInfo*>& UserInfo::blockedList() noexcept { return _blockedList;}

//these have no parent , so they will be deleted in the destructor
//I am putting them in ascending order by id , so that i can use binary search
void UserInfo::addChat(ChatInfo* c) {
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_chatList, c);
}

void UserInfo::addFriend(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_friendList, c);
}
void UserInfo::addToBlockedList(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_blockedList, c);
}
void UserInfo::addToRequestList(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_requestList, c);
}
void UserInfo::addToStrangerList(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_strangerList, c);
}

void UserInfo::addUserByFlag(ContactInfo* contact)
{
    auto flags = contact->flags();
    if (flags & ContactInfo::Status::Friend)
        addFriend(contact);
    else if (flags & ContactInfo::Status::HasRequest)
        addToRequestList(contact);
    else if (flags & ContactInfo::Status::IsBlocked)
        addToBlockedList(contact);
    else
        addToStrangerList(contact);
}

std::vector<ContactInfo*> UserInfo::findUsers(std::vector<int> list)
{
    std::vector<ContactInfo*> newList;
    for (int id : list)
    {
        if(id != _id)
        newList.emplace_back(findUser(id));
    }

    return newList;
}

void UserInfo::removeChat(QString name) {
    for(auto b = _chatList.begin() , e = _chatList.end(); b != e ; b++)
    {
        if((*b)->name() == name)
        {
            (*b)->deleteLater();
            _chatList.erase(b);
            break;
        }
    }
}


void UserInfo::showChatInDebug() const noexcept
{
    /*for(const ChatInfo* chat : _chatList)
    {
        qDebug() << "Id: " << chat->id() << "  Chat: " << chat->name();
        qDebug() << "Members: ";

        for( const int& c : chat->members())
            qDebug() << c << ' ';
        for(const MessageInfo* m : chat->history())
        {
            qDebug() << m->name() << " at " << m->timestamp();
            qDebug() << m->text();
        }
    }*/

    //for(const ContactInfo* info : _friendList)
    //{
    //    qDebug() << "User: " << info->name() << "   Id: " << info->id();
    //    qDebug() << "IsOnline: " << info->online() << ' ' << "LastSeen: "  << info->lastSeen();
    //}
    for(const ChatInfo* chat : _chatList)
    {
        qDebug() << "Chat id: " << chat->id();
    }
}
ChatInfo& UserInfo::getChatById(int id)
{
    return *Tools::binaryIdSearch<ChatInfo*>(_chatList, id);
}
ContactInfo* UserInfo::findUser_KnownLists(int id)
{
    ContactInfo* found = nullptr;

    if (found = Tools::binaryIdSearch(_friendList, id))
        return found;

    if (found = Tools::binaryIdSearch(_requestList, id))
        return found;

    if (found = Tools::binaryIdSearch(_blockedList, id))
        return found;


    return found;
}

ContactInfo* UserInfo::findUser_UnknownList(int id)
{
    return Tools::binaryIdSearch(_strangerList, id);
}
ContactInfo* UserInfo::findUser(int id)
{
    ContactInfo* found = nullptr;
    if (found = findUser_KnownLists(id))
        return found;
    if (found = Tools::binaryIdSearch(_strangerList, id))
        return found;
    return found;
}


void UserInfo::transformChats() noexcept
{
    for (ChatInfo* info : _chatList)
        adaptChat(info);
}
void UserInfo::adaptChat(ChatInfo* info)
{
    if (info->isPrivate())
    {
        auto list = info->members();
        char ind = 0;
        if (list[ind] == _id)
            ind = 1;

        ContactInfo* contact = findUser(list[ind]);
        info->setName(contact->name());
        info->connectSlotsForPrivateChats(contact);
    }
}

void UserInfo::removeFriend(int id)
{
    for (int i = 0 ; i < _friendList.size() ; i++)
    {
        ContactInfo* f = _friendList[i];
        if (_friendList[i]->id() == id)
        {
            f->removeFlags(ContactInfo::Status::Friend);
            emit f->removed(id);
            _friendList.erase(_friendList.begin() + i);
            addToStrangerList(f);
        }
    }
}

void UserInfo::moveUserToUnknownList(ContactInfo* info)
{
    char flags = info->flags();
    ContactInfo* contact = nullptr;
    if ( flags & (char)ContactInfo::Status::Friend)
    {
        contact = Tools::takeItem<ContactInfo*>(_friendList, [info](ContactInfo* info0) {
            return info->id() == info0->id();
            });
    }
    else if (flags & (char)ContactInfo::Status::HasRequest)
    {
        contact = Tools::takeItem<ContactInfo*>(_requestList, [info](ContactInfo* info0) {
            return info->id() == info0->id();
            });
    }
    else if (flags & (char)ContactInfo::Status::IsBlocked)
    {
        contact = Tools::takeItem<ContactInfo*>(_blockedList, [info](ContactInfo* info0) {
            return info->id() == info0->id();
            });
    }

    if (contact)
    {
        contact->removeFlags(ContactInfo::Status::Friend | ContactInfo::Status::HasRequest);
        Tools::insertIntoArrayWhileKeepingOrder(_strangerList, contact);
    }
}

void UserInfo::moveUserToBlocked(ContactInfo* info)
{
    char flags = info->flags();
    ContactInfo* contact = nullptr;
    if (flags & (char)ContactInfo::Status::Friend)
    {
        contact = Tools::takeItem<ContactInfo*>(_friendList, [info](ContactInfo* info0) {
            return info->id() == info0->id();
            });
    }
    else if (flags & (char)ContactInfo::Status::HasRequest)
    {
        contact = Tools::takeItem<ContactInfo*>(_requestList, [info](ContactInfo* info0) {
            return info->id() == info0->id();
            });
    }
    else
    {
        contact = Tools::takeItem<ContactInfo*>(_strangerList, [info](ContactInfo* info0) {
            return info->id() == info0->id();
            });
    }
    if (contact)
    {
        Tools::insertIntoArrayWhileKeepingOrder(_blockedList, contact);
        contact->addFlags(ContactInfo::Status::IsBlocked);
        contact->removeFlags(ContactInfo::Status::Friend | ContactInfo::Status::HasRequest);
    }
}



void UserInfo::clearAllData() noexcept
{
    _name = _password = _email = "";
    clearAccountData();
}
//clears all the chats , friends etc etc
void UserInfo::clearAccountData() noexcept
{
    for(ChatInfo* info : _chatList)
        info->deleteLater();

    for(ChatInfo* info : _requestChatList)
        info->deleteLater();

    for(ContactInfo* contact : _friendList)
        contact->deleteLater();

    for (ContactInfo* contact : _requestList)
        contact->deleteLater();

    for(ContactInfo* contact : _blockedList)
        contact->deleteLater();

    for (ContactInfo* contact : _strangerList)
        contact->deleteLater();

    _chatList.resize(0);
    _requestChatList.resize(0);
    _friendList.resize(0);
    _blockedList.resize(0);
    _strangerList.resize(0);
}

void UserInfo::removeRequest(int id)
{
    //could implement binary search
    for(ContactInfo* request : _requestList)
    {
        if(request->id() == id)
        {
            request->deleteLater();
            _requestList.erase(_requestList.begin() + (request - *_requestList.begin()));
            break;
        }
    }
}

ChatInfo* UserInfo::privateChatById(int id)
{
    for (ChatInfo* info : _chatList)
    {
        if (info->isPrivate())
        {
            auto& list = info->members();
            if (list[0] == id || list[1] == id)
                return info;
        }
    }
    return nullptr;
}

ChatInfo* UserInfo::chatById(int id)
{
    for (ChatInfo* info : _chatList)
        if (info->id() == id)
            return info;

    return nullptr;
}


ChatInfo& UserInfo::firstChat() const {
    if (_chatList.size())
        return *_chatList.front();
    else
        return NullInfo::instance().nullChat();
}

bool UserInfo::chatListEmpty() const { return _chatList.size() == 0; }
