
#ifndef CHATPAGE_H
#define CHATPAGE_H
#include <QWidget>
#include "Network/ServerInfoProcessor.h"
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "CustomWidgets/CustomTextEdit.h"
#include <QScrollArea>
#include <QScrollBar>
#include "Pages/Page.h"
#include "CustomWidgets/ProfilePicLabel.h"
#include <QStackedWidget>
#include "CustomStyledWidgets/CustomButton.h"
#include "CustomWidgets/ContactView_MembersSection.h"
#include <QToolBar>
#include "CustomWidgets/UserSelectorWidget.h"


class MembersSection : public QScrollArea {
    //to be styled in style.qss
    Q_OBJECT
public:
    MembersSection(QWidget* parent , ConnectionHandler& ServerInfoProcessor);
    void setContactList(std::vector<int> contactIdList , ConnectionHandler& processor, Chat& page, int adminId);
    void setAdmin(int admin);
    void removeContact(int id);
    void removeUserView();
    std::vector<ContactInfo*> contactList() const;
private:
    void setupUi();
signals:
    void switchToPrivateChat(int contactId);
private:
    QWidget* pWidget;
    ContactView_MembersSection* pUserView;
    std::vector<ContactInfo*> _contactInfoList;
    std::vector<ContactView_MembersSection*> _viewList;
    QVBoxLayout* pLayout;
    ConnectionHandler& processor;
    QLabel* pMembersLabel;
};


class ChatPage : public Page{
public:
    Q_OBJECT
public:
    ChatPage(QWidget* parent , ConnectionHandler& ServerInfoProcessor , UserSelectorWidget& widget, KeywordCombo& keywordCombo);
    void setChat(ChatInfo& pInfo);
    void setChat(int id);
public slots:
    void onMemberRemoval(int id);
    void onBeingRemoved(bool forcefullyremoved);
    void onMemberAddition(int id);
    void onNewAdmin(int id);
private:
    void setupUi(UserSelectorWidget& widget, KeywordCombo& keywordCombo);
private:
    UserSelectorWidget& userSelector;
    QHBoxLayout* pMainLayout;
    QVBoxLayout* pVerticalLayout;
    QToolBar* pToolBar;
    QAction* pAddButton;
    QAction* pLeaveButton;
    MembersSection* pMembersSection;
    Chat* pChat;

    ChatInfo* pInfo;
    static constexpr int characterLimit = 250;
};

#endif // CHATPAGE_H
