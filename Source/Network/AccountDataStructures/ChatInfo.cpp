#include "ChatInfo.h"
#include "Tools.h"


ChatInfo::ChatInfo(QObject* parent) : QObject(parent) { _private = false; }
void ChatInfo::setMessageHistory(std::vector<MessageInfo*> vec) { _history = vec; }
void ChatInfo::setName(QString name) { _name = std::move(name); emit nameChanged(_name); }
void ChatInfo::setNewName(QString name0)
{
    _name = name0;
    emit nameChanged(_name);
}
void ChatInfo::setMembers(std::vector<int> members_id) { _members = std::move(members_id); }
void ChatInfo::setMembers(const QString& str) { _members = Tools::extractIntsFromArr(str); }

void ChatInfo::setReadOnlyMembers(std::vector<int> members_id) { _readOnlyMembers = std::move(members_id); }
void ChatInfo::setReadOnlyMembers(const QString& str) { _readOnlyMembers = Tools::extractIntsFromArr(str); }
void ChatInfo::setAdminId(int id) { _adminId = id; }
void ChatInfo::setNewAdmin(int id) {
    _adminId = id;
    emit newAdmin(id);
}
void ChatInfo::setId(int id) { _id = std::move(id); }
void ChatInfo::setType(bool isPrivate) { _private = isPrivate; }

void ChatInfo::addMessage(MessageInfo* mes) { _history.emplace_back(mes); }

QString ChatInfo::name() const { return _name; }
int ChatInfo::id() const { return _id; }
bool ChatInfo::isPrivate() const { return _private; }

const std::vector<int>& ChatInfo::members() const { return _members; }
const std::vector<int>& ChatInfo::readOnlyMembers() const { return _readOnlyMembers; }
int ChatInfo::adminId() const { return _adminId; }
const std::vector<MessageInfo*>& ChatInfo::history() const { return _history; }
const MessageInfo& ChatInfo::back() const { return *_history.back(); }
MessageInfo* ChatInfo::addMessage(QString name, QString message)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(std::move(message));
    return _history.emplace_back(m);
}
MessageInfo* ChatInfo::addMessageToQueue(QString name, QString message)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(std::move(message));

    emit newMessageInQueue(*m, _id);
    return _queue.emplace_back(m);
}

void ChatInfo::mediaMessageUploaded(QString fileName)
{
    MessageInfo* m = _mediaMessageQueue.front();
    _mediaMessageQueue.pop();

    m->setText(MessageInfo::imageSign + fileName);

    emit newMessageInQueue(*m, _id);
}

MessageInfo* ChatInfo::addMessageToQueue(QString name, QUrl url)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(MessageInfo::imageSign + "%");

    //emit newMessageInQueue(*m, _id);

    return _mediaMessageQueue.emplace(m);
}
const std::vector<MessageInfo*> ChatInfo::lastNMessages(int start, int n) const
{
    if (int(_history.size()) - start - n - 1 < 0)
    {
        if ((int)_history.size() - start - 1 < 0)
            return {};
        else
            return { _history.begin(),  _history.end() - start - 1 };
    }
    else
        return { _history.end() - start - n - 1,  _history.end() - start - 1 };
}

const std::vector<MessageInfo*> ChatInfo::lastNMessages(int n) const
{
    if (_history.size() <= n)
        return _history;
    else
        return { _history.end() - n - 1  , _history.end() };
}
MessageInfo* ChatInfo::addWaitingMessage(QString name, QString message)
{
    MessageInfo* m = new MessageInfo(this);
    m->setTimestamp(QDateTime::currentDateTime());
    m->setName(std::move(name));
    m->setText(std::move(message));
    return _queue.emplace_back(m);
}
bool  ChatInfo::approveLastInQueue()
{
    if (_queue.empty() == false)
    {
        _history.emplace_back(_queue.back());
        _queue.pop_back();
        return true;
    }
    else
        return false;
}

void ChatInfo::addMessageFromDatabase(QString name, QString message, QString timestamp)
{
    if (_queue.empty() == false)
    {
        auto* m = _queue.back();
        m->outOfQueue();
        _history.emplace_back(m);
        _queue.pop_back();
    }
    else
    {
        MessageInfo* m = new MessageInfo(this, std::move(name), std::move(message), std::move(timestamp));
        emit newMessageAdded(*m);
        _history.emplace_back(m);
    }
}

void ChatInfo::removeMember(int id, int userId, bool forcefullyRemoved)
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



bool ChatInfo::operator<(const ChatInfo* rhs) const { return _id < rhs->id(); }
bool ChatInfo::operator<(ChatInfo* rhs) { return _id < rhs->id(); }
bool ChatInfo::operator>(const ChatInfo* rhs) const { return _id > rhs->id(); }
bool ChatInfo::operator>(ChatInfo* rhs) { return _id > rhs->id(); }
bool ChatInfo::operator==(const ChatInfo* rhs) const { return _id == rhs->id(); }
bool ChatInfo::operator==(ChatInfo* rhs) { return _id == rhs->id(); }
