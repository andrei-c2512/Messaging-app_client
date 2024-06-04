#pragma once
#include <QTcpSocket>

enum class RequestToServer{
    Register,
    Login,
    NewMessage,
    SearchForClient,
    ManageFriendRequest,
    SendFriendRequest,
    CreatePrivateChat,
    RemoveFriend,
    BlockPerson,
    GetInfoForUsers,
    UnblockUser,
    CreateGroupChat,
    UpdateChatName,
    RemoveFromGroup,
    AddPeopleToTheChat,
    UploadId,
    MediaChunk,
    Invalid
};

enum class InfoFromServer {
    SignedIn,  // will inform the user that he has succesfully signed in
    ChatInfo, // will give the user the info of what chats he takes part in and it's messages
    Contacts, // will give the user his friend list , blocked people , people who are offline/online/last seen on yyyy-mm--dd
    NewMessage,
    SearchedUserList,
    NewFriend,   // will give the info of the new friend you just made
    NewFriendRequest,
    NewChat,
    FriendRemoved,
    PersonBlocked,
    YouGotBlocked,
    InfoOfUsers,
    UserUnblocked,
    UserUnblockedYou,
    NewChatName,
    GroupMemberRemoved,
    GroupMembersAdded,
    NecessaryContacts,
    NewAdmin,
    FriendStatus,
    MediaUploadId,
    FileName,
    IncomingFile,
    FileChunk
};

enum class FileExtension {
    INVALID, PNG, JPG, COUNT
};

namespace Converters {
    int suffixType(const QString& suf);
}

namespace MessageSeparators {
    static const QByteArray commandEnd = "~\\";
    static const QByteArray commandBegin = "~";


    //for contact info
    static const QByteArray contactSeps[] = { "\"OwnFriendList\":" , "\"RequestList\":" , "\"BlockedList\":" };

    static const QByteArray contactPrefix = "con";

    static const QByteArray contact_idSep = "\"" + contactPrefix + "Id\":";
    static const QByteArray contact_nameSep = "\"" + contactPrefix + "Name\":";
    static const QByteArray contact_friendListSep = "\"" + contactPrefix + "FriendList\":";
    static const QByteArray contact_onlineSep = "\"" + contactPrefix + "Online\":";
    static const QByteArray contact_lastSeenSep = "\"" + contactPrefix + "LastSeen\":";
    static const QByteArray contact_isBlockedSep = "\"" + contactPrefix + "IsBlocked\":";
    static const QByteArray contact_hasRequestSep = "\"" + contactPrefix + "HasRequest\":";
    //for message info
    static const QByteArray messagePrefix = "mes";

    static const QByteArray message_chatIdSep = "\"" + messagePrefix + "ChatId\":";
    static const QByteArray message_nameSep = "\"" + messagePrefix + "Name\":";
    static const QByteArray message_textSep = "\"" + messagePrefix + "Text\":";
    static const QByteArray message_timestampSep = "\"" + messagePrefix + "Timestamp\":";
    //for chat info
    static const QByteArray chatPrefix = "chat";

    static const QByteArray chat_idSep = "\"" + chatPrefix + "Id\":";
    static const QByteArray chat_nameSep = "\"" + chatPrefix + "Name\":";
    static const QByteArray chat_historySep = "\"" + chatPrefix + "History\":";
    static const QByteArray chat_memberListSep = "\"" + chatPrefix + "MemberList\":";
    static const QByteArray chat_privateSep = "\"" + chatPrefix + "Private\":";
    static const QByteArray chat_isSenderSep = "\"" + chatPrefix + "isSender\":";
    static const QByteArray chat_adminIdSep = "\"" + chatPrefix + "Admin\":";
    static const QByteArray chat_readOnlyListSep = "\"" + chatPrefix + "ReadOnlyList\":";
    static const QByteArray chat_removedSep = "\"" + chatPrefix + "Removed\":";
    static const QByteArray chat_newMembersSep = "\"" + chatPrefix + "NewMembers\":";

    static const QByteArray upload_idSep = "\"uploadId\":";
    static const QByteArray upload_acceptedSep = "\"uploadAccepted\":";
    static const QByteArray upload_nameSep = "\"uploadName\":";
    static const QByteArray upload_senderIdSep = "\"uploadsenderId\":";
    static const QByteArray upload_receiverListSep = "\"uploadReceiverList\":";
    static const QByteArray upload_blobSep = "\"uploadBlob\":";
    static const QByteArray upload_extensionSep = "\"uploadExtension\":";
    static const QByteArray upload_fileSizeSep = "\"uploadSize\":";
    
    //for user info
    static const QByteArray userPrefix = "user";

    static const QByteArray user_idSep = "\"" + userPrefix + "Id\":";
}