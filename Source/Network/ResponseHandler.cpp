#include "ResponseHandler.h"
#include "MessageHandlingBase.h"
#include "TextParser.h"
using namespace MessageSeparators;
using namespace TextParser;

ResponseHandler::ResponseHandler(QObject* parent, AccountInfoStorage& storage0, MediaUploader& uploader0, RequestSender& sender0, MediaCache& cache0)
    : QObject(parent), storage(storage0) , uploader(uploader0) , sender(sender0), mediaCache(cache0)
{}

qsizetype ResponseHandler::processCommand(const QByteArray& message, qsizetype start)
{
    qsizetype end = start;
    try {
        qsizetype requestCodeBegin = message.indexOf(commandBegin, start) + commandBegin.size();
        qsizetype requestCodeEnd = message.indexOf(':', start);

        QByteArray code = message.mid(requestCodeBegin, requestCodeEnd - requestCodeBegin);
        qDebug() << "Request code: " << code;
        qsizetype codeNumber;
        //the index of the end of the current command
        if (code[0] == 'F')
        {
            /*
         *  NameUsed 0
            NameNotFound 1
            IncorrectPassword 2
            EmailUsed 3
        */
            code.erase(std::begin(code));
            codeNumber = code.toInt();
            switch (codeNumber)
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
            end = message.indexOf(commandEnd, requestCodeEnd) + commandEnd.length();
        }
        else if (code[0] == 'I')
        {
            /*
         * SignedIn = 0
         * ChatInfo = 1, // will give the user the info of what chats he takes part in and it's messages
         * Contacts = 2// will give the user his friend list , blocked people , people who are offline/online/last seen on yyyy-mm--dd
         * */
            code.erase(std::begin(code));
            codeNumber = code.toInt();
            qDebug() << "Code number: " << codeNumber;
            switch ((InfoFromServer)codeNumber)
            {
            case InfoFromServer::SignedIn:
                end = processSignInInfo(message, start);
                mediaCache.setDirName(storage.name());
                emit signInAccepted();
                break;
            case InfoFromServer::ChatInfo:
                end = processChatListInfo(message, start);
                emit chatDataReceived();
                break;
            case InfoFromServer::Contacts:
                end = processContactInfo(message, start);
                sender.requestDataOfUnknownUsers(storage);
                emit contactInfoLoaded();
                break;
            case InfoFromServer::NewMessage:
                end = processNewMessageInfo(message, start);
                break;
            case InfoFromServer::SearchedUserList:
                end = processSearchedForList(message, start);
                emit newSearchData();
                break;
            case InfoFromServer::NewFriend:
                end = processNewFriendInfo(message, start);
                emit newFriend();
                break;
            case InfoFromServer::NewFriendRequest:
                end = processNewFriendRequest(message, start);
                emit newFriendRequest();
                break;
            case InfoFromServer::NewChat:
                end = processNewChat(message, start);
                sender.requestDataOfWaitingUsers(_awaitingContactInfoList);
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
                storage.transformChats();
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
            case InfoFromServer::GroupMembersAdded:
                end = processNewGroupMembers(message, start);
                sender.requestDataOfWaitingUsers(_awaitingContactInfoList);
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
            case InfoFromServer::MediaUploadId:
                end = processNewUploadId(message, start);
                break;
            //case InfoFromServer::UploadChunkAccepted:
            //    end = processChunkStatus(message, start);
            //    break;
            case InfoFromServer::FileName:
                end = processUploadName(message, start);
                break;
            case InfoFromServer::IncomingFile:
                end = processFileIncomingFileInfo(message, start);
                break;
            case InfoFromServer::FileChunk:
                end = processChunk(message, start);
                break;
            default:
                qDebug() << "Received unkown message";
                break;
            }
        }
        //acknowledgement
        else if (code[0] == 'A')
        {
            uploader.nextChunk();
        }
        else
        {
            QByteArray error_message = QByteArray("Did not encounter a known command type -> ") + code[0];
            throw std::logic_error(error_message.toStdString());
        }
    }
    catch (std::logic_error& e) {
        qDebug() << e.what();
    }
    return end;
}

qsizetype ResponseHandler::processChunk(const QByteArray& str, qsizetype start)
{
    qsizetype chunkPos = dataPos(str, upload_blobSep , start);
    qsizetype end = str.length() - commandEnd.length() - 1;

    QByteArray chunk = str.sliced(chunkPos , end - chunkPos);

    mediaHandler.addBlob(std::move(chunk));

    return str.indexOf(commandEnd, end) + commandEnd.length();
}
qsizetype ResponseHandler::processFileIncomingFileInfo(const QByteArray& str, qsizetype start)
{
    qsizetype namePos = dataPos(str, upload_nameSep, start);
    qsizetype extensionPos = dataPos(str, upload_extensionSep, namePos);
    qsizetype fileSizePos = dataPos(str, upload_fileSizeSep, extensionPos);

    QByteArray name = TextParser::extractData(str, namePos);
    qsizetype extension = extractData(str, extensionPos).toInt();
    qsizetype fileSize = extractData(str, fileSizePos).toULongLong();

    mediaHandler.prepareNewMedia(name, extension, fileSize);

    return str.indexOf(commandEnd, fileSizePos) + commandEnd.length();
}

qsizetype ResponseHandler::processChunkStatus(const QByteArray& str, qsizetype start)
{
    qsizetype statusPos = str.indexOf(upload_acceptedSep, start) + upload_acceptedSep.length();
    bool accepted = str[statusPos + 1] == 't';

    if (accepted)
        uploader.nextChunk();

    return str.indexOf(commandEnd, statusPos) + commandEnd.length();
}
qsizetype ResponseHandler::processNewGroupMembers(const QByteArray& str, qsizetype start)
{
    qsizetype chatIdPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    qsizetype idListPos = str.indexOf(chat_newMembersSep, chatIdPos) + chat_newMembersSep.length();

    qsizetype chatId = str.mid(chatIdPos + 1, str.indexOf('"', chatIdPos + 1) - chatIdPos - 1).toInt();
    std::vector<int> list = Tools::extractIntsFromArr(str.mid(idListPos + 1, str.indexOf('"', idListPos + 1) - idListPos - 1));

    ChatInfo* chatInfo = storage.chatById(chatId);
    for (int id : list)
    {
        ContactInfo* contactInfo = storage.findUser(id);
        if (contactInfo == nullptr)
            _awaitingContactInfoList.emplace_back(std::make_pair(id, chatId));
        else
            chatInfo->addMember(id);
    }

    return str.indexOf(commandEnd, idListPos) + commandEnd.length();
}
qsizetype ResponseHandler::processNewUploadId(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(upload_idSep, start) + upload_idSep.length();
    QByteArray id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1);

    uploader.nextMedia(id);
    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}
qsizetype ResponseHandler::processNewChatName(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    qsizetype namePos = str.indexOf(chat_nameSep, idPos) + chat_nameSep.length();

    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    QByteArray newName = str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1);

    ChatInfo* chat = storage.chatById(id);
    //new name emits a signal so that it announces the change , the other method does not
    chat->setNewName(std::move(newName));

    return str.indexOf(commandEnd, namePos) + commandEnd.length();
}
qsizetype ResponseHandler::processUserUnblocked(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = storage.findUser(id);
    if (info)
    {
        storage.moveUserToStrangerList(info);
        info->removeFlags(ContactInfo::Status::IsBlocked);

        emit info->removed(info->id());
        emit info->gotBlocked(false);
    }

    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}
qsizetype ResponseHandler::processUserUnblockedYou(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = storage.findUser(id);
    if (info)
    {
        storage.moveUserToStrangerList(info);
        info->removeFlags(ContactInfo::Status::HasBlockedYou);

        emit info->removed(info->id());
        emit info->blockedYou(false);
    }

    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}
qsizetype ResponseHandler::processGroupMemberRemoval(const QByteArray& str, qsizetype start)
{
    qsizetype chatIdPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    qsizetype contactIdPos = str.indexOf(contact_idSep, chatIdPos) + contact_idSep.length();
    qsizetype chatRemovalStatusPos = str.indexOf(chat_removedSep, contactIdPos) + chat_removedSep.length();

    qsizetype chatId = str.mid(chatIdPos + 1, str.indexOf('"', chatIdPos + 1) - chatIdPos - 1).toInt();
    qsizetype contactId = str.mid(contactIdPos + 1, str.indexOf('"', contactIdPos + 1) - contactIdPos - 1).toInt();
    bool removed = str[chatRemovalStatusPos + 1] == 't';

    ChatInfo* info = storage.chatById(chatId);
    info->removeMember(contactId, storage.id(), removed);

    return str.indexOf(commandEnd, contactId) + commandEnd.length();
}
qsizetype ResponseHandler::processListOfStrangers(const QByteArray& str, qsizetype start)
{
    std::function<void(ContactInfo*)> verify;
    if (_awaitingContactInfoList.size())
    {
        verify = [=](ContactInfo* info) {
            for (qsizetype i = 0; i < _awaitingContactInfoList.size(); i++)
            {
                if (info->id() == _awaitingContactInfoList[i].first)
                {
                    ChatInfo* chat = storage.chatById(_awaitingContactInfoList[i].second);
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
    qsizetype idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
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

        QByteArray aux = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());

        vec.emplace_back(contact);
        start = str.indexOf(contact_idSep, lastSeenPos);
        storage.addToStrangerList(contact);

        verify(contact);
    }

    //I don't use a custom add function because the order of the received contact info is the desired type

    for (ContactInfo* info : storage.strangerList())
    {
        qDebug() << "Name: " << info->name();
    }
    return str.indexOf(commandEnd, hasRequestPos);
}
qsizetype ResponseHandler::processPersonBlocked(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = storage.findUser(id);
    storage.moveUserToBlocked(info);

    emit info->gotBlocked(true);

    return str.indexOf(commandEnd, start) + commandEnd.length();
}
qsizetype ResponseHandler::processGettingBlocked(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    ContactInfo* info = storage.findUser(id);
    if (info)
    {
        storage.moveUserToStrangerList(info);
        info->addFlags(ContactInfo::Status::HasBlockedYou);

        emit info->blockedYou(true);
        emit info->removed(info->id());
    }

    return str.indexOf(commandEnd, start) + commandEnd.length();
}
qsizetype ResponseHandler::processNewChat(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    qsizetype namePos = str.indexOf(chat_nameSep, idPos) + chat_nameSep.length();
    qsizetype historyPos = str.indexOf(chat_historySep, namePos) + chat_historySep.length();
    qsizetype memberlistPos = str.indexOf(chat_memberListSep, historyPos) + chat_memberListSep.length();
    qsizetype privatePos = str.indexOf(chat_privateSep, memberlistPos) + chat_privateSep.length();
    qsizetype adminPos = str.indexOf(chat_adminIdSep, privatePos) + chat_adminIdSep.length();
    qsizetype readOnlyListPos = str.indexOf(chat_readOnlyListSep, adminPos) + chat_readOnlyListSep.length();
    qsizetype isSenderPos = str.indexOf(chat_isSenderSep, privatePos) + chat_isSenderSep.length();

    ChatInfo* info = new ChatInfo(this);
    qsizetype chatId = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    info->setId(chatId);
    info->setName(str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1));
    info->setMessageHistory(extractHistoryFromChat(str, historyPos));
    info->setMembers(str.mid(memberlistPos + 1, str.indexOf('"', memberlistPos + 1) - memberlistPos - 1));
    info->setType(str[privatePos + 1] == 't');
    info->setAdminId(str.mid(adminPos + 1, str.indexOf('"', adminPos + 1) - adminPos - 1).toInt());

    auto list = Tools::extractIntsFromArr(str, readOnlyListPos);
    for (int id : list)
    {
        ContactInfo* contact = storage.findUser(id);
        if (contact == nullptr)
        {
            _awaitingContactInfoList.emplace_back(std::make_pair(id, chatId));
        }
    }
    info->setReadOnlyMembers(list);

    bool isSender = str[isSenderPos + 1] == 't';

    storage.addChat(info);
    storage.adaptChat(info);

    connect(info, &ChatInfo::newMessageInQueue, &sender, &RequestSender::addMessage);

    if (_awaitingContactInfoList.empty() == false)
    {
        if (isSender)
            emit createdNewChat(info->id());
        else
            emit addedToNewChat(info->id());
    }

    return str.indexOf(commandEnd, isSenderPos) + commandEnd.length();
}
std::pair<ChatInfo*, int> ResponseHandler::processChatInfo(const QByteArray& str, qsizetype start)
{
    /* example of chat info data
     * ~I1:"Id":"2""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",\"2021-04-05 00:00:00\")"}
     * "MemberList":"{1,2,5}""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",\"2021-04-05 00:00:00\"
     * )"}"MemberList":"{1,2,5}""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",
     * \"2021-04-05 00:00:00\")"}"MemberList":"{1,2,5}""Name":"Group chat3""History":{"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")",
     * "(\"Leo Gabriel\",\"Ce te pisi atata\",\"2021-04-05 00:00:00\")"}"MemberList":"{1,2,5}"\~
     * */

    qsizetype pos = start;

    ChatInfo* chatInfo = new ChatInfo(this);
    //while(lastPos < pos)
    {
        qsizetype idIndex = str.indexOf(chat_idSep, pos) + chat_idSep.length();
        qsizetype nameIndex = str.indexOf(chat_nameSep, idIndex) + chat_nameSep.length();
        qsizetype historyIndex = str.indexOf(chat_historySep, nameIndex) + chat_historySep.length();
        qsizetype isPrivateIndex = str.indexOf(chat_privateSep, historyIndex) + chat_privateSep.length();
        qsizetype memberListIndex = str.indexOf(chat_memberListSep, isPrivateIndex) + chat_memberListSep.length();
        qsizetype adminIndex = str.indexOf(chat_adminIdSep, memberListIndex) + chat_adminIdSep.length();
        qsizetype readOnlyListIndex = str.indexOf(chat_readOnlyListSep, adminIndex) + chat_readOnlyListSep.length();

        //added 2 because ":"" (of length 2) will always follow after nameSep
        QByteArray auW = str.mid(idIndex + 1, str.indexOf('"', idIndex + 1) - idIndex - 1);
        chatInfo->setId(str.mid(idIndex + 1, str.indexOf('"', idIndex + 1) - idIndex - 1).toInt());
        chatInfo->setName(str.mid(nameIndex + 1, str.indexOf('"', nameIndex + 1) - nameIndex - 1));
        chatInfo->setMessageHistory(extractHistoryFromChat(str, historyIndex));
        chatInfo->setType(str[isPrivateIndex + 1] == 't');

        chatInfo->setMembers(Tools::extractIntsFromArr(str, memberListIndex));

        chatInfo->setAdminId(str.mid(adminIndex + 1, str.indexOf('"', adminIndex + 1) - adminIndex - 1).toInt());
        chatInfo->setReadOnlyMembers(Tools::extractIntsFromArr(str, readOnlyListIndex));

        pos = memberListIndex;
    }

    qDebug() << connect(chatInfo, &ChatInfo::newMessageInQueue, &sender, &RequestSender::addMessage);;

    return { chatInfo , pos };
}
qsizetype ResponseHandler::processNewFriendRequest(const QByteArray& str, qsizetype start)
{
    qsizetype idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
    start = str.indexOf(contact_idSep, start);

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
        ContactInfo* contact = new ContactInfo(this, str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1),
            ContactInfo::Status(int(str[onlinePos + 1] == 't')),
            str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

        QByteArray aux = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());

        ContactInfo::ContactStatus flags = ContactInfo::Status::Null;
        if (str[hasRequestPos + 1] == 't')
            flags = flags | ContactInfo::Status::HasRequest;
        if (str[isBlockedPos + 1] == 't')
            flags = flags | ContactInfo::Status::HasBlockedYou;

        contact->setFlags(flags);

        storage.addToRequestList(contact);
        start = str.indexOf(contact_idSep, lastSeenPos);
    }
    return str.indexOf(commandEnd, hasRequestPos) + commandEnd.length();
}
qsizetype ResponseHandler::processSignInInfo(const QByteArray& str, qsizetype start)
{
    qsizetype userIdStart = str.indexOf(user_idSep, start) + user_idSep.length() + 1;
    QByteArray userIdStr = str.mid(userIdStart, str.indexOf('"', userIdStart + 1) - userIdStart);
    storage.setId(userIdStr.toInt());
    return str.indexOf(commandEnd, start) + commandEnd.length();
}
qsizetype ResponseHandler::processSearchedForList(const QByteArray& str, qsizetype start)
{
    std::vector<ContactInfo*> vec;
    qsizetype idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
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
            (ContactInfo::Status)flags,
            str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

        QByteArray aux = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());



        vec.emplace_back(contact);
        start = str.indexOf(contact_idSep, lastSeenPos);
    }

    for (ContactInfo* info : storage.searchedForList())
        info->deleteLater();

    //I don't use a custom add function because the order of the received contact info is the desired type
    storage.setSearchedForList(vec);
    for (ContactInfo* info : storage.searchedForList())
    {
        qDebug() << "Name: " << info->name();
    }
    return str.indexOf(commandEnd, hasRequestPos);
}
qsizetype ResponseHandler::processNewMessageInfo(const QByteArray& str, qsizetype start)
{
    // how the data looks like
    /*   (commandBegin + 'I' + QByteArray::number((int)InfoToClient::NewMessage) + ':' +
                "\"Name\":" + "\"" + sender + "\"" + "\"Text\":" + "\"" + text + "\"" + "\"Timestamp\":" + "\"" + timestamp + "\"" + commandEnd);*/

                // + 2 because of the characters ':' and '"'
    qsizetype chatIdBegin = str.indexOf(message_chatIdSep, start) + message_chatIdSep.length();

    ChatInfo& info = storage.getChatById(/* extracting the chatId from the string and converting it to Integer*/
        str.mid(chatIdBegin + 1, str.indexOf('"', chatIdBegin + 1) - (chatIdBegin + 1)).toInt()
    );

    if (info.approveLastInQueue() == false)
    {
        qDebug() << "Message sent";
        qsizetype nameBegin = str.indexOf(message_nameSep, chatIdBegin) + message_nameSep.length();
        qsizetype textBegin = str.indexOf(message_textSep, nameBegin) + message_textSep.length();
        qsizetype timestampBegin = str.indexOf(message_timestampSep, textBegin) + message_timestampSep.length();

        info.addMessageFromDatabase(str.mid(nameBegin + 1, str.indexOf('"', nameBegin + 1) - nameBegin - 1), /*extracting name*/
            str.mid(textBegin + 1, str.indexOf('"', textBegin + 1) - textBegin - 1), /*extracting text*/
            str.mid(timestampBegin + 1, str.indexOf('"', timestampBegin + 1) - timestampBegin - 1) /*extracting timestamp*/
        );
        return str.indexOf(commandEnd, timestampBegin) + commandEnd.length();
    }
    else
    {
        qDebug() << "New message";
        return str.indexOf(commandEnd, chatIdBegin) + commandEnd.length();
    }
}
qsizetype ResponseHandler::processChatListInfo(const QByteArray& str, qsizetype start)
{
    qsizetype lastPos = -1;
    qsizetype pos = start;


    while (lastPos < pos)
    {
        qsizetype chatListStart = str.indexOf(chat_idSep, pos);
        if (chatListStart == -1)
            break;
        else
        {
            auto result = processChatInfo(str, chatListStart);
            lastPos = pos;
            pos = result.second;
            storage.addChat(result.first);
        }
    }
    storage.showChatInDebug();
    emit newChatData();
    return str.indexOf(commandEnd, pos) + commandEnd.length();
}
qsizetype ResponseHandler::processContactInfo(const QByteArray& str, qsizetype start)
{
    qsizetype ownFriendListPos = str.indexOf(contactSeps[0], start) + contactSeps[0].length();
    qsizetype requestListPos = str.indexOf(contactSeps[1], ownFriendListPos) + contactSeps[1].length();
    qsizetype blockedListPos = str.indexOf(contactSeps[2], requestListPos) + contactSeps[2].length();
    qsizetype endPos = str.indexOf(commandEnd, blockedListPos);

    storage.setFriendList(processContactList(str, ownFriendListPos, requestListPos - contactSeps[1].length()));
    for (ContactInfo* fr : storage.friendList())
        fr->addFlags(ContactInfo::Status::Friend);

    storage.setRequestList(processContactList(str, requestListPos, blockedListPos - contactSeps[2].length()));
    for (ContactInfo* request : storage.requestList())
        request->addFlags(ContactInfo::Status::HasRequest);

    storage.setBlockedList(processContactList(str, blockedListPos, endPos));
    for (ContactInfo* enemy : storage.blockedList())
        enemy->addFlags(ContactInfo::Status::IsBlocked);


    Tools::sort<ContactInfo*>(storage.friendList(), [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    Tools::sort<ContactInfo*>(storage.requestList(), [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    Tools::sort<ContactInfo*>(storage.blockedList(), [](ContactInfo* lhs, ContactInfo* rhs) {
        return *lhs < *rhs;
        });
    return endPos + commandEnd.length();
}
qsizetype ResponseHandler::processNecessaryContacts(const QByteArray& str, qsizetype start)
{
    //beware , you did not intialize the friend id list
    qsizetype end = str.indexOf(commandEnd, start) + commandEnd.length();
    start = str.indexOf(contact_idSep, start) + contact_idSep.length();
    qsizetype last = 0;
    while (start < end && start != -1 && last <= start)
    {
        qsizetype idPos, namePos, friendListPos, onlinePos, lastSeenPos, isBlockedPos, hasRequestPos;
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
        ContactInfo* test = storage.findUser(id);
        if (test != nullptr && id != storage.id()) {
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
        if (std::find(friendIdList.begin(), friendIdList.end(), storage.id()) != friendIdList.end())
            flags = flags | (char)ContactInfo::Status::Friend;

        ContactInfo* contact = new ContactInfo(this, str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1),
            ContactInfo::Status(flags),
            str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

        contact->setId(id);

        storage.addUserByFlag(contact);

        last = start;
        start = str.indexOf(contact_idSep, lastSeenPos) + contact_idSep.length();

        qDebug() << "Needed id: " << id;
    }
    return end;
}
std::vector<ContactInfo*>  ResponseHandler::processContactList(const QByteArray& str, qsizetype start, qsizetype end)
{
    //beware , you did not intialize the friend id list
    std::vector<ContactInfo*> vec;
    while (start < end && start != -1)
    {
        qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
        qsizetype namePos = str.indexOf(contact_nameSep, idPos) + contact_nameSep.length();
        qsizetype onlinePos = str.indexOf(contact_onlineSep, namePos) + contact_onlineSep.length();
        qsizetype lastSeenPos = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();

        // + 2 because - once to go to next pos '"' , and another to go to the non '"' pos
        //because otherwise , it would return the pos of the first '"' which will make it read an empty string
        ContactInfo* contact = new ContactInfo(this, str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1),
            (str[onlinePos + 1] == 't') ? ContactInfo::Status::Online : ContactInfo::Status::Null,
            str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

        QByteArray aux = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1);
        contact->setId(aux.toInt());

        vec.emplace_back(contact);
        start = str.indexOf(contact_idSep, lastSeenPos);
    }
    return vec;
}
qsizetype ResponseHandler::processNewFriendInfo(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    qsizetype namePos = str.indexOf(contact_nameSep, idPos) + contact_nameSep.length();
    qsizetype onlinePos = str.indexOf(contact_onlineSep, namePos) + contact_onlineSep.length();
    qsizetype lastSeenPos = str.indexOf(contact_lastSeenSep, onlinePos) + contact_lastSeenSep.length();
    qsizetype friendListPos = str.indexOf(contact_friendListSep, lastSeenPos) + contact_friendListSep.length();

    ContactInfo* contact = new ContactInfo(this);
    contact->setId(str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt());
    contact->setName(str.mid(namePos + 1, str.indexOf('"', namePos + 1) - namePos - 1));
    if (str[onlinePos + 1] == 't')
        contact->setFlags(ContactInfo::Status::Online);

    contact->setLastSeen(str.mid(lastSeenPos + 1, str.indexOf('"', lastSeenPos + 1) - lastSeenPos - 1));

    storage.addFriend(contact);
    return str.indexOf(commandEnd, friendListPos) + commandEnd.length();
}
qsizetype ResponseHandler::processNewAdmin(const QByteArray& str, qsizetype start)
{
    qsizetype chatIdPos = str.indexOf(chat_idSep, start) + chat_idSep.length();
    qsizetype contactIdPos = str.indexOf(contact_idSep, start) + contact_idSep.length();

    qsizetype chatId = str.mid(chatIdPos + 1, str.indexOf('"', chatIdPos + 1) - chatIdPos - 1).toInt();
    qsizetype contactId = str.mid(contactIdPos + 1, str.indexOf('"', contactIdPos + 1) - contactIdPos - 1).toInt();

    ChatInfo* chatInfo = storage.chatById(chatId);
    chatInfo->setNewAdmin(contactId);

    return str.indexOf(commandEnd, contactIdPos) + commandEnd.length();
}
std::vector<MessageInfo*> ResponseHandler::extractHistoryFromChat(const QByteArray& str, qsizetype start)
{
    /* example : {"(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")","(Leo,\"Ce te pisi atata\",\"2021-04-05 00:00:00\")"}
     * "\"" marks the beginning of a text while "\"" marks the end
     * However not all texts start like this , only those who contain a space
     * */
    qsizetype pos = start;
    qsizetype lastPos = -1;
    std::vector<MessageInfo*>history;
    while (lastPos < pos)
    {
        auto result = processMessageInfo(str, pos);
        lastPos = pos;
        pos = result.second;
        history.emplace_back(result.first);
    }
    return history;
}
std::pair<MessageInfo*, int> ResponseHandler::processMessageInfo(const QByteArray& str, qsizetype start)
{
    /* example of a message: "(\"Andrei alexander\",\"Ha ba la baie\",\"2021-03-05 11:34:05\")"
     * */
     /* qsizetype pos = start;
     const QByteArray paramSeparator = "\\\"";

     const qsizetype sepLength = paramSeparator.length();
     MessageInfo* m = new MessageInfo(this);

     {
         qsizetype paramBeg = str.indexOf(paramSeparator , pos);
         qsizetype paramEnd = str.indexOf(paramSeparator , paramBeg + sepLength);

         m->setAt(i , str.mid(paramBeg + sepLength , paramEnd - paramBeg - sepLength));

         pos = paramEnd + sepLength;
     }

     qsizetype paranthesesPos =  str.indexOf('(' , pos);
     qsizetype accoladeEnd = str.indexOf('}' , pos);
     if(accoladeEnd < paranthesesPos)
         return {m , -1};
     else
         return {m , str.indexOf('(' , pos)};*/

    MessageInfo* m = new MessageInfo(this);

    qsizetype nameBegin = dataPos(str, message_nameSep, start);
    qsizetype textBegin = dataPos(str, message_textSep, nameBegin);
    qsizetype timestampBegin = dataPos(str, message_timestampSep, textBegin);
    qsizetype paranthesesEndPos = str.indexOf(')', timestampBegin);

    m->setName(str.mid(nameBegin, textBegin - nameBegin - message_textSep.length()));
    m->setText(str.mid(textBegin, timestampBegin - textBegin - message_timestampSep.length()).replace("\\\"", "\""));
    /* the character ')' marks the end of the message*/
    m->setTimestamp(str.mid(timestampBegin, paranthesesEndPos - timestampBegin - 1));

    qsizetype paranthesesBegPos = str.indexOf('(', timestampBegin);
    qsizetype accoladeEnd = str.indexOf('}', timestampBegin);
    if (accoladeEnd < paranthesesBegPos)
        return { m , -1 };
    else
        return { m , str.indexOf('(' , paranthesesEndPos) };
}
qsizetype ResponseHandler::processFriendRemoval(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();

    storage.removeFriend(id);

    return str.indexOf(commandEnd, idPos) + commandEnd.length();
}
qsizetype ResponseHandler::processFriendStatus(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(contact_idSep, start) + contact_idSep.length();
    qsizetype onlinePos = str.indexOf(contact_onlineSep, idPos) + contact_onlineSep.length();

    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    bool online = str[onlinePos + 1] == 't';

    ContactInfo* contact = storage.findUser(id);
    if (online)
        contact->addFlags(ContactInfo::Status::Online);
    else
        contact->removeFlags(ContactInfo::Status::Online);

    return str.indexOf(commandEnd, onlinePos) + commandEnd.length();
}

qsizetype ResponseHandler::processUploadName(const QByteArray& str, qsizetype start)
{
    qsizetype idPos = str.indexOf(upload_idSep, start) + upload_idSep.length();
    qsizetype uploadNamePos = str.indexOf(upload_nameSep, idPos) + upload_nameSep.length();

    int id = str.mid(idPos + 1, str.indexOf('"', idPos + 1) - idPos - 1).toInt();
    QByteArray name = str.mid(uploadNamePos + 1, str.indexOf('"', uploadNamePos + 1) - uploadNamePos - 1);

    UploadData media = uploader.readyMedia(id);
    ChatInfo* info = storage.chatById(media.chatId);

    if (info != nullptr)
    {
        mediaCache.addImage(media.url, name);
        info->mediaMessageUploaded(name);
    }
    return str.indexOf(commandEnd, uploadNamePos) + commandEnd.length();
}

void ResponseHandler::reset() { userInfoReceived = false; }