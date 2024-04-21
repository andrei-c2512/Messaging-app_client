#include "ChatHistory.h"
#include "CustomStyledWidgets/CustomButton.h"
#include <QStackedWidget>

class ChatTextEdit : public CustomTextEdit {
    Q_OBJECT
public:
    ChatTextEdit(QWidget* parent, const ServerInfoProcessor& serverInfoProcessor0);
    void setCharacterLimit(int limit);
signals:
    void messageCreated(const QString& name, const QString& message);
protected:
    void keyPressEvent(QKeyEvent* event) override;
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
    ChatMessageBar(QWidget* parent, const ServerInfoProcessor& serverInfoProcessor0);
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
class BlockUI : public QWidget {
public:
    enum class Mode {
        Invalid,
        YouAreBlocked,
        YouBlocked,
        ForcefullyRemoved,
        Left
    };
public:
    BlockUI(QWidget* parent = nullptr);
    void setBlockMode(Mode mode, ContactInfo* info, ServerInfoProcessor& processor);
    void setRemovedMode(Mode mode);
private:
    void setupUi();
private:
    QVBoxLayout* pLayout;
    QLabel* pLabel;
    CustomButton* pUnblockButton;
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
    bool isRequieredLength() const;
private:
    QString title;
};


class Chat: public QWidget {
public:
    Q_OBJECT
public:
    Chat(QWidget* parent, ServerInfoProcessor& ServerInfoProcessor);
    void setChat(ChatInfo& pInfo);
    void setChat(int id);
    TitleTextEdit& groupNameEdit();
    int chatId() const;
    void onGettingRemoved(bool forcefully);
public slots:
    void createRoom();
    void onGettingBlocked(bool blocked);
    void onBlocking(bool blocked);
    void updateName(const QString& str);
private:
    void setupUi();
    void signalsAndSlots();
private:
    QVBoxLayout* pMainLayout;

    TitleTextEdit* pGroupName;

    ChatHistory* pChat;
    ChatInfo* lastPrivateChat;
    ChatInfo* lastChat;
    ContactInfo* lastPrivateChatUser;

    QStackedWidget* pStackedWidget;
    ChatMessageBar* pMessageBar;
    BlockUI* pBlockUI;

    ServerInfoProcessor& processor;
    static constexpr int characterLimit = 250;
};

