#include "RequestSender.h"

RequestSender::RequestSender(QObject* parent, QTcpSocket& socket)
    :QObject(parent) , _socket(socket)
{
}
void RequestSender::registerIntoDatabase(const QString& name, const QString& email, const QString& password)
{
    QString str = QString::number((int)RequestToServer::Register) + '(' + name + ',' + email + ',' + password + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
    emit sentRegisterRequest();
}

void RequestSender::logIntoDatabase(const QString& name, const QString& password)
{
    QString str = QString::number((int)RequestToServer::Login) + '(' + name + ',' + password + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
    emit sentLoginRequest();
}
void RequestSender::addMessage(const MessageInfo& info, int chatId)
{
    QString str = QString::number((int)RequestToServer::NewMessage) +
        " (" + QString::number(std::move(chatId)) + " ," +
        info.name() + " ," +
        info.text().replace(',', "\\,").replace('(', "\\(").replace(')', "\\)") + " ," +
        info.timestamp().date().toString("yyyy-MM-dd") + ' ' + info.timestamp().time().toString() +
        " )";

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::searchForPeople(QString criteria)
{
    QString str = QString::number((int)RequestToServer::SearchForClient) + '(' +
        QString::number(_id) + ',' +
        criteria + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::createPrivateChatWithFriend(int friendId)
{
    QString str = QString::number((int)RequestToServer::CreatePrivateChat) + '(' + QString::number(_id) + ',' + QString::number(friendId) + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::createGroupChat(std::vector<int> list)
{
    QString str = QString::number((int)RequestToServer::CreateGroupChat) + '(' + QString::number(_id);
    for (int& id : list)
        str += ',' + QString::number(id);
    str += ')';

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::manageFriendRequest(bool accepted, int userId)
{
    // commandNumber(userId , friendRequestUserId  , accepted)
    QString str = QString::number((int)RequestToServer::ManageFriendRequest) +
        '(' + QString::number(_id) + ',' + QString::number(userId) + ',' + QString::number(accepted) + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::sendFriendRequest(int userId)
{
    // commandNumber(userId , friendRequestUserId )
    QString str = QString::number((int)RequestToServer::SendFriendRequest) + '(' + QString::number(_id) + ',' +
        QString::number(userId) + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::removeFriend(int userId)
{
    QString str = QString::number((int)RequestToServer::RemoveFriend) + '(' + QString::number(_id) + ',' +
        QString::number(userId) + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::blockUser(int id)
{
    QString str = QString::number((int)RequestToServer::BlockPerson) + '(' + QString::number(_id) + ',' +
        QString::number(id) + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::unblockUser(int id)
{
    QString str = QString::number((int)RequestToServer::UnblockUser) + '(' + QString::number(_id) + ',' +
        QString::number(id) + ')';

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::removeFromGroup(int chatId, int userId)
{
    QString str = QString::number((int)RequestToServer::RemoveFromGroup) + '(' + QString::number(chatId) + ',' +
        QString::number(userId) + ',' + "true" + ')';

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::getInfoForUsers(std::vector<int> list)
{
    //CmdNum(id1 , id2 , .... , idn , userId)
    QString str = QString::number((int)RequestToServer::GetInfoForUsers) + '(';
    for (int num : list)
        str += QString::number(num) + ',';
    str += QString::number(_id) + ')';

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::updateChatName(int chatId, QString newName)
{
    QString str = QString::number((int)RequestToServer::UpdateChatName) + " (" +
        QString::number(chatId) + " ," + newName.replace(',', "\,").replace('(', "\(").replace(')', "\)") + " )";

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::leaveFromGroup(int chatId)
{
    QString str = QString::number((int)RequestToServer::RemoveFromGroup) + '(' + QString::number(chatId) + ',' +
        QString::number(_id) + ',' + "false" + ')';

    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::addPeopleToGroup(int chatId, std::vector<int> idList)
{
    QString str = QString::number((int)RequestToServer::AddPeopleToTheChat) + '(';

    for (int id : idList)
        str += QString::number(id) + ',';

    str += QString::number(chatId) + ')';
    _socket.write(str.toUtf8());
    _socket.flush();
}
void RequestSender::sendMedia(QByteArray fileName, QByteArray media)
{
    QByteArray cmdNum = QByteArray::number((int)RequestToServer::MediaChunk);
    QByteArray cmd;
    cmd.reserve(cmdNum.size() + 3 + fileName.size() + media.size());
    cmd.append(cmdNum).append('(').append(std::move(fileName)).append(',').append(std::move(media)).append(')');

    _socket.write(cmd);
    _socket.flush();
}
void RequestSender::requestDataOfUnknownUsers(AccountInfoStorage& storage)
{
    std::vector<int> unknownUsersIdList;
    std::map<int, int> uniqueIds;
    for (ChatInfo* chat : storage.chatList())
    {
        auto& idList = chat->members();
        for (int id : idList)
        {
            if (storage.findUser_KnownLists(id) == nullptr && id != _id)
                uniqueIds.emplace(std::make_pair(id, id));
        }
    }

    unknownUsersIdList.reserve(uniqueIds.size());
    for (const auto& pair : uniqueIds)
        unknownUsersIdList.emplace_back(pair.first);

    if (unknownUsersIdList.size() == 0)
        emit allUserInfoReceived();
    else
        getInfoForUsers(unknownUsersIdList);
}
void RequestSender::requestDataOfWaitingUsers(std::vector<std::pair<int, int>>& awaitingContactInfoList)
{
    if (awaitingContactInfoList.size() == 0) return;

    std::vector<int> list(awaitingContactInfoList.size());
    for (int i = 0; i < list.size(); i++)
        list[i] = awaitingContactInfoList[i].first;

    getInfoForUsers(list);
}
void RequestSender::sendAck(){}
void RequestSender::setId(int id) { _id = id; }