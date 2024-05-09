#include "MessageHandlingBase.h"

int Converters::suffixType(const QString& suf)
{
    if (suf == "jpg")
        return (int)FileExtension::JPG;
    if (suf == "png")
        return (int)FileExtension::PNG;

    return (int)FileExtension::INVALID;
}

//void MessageSender::registerIntoDatabase()
//{
//    QString str = QString::number((int)RequestToServer::Register) + '(' + _name + ',' + _email + ',' + _password + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//    emit sentRegisterRequest();
//}
//
//void MessageSender::logIntoDatabase()
//{
//    QString str = QString::number((int)RequestToServer::Login) + '(' + _name + ',' + _password + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//    emit sentLoginRequest();
//}
//void MessageSender::addMessage(const MessageInfo& info, int chatId)
//{
//    QString str = QString::number((int)RequestToServer::NewMessage) +
//        " (" + QString::number(std::move(chatId)) + " ," +
//        info.name() + " ," +
//        info.text().replace(',', "\\,").replace('(', "\\(").replace(')', "\\)") + " ," +
//        info.timestamp().date().toString("yyyy-MM-dd") + ' ' + info.timestamp().time().toString() +
//        " )";
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//void MessageSender::searchForPeople(QString criteria)
//{
//    QString str = QString::number((int)RequestToServer::SearchForClient) + '(' +
//        QString::number(_id) + ',' +
//        criteria + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//void MessageSender::createPrivateChatWithFriend(int friendId)
//{
//    QString str = QString::number((int)RequestToServer::CreatePrivateChat) + '(' + QString::number(_id) + ',' + QString::number(friendId) + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::createGroupChat(std::vector<int> list)
//{
//    QString str = QString::number((int)RequestToServer::CreateGroupChat) + '(' + QString::number(_id);
//    for (int& id : list)
//        str += ',' + QString::number(id);
//    str += ')';
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//void MessageSender::manageFriendRequest(bool accepted, int userId)
//{
//    // commandNumber(userId , friendRequestUserId  , accepted)
//    QString str = QString::number((int)RequestToServer::ManageFriendRequest) +
//        '(' + QString::number(_id) + ',' + QString::number(userId) + ',' + QString::number(accepted) + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//void MessageSender::sendFriendRequest(int userId)
//{
//    // commandNumber(userId , friendRequestUserId )
//    QString str = QString::number((int)RequestToServer::SendFriendRequest) + '(' + QString::number(_id) + ',' +
//        QString::number(userId) + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//void MessageSender::removeFriend(int userId)
//{
//    QString str = QString::number((int)RequestToServer::RemoveFriend) + '(' + QString::number(_id) + ',' +
//        QString::number(userId) + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//void MessageSender::blockUser(int id)
//{
//    QString str = QString::number((int)RequestToServer::BlockPerson) + '(' + QString::number(_id) + ',' +
//        QString::number(id) + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::unblockUser(int id)
//{
//    QString str = QString::number((int)RequestToServer::UnblockUser) + '(' + QString::number(_id) + ',' +
//        QString::number(id) + ')';
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::removeFromGroup(int chatId, int userId)
//{
//    QString str = QString::number((int)RequestToServer::RemoveFromGroup) + '(' + QString::number(chatId) + ',' +
//        QString::number(userId) + ',' + "true" + ')';
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::getInfoForUsers(std::vector<int> list)
//{
//    //CmdNum(id1 , id2 , .... , idn , userId)
//    QString str = QString::number((int)RequestToServer::GetInfoForUsers) + '(';
//    for (int num : list)
//        str += QString::number(num) + ',';
//    str += QString::number(_id) + ')';
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::updateChatName(int chatId, QString newName)
//{
//    QString str = QString::number((int)RequestToServer::UpdateChatName) + " (" +
//        QString::number(chatId) + " ," + newName.replace(',', "\,").replace('(', "\(").replace(')', "\)") + " )";
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::leaveFromGroup(int chatId)
//{
//    QString str = QString::number((int)RequestToServer::RemoveFromGroup) + '(' + QString::number(chatId) + ',' +
//        QString::number(_id) + ',' + "false" + ')';
//
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::addPeopleToGroup(int chatId, std::vector<int> idList)
//{
//    QString str = QString::number((int)RequestToServer::AddPeopleToTheChat) + '(';
//
//    for (int id : idList)
//        str += QString::number(id) + ',';
//
//    str += QString::number(chatId) + ')';
//    _socket->write(str.toUtf8());
//    _socket->flush();
//}
//
//void MessageSender::sendMedia(QByteArray fileName, QByteArray media)
//{
//    QByteArray cmdNum = QByteArray::number((int)RequestToServer::MediaChunk);
//    QByteArray cmd;
//    cmd.reserve(cmdNum.size() + 3 + fileName.size() + media.size());
//    cmd.append(cmdNum).append('(').append(std::move(fileName)).append(',').append(std::move(media)).append(')');
//
//    _socket->write(cmd);
//    _socket->flush();
//}
