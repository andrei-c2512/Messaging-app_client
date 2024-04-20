
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



class MembersSection : public QScrollArea {
    //to be styled in style.qss
    Q_OBJECT
public:
    MembersSection(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor);
    void setContactList(std::vector<int> contactIdList , ServerInfoProcessor& processor, Chat& page, int adminId);
    void setAdmin(int admin);
private:
    void setupUi();
private:
    QWidget* pWidget;
    ContactView_MembersSection* pUserView;
    std::vector<ContactInfo*> _contactInfoList;
    std::vector<ContactView_MembersSection*> _viewList;
    QVBoxLayout* pLayout;
    ServerInfoProcessor& processor;
    QLabel* pMembersLabel;
};


class ChatPage : public Page{
public:
    Q_OBJECT
public:
    ChatPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor);
    void setChat(ChatInfo& pInfo);
    void setChat(int id);
private:
    void setupUi();
private:
    QHBoxLayout* pMainLayout;
    MembersSection* pMembersSection;
    Chat* pChat;

    static constexpr int characterLimit = 250;
};

#endif // CHATPAGE_H
