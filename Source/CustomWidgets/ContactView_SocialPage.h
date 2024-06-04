#pragma once
#include "ContactView.h"
#include "Pages/Subpages/ChatPage.h"
#include "Network/ServerInfoProcessor.h"


enum class SearchType {
    GroupSelection,
    FriendList,
    RequestList,
    AddFriend,
    BlockedList,
    Count
};

class RequestOptions : public QWidget {
    Q_OBJECT
public:
    RequestOptions(QWidget* parent = nullptr);
    CustomButton& acceptBtn();
    CustomButton& rejectBtn();
    void disconnectAll();
private:
    void setupUi();
private:
    QHBoxLayout* pLayout;
    CustomButton* pAcceptBtn;
    CustomButton* pRejectBtn;
    std::vector<QMetaObject::Connection> _connectionList;
};

class FriendOptions : public QWidget {
    Q_OBJECT
public:
    FriendOptions(QWidget* parent = nullptr);
    CustomButton& sendMessageBtn();
    QAction& removeAction();
    QAction& blockAction();
    QAction& callAction();
    void disconnectAll();
private:
    void setupUi();
private:
    QHBoxLayout* pLayout;
    CustomButton* pSendMessageBtn;
    CustomButton* pMoreOptionsBtn;
    QMenu* pMenu;
    QAction* removeAct;
    QAction* blockAct;
    QAction* callAct;
};

class StrangerOptions : public QWidget {
    Q_OBJECT
public:
    StrangerOptions(QWidget* parent = nullptr);
    TwoStateButton& addFriendBtn();
    void disconnectAll();
private:
    void setupUi();
private:
    //in the future , I want to add another MENU button that has the options to: see the profile , their friends or blocck the person
    QHBoxLayout* pLayout;
    TwoStateButton* pAddFriendButton;
};


class ContactView_SocialPage : public ContactView {
public:
	ContactView_SocialPage(QWidget* parent = nullptr);
    void attatchOptions(SearchType optionType, ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget);
private:
    [[nodiscard]] QWidget* friendOptions(ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget);
    [[nodiscard]] QWidget* strangerOptions(ConnectionHandler& processor);
    [[nodiscard]] QWidget* requestOptions(ConnectionHandler& processor);
    [[nodiscard]] QWidget* blockedOptions(ConnectionHandler& processor);

    void connectOptions(SearchType type, ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget);
    void connectFriendOptions(ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget);
    void connectStrangerOptions(ConnectionHandler& processor);
    void connectRequestOptions(ConnectionHandler& processor);
    void connectBlockedOptions(ConnectionHandler& processor);
private:
    SearchType _searchType;
    QWidget* pOptions;
    std::vector<QWidget*> optionList;
};