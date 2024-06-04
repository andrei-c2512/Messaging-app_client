#include "ChatHistory.h"
#include "CustomStyledWidgets/CustomButton.h"
#include <QStackedWidget>
#include "UserSelectorWidget.h"
#include <QFileDialog>
#include "MediaLabel.h"
#include "ChatTextEdit.h"


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

class MediaScrollArea : public QScrollArea {
    Q_OBJECT
public:
    MediaScrollArea(QWidget* parent = nullptr);
    int count() const;
    void clear();
    const std::vector<QUrl>& mediaUrls() const;
signals:
    void addedMedia();
    void removedMedia();
public slots:
    void addImages(QStringList list);
    void onRemove(int index);
private:
    void setupUi();
private:
    QWidget* pWidget;
    std::vector<QUrl> url_list;
    std::vector<MediaLabel*> label_list;
    QHBoxLayout* pLayout;
    int _maxDim;
};

class ChatTextEdit : public STextEdit {
    Q_OBJECT
public:
    ChatTextEdit(QWidget* parent, const ConnectionHandler& serverInfoProcessor0, KeywordCombo& combo, MediaScrollArea& mediaArea);
    void setCharacterLimit(int limit);
signals:
    void messageCreated(const QString& name, const QString& message);
    void mediaMessageCreated(const QString& name, const std::vector<QUrl>& list);
protected:
    void keyPressEvent(QKeyEvent* event) override;
private:
    void sendMessage();
    void sendMedia();
private:
    const ConnectionHandler& serverInfoProcessor;
    MediaScrollArea& mediaArea;
    int _limit = 100;
};

class ChatMessageBar : public QWidget {
public:
    ChatMessageBar(QWidget* parent, const ConnectionHandler& serverInfoProcessor0 ,KeywordCombo& keywordCombo);
    void setCharacterLimit(int n);
    ChatTextEdit& textEdit();
private:
    void setupUi(const ConnectionHandler& serverInfoProcessor0, KeywordCombo& keywordCombo);
private:
    QVBoxLayout* pLayout;
    QHBoxLayout*  pBarLayout;
    KeywordCombo& keywordCombo;
    MediaScrollArea* pMediaArea;
    CustomButton* pUploadFileBtn;
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
    void setBlockMode(Mode mode, ContactInfo* info, ConnectionHandler& processor);
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
    Chat(QWidget* parent, ConnectionHandler& ServerInfoProcessor , UserSelectorWidget& widget, KeywordCombo& keywordCombo);
    void setChat(ChatInfo& pInfo);
    void setChat(int id);
    TitleTextEdit& groupNameEdit();
    int chatId() const;
    void onGettingRemoved(bool forcefully);
    void setMemberKeyWords(const std::vector<QString>& list);
    void setMediaKeyWords(const std::vector<QString>& list);
public slots:
    void createRoom();
    void onGettingBlocked(bool blocked);
    void onBlocking(bool blocked);
    void updateName(const QString& str);
private:
    void setupUi(KeywordCombo& keywordCombo);
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

    ConnectionHandler& processor;
    static constexpr int characterLimit = 250;
};

