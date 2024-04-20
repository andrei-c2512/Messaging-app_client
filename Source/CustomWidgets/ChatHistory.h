#include <QWidget>
#include "Network/ServerInfoProcessor.h"
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "CustomWidgets/CustomTextEdit.h"
#include <QScrollArea>
#include <QScrollBar>
#include "CustomWidgets/ProfilePicLabel.h"

class MessageEdit : public CustomTextEdit {
public:
    using CustomTextEdit::CustomTextEdit;
    QSize sizeHint() const override;
};

class ChatRecord : public QWidget {
    Q_OBJECT
public:
    ChatRecord(QWidget* parent = nullptr);
    ChatRecord(QWidget* parent, MessageInfo* pInfo0, bool waitingForResponse = false);
    void setInfo(MessageInfo* info);
    void setMessage(const QString& str);
    void setName(const QString& name);
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

class ChatScrollArea : public QScrollArea {
    Q_OBJECT
public:
    ChatScrollArea(QWidget* parent = nullptr);
    void setScrollAreaWidget(QWidget* widget);
protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void moveScrollBarToBottom(int min, int max);
private:
    QWidget* pScrollAreaWidget;
};



class ChatHistory : public QWidget {
    Q_OBJECT
public:
    ChatHistory(QWidget* parent = nullptr);
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
    void addRecord(const QString& name, const QString& message);
    void addRecord(MessageInfo& message);
private:
    std::vector<ChatRecord*> recordList;
    QVBoxLayout* pHistoryLayout;
    QWidget* pScrollAreaWidget;
    ChatScrollArea* pScrollArea;
    ChatInfo* pInfo;

    QHash<QString, int> userHash;
    int lastVal;
    int sequence;
    static constexpr int messagesLoaded = 50;
};