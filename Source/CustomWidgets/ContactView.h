
#ifndef CONTACTVIEW_H
#define CONTACTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "ProfilePicLabel.h"
#include "CustomStyledWidgets/CustomButton.h"
#include "Network/ServerInfoProcessor.h"
#include "CustomStyledWidgets/SelectableButton.h"
#include <QMenu>
#include "Pages/Subpages/ChatPage.h"
#include <QStackedWidget>
#include "StyleBase/ImagePainter.h"
enum class SearchType {
    FriendList,
    RequestList,
    AddFriend,
    BlockedList,
    Count
};




class RequestOptions : public QWidget{
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

class FriendOptions : public QWidget{
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

class StrangerOptions : public QWidget{
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


class ContactView : public QWidget{
    Q_OBJECT
public:
    ContactView(QWidget* parent = nullptr);
    void setContactInfo(ContactInfo& info);
    void setProfilePicture(QPixmap pixmap);
    void setName(QString name);

    QString name() const;
    QPixmap pixmap() const;

    void setNameVisible(bool visible);
    void attatchOptions(SearchType optionType , ServerInfoProcessor& processor , ChatPage& page, QStackedWidget& widget);

    bool operator==(const ContactView& rhs);
signals:
    void remove();
private:
    //creates this type of options , connects the buttons properly but does NOT attatch it to a widget
    [[nodiscard]] QWidget* friendOptions(ServerInfoProcessor& processor, ChatPage& page , QStackedWidget& widget);
    [[nodiscard]] QWidget* strangerOptions(ServerInfoProcessor& processor);
    [[nodiscard]] QWidget* requestOptions(ServerInfoProcessor& processor);
    [[nodiscard]] QWidget* blockedOptions(ServerInfoProcessor& processor);

    void connectOptions(SearchType type, ServerInfoProcessor& processor, ChatPage& page, QStackedWidget& widget);
    void connectFriendOptions(ServerInfoProcessor& processor, ChatPage& page, QStackedWidget& widget);
    void connectStrangerOptions(ServerInfoProcessor& processor);
    void connectRequestOptions(ServerInfoProcessor& processor);
    void connectBlockedOptions(ServerInfoProcessor& processor);
private:
    const ContactInfo* pInfo;
    QHBoxLayout* pLayout;
    ProfilePicLabel*      pProfilePicture;
    QLabel*      pName;
    const QSize iconSize = {32 , 32};

    QWidget* pOptions;
    std::vector<QWidget*> optionList;
};

#endif // CONTACTVIEW_H
