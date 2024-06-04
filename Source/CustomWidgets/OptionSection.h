
#ifndef OPTIONSECTION_H
#define OPTIONSECTION_H
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QHBoxLayout>
#include "CustomStyledWidgets/SelectableButton.h"
#include <QLabel>
#include "Network/ServerInfoProcessor.h"
#include "CustomWidgets/ProfilePicLabel.h"
#include <QStackedWidget>
#include <QDateTime>
#include "CustomWidgets/ProfileView.h"
#include <QVariantAnimation>
#include "Pages/Subpages/SubpageManager.h"
#include "CustomWidgets/UserSelectorWidget.h"

class ChatView : public QWidget{
    Q_OBJECT
public:
    ChatView(QWidget* parent ,  const ChatInfo& info);
    void setChatInfo(const ChatInfo& info);
    void setTextVisibile(bool text);
    bool needsToBeDeleted();
    bool isActive() const;
    void setHighlighted(bool status);
    void setActive(bool active);
private:
    void setupUi();
signals:
    void clicked(int chatId);
    void chatDeleted();
protected:
    void mousePressEvent(QMouseEvent* ev) override;
    void resizeEvent(QResizeEvent* ev) override;
private:
    const ChatInfo* pInfo;

    QHBoxLayout* pLayout;
    QVBoxLayout* pTextLayout;
    ProfilePicLabel* pPfpLabel;
    QLabel*          pName;
    QLabel*          pLastMessage;
    QLabel* pNewMessageLabel;
    static constexpr int maxCharShowcase = 15;
    bool _needsToBeDeleted;
    bool _highlighted;
    bool _active;
};

class ChatListWidget : public QWidget{
    Q_OBJECT
public:
    enum class Type{
        ChatList,
        RequestList
    };
public:
    ChatListWidget(QWidget* parent , const ConnectionHandler& infoProcessor , SubpageManager& manager , Type type);
    void setTextVisible(bool visible);
private:
    //void setServerInfoProcessor(const ServerInfoProcessor& infoProcessor);
    void calculatePlaces();
    void addViews(int n);
    void removeViews(int n);
private slots:
    void rearrange();
    void removeUnnecesaryChatView();
private:
    void setupUi();
    QVBoxLayout* pScrollAreaLayout;
    //I would have the chatInfo list however I need the whole object so that it emits a signal when it receives new chat info
    const ConnectionHandler& infoProcessor;
    SubpageManager& subpageManager;
    const std::vector<ChatInfo*>* chatInfo_list;
    //an array that memorizes the index of the chat in 'chatInfo_list' that is on the n-th place when it comes to how recent the last message of the chat was
    //for example the first number will be 3 because the fourth chat(in that list) is the most recent , second number will be maybe 2 etc
    std::vector<int> chatOrder;
    std::vector<ChatView*> chatView_list;
    Type type;
    QLabel* pEmptyListText;

    ChatView* lastChatView = nullptr;
};

class ChatListView : public QWidget{
    Q_OBJECT
public:
    ChatListView(QWidget* widget ,  const ConnectionHandler& infoProcessor , SubpageManager& manager);
    void setTextVisible(bool visible);
    void setList(ChatListWidget::Type type);
public slots:
    void setToChatList();
    void setToRequestList();
private:
    void setupUi();
private:
    QHBoxLayout* pLayout;
    QScrollArea*    pScrollArea;

    ChatListWidget* pChatList;
    ChatListWidget* pRequestList;

    QStackedWidget* pStackedWidget;

    const ConnectionHandler& serverProcessor;
    SubpageManager& subpageManager;
};


class OptionSection : public QWidget{
    Q_OBJECT
public:
    OptionSection(QWidget* parent  , const ConnectionHandler& processor , SubpageManager& subpageManager0 , UserSelectorWidget& userSelectorWidget);
    int minimumWidth() const;
    int maximumWidth() const;
    void setWidgetsVisible(bool visible);
private:
    void setupUi(UserSelectorWidget& userSelectorWidget);
    void connectButtons() const;
protected:
    bool event(QEvent* ev) override;
private:
    QVBoxLayout*      pMainLayout;

    QHBoxLayout* pConversationHeaderLayout;
    QLabel* pConversationIcon;
    QLabel* pConversationHeader;

    QHBoxLayout*      pButtonLayout;
    QLabel* pDotLabel;
    SelectableButton* pChatsButton;
    SelectableButton* pRequestsButton;

    CustomButton* pMessageButton;
    //there is a weird read acces violation if I check visibility with isVisible() func
    CustomButton*     pSocialButton;

    ProfileView*      pProfileView;
    ChatListView*     pListView;

    const ConnectionHandler& serverProcessor;
    SubpageManager &            subpageManager;
    int _minimumWidth;
    int _maximumWidth;

    
    QVariantAnimation* pAnimation;
};

class OptionSectionFrame : public QWidget{
    Q_OBJECT
public:
    OptionSectionFrame(QWidget* parent ,  const ConnectionHandler& processor , SubpageManager& pSubpageManager0 , UserSelectorWidget& userSelectorWidget);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    OptionSection* pSection;
};

#endif // OPTIONSECTION_H
