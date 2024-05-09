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
    ChatRecord(QWidget* parent, MessageInfo* pInfo0, const std::vector<QString>& memberNames, bool waitingForResponse = false);
    void setInfo(MessageInfo* info, const std::vector<QString>& memberNames);
    void setInfoWithMedia(MessageInfo* info , QUrl url);
    void setMessage(const QString& str);
    void setName(const QString& name);
    QString message() const;
    QString name() const;
    void setStatus(bool isFirstMessageOfSequence);
private:
    void setupUi(bool waitingForResponse);
    void applyTags(const std::vector<QString>& memberNames);
    static QString dateToString(QDateTime time);
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
signals:
    void loadMore();
protected:
    void resizeEvent(QResizeEvent* event);
public slots:
    void moveScrollBarToBottom();
private:
    QWidget* pScrollAreaWidget;
    bool _moveScrollBar;

    int oldMin;
    int oldMax;
};



class ChatHistory : public QWidget {
    Q_OBJECT
public:
    ChatHistory(QWidget* parent , ServerInfoProcessor& processor0);
    void setChatInfo(ChatInfo* info);
    const ChatRecord& recordAt(int index);
    int chatId() const;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
private:
    void addNRecords(int n);
    void deleteNRecords(int n);
    void empty();
private:
    void setupUi();
    void setupMessage(MessageInfo& info, ChatRecord* record , ChatRecord* previous);
public slots:
    void addRecord(const QString& name, const QString& message);
    void addMediaRecord(const QString& name, const QUrl& url);
    void addMediaRecords(const QString& name, const std::vector<QUrl>& urls);
    void addRecord(MessageInfo& message);
    void onLoadMore();
private:
    std::vector<ChatRecord*> recordList;
    QVBoxLayout* pHistoryLayout;
    QWidget* pScrollAreaWidget;
    ChatScrollArea* pScrollArea;
    ChatInfo* pInfo;
    std::vector<QString> memberNames;

    QHash<QString, int> userHash;
    int lastVal;
    int sequence;
    static constexpr int messagesLoaded = 25;
    int lastMessageLoadedIndex;

    ServerInfoProcessor& processor;
};