#pragma once
#include "AccountInfoStorage.h"
#include "MediaUploader.h"
#include "MediaCache.h"
#include "RequestSender.h"

class ResponseHandler : public QObject{
    Q_OBJECT
public:
	ResponseHandler(QObject* parent , AccountInfoStorage& storage0 , MediaUploader& uploader0 , RequestSender& sender , MediaCache& cache);
    int processCommand(const QString& str, int start);
    int processSignInInfo(const QString& str, int start);
    int processChatListInfo(const QString& str, int start);
    std::pair<ChatInfo*, int> processChatInfo(const QString& str, int start);
    std::pair<MessageInfo*, int> processMessageInfo(const QString& str, int start);
    std::vector<MessageInfo*> extractHistoryFromChat(const QString& str, int start);
    int processNewMessageInfo(const QString& str, int start);
    int processContactInfo(const QString& str, int start);
    std::vector<ContactInfo*>  processContactList(const QString& str, int start, int end);
    int processNewFriendInfo(const QString& str, int start);
    int processSearchedForList(const QString& str, int start);
    int processNewFriendRequest(const QString& str, int start);
    int processNewChat(const QString& str, int start);
    int processFriendRemoval(const QString& str, int start);
    int processGettingBlocked(const QString& str, int start);
    int processPersonBlocked(const QString& str, int start);
    int processListOfStrangers(const QString& str, int start);
    int processUserUnblocked(const QString& str, int start);
    int processUserUnblockedYou(const QString& str, int start);
    int processNewChatName(const QString& str, int start);
    int processGroupMemberRemoval(const QString& str, int start);
    int processNewGroupMembers(const QString& str, int start);
    int processNecessaryContacts(const QString& str, int start);
    int processNewAdmin(const QString& str, int start);
    int processFriendStatus(const QString& str, int start);
    int processNewUploadId(const QString& str, int start);
    int processChunkStatus(const QString& str, int start);
    int processUploadName(const QString& str, int start);

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
    void createdNewChat(int id);
    void addedToNewChat(int id);
    void unknownListReceived();
    void allUserInfoReceived();
    void accountDataCleared();
private:
	AccountInfoStorage& storage;
    MediaUploader& uploader;
    RequestSender& sender;
    MediaCache& mediaCache;
    bool userInfoReceived;
    std::vector<std::pair<int, int>> _awaitingContactInfoList;
};