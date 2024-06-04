#pragma once
#include "AccountInfoStorage.h"
#include "MediaUploader.h"
#include "MediaCache.h"
#include "RequestSender.h"
#include "MediaHandler.h"


class ResponseHandler : public QObject{
    Q_OBJECT
public:
	ResponseHandler(QObject* parent , AccountInfoStorage& storage0 , MediaUploader& uploader0 , RequestSender& sender , MediaCache& cache);
    qsizetype processCommand(const QByteArray& str, qsizetype start);
    qsizetype processSignInInfo(const QByteArray& str, qsizetype start);
    qsizetype processChatListInfo(const QByteArray& str, qsizetype start);
    std::pair<ChatInfo*, int> processChatInfo(const QByteArray& str, qsizetype start);
    std::pair<MessageInfo*, int> processMessageInfo(const QByteArray& str, qsizetype start);
    std::vector<MessageInfo*> extractHistoryFromChat(const QByteArray& str, qsizetype start);
    qsizetype processNewMessageInfo(const QByteArray& str, qsizetype start);
    qsizetype processContactInfo(const QByteArray& str, qsizetype start);
    std::vector<ContactInfo*>  processContactList(const QByteArray& str, qsizetype start, qsizetype end);
    qsizetype processNewFriendInfo(const QByteArray& str, qsizetype start);
    qsizetype processSearchedForList(const QByteArray& str, qsizetype start);
    qsizetype processNewFriendRequest(const QByteArray& str, qsizetype start);
    qsizetype processNewChat(const QByteArray& str, qsizetype start);
    qsizetype processFriendRemoval(const QByteArray& str, qsizetype start);
    qsizetype processGettingBlocked(const QByteArray& str, qsizetype start);
    qsizetype processPersonBlocked(const QByteArray& str, qsizetype start);
    qsizetype processListOfStrangers(const QByteArray& str, qsizetype start);
    qsizetype processUserUnblocked(const QByteArray& str, qsizetype start);
    qsizetype processUserUnblockedYou(const QByteArray& str, qsizetype start);
    qsizetype processNewChatName(const QByteArray& str, qsizetype start);
    qsizetype processGroupMemberRemoval(const QByteArray& str, qsizetype start);
    qsizetype processNewGroupMembers(const QByteArray& str, qsizetype start);
    qsizetype processNecessaryContacts(const QByteArray& str, qsizetype start);
    qsizetype processNewAdmin(const QByteArray& str, qsizetype start);
    qsizetype processFriendStatus(const QByteArray& str, qsizetype start);
    qsizetype processNewUploadId(const QByteArray& str, qsizetype start);
    qsizetype processChunkStatus(const QByteArray& str, qsizetype start);
    qsizetype processUploadName(const QByteArray& str, qsizetype start);
    qsizetype processChunk(const QByteArray& str, qsizetype start);
    qsizetype processFileIncomingFileInfo(const QByteArray& str, qsizetype start);
    void reset();
signals:
    //request failures
    void nameUsedError();
    void nameNotFoundError();
    void incorrectPasswordError();
    void emailUsedError();
signals:
    void signInAccepted();
    void contactInfoLoaded();
    void newChatData();
    void newFriend();
    void chatDataReceived();
    void newFriendRequest();
    void newSearchData();
    void createdNewChat(qsizetype id);
    void addedToNewChat(qsizetype id);
    void unknownListReceived();
    void allUserInfoReceived();
    void accountDataCleared();
private:
	AccountInfoStorage& storage;
    MediaUploader& uploader;
    RequestSender& sender;
    MediaCache& mediaCache;
    MediaHandler mediaHandler;

    bool userInfoReceived;
    std::vector<std::pair<int, int>> _awaitingContactInfoList;
};