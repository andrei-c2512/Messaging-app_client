#include "AccountInfoStorage.h"
#include "Tools.h"
#include "AccountDataStructures/NullInfo.h"

AccountInfoStorage::AccountInfoStorage(QObject* parent ) : QObject(parent){
}


void AccountInfoStorage::setName(QString str) { _name = std::move(str);}
void AccountInfoStorage::setPassword(QString pas) { _password = std::move(pas);}
void AccountInfoStorage::setEmail(QString email) { _email = std::move(email);}

QString AccountInfoStorage::name() const { return _name;}
QString AccountInfoStorage::password() const { return _password;}
QString AccountInfoStorage::email() const{ return _email;}
int AccountInfoStorage::id() const { return _id; }

const std::vector<ChatInfo*>& AccountInfoStorage::chatList() const noexcept { return _chatList;}
const std::vector<ChatInfo*>& AccountInfoStorage::requestList() const noexcept{ return _requestChatList;}
std::vector<ContactInfo*>& AccountInfoStorage::friendList() noexcept { return _friendList;}
std::vector<ContactInfo*>& AccountInfoStorage::requestList() noexcept { return _requestList;}
std::vector<ContactInfo*>& AccountInfoStorage::blockedList() noexcept { return _blockedList;}

//these have no parent , so they will be deleted in the destructor
//I am putting them in ascending order by id , so that i can use binary search
void AccountInfoStorage::addChat(ChatInfo* c) {
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_chatList, c);
}

void AccountInfoStorage::addFriend(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_friendList, c);
}
void AccountInfoStorage::addToBlockedList(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_blockedList, c);
}
void AccountInfoStorage::addToRequestList(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_requestList, c);
}
void AccountInfoStorage::addToStrangerList(ContactInfo* c)
{
    c->setParent(this);
    Tools::insertIntoArrayWhileKeepingOrder(_strangerList, c);
}

void AccountInfoStorage::addUserByFlag(ContactInfo* contact)
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

std::vector<ContactInfo*> AccountInfoStorage::findUsers(std::vector<int> list)
{
    std::vector<ContactInfo*> newList;
    for (int id : list)
    {
        if(id != _id)
        newList.emplace_back(findUser(id));
    }

    return newList;
}

void AccountInfoStorage::removeChat(QString name) {
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


void AccountInfoStorage::showChatInDebug() const noexcept
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
    //for(const ChatInfo* chat : _chatList)
    //{
    //    qDebug() << "Chat id: " << chat->id();
    //}
}
ChatInfo& AccountInfoStorage::getChatById(int id)
{
    return *Tools::binaryIdSearch<ChatInfo*>(_chatList, id);
}
ContactInfo* AccountInfoStorage::findUser_KnownLists(int id) const
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

ContactInfo* AccountInfoStorage::findUser_StrangerList(int id) const
{
    return Tools::binaryIdSearch(_strangerList, id);
}
ContactInfo* AccountInfoStorage::findUser(int id) const
{
    ContactInfo* found = nullptr;
    if (found = findUser_KnownLists(id))
        return found;
    if (found = Tools::binaryIdSearch(_strangerList, id))
        return found;
    return found;
}


void AccountInfoStorage::transformChats() noexcept
{
    for (ChatInfo* info : _chatList)
        adaptChat(info);
}
void AccountInfoStorage::adaptChat(ChatInfo* info)
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

void AccountInfoStorage::removeFriend(int id)
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

void AccountInfoStorage::moveUserToStrangerList(ContactInfo* info)
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

void AccountInfoStorage::moveUserToBlocked(ContactInfo* info)
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



void AccountInfoStorage::clearAllData() noexcept
{
    _name = _password = _email = "";
    clearAccountData();
}
//clears all the chats , friends etc etc
void AccountInfoStorage::clearAccountData() noexcept
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
    emit accountDataCleared();
}

void AccountInfoStorage::removeRequest(int id)
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

ChatInfo* AccountInfoStorage::privateChatById(int id)
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

ChatInfo* AccountInfoStorage::chatById(int id)
{
    for (ChatInfo* info : _chatList)
        if (info->id() == id)
            return info;

    return nullptr;
}


ChatInfo& AccountInfoStorage::firstChat() const {
    if (_chatList.size())
        return *_chatList.front();
    else
        return NullInfo::instance().nullChat();
}

bool AccountInfoStorage::chatListEmpty() const { return _chatList.size() == 0; }

std::vector<QString> AccountInfoStorage::namesForContacts(std::vector<int> idList) const
{
    std::vector<QString> nameList;
    nameList.reserve(idList.size());
    for (int i = 0 ; i < idList.size() ; i++)
    {
       if (_id != idList[i])
       {
           ContactInfo* c = findUser(idList[i]);
           assert(c != nullptr);
           nameList.emplace_back(c->name());
       }
       else
           nameList.emplace_back(_name);
    }
    nameList.shrink_to_fit();
    return nameList;
}

void AccountInfoStorage::setId(int id) { _id = id; }
void AccountInfoStorage::setFriendList(std::vector<ContactInfo*> list) { _friendList = std::move(list); }
void AccountInfoStorage::setRequestList(std::vector<ContactInfo*> list) { _requestList = std::move(list); }
void AccountInfoStorage::setBlockedList(std::vector<ContactInfo*> list) { _blockedList = std::move(list); }
void AccountInfoStorage::setSearchedForList(std::vector<ContactInfo*> list) { _searchedForUsers = std::move(list); }

std::vector<ContactInfo*>& AccountInfoStorage::searchedForList() noexcept { return _searchedForUsers; }
std::vector<ContactInfo*>& AccountInfoStorage::strangerList() { return _strangerList; }
