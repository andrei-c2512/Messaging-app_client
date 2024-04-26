#include "ServerInfoProcessor.h"


ServerInfoProcessor::ServerInfoProcessor(QObject* object)
    :UserInfo(object)
{
    loggedIn = false;
    userInfoReceived = false;
    _socket = new QTcpSocket(this);
    connect(_socket , &QTcpSocket::readyRead , this , &ServerInfoProcessor::onReadyRead);
    connect(_socket , &QTcpSocket::connected , this , [=](){
        qDebug() << "Connected to server";
        if(loggedIn)
        {
            logIntoDatabase();
        }
        this->connectTimer->stop();
    });
    connect(_socket , &QTcpSocket::disconnected , this , [=](){
        qDebug() << "Disconnected from server";
        clearAccountData();
        connectTimer->start();
        userInfoReceived = false;
        emit accountDataCleared();
    });
    connectTimer = new QTimer(this);
    connectTimer->setInterval(1000);
    connectTimer->start();

    connect(connectTimer , &QTimer::timeout , this , &ServerInfoProcessor::attemptConnection);
}

void ServerInfoProcessor::attemptConnection(){ _socket->connectToHost(QHostAddress::LocalHost , 19704);}

void ServerInfoProcessor::setName(QString str) { _name = std::move(str); loggedIn = false;}
void ServerInfoProcessor::setEmail(QString str) { _email = std::move(str);  loggedIn = false;}
void ServerInfoProcessor::setPassword(QString str) { _password = std::move(str); loggedIn = false;}
void ServerInfoProcessor::registerIntoDatabase()
{
    QString str = QString::number((int)RequestToServer::Register) + '(' + _name + ',' + _email + ',' +  _password + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
    emit sentRegisterRequest();
}

void ServerInfoProcessor::logIntoDatabase()
{
    QString str = QString::number((int)RequestToServer::Login) + '(' + _name + ',' +  _password + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
    emit sentLoginRequest();
}
void ServerInfoProcessor::addMessage(const MessageInfo& info , int chatId)
{
    QString str = QString::number((int)RequestToServer::NewMessage) +
                  " (" + QString::number(std::move(chatId)) + " ," +
                  info.name() + " ," +
                  info.text().replace(',' , "\\,").replace('(' , "\\(").replace(')' , "\\)") + " ," +
                  info.timestamp().date().toString("yyyy-MM-dd") + ' ' + info.timestamp().time().toString() +
                  " )";

    _socket->write(str.toUtf8());
    _socket->flush();
}
void ServerInfoProcessor::searchForPeople( QString criteria)
{
    QString str = QString::number((int)RequestToServer::SearchForClient) + '(' +
                  QString::number(_id) + ',' +
                  criteria + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
}
void ServerInfoProcessor::createPrivateChatWithFriend(int friendId)
{
    QString str = QString::number((int)RequestToServer::CreatePrivateChat) + '(' + QString::number(_id) + ',' + QString::number(friendId) + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::createGroupChat(std::vector<int> list)
{
    QString str = QString::number((int)RequestToServer::CreateGroupChat) + '(' + QString::number(_id);
    for (int& id : list)
        str += ',' + QString::number(id);
    str += ')';

    _socket->write(str.toUtf8());
    _socket->flush();
}
void ServerInfoProcessor::manageFriendRequest(bool accepted , int userId)
{
    // commandNumber(userId , friendRequestUserId  , accepted)
    QString str = QString::number((int)RequestToServer::ManageFriendRequest) +
                  '(' + QString::number(_id) + ',' + QString::number(userId) + ',' + QString::number(accepted) + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
}
void ServerInfoProcessor::sendFriendRequest(int userId)
{
    // commandNumber(userId , friendRequestUserId )
    QString str = QString::number((int)RequestToServer::SendFriendRequest) + '(' + QString::number(_id) + ',' +
                  QString::number(userId) + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
}
void ServerInfoProcessor::removeFriend(int userId)
{
    QString str = QString::number((int)RequestToServer::RemoveFriend) + '(' + QString::number(_id) + ',' +
                  QString::number(userId) + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
}
void ServerInfoProcessor::blockUser(int id)
{
    QString str = QString::number((int)RequestToServer::BlockPerson) + '(' + QString::number(_id) + ',' +
                    QString::number(id) + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::unblockUser(int id)
{
    QString str = QString::number((int)RequestToServer::UnblockUser) + '(' + QString::number(_id) + ',' +
        QString::number(id) + ')';

    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::removeFromGroup(int chatId, int userId)
{
    QString str = QString::number((int)RequestToServer::RemoveFromGroup) + '(' + QString::number(chatId) + ',' + 
        QString::number(userId) + ',' + "true" + ')';

    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::getInfoForUsers(std::vector<int> list)
{
    //CmdNum(id1 , id2 , .... , idn , userId)
    QString str = QString::number((int)RequestToServer::GetInfoForUsers) + '(';
    for (int num : list)
        str += QString::number(num) + ',';
    str += QString::number(_id) + ')';

    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::updateChatName(int chatId, QString newName)
{
    QString str = QString::number((int)RequestToServer::UpdateChatName) + " (" +
        QString::number(chatId) + " ," + newName.replace(',', "\,").replace('(', "\(").replace(')', "\)") + " )";

    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::leaveFromGroup(int chatId)
{
    QString str = QString::number((int)RequestToServer::RemoveFromGroup) + '(' + QString::number(chatId) + ',' +
        QString::number(_id) + ',' + "false" + ')';

    _socket->write(str.toUtf8());
    _socket->flush();
}

void ServerInfoProcessor::addPeopleToGroup(int chatId , std::vector<int> idList)
{
    QString str = QString::number((int)RequestToServer::AddPeopleToTheChat) + '(';

    for (int id : idList)
        str += QString::number(id) + ',';

    str += QString::number(chatId) + ')';
    _socket->write(str.toUtf8());
    _socket->flush();
} 


void ServerInfoProcessor::onReadyRead()
{
    QString message = _socket->readAll();
    qDebug() <<" Message from server: " << message;

    int lastPos = -1;
    int pos = message.indexOf('~');
    while(pos > lastPos && pos < message.length())
    {
        qDebug() << message.mid(pos);
        lastPos = pos;
        pos = processCommand(message , pos);
    }
    showChatInDebug();

}
int ServerInfoProcessor::processCommand(const QString& message , int start)
{
    int end = start;
    try {
        int requestCodeBegin = message.indexOf(commandBegin , start) + commandBegin.size();
        int requestCodeEnd = message.indexOf(':' , start);

        QString code = message.mid(requestCodeBegin, requestCodeEnd - requestCodeBegin);
        qDebug() << "Request code: " << code;
        int codeNumber;
        //the index of the end of the current command
        if(code[0] == 'F')
        {
            /*
         *  NameUsed 0
            NameNotFound 1
            IncorrectPassword 2
            EmailUsed 3
        */
            code.erase(std::begin(code));
            codeNumber = code.toInt();
            switch(codeNumber)
            {
            case 0:
                emit nameUsedError();
                break;
            case 1:
                emit nameNotFoundError();
                break;
            case 2:
                emit incorrectPasswordError();
                break;
            case 3:
                emit emailUsedError();
                break;
            default:
                qDebug() << "Received unkown message";
                break;
            }
            end = message.indexOf(commandEnd , requestCodeEnd) + commandEnd.length();
        }
        else if(code[0] == 'I')
        {
            /*
         * SignedIn = 0
         * ChatInfo = 1, // will give the user the info of what chats he takes part in and it's messages
         * Contacts = 2// will give the user his friend list , blocked people , people who are offline/online/last seen on yyyy-mm--dd
         * */
            code.erase(std::begin(code));
            codeNumber = code.toInt();
            qDebug() << "Code number: " << codeNumber;
            switch((InfoFromServer)codeNumber)
            {
            case InfoFromServer::SignedIn:
                end = processSignInInfo(message , start);
                loggedIn = true;
                emit signInAccepted();
                break;
            case InfoFromServer::Chats:
                end = processChatListInfo(message , start);
                emit chatDataReceived();
                break;
            case InfoFromServer::Contacts:
                end = processContactInfo(message , start);
                requestDataOfUnknownUsers();
                emit contactInfoLoaded();
                break;
            case InfoFromServer::NewMessage:
                end = processNewMessageInfo(message , start);
                break;
            case InfoFromServer::SearchedUserList:
                end = processSearchedForList(message , start);
                emit newSearchData();
                break;
            case InfoFromServer::NewFriend:
                end = processNewFriendInfo(message , start);
                emit newFriend();
                break;
            case InfoFromServer::NewFriendRequest:
                end = processNewFriendRequest(message , start);
                emit newFriendRequest();
                break;
            case InfoFromServer::NewChat:
                end = processNewChat(message, start);
                requestDataOfWaitingUsers();
                break;
            case InfoFromServer::FriendRemoved:
                end = processFriendRemoval(message, start);
                break;
            case InfoFromServer::PersonBlocked:
                end = processPersonBlocked(message, start);
                break;
            case InfoFromServer::YouGotBlocked:
                end = processGettingBlocked(message, start);
                break;
            case InfoFromServer::InfoOfUsers:
                end = processListOfStrangers(message, start);
                transformChats();
                if (userInfoReceived == false)
                {
                    userInfoReceived = true;
                    emit allUserInfoReceived();
                }
                emit unknownListReceived();
                break;
            case InfoFromServer::UserUnblocked:
                end = processUserUnblocked(message, start);
                break;
            case InfoFromServer::UserUnblockedYou:
                end = processUserUnblockedYou(message, start);
                break;
            case InfoFromServer::NewChatName:
                end = processNewChatName(message, start);
                break;
            case InfoFromServer::GroupMemberRemoved:
                end = processGroupMemberRemoval(message, start);
                break;
            case InfoFromServer::GroupMemberAdded:
                end = processNewGroupMembers(message, start);
                requestDataOfWaitingUsers();
                break;
            case InfoFromServer::NecessaryContacts:
                end = processNecessaryContacts(message, start);
                break;
            case InfoFromServer::NewAdmin:
                end = processNewAdmin(message, start);
                break;
            case InfoFromServer::FriendStatus:
                end = processFriendStatus(message, start);
                break;
            default:
                qDebug() << "Received unkown message";
                break;
            }
        }
        else
        {
            QString error_message = QString("Did not encounter a known command type -> ") + code[0];
            throw std::logic_error(error_message.toStdString());
        }
    } catch (std::logic_error& e) {
        qDebug() << e.what();
    }
    return end;
}

int ServerInfoProcessor::processNewGroupMembers(const QString& str, int start)
{
    int chatIdPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    int idListPos = str.indexOf(chat_newMembersSep, chatIdPos) + chat_newMembersSep.length();

    int chatId = str.mid(chatIdPos + 1, str.indexOf('"', chatIdPos + 1) - chatIdPos - 1).toInt();
    std::vector<int> list = Tools::extractIntsFromArr(str.mid(idListPos + 1, str.indexOf('"', idListPos + 1) - idListPos - 1));

    ChatInfo* chatInfo = chatById(chatId);
    for (int id : list)
    {
        ContactInfo* contactInfo = findUser(id);
        if (contactInfo == nullptr)
            _awaitingContactInfoList.emplace_back(std::make_pair(id , chatId));
        else
            chatInfo->addMember(id);
    }

    return str.indexOf(commandEnd, idListPos) + commandEnd.length();
}

int ServerInfoProcessor::processNewChatName(const QString& str, int start)
{
    int idPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    int namePos = str.indexOf(chat_nameSep, idPos) + chat_nameSep.length();

    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    QString newName = str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1);

    ChatInfo* chat = chatById(id);
    //new name emits a signal so that it announces the change , the other method does not
    chat->setNewName(std::move(newName));

    return str.indexOf(commandEnd, namePos) + commandEnd.length();
}

int ServerInfoProcessor::processUserUnblocked(const QString& str, int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = findUser(id);
    if (info)
    {
        moveUserToUnknownList(info);
        info->removeFlags(ContactInfo::Status::IsBlocked);

        emit info->removed(info->id());
        emit info->gotBlocked(false);
    }

    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}
int ServerInfoProcessor::processUserUnblockedYou(const QString& str, int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = findUser(id);
    if (info)
    {
        moveUserToUnknownList(info);
        info->removeFlags(ContactInfo::Status::HasBlockedYou);

        emit info->removed(info->id());
        emit info->blockedYou(false);
    }

    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}

int ServerInfoProcessor::processGroupMemberRemoval(const QString& str, int start)
{
    int chatIdPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    int contactIdPos = str.indexOf(contact_idSep, chatIdPos) + contact_idSep.length();
    int chatRemovalStatusPos = str.indexOf(chat_removedSep, contactIdPos) + chat_removedSep.length();

    int chatId = str.mid(chatIdPos + 1, str.indexOf('"', chatIdPos + 1) - chatIdPos - 1).toInt();
    int contactId = str.mid(contactIdPos + 1, str.indexOf('"', contactIdPos + 1) - contactIdPos - 1).toInt();
    bool removed = str[chatRemovalStatusPos + 1] == 't';

    ChatInfo* info = chatById(chatId);
    info->removeMember(contactId , _id , removed);

    return str.indexOf(commandEnd, contactId) + commandEnd.length();
}

int ServerInfoProcessor::processListOfStrangers(const QString& str, int start)
{
    std::function<void(ContactInfo*)> verify;
    if (_awaitingContactInfoList.size())
    {
        verify = [=](ContactInfo* info) {
            for (int i = 0; i < _awaitingContactInfoList.size(); i++)
            {
                if (info->id() == _awaitingContactInfoList[i].first)
                {
                    ChatInfo* chat = chatById(_awaitingContactInfoList[i].second);
                    chat->addMember(info->id());
                    _awaitingContactInfoList.erase(_awaitingContactInfoList.begin() + i);
                    break;
                }
            }
        };
    }
    else
    {
        verify = [](ContactInfo* info) {
            Q_UNUSED(info)
        };
    }

    //this was before adding the flag macros
    std::vector<ContactInfo*> vec;
    int idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
    start = str.indexOf(contact_idSep, start);
    while (start != -1)
    {
        idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
        namePos = str.indexOf(contact_nameSep, idPos) + contact_nameSep.length();
        friendListPos = str.indexOf(contact_friendListSep, namePos) + contact_friendListSep.length();
        onlinePos = str.indexOf(contact_onlineSep, friendListPos) + contact_onlineSep.length();
        lastSeenPos = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();
        isBlockedPos = str.indexOf(contact_isBlockedSep, lastSeenPos) + contact_isBlockedSep.length();
        hasRequestPos = str.indexOf(contact_hasRequestSep, isBlockedPos) + contact_hasRequestSep.length();

        // + 2 because - once to go to next pos '"' , and another to go to the non '"' pos
        //because otherwise , it would return the pos of the first '"' which will make it read an empty string

        char flags = 0;
        if (str[hasRequestPos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::HasRequest;
        if (str[isBlockedPos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::HasBlockedYou;
        if (str[onlinePos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::Online;

        ContactInfo* contact = new ContactInfo(this, str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1),
            ContactInfo::Status(flags),
            str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

        QString aux = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());
        
        vec.emplace_back(contact);
        start = str.indexOf(contact_idSep, lastSeenPos);
        UserInfo::addToStrangerList(contact);

        verify(contact);
    }

    //I don't use a custom add function because the order of the received contact info is the desired type

    for (ContactInfo* info : _strangerList)
    {
        qDebug() << "Name: " << info->name();
    }
    return str.indexOf(commandEnd, hasRequestPos);
}

int ServerInfoProcessor::processPersonBlocked(const QString& str, int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = findUser(id);
    moveUserToBlocked(info);

    emit info->gotBlocked(true);

    return str.indexOf(commandEnd, start) + commandEnd.length();
}

int ServerInfoProcessor::processGettingBlocked(const QString& str, int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = findUser(id);
    if (info)
    {
        moveUserToUnknownList(info);
        info->addFlags(ContactInfo::Status::HasBlockedYou);

        emit info->blockedYou(true);
        emit info->removed(info->id());
    }

    return str.indexOf(commandEnd, start) + commandEnd.length();
}



int ServerInfoProcessor::processNewChat(const QString& str, int start)
{
    int idPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    int namePos = str.indexOf(chat_nameSep, idPos) + chat_nameSep.length();
    int historyPos = str.indexOf(chat_historySep, namePos) + chat_historySep.length();
    int memberlistPos = str.indexOf(chat_memberListSep, historyPos) + chat_memberListSep.length();
    int privatePos = str.indexOf(chat_privateSep, memberlistPos) + chat_privateSep.length();
    int adminPos = str.indexOf(chat_adminIdSep, privatePos) + chat_adminIdSep.length();
    int readOnlyListPos = str.indexOf(chat_readOnlyListSep, adminPos) + chat_readOnlyListSep.length();
    int isSenderPos = str.indexOf(chat_isSenderSep, privatePos) + chat_isSenderSep.length();

    ChatInfo* info = new ChatInfo(this);
    int chatId = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    info->setId(chatId);
    info->setName(str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1));
    info->setMessageHistory(extractHistoryFromChat(str, historyPos));
    info->setMembers(str.mid(memberlistPos + 1, str.indexOf('"', memberlistPos + 1) - memberlistPos - 1));
    info->setType(str[privatePos + 1] == 't');
    info->setAdminId(str.mid(adminPos + 1, str.indexOf('"', adminPos + 1) - adminPos - 1).toInt());

    auto list = Tools::extractIntsFromArr(str, readOnlyListPos);
    for (int id : list)
    {
        ContactInfo* contact = findUser(id);
        if (contact == nullptr)
        {
            _awaitingContactInfoList.emplace_back(std::make_pair(id, chatId));
        }
    }
    info->setReadOnlyMembers(list);

    bool isSender = str[isSenderPos + 1] == 't';

    UserInfo::addChat(info);
    adaptChat(info);

    connect(info, SIGNAL(newMessageInQueue(const MessageInfo&, int)), this, SLOT(addMessage(const MessageInfo&, int)));

    if (_awaitingContactInfoList.empty() == false)
    {
        if (isSender)
            emit createdNewChat(info->id());
        else
            emit addedToNewChat(info->id());
    }

    return str.indexOf(commandEnd, isSenderPos) + commandEnd.length();
}
std::pair<ChatInfo*, int> ServerInfoProcessor::processChatInfo(const QString& str, int start)
{
    /* example of chat info data
     * ~I1:"Id":"2""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",\"2021-04-05 00:00:00\")"}
     * "MemberList":"{1,2,5}""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",\"2021-04-05 00:00:00\"
     * )"}"MemberList":"{1,2,5}""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",
     * \"2021-04-05 00:00:00\")"}"MemberList":"{1,2,5}""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")",
     * "(\"Leo Gabriel\",\"Ce te pisi atata\",\"2021-04-05 00:00:00\")"}"MemberList":"{1,2,5}"\~
     * */

    int pos = start;

    ChatInfo* chatInfo = new ChatInfo(this);
    //while(lastPos < pos)
    {
        int idIndex = str.indexOf(chat_idSep, pos) + chat_idSep.length();
        int nameIndex = str.indexOf(chat_nameSep, idIndex) + chat_nameSep.length();
        int historyIndex = str.indexOf(chat_historySep, nameIndex) + chat_historySep.length();
        int isPrivateIndex = str.indexOf(chat_privateSep, historyIndex) + chat_privateSep.length();
        int memberListIndex = str.indexOf(chat_memberListSep, isPrivateIndex) + chat_memberListSep.length();
        int adminIndex = str.indexOf(chat_adminIdSep, memberListIndex) + chat_adminIdSep.length();
        int readOnlyListIndex = str.indexOf(chat_readOnlyListSep, adminIndex) + chat_readOnlyListSep.length();

        //added 2 because ":"" (of length 2) will always follow after nameSep
        QString auW = str.mid(idIndex + 1, str.indexOf('"', idIndex + 1) - idIndex - 1);
        chatInfo->setId(str.mid(idIndex + 1, str.indexOf('"', idIndex + 1) - idIndex - 1).toInt());
        chatInfo->setName(str.mid(nameIndex + 1, str.indexOf('"', nameIndex + 1) - nameIndex - 1));
        chatInfo->setMessageHistory(extractHistoryFromChat(str, historyIndex));
        chatInfo->setType(str[isPrivateIndex + 1] == 't');

        chatInfo->setMembers(Tools::extractIntsFromArr(str, memberListIndex));
        auto arr = Tools::extractIntsFromArr(str, memberListIndex);
        QString output;
        for (int nr : arr)
            output += QString::number(nr) + ',';
        qDebug() << output;

        chatInfo->setAdminId(str.mid(adminIndex + 1, str.indexOf('"', adminIndex + 1) - adminIndex - 1).toInt());
        chatInfo->setReadOnlyMembers(Tools::extractIntsFromArr(str, readOnlyListIndex));

        pos = memberListIndex;
    }

    qDebug() << connect(chatInfo, SIGNAL(newMessageInQueue(const MessageInfo&, int)), this, SLOT(addMessage(const MessageInfo&, int)));

    return { chatInfo , pos };
}
int ServerInfoProcessor::processNewFriendRequest(const QString& str , int start)
{
    int idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
    start = str.indexOf(contact_idSep , start);

    {
        idPos         = str.indexOf(contact_idSep , start) + contact_idSep.length();
        namePos       = str.indexOf(contact_nameSep , idPos) + contact_nameSep.length();
        friendListPos = str.indexOf(contact_friendListSep , namePos) + contact_friendListSep.length();
        onlinePos     = str.indexOf(contact_onlineSep , friendListPos) + contact_onlineSep.length();
        lastSeenPos   = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();
        isBlockedPos  = str.indexOf(contact_isBlockedSep , lastSeenPos) + contact_isBlockedSep.length();
        hasRequestPos = str.indexOf(contact_hasRequestSep , isBlockedPos) + contact_hasRequestSep.length();

        // + 2 because - once to go to next pos '"' , and another to go to the non '"' pos
        //because otherwise , it would return the pos of the first '"' which will make it read an empty string
        ContactInfo* contact = new ContactInfo(this , str.mid(namePos + 1 , str.indexOf('"' , namePos + 1) - namePos - 1),
                                               ContactInfo::Status(int(str[onlinePos + 1] == 't')),
                                               str.mid(lastSeenPos + 1, str.indexOf('"' , lastSeenPos + 1) - lastSeenPos - 1));

        QString aux = str.mid(idPos + 1 , str.indexOf('"' , idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());

        ContactInfo::ContactStatus flags = ContactInfo::Status::Null;
        if (str[hasRequestPos + 1] == 't')
            flags = flags | ContactInfo::Status::HasRequest;
        if(str[isBlockedPos + 1] == 't')
            flags = flags | ContactInfo::Status::HasBlockedYou;

        contact->setFlags(flags);

        UserInfo::addToRequestList(contact);
        start = str.indexOf(contact_idSep , lastSeenPos);
    }
    return str.indexOf(commandEnd , hasRequestPos) + commandEnd.length();
}
int ServerInfoProcessor::processSignInInfo(const QString& str , int start)
{
    int userIdStart = str.indexOf(user_idSep , start) + user_idSep.length() + 1;
    QString userIdStr = str.mid( userIdStart , str.indexOf('"' , userIdStart + 1) -  userIdStart);
    _id = userIdStr.toInt();
    qDebug() << "User id: " << _id;
    return str.indexOf(commandEnd , start) + commandEnd.length();
}

int ServerInfoProcessor::processSearchedForList(const QString& str , int start)
{
    std::vector<ContactInfo*> vec;
    int idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
    start = str.indexOf(contact_idSep , start);
    while(start != -1)
    {
        idPos         = str.indexOf(contact_idSep , start) + contact_idSep.length();
        namePos       = str.indexOf(contact_nameSep , idPos) + contact_nameSep.length();
        friendListPos = str.indexOf(contact_friendListSep , namePos) + contact_friendListSep.length();
        onlinePos     = str.indexOf(contact_onlineSep , friendListPos) + contact_onlineSep.length();
        lastSeenPos   = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();
        isBlockedPos  = str.indexOf(contact_isBlockedSep , lastSeenPos) + contact_isBlockedSep.length();
        hasRequestPos = str.indexOf(contact_hasRequestSep , isBlockedPos) + contact_hasRequestSep.length();

        // + 2 because - once to go to next pos '"' , and another to go to the non '"' pos
        //because otherwise , it would return the pos of the first '"' which will make it read an empty string


        char flags = 0;
        if (str[hasRequestPos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::HasRequest;
        if (str[isBlockedPos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::HasBlockedYou;
        if(str[onlinePos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::Online;


        ContactInfo* contact = new ContactInfo(this , str.mid(namePos + 1 , str.indexOf('"' , namePos + 1) - namePos - 1),
                                               (ContactInfo::Status)flags,
                                               str.mid(lastSeenPos + 1, str.indexOf('"' , lastSeenPos + 1) - lastSeenPos - 1));

        QString aux = str.mid(idPos + 1 , str.indexOf('"' , idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());

 

        vec.emplace_back(contact);
        start = str.indexOf(contact_idSep , lastSeenPos);
    }

    for(ContactInfo* info : _searchedForUsers)
        info->deleteLater();

    //I don't use a custom add function because the order of the received contact info is the desired type
    _searchedForUsers = vec;
    for(ContactInfo* info : _searchedForUsers)
    {
        qDebug() << "Name: " << info->name();
    }
    return str.indexOf(commandEnd , hasRequestPos);
}

int ServerInfoProcessor::processNewMessageInfo(const QString& str, int start)
{
    // how the data looks like
    /*   (commandBegin + 'I' + QString::number((int)InfoToClient::NewMessage) + ':' +
                "\"Name\":" + "\"" + sender + "\"" + "\"Text\":" + "\"" + text + "\"" + "\"Timestamp\":" + "\"" + timestamp + "\"" + commandEnd);*/

    // + 2 because of the characters ':' and '"'
    int chatIdBegin = str.indexOf(message_chatIdSep , start) + message_chatIdSep.length();

    ChatInfo& info = UserInfo::getChatById(/* extracting the chatId from the string and converting it to Integer*/
                                           str.mid(chatIdBegin + 1 , str.indexOf('"' , chatIdBegin + 1) - (chatIdBegin + 1)).toInt()
        );

    if(info.approveLastInQueue() == false)
    {
        qDebug() << "Message sent";
        qsizetype nameBegin = str.indexOf(message_nameSep , chatIdBegin) + message_nameSep.length();
        qsizetype textBegin = str.indexOf(message_textSep , nameBegin) + message_textSep.length();
        qsizetype timestampBegin = str.indexOf(message_timestampSep , textBegin) + message_timestampSep.length();

        info.addMessageFromDatabase(str.mid(nameBegin + 1      , str.indexOf('"' , nameBegin + 1     ) - nameBegin - 1), /*extracting name*/
                                    str.mid(textBegin + 1      , str.indexOf('"' , textBegin + 1     ) - textBegin - 1), /*extracting text*/
                                    str.mid(timestampBegin + 1 , str.indexOf('"' , timestampBegin + 1) - timestampBegin - 1) /*extracting timestamp*/
                                    );
        return str.indexOf(commandEnd , timestampBegin) + commandEnd.length();
    }
    else
    {
        qDebug() << "New message";
        return str.indexOf(commandEnd , chatIdBegin) + commandEnd.length();
    }
}


int ServerInfoProcessor::processChatListInfo(const QString& str , int start)
{
    int lastPos = -1;
    int pos = start;


    while(lastPos < pos)
    {
        int chatListStart = str.indexOf(chat_idSep , pos);
        if(chatListStart == -1)
            break;
        else
        {
            auto result = processChatInfo(str , chatListStart);
            lastPos = pos;
            pos = result.second;
            UserInfo::addChat(result.first);
        }
    }
    showChatInDebug();
    emit newChatData();
    return str.indexOf(commandEnd , pos) + commandEnd.length();
}


int ServerInfoProcessor::processContactInfo(const QString& str , int start)
{
    int ownFriendListPos = str.indexOf(contactSeps[0] , start) + contactSeps[0].length();
    int requestListPos = str.indexOf(contactSeps[1] , ownFriendListPos) + contactSeps[1].length();
    int blockedListPos = str.indexOf(contactSeps[2] , requestListPos) + contactSeps[2].length();
    int endPos = str.indexOf(commandEnd , blockedListPos );

    _friendList = processContactList(str , ownFriendListPos , requestListPos - contactSeps[1].length());
    for (ContactInfo* fr : _friendList)
        fr->addFlags(ContactInfo::Status::Friend);

    _requestList = processContactList(str , requestListPos , blockedListPos - contactSeps[2].length());
    for (ContactInfo* request : _requestList)
        request->addFlags(ContactInfo::Status::HasRequest);

    _blockedList = processContactList(str , blockedListPos , endPos);
    for (ContactInfo* enemy : _blockedList)
        enemy->addFlags(ContactInfo::Status::IsBlocked);


    Tools::sort<ContactInfo*>(_friendList, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    Tools::sort<ContactInfo*>(_requestList, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    Tools::sort<ContactInfo*>(_blockedList, [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    return endPos + commandEnd.length();
}

int ServerInfoProcessor::processNecessaryContacts(const QString& str, int start)
{
    //beware , you did not intialize the friend id list
    int end = str.indexOf(commandEnd, start) + commandEnd.length();
    start = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int last = 0;
    while (start < end && start != -1 && last <= start)
    {
        int idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
        idPos = start;
        namePos = str.indexOf(contact_nameSep, idPos) + contact_nameSep.length();
        friendListPos = str.indexOf(contact_friendListSep, namePos) + contact_friendListSep.length();
        onlinePos = str.indexOf(contact_onlineSep, friendListPos) + contact_onlineSep.length();
        lastSeenPos = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();
        isBlockedPos = str.indexOf(contact_isBlockedSep, lastSeenPos) + contact_isBlockedSep.length();
        hasRequestPos = str.indexOf(contact_hasRequestSep, isBlockedPos) + contact_hasRequestSep.length();

        // + 2 because - once to go to next pos '"' , and another to go to the non '"' pos
        //because otherwise , it would return the pos of the first '"' which will make it read an empty string

        int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
        ContactInfo* test = findUser(id);
        if (test != nullptr && id != _id) {
            last = start;
            start = str.indexOf(contact_idSep, lastSeenPos) + contact_idSep.length();
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        char flags = 0;
        if (str[hasRequestPos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::HasRequest;
        if (str[isBlockedPos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::HasBlockedYou;
        if (str[onlinePos + 1] == 't')
            flags = flags | (char)ContactInfo::Status::Online;

        std::vector<int> friendIdList = Tools::extractIntsFromArr(str.mid(friendListPos + 1, str.indexOf('"', friendListPos + 1) - friendListPos - 1));
        if (std::find(friendIdList.begin(), friendIdList.end(), _id) != friendIdList.end())
            flags = flags | (char)ContactInfo::Status::Friend;

        ContactInfo* contact = new ContactInfo(this, str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1),
            ContactInfo::Status(flags),
            str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

        contact->setId(id);

        UserInfo::addUserByFlag(contact);

        last = start;
        start = str.indexOf(contact_idSep, lastSeenPos) + contact_idSep.length();

        qDebug() << "Needed id: " << id;
    }
    return end;
}

std::vector<ContactInfo*>  ServerInfoProcessor::processContactList(const QString& str , int start , int end)
{
    //beware , you did not intialize the friend id list
    std::vector<ContactInfo*> vec;
    while(start < end && start != -1)
    {
        qsizetype idPos = str.indexOf(contact_idSep , start) + contact_idSep.length();
        qsizetype namePos = str.indexOf(contact_nameSep , idPos) + contact_nameSep.length();
        qsizetype onlinePos = str.indexOf(contact_onlineSep , namePos) + contact_onlineSep.length();
        qsizetype lastSeenPos = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();

        // + 2 because - once to go to next pos '"' , and another to go to the non '"' pos
        //because otherwise , it would return the pos of the first '"' which will make it read an empty string
        ContactInfo* contact = new ContactInfo(this , str.mid(namePos + 1 , str.indexOf('"' , namePos + 1) - namePos - 1),
                                               (str[onlinePos + 1] == 't') ? ContactInfo::Status::Online : ContactInfo::Status::Null,
                                               str.mid(lastSeenPos + 1, str.indexOf('"' , lastSeenPos + 1) - lastSeenPos - 1));

        QString aux = str.mid(idPos + 1 , str.indexOf('"' , idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());

        vec.emplace_back(contact);
        start = str.indexOf(contact_idSep , lastSeenPos);
    }
    return vec;
}

int ServerInfoProcessor::processNewFriendInfo(const QString& str , int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int namePos = str.indexOf(contact_nameSep , idPos) + contact_nameSep.length();
    int onlinePos = str.indexOf(contact_onlineSep , namePos) + contact_onlineSep.length();
    int lastSeenPos = str.indexOf(contact_lastSeenSep , onlinePos) + contact_lastSeenSep.length();
    int friendListPos = str.indexOf(contact_friendListSep , lastSeenPos) + contact_friendListSep.length();

    ContactInfo* contact = new ContactInfo(this);
    contact->setId(str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt());
    contact->setName( str.mid(namePos + 1 , str.indexOf('"' , namePos + 1) - namePos - 1));
    if(str[onlinePos + 1] == 't')
        contact->setFlags(ContactInfo::Status::Online);

    contact->setLastSeen(str.mid(lastSeenPos + 1, str.indexOf('"' , lastSeenPos + 1) - lastSeenPos - 1));

    UserInfo::addFriend(contact);
    return str.indexOf(commandEnd , friendListPos) + commandEnd.length();
}

int ServerInfoProcessor::processNewAdmin(const QString& str, int start)
{
    int chatIdPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    int contactIdPos = str.indexOf(contact_idSep, start) + contact_idSep.length();

    int chatId = str.mid(chatIdPos + 1, str.indexOf('"', chatIdPos + 1) - chatIdPos - 1).toInt();
    int contactId = str.mid(contactIdPos + 1, str.indexOf('"', contactIdPos + 1) - contactIdPos - 1).toInt();

    ChatInfo* chatInfo = chatById(chatId);
    chatInfo->setNewAdmin(contactId);

    return str.indexOf(commandEnd, contactIdPos) + commandEnd.length();
}


std::vector<MessageInfo*> ServerInfoProcessor::extractHistoryFromChat(const QString& str , int start)
{
    /* example : {"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",\"2021-04-05 00:00:00\")"}
     * "\"" marks the beginning of a text while "\"" marks the end
     * However not all texts start like this , only those who contain a space
     * */
    int pos = start;
    int lastPos = -1;
    std::vector<MessageInfo*>history;
    while(lastPos < pos)
    {
        auto result = processMessageInfo(str , pos);
        lastPos = pos;
        pos = result.second;
        history.emplace_back(result.first);
    }
    return history;
}

std::pair<MessageInfo*, int> ServerInfoProcessor::processMessageInfo(const QString& str , int start)
{
    /* example of a message: "(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")"
     * */
    /* int pos = start;
    const QString paramSeparator = "\\\"";

    const int sepLength = paramSeparator.length();
    MessageInfo* m = new MessageInfo(this);

    {
        int paramBeg = str.indexOf(paramSeparator , pos);
        int paramEnd = str.indexOf(paramSeparator , paramBeg + sepLength);

        m->setAt(i , str.mid(paramBeg + sepLength , paramEnd - paramBeg - sepLength));

        pos = paramEnd + sepLength;
    }

    int paranthesesPos =  str.indexOf('(' , pos);
    int accoladeEnd = str.indexOf('}' , pos);
    if(accoladeEnd < paranthesesPos)
        return {m , -1};
    else
        return {m , str.indexOf('(' , pos)};*/

    MessageInfo* m = new MessageInfo(this);

    int nameBegin = str.indexOf(message_nameSep , start) + message_nameSep.length();
    int textBegin = str.indexOf(message_textSep , nameBegin) + message_textSep.length();
    int timestampBegin = str.indexOf(message_timestampSep , textBegin) + message_timestampSep.length();
    int paranthesesEndPos =  str.indexOf(')' , timestampBegin);

    m->setName(str.mid(nameBegin , textBegin - nameBegin - message_textSep.length()));
    m->setText(str.mid(textBegin , timestampBegin - textBegin - message_timestampSep.length()).replace("\\\"" , "\""));
    /* the character ')' marks the end of the message*/
    m->setTimestamp(str.mid(timestampBegin , paranthesesEndPos - timestampBegin - 1));

    int paranthesesBegPos =  str.indexOf('(' , timestampBegin);
    int accoladeEnd = str.indexOf('}' , timestampBegin);
    if(accoladeEnd < paranthesesBegPos)
        return {m , -1};
    else
        return {m , str.indexOf('(' , paranthesesEndPos)};
}

int ServerInfoProcessor::processFriendRemoval(const QString& str, int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    UserInfo::removeFriend(id);

    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}

int ServerInfoProcessor::processFriendStatus(const QString& str, int start)
{
    int idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int onlinePos = str.indexOf(contact_onlineSep, idPos) + contact_onlineSep.length();

    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    bool online = str[onlinePos + 1] == 't';

    ContactInfo* contact = findUser(id);
    if (online)
        contact->addFlags(ContactInfo::Status::Online);
    else
        contact->removeFlags(ContactInfo::Status::Online);

    return str.indexOf(commandEnd, onlinePos) + commandEnd.length();
}


std::vector<int> ServerInfoProcessor::extractIntsFromArr(const QString& str)
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

void ServerInfoProcessor::requestDataOfUnknownUsers()
{
    std::vector<int> unknownUsersIdList;
    std::map<int, int> uniqueIds;
    for (ChatInfo* chat : _chatList)
    {
        auto& idList = chat->members();
        for (int id : idList)
        {
            if (findUser_KnownLists(id) == nullptr && id != _id)
                uniqueIds.emplace(std::make_pair(id, id));
        }
    }

    unknownUsersIdList.reserve(uniqueIds.size());
    for (const auto& pair : uniqueIds)
        unknownUsersIdList.emplace_back(pair.first);

    if(unknownUsersIdList.size() == 0)
        emit allUserInfoReceived();
    else
        getInfoForUsers(unknownUsersIdList);
}
void ServerInfoProcessor::requestDataOfWaitingUsers()
{
    if (_awaitingContactInfoList.size() == 0) return;

    std::vector<int> list(_awaitingContactInfoList.size());
    for (int i = 0; i < list.size(); i++)
        list[i] = _awaitingContactInfoList[i].first;

    getInfoForUsers(list);
}

const QTcpSocket& ServerInfoProcessor::socket() const { return *_socket;}
std::vector<ContactInfo*>& ServerInfoProcessor::searchedForUsers() noexcept { return _searchedForUsers; }
