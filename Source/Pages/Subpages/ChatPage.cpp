#include "ChatPage.h"
#include "StyleBase/StyleRepository.h"

static QDate now = QDate::currentDate();

QSize  MessageEdit::sizeHint() const
{
    //return QSize(CustomTextEdit::sizeHint().width() , document().height;
    return QSize();
}

ChatRecord::ChatRecord(QWidget* parent) : QWidget(parent){
    setupUi(false);
}

ChatRecord::ChatRecord(QWidget* parent , MessageInfo* pInfo0 , bool waitingForResponse) : QWidget(parent)
{
    setupUi(waitingForResponse);
    setInfo(pInfo0);
    connect(pInfo0 , &MessageInfo::outOfQueue , this , [=](){
        pMainLayout->setStretch(0 , 0);
    });
}
void ChatRecord::setupUi(bool waitingForResponse)
{
    QMargins noMargins = QMargins(0, 0, 0, 0);
    QFont chatFont = StyleRepository::Base::chatFont();

    pProfilePicture = new ProfilePicLabel;
    pProfilePicture->setPixmap1(StyleRepository::Base::defaultPfpPixmap());
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    policy.setRetainSizeWhenHidden(true);
    pProfilePicture->setSizePolicy(policy);

    pTime = new QLabel;
    pTime->setFont(StyleRepository::Base::informativeTextFont());

    pName = new QLabel;
    pName->setFont(chatFont);
    pName->setContentsMargins(noMargins);

    pNameAndTimeLayout = new QHBoxLayout;
    pNameAndTimeLayout->addWidget(pName);
    pNameAndTimeLayout->addWidget(pTime);
    pNameAndTimeLayout->addStretch(1);
    pNameAndTimeLayout->setContentsMargins(noMargins);

    pMessage = new CustomTextEdit;
    pMessage->setFont(chatFont);
    pMessage->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    pMessage->setReadOnly(true);
    pMessage->setShrinkToFit(true);
    pMessage->setObjectName("Message");
    pMessage->setContentsMargins(noMargins);
    pMessage->document()->setDocumentMargin(0);

    pUILayout = new QVBoxLayout;
    pUILayout->addLayout(pNameAndTimeLayout);
    pUILayout->addWidget(pMessage);
    pUILayout->addStretch(1);
    pUILayout->setContentsMargins(noMargins);

    pMainLayout = new QHBoxLayout(this);
    pMainLayout->addWidget(pProfilePicture, 0, Qt::AlignTop);
    pMainLayout->addLayout(pUILayout);
    pMainLayout->setContentsMargins(noMargins);

    setContentsMargins(noMargins);
}

void ChatRecord::setStatus(bool isFirstMessageOfSequence)
{
    pName->setVisible(isFirstMessageOfSequence);
    pTime->setVisible(isFirstMessageOfSequence);
    pProfilePicture->setVisible(isFirstMessageOfSequence);
}

void ChatRecord::setInfo(MessageInfo* info)
{

    pInfo = info;
    connect(info , &ChatInfo::destroyed , this , [=](){
        pInfo = &NullInfo::instance().nullMessage();
    });
    pMessage->setText(info->text());
    pName->setText(info->name());

    QDate date = pInfo->timestamp().date();

    if(now.year() == date.year() && now.month() == date.month())
    {
        if(now.day() == date.day())
            pTime->setText("Today at " + info->timestamp().time().toString());
        else if(now.day() - date.day() == 1)
            pTime->setText("Yesterday at " + info->timestamp().time().toString());
        else
            pTime->setText(pInfo->timestamp().toString());
    }
    else
        pTime->setText(pInfo->timestamp().toString());
}

ChatScrollArea::ChatScrollArea(QWidget* parent ) : QScrollArea(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalScrollBar()->setEnabled(false);
    QScrollBar* verticalScrollBar = QScrollArea::verticalScrollBar();
    connect(verticalScrollBar , SIGNAL(rangeChanged(int , int)) , this , SLOT(moveScrollBarToBottom(int , int)));
    setObjectName("ChatScrollArea");
}
void ChatScrollArea::moveScrollBarToBottom(int min , int max)
{
    Q_UNUSED(min);
    QScrollBar* verticalScrollBar = QScrollArea::verticalScrollBar();
    verticalScrollBar->setValue(std::move(max));
}

void ChatScrollArea::setScrollAreaWidget(QWidget* widget) { pScrollAreaWidget = widget; }

void ChatScrollArea::resizeEvent(QResizeEvent* event)
{
    //setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());
    QScrollArea::resizeEvent(event);
}


void ChatRecord::setMessage(const QString& str){ pMessage->setText(str);}
void ChatRecord::setName(const QString& name){ pName->setText(name + ':');}
QString ChatRecord::message() const { return pMessage->toPlainText();}
QString ChatRecord::name() const{ return pName->text();}

Chat::Chat(QWidget* parent) : QWidget(parent)
{setupUi();
}
void Chat::setupUi()
{
    const int recordSpacing = 0;
    pScrollAreaWidget = new QWidget;
    pScrollAreaWidget->setObjectName("ListContainer");
    pHistoryLayout = new QVBoxLayout(pScrollAreaWidget);
    pHistoryLayout->addStretch(1);
    pHistoryLayout->setSpacing(recordSpacing);

    pScrollArea = new ChatScrollArea();
    pScrollArea->setWidget(pScrollAreaWidget);
    pScrollArea->setScrollAreaWidget(pScrollAreaWidget);
    pScrollArea->setWidgetResizable(true);
    QHBoxLayout* pMainLayout = new QHBoxLayout(this);
    pMainLayout->addWidget(pScrollArea);
    pInfo = nullptr;
    setObjectName("ListContainer");
}
void Chat::addNRecords(int n)
{
    int lastSize = recordList.size();
    recordList.resize(lastSize + n);

    for(int i = lastSize ; i < lastSize + n ; i++)
    {
        recordList[i] =  new ChatRecord(nullptr , &NullInfo::instance().nullMessage());
        pHistoryLayout->insertWidget(pHistoryLayout->count() - 1 , recordList[i]);
    }
    //std::for_each(std::begin(recordList) + lastSize, std::end(recordList) , [=](ChatRecord* record){
    //    record =  new ChatRecord(nullptr , &NullInfo::instance().nullMessage());
    //    pHistoryLayout->insertWidget(pHistoryLayout->count() - 1 , record);
    //});
}
void Chat::deleteNRecords(int n)
{
    if(n == 0) return;
    std::for_each(std::end(recordList) - n, std::end(recordList) , [=](ChatRecord* record){
        record->deleteLater();
    });
    recordList.resize(recordList.size() - n);
}


void Chat::addRecord(const QString& name , const QString& message)
{
    MessageInfo* messageInfo = pInfo->addMessageToQueue(name , message);
    ChatRecord* record = new ChatRecord(nullptr , messageInfo);

    setupMessage(*messageInfo, record);

    recordList.emplace_back(record);
    pHistoryLayout->insertWidget(pHistoryLayout->count() - 1 , record);
}
void Chat::addRecord(MessageInfo& message)
{
    ChatRecord* record = new ChatRecord(nullptr , &message);

    setupMessage(message, record);

    recordList.emplace_back(record);
    pHistoryLayout->insertWidget(pHistoryLayout->count() - 1 , record);
}
void Chat::setChatInfo(ChatInfo* info) {
    if(pInfo)
        disconnect(pInfo ,  SIGNAL(newMessageAdded(MessageInfo&)), this, SLOT(addRecord(MessageInfo&)));

    pInfo = info;
    connect(pInfo , &ChatInfo::destroyed , this , [=](){
        pInfo = &NullInfo::instance().nullChat();
    });
    connect(info , SIGNAL(newMessageAdded(MessageInfo&)) , this , SLOT(addRecord(MessageInfo&)));
    std::vector<MessageInfo*> infoList = pInfo->lastNMessages(messagesLoaded);

    int recordDiff = int(infoList.size() - recordList.size());
    if(recordDiff > 0)
        addNRecords(recordDiff);
    else
        deleteNRecords(-recordDiff);

    userHash.clear();
    sequence = 1;
    lastVal = -1;


    int& val = userHash[infoList[0]->name()];
    if (val == 0)
    {
        val = sequence;
        sequence++;
    }
    lastVal = val;
    recordList[0]->setInfo(infoList[0]);
    for(int i = 1; i < infoList.size() ; i++)
    {
        setupMessage(*infoList[i], recordList[i]);
        recordList[i]->setInfo(infoList[i]);
    }

}
void Chat::setupMessage(MessageInfo& info, ChatRecord* record)
{
    int& val = userHash[info.name()];
    if (val == 0)
    {
        val = sequence;
        sequence++;
    }

    if (val == lastVal)
    {
        record->setStatus(false);
        QMargins margins = QMargins(0, 0, 0, 0);
        record->setContentsMargins(margins);
    }
    else
    {
        record->setStatus(true);
        lastVal = val;

        QMargins margins = QMargins(0, 30, 0, 0);
        record->setContentsMargins(margins);
    }
    record->setInfo(&info);
}

void Chat::empty()
{
    std::for_each(std::begin(recordList) , std::end(recordList) , [=](ChatRecord* record){
        record->deleteLater();
    });
    recordList.resize(0);
}
int Chat::chatId() const { return pInfo->id(); }

const ChatRecord& Chat::recordAt(int index){ return *recordList[index];}

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
    painter.drawText(rect(), Qt::AlignCenter , text());

}

TitleTextEdit::TitleTextEdit(QWidget* parent) : CustomTextEdit(parent)
{}

void TitleTextEdit::setTitle(const QString& str) { 
    title = str; 
    if(hasFocus() == false)
        setText(str);
}
void TitleTextEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    //0x01000004 and 0x01000005 are the value for both enters on the keyboard
    if (!document()->isEmpty() && (key == 0x01000004 || key == 0x01000005))
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
void TitleTextEdit::focusOutEvent(QFocusEvent* event)
{
    setText(title);
    CustomTextEdit::focusOutEvent(event);
}

ChatTextEdit::ChatTextEdit(QWidget* parent , const ServerInfoProcessor& serverInfoProcessor0 ) : CustomTextEdit(parent) , serverInfoProcessor(serverInfoProcessor0)
{
    setPlaceholderText("Type...");
}
void ChatTextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    //0x01000004 and 0x01000005 are the value for both enters on the keyboard

    if(toPlainText().length() <= _limit &&!document()->isEmpty() && (key == 0x01000004 || key == 0x01000005))
        sendMessage();
    else 
        CustomTextEdit::keyPressEvent(event);
}
void ChatTextEdit::setCharacterLimit(int limit) { _limit = limit; }
void ChatTextEdit::sendMessage()
{
    emit messageCreated(serverInfoProcessor.name() , CustomTextEdit::toPlainText());
    CustomTextEdit::clear();
}

ChatMessageBar::ChatMessageBar(QWidget* parent , const ServerInfoProcessor& serverInfoProcessor0) : QWidget(parent)
{
    setupUi(serverInfoProcessor0);
}

ChatTextEdit& ChatMessageBar::textEdit() { return *pTextEdit; }
void ChatMessageBar::setCharacterLimit(int n) { 
    _chLimit = n;
    pTextEdit->setCharacterLimit(n);
}
void ChatMessageBar::setupUi(const ServerInfoProcessor& serverInfoProcessor0)
{
    QMargins noMargins(0, 0, 0, 0);
    pTextEdit = new ChatTextEdit(nullptr , serverInfoProcessor0);
    pLimitShower = new WarningTextLabel;
    pLimitShower->setColorForState("white", false);
    pLimitShower->setColorForState(QColor(235, 81, 96), true);
    pLimitShower->setState(false);
    QSizePolicy policy;
    policy.setRetainSizeWhenHidden(true);
    pLimitShower->setSizePolicy(policy);

    pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(noMargins);
    pLayout->addWidget(pTextEdit);
    pLayout->addWidget(pLimitShower , Qt::AlignRight);


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

MembersSection::MembersSection(QWidget* parent, ServerInfoProcessor& ServerInfoProcessor) : QScrollArea(parent) , processor(ServerInfoProcessor)
{
    setupUi();
    setObjectName("SelectorContactList");
}
void MembersSection::setContactList(std::vector<int> contactIdList)
{
    auto contactList = processor.findUsers(contactIdList);
    short dif = short(_viewList.size()) - short(contactList.size());
    short oldSize = (short)_viewList.size();
    if (dif < 0)
    {
        dif = -dif;
        _viewList.resize(oldSize + dif);
        auto e = _viewList.end();
        for (auto b = _viewList.begin() + oldSize; b != e; ++b)
        {
            *b = new ContactView();
            (*b)->setNameFont(StyleRepository::Base::standardFont());
            connect(*b, &ContactView::remove, this, [=]() {
                for (int i = 0; i < _viewList.size(); i++)
                {
                    if (*_viewList[i] == (**b))
                    {
                        (*b)->deleteLater();
                        _viewList.erase(_viewList.begin() + i);
                        break;
                    }
                }
                });

            pLayout->insertWidget(pLayout->count() - 1, *b);
        }

    }
    else if (dif > 0)
    {
        for (short i = oldSize - dif; i < oldSize; i++)
            _viewList[i]->setVisible(false);
    }
    for (short i = 0; i < contactList.size(); i++)
    {
        _viewList[i]->setVisible(true);
        _viewList[i]->setContactInfo(*contactList[i]);
    }

    _contactInfoList = contactList;
    pMembersLabel->setText("Members - " + QString::number(contactIdList.size() - 1));
}


void MembersSection::setupUi()
{
    setWidgetResizable(true);
    pWidget = new QWidget;
    pWidget->setObjectName("ListContainer");

    pMembersLabel = new QLabel;
    const int verticalMargin = 16;
    pMembersLabel->setContentsMargins(0, verticalMargin, 0, verticalMargin);
    pMembersLabel->setFont(StyleRepository::Base::membersHeaderFont());

    pLayout = new QVBoxLayout(pWidget);
    pLayout->addWidget(pMembersLabel);
    pLayout->addStretch(1);
    setWidget(pWidget);
}



BlockUI::BlockUI(QWidget* parent) : QWidget(parent)
{
    setupUi();
}
void BlockUI::setMode(Mode mode , ContactInfo* info , ServerInfoProcessor& processor)
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


ChatPage::ChatPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor) : Page(parent , ServerInfoProcessor)
{
    setupUi();
    signalsAndSlots();
    lastPrivateChat = nullptr;
    lastPrivateChatUser = nullptr;
    connect(&ServerInfoProcessor, &ServerInfoProcessor::unknownListReceived, this, [= , &ServerInfoProcessor]() {
        setChat(ServerInfoProcessor.firstChat());
        });

    lastChat = nullptr;
}

void ChatPage::updateName(const QString& str)
{
    pGroupName->setText(str);
}

void ChatPage::signalsAndSlots()
{
    connect(&pMessageBar->textEdit(), SIGNAL(messageCreated(const QString&, const QString&)), pChat, SLOT(addRecord(const QString&, const QString&)));
}
void ChatPage::setupUi()
{
    pMainLayout = new QVBoxLayout;
    pGroupName = new TitleTextEdit;
    pGroupName->setFont(StyleRepository::Base::headerFont());
    pGroupName->setShrinkToFit(true);
    pGroupName->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    connect(pGroupName, &TitleTextEdit::titleChanged, this, [=](QString str) {
        serverInfoProcessor.updateChatName(pChat->chatId(), str);
        });
    pChat = new Chat;


    pMessageBar = new ChatMessageBar(nullptr , serverInfoProcessor);
    pMessageBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred , QSizePolicy::MinimumExpanding));
    pMessageBar->textEdit().setShrinkToFit(true);
    pMessageBar->textEdit().setFont(StyleRepository::Base::chatFont());
    pMessageBar->textEdit().setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    pMessageBar->setCharacterLimit(characterLimit);

    pBlockUI = new BlockUI;

    pStackedWidget = new QStackedWidget;
    pStackedWidget->addWidget(pMessageBar);
    pStackedWidget->addWidget(pBlockUI);
    pStackedWidget->setCurrentWidget(pMessageBar);

    pMainLayout->addWidget(pGroupName , 5);
    pMainLayout->addWidget(pChat     , 75);
    pMainLayout->addWidget(pStackedWidget , 0);
    
    pMainLayout->addStretch(5);

    pMembersSection = new MembersSection(nullptr , serverInfoProcessor);
    pChatElementsLayout = new QHBoxLayout(this);
    pChatElementsLayout->addLayout(pMainLayout , 4);
    pChatElementsLayout->addWidget(pMembersSection, 1);
}
void ChatPage::setChat(ChatInfo& info)
{
    if (lastChat)
        disconnect(lastChat, &ChatInfo::nameChanged, this, &ChatPage::updateName);
    connect(lastChat, &ChatInfo::nameChanged, this, &ChatPage::updateName);

    if (info.isPrivate())
    {
        pGroupName->setReadOnly(true);
        BlockUI::Mode mode = BlockUI::Mode::Invalid;
        auto& list = info.members();
        int i = 0;
        if (list[i] == serverInfoProcessor.id())
            i = 1;
        if (lastPrivateChat)
        {
            disconnect(lastPrivateChat, &ChatInfo::gotBlocked, this, &ChatPage::onBlocking);
            disconnect(lastPrivateChat, &ChatInfo::blockedYou, this, &ChatPage::onGettingBlocked);
        }

        ContactInfo* contactInfo = serverInfoProcessor.findUser(list[i]);

        char flags = contactInfo->flags();
        if (flags & (char)ContactInfo::Status::HasBlockedYou)
            mode = BlockUI::Mode::YouAreBlocked;
        else if (flags & (char)ContactInfo::Status::IsBlocked)
            mode = BlockUI::Mode::YouBlocked;

        if (mode != BlockUI::Mode::Invalid)
        {
            pBlockUI->setMode(mode, contactInfo, serverInfoProcessor);
            pStackedWidget->setCurrentWidget(pBlockUI);
        }
        else
            pStackedWidget->setCurrentWidget(pMessageBar);

        lastPrivateChatUser = contactInfo;

        connect(&info, &ChatInfo::gotBlocked, this, &ChatPage::onBlocking);
        connect(&info, &ChatInfo::blockedYou, this, &ChatPage::onGettingBlocked);

        lastPrivateChat = &info;
    }
    else
    {
        pGroupName->setReadOnly(false);
        if (lastPrivateChat)
        {
            disconnect(lastPrivateChat, &ChatInfo::gotBlocked, this, &ChatPage::onBlocking);
            disconnect(lastPrivateChat, &ChatInfo::blockedYou, this, &ChatPage::onGettingBlocked);
        }
        lastPrivateChat = nullptr;
        pStackedWidget->setCurrentWidget(pMessageBar);
    }

    pGroupName->setTitle(info.name());
    pChat->setChatInfo(&info);
    pMembersSection->setContactList(info.members());
    lastChat = &info;
}

void ChatPage::onGettingBlocked(bool blocked)
{
    if (blocked)
    {
        pBlockUI->setMode(BlockUI::Mode::YouAreBlocked, lastPrivateChatUser, serverInfoProcessor);
        pStackedWidget->setCurrentWidget(pBlockUI);
    }
    else
        pStackedWidget->setCurrentWidget(pMessageBar);
}
void ChatPage::onBlocking(bool blocked)
{
    if (blocked)
    {
        pBlockUI->setMode(BlockUI::Mode::YouBlocked, lastPrivateChatUser, serverInfoProcessor);
        pStackedWidget->setCurrentWidget(pBlockUI);
    }
    else
        pStackedWidget->setCurrentWidget(pMessageBar);
}

void ChatPage::setChat(int id)
{
    setChat(serverInfoProcessor.getChatById(id));
}
TitleTextEdit& ChatPage::groupNameEdit(){ return *pGroupName;}

void ChatPage::createRoom()
{
}
