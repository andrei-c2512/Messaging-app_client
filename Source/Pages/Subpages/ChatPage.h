
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
signals:
    void messageCreated(const QString& name , const QString& message);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    void sendMessage();
private:
    const ServerInfoProcessor& serverInfoProcessor;
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
class ChatPage : public Page{
public:
    Q_OBJECT
public:
    ChatPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor);
    void setChat(ChatInfo& pInfo);
    void setChat(int id);
    CustomTextEdit& groupNameEdit();
public slots:
    void createRoom();
    void onGettingBlocked(bool blocked);
    void onBlocking(bool blocked);
private:
    void setupUi();
    void signalsAndSlots();
private:
    QVBoxLayout*    pMainLayout;
    CustomTextEdit* pGroupName;
    Chat*           pChat;
    ChatInfo* lastPrivateChat;
    ContactInfo* lastPrivateChatUser;


    QStackedWidget* pStackedWidget;
    ChatTextEdit* pTextEdit;
    BlockUI* pBlockUI;
};

#endif // CHATPAGE_H
