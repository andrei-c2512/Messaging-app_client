
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
#include "CustomWidgets/ContactView.h"

class MessageEdit: public CustomTextEdit {
public:
    using CustomTextEdit::CustomTextEdit;
    QSize sizeHint() const override;
};

class ChatRecord: public QWidget{
    Q_OBJECT
public:
    ChatRecord(QWidget* parent = nullptr);
    ChatRecord(QWidget* parent , MessageInfo* pInfo0 , bool waitingForResponse = false);
    void setInfo(MessageInfo* info);
    void setMessage(const QString& str);
    void setName( const QString& name);
    QString message() const;
    QString name() const;

    void setStatus(bool isFirstMessageOfSequence);
private:
    void setupUi(bool waitingForResponse);
private:
    QHBoxLayout* pMainLayout;
    QHBoxLayout* pNameAndTimeLayout;
    ProfilePicLabel* pProfilePicture;

    QVBoxLayout* pUILayout;
    QLabel* pName;
    CustomTextEdit* pMessage;
    QLabel* pTime;

    MessageInfo* pInfo;

};

class ChatScrollArea : public QScrollArea{
    Q_OBJECT
public:
    ChatScrollArea(QWidget* parent = nullptr);
    void setScrollAreaWidget(QWidget* widget);
protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void moveScrollBarToBottom(int min , int max);
private:
    QWidget* pScrollAreaWidget;
};



class Chat : public QWidget{
    Q_OBJECT
public:
    Chat(QWidget* parent = nullptr );
    void setChatInfo(ChatInfo* info);
    const ChatRecord& recordAt(int index);
    int chatId() const;
private:
    void addNRecords(int n);
    void deleteNRecords(int n);
    void empty();
private:
    void setupUi();
    void setupMessage(MessageInfo& info, ChatRecord* record);
public slots:
    void addRecord(const QString& name , const QString& message);
    void addRecord(MessageInfo& message);
private:
    std::vector<ChatRecord*> recordList;
    QVBoxLayout* pHistoryLayout;
    QWidget*     pScrollAreaWidget;
    ChatScrollArea* pScrollArea;
    ChatInfo* pInfo;

    QHash<QString, int> userHash;
    int lastVal;
    int sequence;
    static constexpr int messagesLoaded = 50;
};

class ChatTextEdit : public CustomTextEdit{
    Q_OBJECT
public:
    ChatTextEdit(QWidget* parent , const ServerInfoProcessor& serverInfoProcessor0);
    void setCharacterLimit(int limit);
signals:
    void messageCreated(const QString& name , const QString& message);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    void sendMessage();
private:
    const ServerInfoProcessor& serverInfoProcessor;
    int _limit = 100;
};

class WarningTextLabel : public QLabel {
public:
    WarningTextLabel(QWidget* parent = nullptr);
    void setState(bool state);
    void setColorForState(QColor color, bool active);
protected:
    void paintEvent(QPaintEvent* ev);
private:
    bool active = false;
    std::array<QColor, 2> colors;
};
class ChatMessageBar : public QWidget {
public:
    ChatMessageBar(QWidget* parent , const ServerInfoProcessor& serverInfoProcessor0);
    void setCharacterLimit(int n);
    ChatTextEdit& textEdit();
private:
    void setupUi(const ServerInfoProcessor& serverInfoProcessor0);
private:
    QVBoxLayout* pLayout;
    ChatTextEdit* pTextEdit;
    WarningTextLabel* pLimitShower;
    int _chLimit;
};
class BlockUI : public QWidget{
public:
    enum class Mode {
        Invalid,
        YouAreBlocked,
        YouBlocked
    };
public:
    BlockUI(QWidget* parent = nullptr);
    void setMode(Mode mode , ContactInfo* info , ServerInfoProcessor& processor);
private:
    void setupUi();
private:
    QVBoxLayout*  pLayout;
    QLabel*       pLabel;
    CustomButton* pUnblockButton;
};



class MembersSection : public QScrollArea {
    //to be styled in style.qss
    Q_OBJECT
public:
    MembersSection(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor);
    void setContactList(std::vector<int> contactIdList);
private:
    void setupUi();
private:
    QWidget* pWidget;
    std::vector<ContactInfo*> _contactInfoList;
    std::vector<ContactView*> _viewList;
    QVBoxLayout* pLayout;
    ServerInfoProcessor& processor;
    QLabel* pMembersLabel;
};

class TitleTextEdit : public CustomTextEdit {
    Q_OBJECT
public:
    TitleTextEdit(QWidget* parent = nullptr);
    void setTitle(const QString& str);
signals:
    void titleChanged(QString str);
protected:
    void keyPressEvent(QKeyEvent* event);
    void focusOutEvent(QFocusEvent* event);
private:
    QString title;
};

class ChatPage : public Page{
public:
    Q_OBJECT
public:
    ChatPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor);
    void setChat(ChatInfo& pInfo);
    void setChat(int id);
    TitleTextEdit& groupNameEdit();
public slots:
    void createRoom();
    void onGettingBlocked(bool blocked);
    void onBlocking(bool blocked);
    void updateName(const QString& str);
private:
    void setupUi();
    void signalsAndSlots();
private:
    QHBoxLayout* pChatElementsLayout;
    QVBoxLayout*    pMainLayout;
    MembersSection* pMembersSection;
    TitleTextEdit* pGroupName;
    Chat*           pChat;
    ChatInfo* lastPrivateChat;
    ChatInfo* lastChat;
    ContactInfo* lastPrivateChatUser;

    QStackedWidget* pStackedWidget;
    ChatMessageBar* pMessageBar;
    BlockUI* pBlockUI;

    static constexpr int characterLimit = 250;
};

#endif // CHATPAGE_H
