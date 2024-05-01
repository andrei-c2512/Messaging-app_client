#include "Chat.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ButtonStyleRepository.h"

WarningTextLabel::WarningTextLabel(QWidget* parent)
{
    active = false;
}
void WarningTextLabel::setColorForState(QColor color, bool active0)
{
    colors[int(active0)] = color;
}
void WarningTextLabel::setState(bool state) {
    active = state;
    update();
}
void WarningTextLabel::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    QPen pen(colors[int(active)]);
    painter.setPen(pen);
    painter.setFont(font());
    painter.drawText(rect(), Qt::AlignCenter, text());

}

TitleTextEdit::TitleTextEdit(QWidget* parent) : CustomTextEdit(parent)
{}

void TitleTextEdit::setTitle(const QString& str) {
    title = str;
    if (hasFocus() == false)
        setText(str);
}
void TitleTextEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    //0x01000004 and 0x01000005 are the value for both enters on the keyboard
    if (isRequieredLength() && (key == 0x01000004 || key == 0x01000005))
        emit titleChanged(toPlainText());
    //in case I would like to restrict some letters in the future
    else
    {
        switch (key)
        {
        default:
            CustomTextEdit::keyPressEvent(event);
            break;
        }
    }
}
bool TitleTextEdit::isRequieredLength() const
{
    int l = document()->characterCount();
    return l >= 4 && l < 50;
}
void TitleTextEdit::focusOutEvent(QFocusEvent* event)
{
    setText(title);
    CustomTextEdit::focusOutEvent(event);
}

ChatTextEdit::ChatTextEdit(QWidget* parent, const ServerInfoProcessor& serverInfoProcessor0, KeywordCombo& combo)
    : STextEdit(parent , combo), serverInfoProcessor(serverInfoProcessor0)
{
    setPlaceholderText("Type...");
    addNewCollection(Qt::Key_At, {});
    addNewCollection(Qt::Key_Asterisk, {});
}
void ChatTextEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    //0x01000004 and 0x01000005 are the value for both enters on the keyboard

    STextEdit::keyPressEvent(event);
    if ((key == 0x01000004 || key == 0x01000005))
    {     
        if(!usedTabOrEnter() && toPlainText().length() <= _limit && !document()->isEmpty())
            sendMessage();
    }

}
void ChatTextEdit::setCharacterLimit(int limit) { _limit = limit; }
void ChatTextEdit::sendMessage()
{
    emit messageCreated(serverInfoProcessor.name(), CustomTextEdit::toPlainText());
    CustomTextEdit::clear();
}

MediaScrollArea::MediaScrollArea(QWidget* parent) : QScrollArea(parent) 
{
    setupUi();
    _maxDim = 200;
}

void MediaScrollArea::addImages(QStringList list)
{
    char oldSize = label_list.size();
    label_list.resize(label_list.size() + list.size());
    for (char i = oldSize; i < label_list.size(); i++)
    {
        label_list[i] = new MediaLabel;
        QPixmap pixmap = QPixmap(list[i - oldSize]);
        if(pixmap.width() > pixmap.height())
            label_list[i]->setPixmap(pixmap.scaledToWidth(_maxDim));
        else
            label_list[i]->setPixmap(pixmap.scaledToHeight(_maxDim));

        label_list[i]->setIndex(i);
        pLayout->insertWidget(0 , label_list[i]);  
        connect(label_list[i], &MediaLabel::remove, this, &MediaScrollArea::onRemove);
    }
}

void MediaScrollArea::onRemove(int index)
{
    label_list[index]->deleteLater();
    label_list.erase(label_list.begin() + index);
    for (int i = index; i < label_list.size(); i++)
        label_list[i]->setIndex(i);

    if (label_list.empty())
        setVisible(false);
}
void MediaScrollArea::setupUi()
{
    pWidget = new QWidget;
    pLayout = new QHBoxLayout(pWidget);
    pLayout->addStretch(1);
    setWidget(pWidget);
    setWidgetResizable(true);
}

ChatMessageBar::ChatMessageBar(QWidget* parent, const ServerInfoProcessor& serverInfoProcessor0, KeywordCombo& keywordCombo0) 
    : QWidget(parent) , keywordCombo(keywordCombo0)
{
    setupUi(serverInfoProcessor0 , keywordCombo0);
}

ChatTextEdit& ChatMessageBar::textEdit() { return *pTextEdit; }
void ChatMessageBar::setCharacterLimit(int n) {
    _chLimit = n;
    pTextEdit->setCharacterLimit(n);
}
void ChatMessageBar::setupUi(const ServerInfoProcessor& serverInfoProcessor0, KeywordCombo& keywordCombo)
{
    QMargins noMargins(0, 0, 0, 0);


    pUploadFileBtn = new CustomButton(nullptr , ButtonStyleRepository::uploadFileButton());
    connect(pUploadFileBtn, &CustomButton::clicked, this, [=]() {
        QStringList list = QFileDialog::getOpenFileNames(this, "Upload file", "C://");
        if (list.size())
        {
            pMediaArea->addImages(list);
            pMediaArea->setVisible(true);
        }
        });
    pTextEdit = new ChatTextEdit(nullptr, serverInfoProcessor0 , keywordCombo);

    connect(pTextEdit, &ChatTextEdit::newList, &keywordCombo, &KeywordCombo::setKeywords);
    connect(pTextEdit, &ChatTextEdit::keywordDiscontinued, &keywordCombo, &KeywordCombo::hide);
    keywordCombo.setTextEdit(pTextEdit);

    pBarLayout = new QHBoxLayout;
    pBarLayout->addWidget(pUploadFileBtn, 0,  Qt::AlignTop | Qt::AlignLeft);
    pBarLayout->addWidget(pTextEdit , 1);
    pBarLayout->setSpacing(5);

    pLimitShower = new WarningTextLabel;
    pLimitShower->setColorForState("white", false);
    pLimitShower->setColorForState(QColor(235, 81, 96), true);
    pLimitShower->setState(false);
    QSizePolicy policy;
    policy.setRetainSizeWhenHidden(true);
    pLimitShower->setSizePolicy(policy);

    pMediaArea = new MediaScrollArea;
    pMediaArea->setVisible(false);

    pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(noMargins);
    pLayout->addWidget(pMediaArea);
    pLayout->addLayout(pBarLayout);
    pLayout->addWidget(pLimitShower, Qt::AlignRight);


    connect(pTextEdit, &ChatTextEdit::textChanged, this, [=]() {
        int dif = _chLimit - pTextEdit->toPlainText().length();
    if (dif <= 100)
    {
        pLimitShower->setVisible(true);
        pLimitShower->setText(QString::number(dif));
        pLimitShower->setState(dif < 0);
    }
    else
        pLimitShower->setVisible(false);
        });
}

BlockUI::BlockUI(QWidget* parent) : QWidget(parent)
{
    setupUi();
}
void BlockUI::setBlockMode(Mode mode, ContactInfo* info, ServerInfoProcessor& processor)
{
    pUnblockButton->disconnect();
    if (mode == Mode::YouAreBlocked)
    {
        pLabel->setText(info->name() + " has blocked YOU");
        pUnblockButton->setVisible(false);
    }
    else
    {
        pLabel->setText("You have blocked " + info->name());
        pUnblockButton->setVisible(true);
        connect(pUnblockButton, &CustomButton::clicked, this, [&processor, info]() {
            processor.unblockUser(info->id());
            });
    }
}

void BlockUI::setRemovedMode(Mode mode)
{
    if (mode == Mode::ForcefullyRemoved)
    {
        pLabel->setText("You have been removed from the group");
        pUnblockButton->setVisible(false);
    }
    else if (mode == Mode::Left)
    {
        pLabel->setText("You have left the group");
        pUnblockButton->setVisible(false);
    }
}

void BlockUI::setupUi()
{
    pLabel = new QLabel;
    pLabel->setFont(StyleRepository::Base::mediumSizeButtonFont());
    pLabel->setAlignment(Qt::AlignCenter);

    pUnblockButton = new CustomButton();
    pUnblockButton->setText("Unblock");
    pUnblockButton->setFont(StyleRepository::Base::mediumSizeButtonFont());
    pUnblockButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(pUnblockButton);
    btnLayout->addStretch(1);

    pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pLabel);
    pLayout->addLayout(btnLayout);
}


Chat::Chat(QWidget* parent, ServerInfoProcessor& ServerInfoProcessor, UserSelectorWidget& widget, KeywordCombo& keywordCombo) 
    : QWidget(parent) , processor(ServerInfoProcessor)
{
    setupUi(keywordCombo);
    signalsAndSlots();
    lastPrivateChat = nullptr;
    lastPrivateChatUser = nullptr;
    connect(&ServerInfoProcessor, &ServerInfoProcessor::allUserInfoReceived, this, [=, &ServerInfoProcessor]() {
        if (ServerInfoProcessor.chatListEmpty() == false)
            setChat(ServerInfoProcessor.firstChat());
        });

    connect(&widget, &UserSelectorWidget::switchToChat, this, [=](int chatId) {
        setChat(chatId);
        });

    connect(&ServerInfoProcessor, &ServerInfoProcessor::accountDataCleared, this, [=]() {
        lastChat = nullptr;
        });
    lastChat = nullptr;
}

void Chat::updateName(const QString& str)
{
    pGroupName->setText(str);
    pGroupName->setTitle(str);
}

void Chat::signalsAndSlots()
{
    connect(&pMessageBar->textEdit(), SIGNAL(messageCreated(const QString&, const QString&)), pChat, SLOT(addRecord(const QString&, const QString&)));
}
void Chat::setupUi(KeywordCombo& keywordCombo)
{
    pGroupName = new TitleTextEdit;
    pGroupName->setFont(StyleRepository::Base::headerFont());
    pGroupName->setShrinkToFit(true);
    pGroupName->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    connect(pGroupName, &TitleTextEdit::titleChanged, this, [=](QString str) {
        processor.updateChatName(pChat->chatId(), str);
        });
    pChat = new ChatHistory;


    pMessageBar = new ChatMessageBar(nullptr, processor , keywordCombo);
    pMessageBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding));
    pMessageBar->textEdit().setShrinkToFit(true);
    pMessageBar->textEdit().setFont(StyleRepository::Base::chatFont());
    pMessageBar->textEdit().setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    pMessageBar->setCharacterLimit(characterLimit);

    pBlockUI = new BlockUI;

    pStackedWidget = new QStackedWidget;
    pStackedWidget->addWidget(pMessageBar);
    pStackedWidget->addWidget(pBlockUI);
    pStackedWidget->setCurrentWidget(pMessageBar);

    pMainLayout = new QVBoxLayout(this);

    pMainLayout->addWidget(pGroupName, 5);
    pMainLayout->addWidget(pChat, 75);
    pMainLayout->addWidget(pStackedWidget, 0);

    pMainLayout->addStretch(5);
}
void Chat::setChat(ChatInfo& info)
{
    if (lastChat)
        disconnect(lastChat, &ChatInfo::nameChanged, this, &Chat::updateName);
    connect(&info, &ChatInfo::nameChanged, this, &Chat::updateName);

    if (info.isPrivate())
    {
        pGroupName->setReadOnly(true);
        BlockUI::Mode mode = BlockUI::Mode::Invalid;
        auto& list = info.members();
        int i = 0;
        if (list[i] == processor.id())
            i = 1;
        if (lastPrivateChat)
        {
            disconnect(lastPrivateChat, &ChatInfo::gotBlocked, this, &Chat::onBlocking);
            disconnect(lastPrivateChat, &ChatInfo::blockedYou, this, &Chat::onGettingBlocked);
        }

        ContactInfo* contactInfo = processor.findUser(list[i]);

        char flags = contactInfo->flags();
        if (flags & (char)ContactInfo::Status::HasBlockedYou)
            mode = BlockUI::Mode::YouAreBlocked;
        else if (flags & (char)ContactInfo::Status::IsBlocked)
            mode = BlockUI::Mode::YouBlocked;

        if (mode != BlockUI::Mode::Invalid)
        {
            pBlockUI->setBlockMode(mode, contactInfo, processor);
            pStackedWidget->setCurrentWidget(pBlockUI);
        }
        else
            pStackedWidget->setCurrentWidget(pMessageBar);

        lastPrivateChatUser = contactInfo;

        connect(&info, &ChatInfo::gotBlocked, this, &Chat::onBlocking);
        connect(&info, &ChatInfo::blockedYou, this, &Chat::onGettingBlocked);

        lastPrivateChat = &info;
    }
    else
    {
        pGroupName->setReadOnly(false);
        if (lastPrivateChat)
        {
            disconnect(lastPrivateChat, &ChatInfo::gotBlocked, this, &Chat::onBlocking);
            disconnect(lastPrivateChat, &ChatInfo::blockedYou, this, &Chat::onGettingBlocked);
        }
        lastPrivateChat = nullptr;
        pStackedWidget->setCurrentWidget(pMessageBar);
    }

    pGroupName->setTitle(info.name());
    pChat->setChatInfo(&info);
    setMemberKeyWords(processor.namesForContacts(info.members()));
    lastChat = &info;
}

void Chat::setMemberKeyWords(const std::vector<QString>& list) {
    pMessageBar->textEdit().setWordsForKey(Qt::Key_At, list);
}
void Chat::setMediaKeyWords(const std::vector<QString>& list){
    pMessageBar->textEdit().setWordsForKey(Qt::Key_Asterisk, list);
}

void Chat::onGettingBlocked(bool blocked)
{
    if (blocked)
    {
        pBlockUI->setBlockMode(BlockUI::Mode::YouAreBlocked, lastPrivateChatUser, processor);
        pStackedWidget->setCurrentWidget(pBlockUI);
    }
    else
        pStackedWidget->setCurrentWidget(pMessageBar);
}
void Chat::onBlocking(bool blocked)
{
    if (blocked)
    {
        pBlockUI->setBlockMode(BlockUI::Mode::YouBlocked, lastPrivateChatUser, processor);
        pStackedWidget->setCurrentWidget(pBlockUI);
    }
    else
        pStackedWidget->setCurrentWidget(pMessageBar);
}

void Chat::onGettingRemoved(bool forcefully)
{
    BlockUI::Mode mode = (forcefully) ? BlockUI::Mode::ForcefullyRemoved : BlockUI::Mode::Left;
    pBlockUI->setRemovedMode(mode);
    pStackedWidget->setCurrentWidget(pBlockUI);
}

void Chat::setChat(int id)
{
    setChat(processor.getChatById(id));
}
TitleTextEdit& Chat::groupNameEdit() { return *pGroupName; }

void Chat::createRoom()
{
}

int Chat::chatId() const { return lastChat->id(); }