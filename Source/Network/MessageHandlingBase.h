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
    MediaUploadId,
    MediaChunk,
    
    Invalid
};

enum class InfoFromServer {
    SignedIn,  // will inform the user that he has succesfully signed in
    Chats, // will give the user the info of what chats he takes part in and it's messages
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
    GroupMemberAdded,
    NecessaryContacts, //this is sent before a command that needs you to have certain contact data
    NewAdmin,
    FriendStatus,
    UploadId,
    ChunkAccepted,
    FileName
};

enum class FileExtension {
    INVALID, PNG, JPG, COUNT
};

namespace Converters {
    int suffixType(const QString& suf);
}

namespace MessageSeparators {
    static const QString commandEnd = "~\\";
    static const QString commandBegin = "~";


    //for contact info
    static const QString contactSeps[] = { "\"OwnFriendList\":" , "\"RequestList\":" , "\"BlockedList\":" };

    static const QString contactPrefix = "con";

    static const QString contact_idSep = "\"" + contactPrefix + "Id\":";
    static const QString contact_nameSep = "\"" + contactPrefix + "Name\":";
    static const QString contact_friendListSep = "\"" + contactPrefix + "FriendList\":";
    static const QString contact_onlineSep = "\"" + contactPrefix + "Online\":";
    static const QString contact_lastSeenSep = "\"" + contactPrefix + "LastSeen\":";
    static const QString contact_isBlockedSep = "\"" + contactPrefix + "IsBlocked\":";
    static const QString contact_hasRequestSep = "\"" + contactPrefix + "HasRequest\":";
    //for message info
    static const QString messagePrefix = "mes";

    static const QString message_chatIdSep = "\"" + messagePrefix + "ChatId\":";
    static const QString message_nameSep = "\"" + messagePrefix + "Name\":";
    static const QString message_textSep = "\"" + messagePrefix + "Text\":";
    static const QString message_timestampSep = "\"" + messagePrefix + "Timestamp\":";
    //for chat info
    static const QString chatPrefix = "chat";

    static const QString chat_idSep = "\"" + chatPrefix + "Id\":";
    static const QString chat_nameSep = "\"" + chatPrefix + "Name\":";
    static const QString chat_historySep = "\"" + chatPrefix + "History\":";
    static const QString chat_memberListSep = "\"" + chatPrefix + "MemberList\":";
    static const QString chat_privateSep = "\"" + chatPrefix + "Private\":";
    static const QString chat_isSenderSep = "\"" + chatPrefix + "isSender\":";
    static const QString chat_adminIdSep = "\"" + chatPrefix + "Admin\":";
    static const QString chat_readOnlyListSep = "\"" + chatPrefix + "ReadOnlyList\":";
    static const QString chat_removedSep = "\"" + chatPrefix + "Removed\":";
    static const QString chat_newMembersSep = "\"" + chatPrefix + "NewMembers\":";

    static const QString upload_idSep = "\"uploadId\":";
    static const QString upload_acceptedSep = "\"uploadAccepted\":";
    static const QString upload_nameSep = "\"uploadName\":";
    static const QString upload_senderIdSep = "\"uploadsenderId\":";
    static const QString upload_receiverListSep = "\"uploadReceiverList\":";
    //for user info
    static const QString userPrefix = "user";

    static const QString user_idSep = "\"" + userPrefix + "Id\":";
}