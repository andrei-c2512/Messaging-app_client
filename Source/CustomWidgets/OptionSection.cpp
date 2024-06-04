#include "OptionSection.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ButtonStyleRepository.h"
#include "Tools.h"
#include "Network/AccountDataStructures/NullInfo.h"

bool imgInit = false;
static const QSize exclamationImgSize = QSize(32, 32);
static QImage exclamationImage;

ChatView::ChatView(QWidget* parent , const ChatInfo& info) : QWidget(parent) , pInfo(&info)
{
    _needsToBeDeleted = false;
    setAttribute(Qt::WA_StyledBackground);
    setupUi();
    _active = false;
    _highlighted = false;
    //have to do it like this because otherwise the program will crash. I can't make a pixmap without the QApplication being built
    if (imgInit == false)
    {
        exclamationImage = ImagePainter::paintImage(QPixmap(":/Images/Images/Icons/ExclamationMark.png").scaled(exclamationImgSize), "yellow");
    }
    setChatInfo(info);

}

void ChatView::setChatInfo(const ChatInfo& info)
{
    pInfo = &info;
    connect(pInfo , &ChatInfo::destroyed , this , [=](){
        pInfo = &NullInfo::instance().nullChat();
        _needsToBeDeleted = true;
        emit chatDeleted();
    });
    connect(pInfo, &ChatInfo::newMessageAdded, this, [=](MessageInfo& info) {
        Q_UNUSED(info);
        if (_active == false)
            setHighlighted(true);
    });
    connect(pInfo, &ChatInfo::nameChanged, this, [=](const QString& str) {
        pName->setText(str);
    });
    connect(pInfo, &ChatInfo::memberRemoved, this, [=](int id) {

    });
    connect(pInfo, &ChatInfo::removed, this, [=]() {
        emit chatDeleted();
    });
    const MessageInfo& back = info.back();
    pName->setText(info.name());
    QString shownMessage = back.name() + ':';
    QString lastChatMessage = back.text();
    int shownCharacters = maxCharShowcase - shownMessage.length();


    if(shownCharacters < lastChatMessage.length())
    {
        shownMessage.append(lastChatMessage.mid(0 , shownCharacters));
        shownMessage.append("...");
    }
    else
    {
        shownMessage.append(lastChatMessage);
    }
    pLastMessage->setText(std::move(shownMessage));
}

void ChatView::setupUi()
{
    QMargins noMargins(0, 0, 0, 0);

    pPfpLabel = new ProfilePicLabel;
    QPixmap pixmap = QPixmap(":/Images/Images/Icons/OpenEye.png");
    pixmap = pixmap.scaled(QSize(32 , 32));

    ImagePainter painter(this , pixmap);
    painter.setColor(StyleRepository::Base::textColor());
    pPfpLabel->setPixmap1(QPixmap::fromImage(painter.image()));

    pName = new QLabel;
    pName->setContentsMargins(noMargins);
    pName->setFont(StyleRepository::Base::chatViewTitleFont());

    pLastMessage = new QLabel;
    pLastMessage->setFont(StyleRepository::Base::chatViewMessageFont());
    pLastMessage->setContentsMargins(noMargins);

    pTextLayout = new QVBoxLayout;
    pTextLayout->setSpacing(0);
    pTextLayout->setContentsMargins(0, 0, 0, 0);
    pTextLayout->addWidget(pName);
    pTextLayout->addWidget(pLastMessage);
    pTextLayout->setContentsMargins(noMargins);

    pNewMessageLabel = new QLabel(this);
    pNewMessageLabel->setPixmap(QPixmap::fromImage(exclamationImage));
    pNewMessageLabel->setGeometry(QRect(QPoint(), exclamationImgSize));
    pNewMessageLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    pNewMessageLabel->setVisible(false);

    pLayout = new QHBoxLayout(this);
    pLayout->addWidget(pPfpLabel);
    pLayout->addLayout(pTextLayout);
    pLayout->addStretch(1);
}

bool ChatView::needsToBeDeleted() { return _needsToBeDeleted;}
bool ChatView::isActive() const { return _active; }

void ChatView::setActive(bool active) { 
    _active = active; 
    if (_active)
        setHighlighted(false);
}
void ChatView::setTextVisibile(bool visible)
{
    pName->setVisible(visible);
    pLastMessage->setVisible(visible);
    /*if (visible)
        pNewMessageLabel->setVisible(true);
    else
        pNewMessageLabel->setVisible(false);*/

}
void ChatView::setHighlighted(bool status) 
{ 
   _highlighted = status; 
   pNewMessageLabel->setVisible(status);
}

void ChatView::mousePressEvent(QMouseEvent* ev)
{
    if (_highlighted)
    {
        _highlighted = false;
        pNewMessageLabel->setVisible(false);
    }
    emit clicked(pInfo->id());
    QWidget::mousePressEvent(ev);
}

void ChatView::resizeEvent(QResizeEvent* ev)
{
    QSize newSize = ev->size();
    //pNewMessageLabel->setGeometry(QRect(QPoint(newSize.width() - pNewMessageLabel->width(), newSize.height() - pNewMessageLabel->height()), pNewMessageLabel->size()));
    QWidget::resizeEvent(ev);
}



ChatListWidget::ChatListWidget(QWidget* parent , const ConnectionHandler& infoProcessor0 , SubpageManager& manager ,  Type type0)
    :QWidget(parent) , infoProcessor(infoProcessor0) , subpageManager(manager) ,  type(type0)
{
    setAttribute(Qt::WA_StyledBackground);

    chatInfo_list = (type == ChatListWidget::Type::ChatList) ? &infoProcessor.storage().chatList() : &infoProcessor.storage().requestList();
    calculatePlaces();
    connect(&infoProcessor.handler(), &ResponseHandler::newChatData, this, [this]() {
        auto& obj = *this;
        obj.chatInfo_list = (obj.type == ChatListWidget::Type::ChatList) ? &obj.infoProcessor.storage().chatList() : &obj.infoProcessor.storage().requestList();
        obj.rearrange();
    });
    connect(&infoProcessor.handler(), &ResponseHandler::createdNewChat, this, [=, &manager](int id) {
        ChatInfo* info = Tools::binaryIdSearch(infoProcessor.storage().chatList(), id);

        ChatView* v = new ChatView(nullptr , *info);
        chatView_list.emplace_back(v);
        pScrollAreaLayout->insertWidget(0 , v);
        manager.chatPage().setChat(info->id());
        manager.setPage(SubpageManager::Type::Chat);

        connect(v, &ChatView::clicked, &subpageManager, [=](int id) {
            subpageManager.chatPage().setChat(id);
        subpageManager.setPage(SubpageManager::Type::Chat);
        v->setActive(true);
        if (lastChatView)
            lastChatView->setActive(false);

        lastChatView = v;
         });

    });
    connect(&infoProcessor.handler(), &ResponseHandler::addedToNewChat, this, [=](int id) {
        ChatInfo* info = Tools::binaryIdSearch(infoProcessor.storage().chatList(), id);

        ChatView* v = new ChatView(nullptr, *info);
        chatView_list.emplace_back(v);
        pScrollAreaLayout->insertWidget(0, v);
        v->setHighlighted(true);
        connect(v, &ChatView::clicked, &subpageManager, [=](int id) {
            subpageManager.chatPage().setChat(id);
        subpageManager.setPage(SubpageManager::Type::Chat);
        v->setActive(true);
        if (lastChatView)
            lastChatView->setActive(false);

        lastChatView = v;
            });
        });

    setupUi();
}
/*void ChatListWidget::setServerInfoProcessor(const ServerInfoProcessor& infoProcessor){
    pInfoProcessor = infoProcessor;
    chatInfo_list = (type == ChatListWidget::Type::ChatList) ? &pInfoProcessor.chatList() : &pInfoProcessor.requestList();
    calculatePlaces();
    connect(&pInfoProcessor, &ServerInfoProcessor::newChatData , this , [this](){
        auto& obj = *this;
        obj.chatInfo_list = (obj.type == ChatListWidget::Type::ChatList) ? &obj.pInfoProcessor.chatList() : &obj.pInfoProcessor.requestList();
        obj.rearrange();
    });
}*/

void ChatListWidget::removeUnnecesaryChatView()
{
    auto end = chatView_list.end();
    for(auto begin = chatView_list.begin() ; begin != end ; begin++)
    {
        ChatView* pView = *begin;
        if(pView->needsToBeDeleted())
        {
            pView->deleteLater();
            chatView_list.erase(begin);
            break;
        }
    }
}

void ChatListWidget::calculatePlaces()
{
    //a list of the last message's timestamp in every chat
    std::vector<std::pair<QDateTime,int>> lastTimestamp_list(chatInfo_list->size());
    chatOrder.resize(chatInfo_list->size());
    for(unsigned i = 0 ; i < chatInfo_list->size() ; i++)
    {
        //beware of the dereferencing of the array
        lastTimestamp_list[i] = {(*chatInfo_list)[i]->back().timestamp() , i};
    }
    std::sort(std::begin(lastTimestamp_list) , std::end(lastTimestamp_list) , [](std::pair<QDateTime,int>& lhs ,std::pair<QDateTime,int>& rhs){
        return lhs.first < rhs.first;
    });

    for(unsigned i = 0 ; i < chatInfo_list->size() ; i++)
        chatOrder[i] = lastTimestamp_list[i].second;
}
void ChatListWidget::addViews(int n)
{
    while(n)
    {
        ChatView* pView = new ChatView(nullptr , NullInfo::instance().nullChat());
        connect(pView, &ChatView::chatDeleted, this, [=]() {
            removeUnnecesaryChatView();
            if (pView->isActive())
            {
                lastChatView = nullptr;
            }
        });;

        chatView_list.emplace_back(pView);
        pScrollAreaLayout->insertWidget(0 , pView);
        n--;
        connect(pView , &ChatView::clicked , &subpageManager , [=](int id){
            subpageManager.chatPage().setChat(id);
            subpageManager.setPage(SubpageManager::Type::Chat);
            pView->setActive(true);
            if(lastChatView)
                lastChatView->setActive(false);
            
        });
    }
}
void ChatListWidget::removeViews(int n)
{
    n = (n > chatView_list.size()) ? chatView_list.size() : n;
    while(n)
    {
        chatView_list[0]->deleteLater();
        chatView_list.erase(chatView_list.begin());
        n--;
    }
}

void ChatListWidget::rearrange()
{
    //making the number of views equal to the number of chats
    if(chatInfo_list->size() > chatView_list.size())
        addViews(int(chatInfo_list->size() - chatView_list.size()));

    else if(chatInfo_list->size() < chatView_list.size())
        removeViews(int(chatView_list.size() - chatInfo_list->size()));

    //calculates the array that holds the indexes of the most recent chats in order
    calculatePlaces();
    //insert the data into the chat views
    const std::vector<ChatInfo*>& list = (*chatInfo_list);
    for(int i = 0 ; i < chatView_list.size() ; i++)
    {
        int chatInfoIndex = chatOrder[i];
        chatView_list[i]->setChatInfo(*list[chatInfoIndex]);
    }

    pEmptyListText->setVisible(!chatView_list.size());
}

void ChatListWidget::setupUi()
{
    QMargins noMargins = { 0 , 0 , 0  ,0};
    setContentsMargins(noMargins);
    pEmptyListText = new QLabel;
    pEmptyListText->setFont(StyleRepository::Base::chatFont());
    pEmptyListText->setText(
        (type == Type::ChatList) ? "Looks like you don't have anyone to talk to" : "No one wants to talk to you"
        );
    pEmptyListText->setWordWrap(true);

    pScrollAreaLayout = new QVBoxLayout(this);
    pScrollAreaLayout->setContentsMargins(noMargins);
    pScrollAreaLayout->addWidget(pEmptyListText);
    addViews(chatInfo_list->size());
    for(ChatView* view : chatView_list)
        pScrollAreaLayout->addWidget(view);

    pScrollAreaLayout->addStretch(1);

    if(!chatView_list.size())
        pEmptyListText->setVisible(false);
}

void ChatListWidget::setTextVisible(bool visible)
{
    for(ChatView* view : chatView_list)
        view->setTextVisibile(visible);

    if(visible)
        pEmptyListText->setVisible(!chatView_list.size());
    else
        pEmptyListText->setVisible(false);
}


ChatListView::ChatListView(QWidget* widget , const ConnectionHandler& processor , SubpageManager& manager)
    :QWidget(widget) , serverProcessor(processor) , subpageManager(manager)
{
    setAttribute(Qt::WA_StyledBackground);
    setupUi();
}

void ChatListView::setToChatList(){pStackedWidget->setCurrentWidget(pChatList);}
void ChatListView::setToRequestList(){pStackedWidget->setCurrentWidget(pRequestList);}

void ChatListView::setupUi()
{
    QMargins noMargins = { 0 , 0 , 0  ,0};
    setContentsMargins(noMargins);
    pChatList = new ChatListWidget(nullptr , serverProcessor , subpageManager ,  ChatListWidget::Type::ChatList);
    pRequestList = new ChatListWidget(nullptr , serverProcessor , subpageManager , ChatListWidget::Type::RequestList);

    pStackedWidget = new QStackedWidget;
    pStackedWidget->setContentsMargins(noMargins);
    pStackedWidget->addWidget(pChatList);
    pStackedWidget->addWidget(pRequestList);

    pScrollArea = new QScrollArea;
    pScrollArea->setContentsMargins(noMargins);
    pScrollArea->setWidgetResizable(true);
    pScrollArea->setWidget(pStackedWidget);

    pScrollArea  ->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    //pScrollArea->horizontalScrollBar()->setEnabled(false);
    pScrollArea->setStyleSheet(StyleRepository::Base::qSliderStyle());

    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pLayout->addWidget(pScrollArea);
}
void ChatListView::setTextVisible(bool visible)
{
    if(pStackedWidget->currentIndex() == 0)
        pChatList->setTextVisible(visible);
    else
        pRequestList->setTextVisible(visible);
}
void ChatListView::setList(ChatListWidget::Type type)
{
    if(type == ChatListWidget::Type::ChatList)
        pStackedWidget->setCurrentWidget(pChatList);
    else if(type == ChatListWidget::Type::RequestList)
        pStackedWidget->setCurrentWidget(pRequestList);
}

OptionSection::OptionSection(QWidget* parent , const ConnectionHandler& processor , SubpageManager& subpageManager0 , UserSelectorWidget& userSelectorWidget)
    :QWidget(parent) , serverProcessor(processor) , subpageManager(subpageManager0)
{
    setAttribute(Qt::WA_Hover);
    setAttribute(Qt::WA_StyledBackground);
    setupUi(userSelectorWidget);
    connectButtons();
    _minimumWidth = 72;
    _maximumWidth = 256;
    const int duration = 250;
    pAnimation = new QVariantAnimation(this);
    pAnimation->setStartValue(_minimumWidth);
    pAnimation->setEndValue(_maximumWidth);
    pAnimation->setEasingCurve(QEasingCurve::OutQuart);
    pAnimation->setDuration(duration);

    connect(pAnimation , &QVariantAnimation::valueChanged , this , [this](const QVariant& var){
        QRect rect = this->rect();
        rect.setWidth(var.toInt());
        this->setGeometry(std::move(rect));
    });
    connect(&serverProcessor.handler(), &ResponseHandler::signInAccepted, this, [this]() {
        auto& t = *this;
        t.pProfileView->setName(t.serverProcessor.storage().name());
    });
}

bool OptionSection::event(QEvent* ev)
{
    auto type = ev->type();
    switch(type)
    {
    case QEvent::HoverEnter:
        setWidgetsVisible(true);
        pAnimation->setDirection(QAbstractAnimation::Direction::Forward);
        pAnimation->setDuration(250);
        pAnimation->start();
        break;
    case QEvent::HoverLeave:
        setWidgetsVisible(false);
        pAnimation->setDirection(QAbstractAnimation::Direction::Backward);
        pAnimation->setDuration(0);
        pAnimation->start();
        break;
    default:
        break;
    }

    return QWidget::event(ev);
}
void OptionSection::setWidgetsVisible(bool visible)
{
    pChatsButton->setVisible(visible);
    pRequestsButton->setVisible(visible);
    pDotLabel->setVisible(!visible);
    pProfileView->setNameVisible(visible);
    pConversationHeader->setVisible(visible);
    pListView->setTextVisible(visible);
    pSocialButton->setTextVisibility(visible);
    pMessageButton->setVisible(visible);

}
void OptionSection::setupUi(UserSelectorWidget& userSelectorWidget)
{

    pChatsButton = new SelectableButton;
    pChatsButton->setFont(StyleRepository::Base::mediumSizeButtonFont());
    pChatsButton->setText("Chats");

    pRequestsButton = new SelectableButton;
    pRequestsButton->setFont(StyleRepository::Base::mediumSizeButtonFont());
    pRequestsButton->setText("Requests");


    pMessageButton = new CustomButton(nullptr , ButtonStyleRepository::plusButton());
    connect(pMessageButton, &CustomButton::clicked, this, [=, &userSelectorWidget]() {
        userSelectorWidget.setRole(UserSelectorWidget::Role::MakeGroupChat);
        userSelectorWidget.flip(Tools::windowPos(pMessageButton) + QPoint(pMessageButton->width() , pMessageButton->height()));
        });

    pDotLabel = new QLabel;
    QPixmap dotPix = QPixmap::fromImage(ImagePainter::paintImage(QPixmap(":/Images/Images/Icons/DotIcon.png"), QColor("white")));
    int btnDim = pRequestsButton->sizeHint().height();
    dotPix = dotPix.scaled(btnDim, btnDim);

    pDotLabel->setPixmap(dotPix);
    pDotLabel->setVisible(false);
    pButtonLayout = new QHBoxLayout;
    pButtonLayout->addWidget(pChatsButton);
    pButtonLayout->addWidget(pRequestsButton);
    pButtonLayout->addWidget(pMessageButton);
    pButtonLayout->addWidget(pDotLabel);
    pButtonLayout->addStretch(1);


    pConversationHeader = new QLabel;
    pConversationHeader->setText("Messages");
    pConversationHeader->setFont(StyleRepository::Base::optionSectionHeaderFont());

    pConversationIcon = new QLabel;
    QPixmap convPixmap = QPixmap(":/Images/Images/Icons/ChatListIcon.png");
    QSize btnSizeHint = pRequestsButton->sizeHint();
    convPixmap = convPixmap.scaled(QSize(btnSizeHint.height(), btnSizeHint.height()));
    ImagePainter painter(this, convPixmap);
    painter.setColor(StyleRepository::Base::textColor());

    pConversationIcon->setPixmap(QPixmap::fromImage(painter.image()));

    pConversationHeaderLayout = new QHBoxLayout;
    pConversationHeaderLayout->addWidget(pConversationIcon);
    pConversationHeaderLayout->addWidget(pConversationHeader);
    pConversationHeaderLayout->addStretch(1);

    pProfileView = new ProfileView;
    //pProfileView->setProfilePicture(QPixmap(":/images/defaultPfp.png"));
    pProfileView->setName(serverProcessor.storage().name());

    pListView = new ChatListView(nullptr , serverProcessor , subpageManager);


    pSocialButton = new CustomButton(nullptr , ButtonStyleRepository::friendListButton());
    pSocialButton->setFont(StyleRepository::Base::headerFont());
    pSocialButton->setText("Social");

    connect(pSocialButton , &CustomButton::clicked , this , [=](){
        subpageManager.setPage(SubpageManager::Type::Social);
    });
    pMainLayout = new QVBoxLayout(this);
    pMainLayout->addWidget(pProfileView);
    pMainLayout->addStretch(1);
    pMainLayout->addLayout(pConversationHeaderLayout);
    pMainLayout->addLayout(pButtonLayout);
    pMainLayout->addWidget(pListView , 2);
    pMainLayout->addStretch(1);
    pMainLayout->addWidget(pSocialButton);
    pMainLayout->addStretch(1);

    setWidgetsVisible(false);
}
int OptionSection::minimumWidth() const { return _minimumWidth;}
int OptionSection::maximumWidth() const { return _maximumWidth;}

void OptionSection::connectButtons() const
{
    connect(pRequestsButton , &SelectableButton::clicked , this , [this](int n){
            Q_UNUSED(n);
            auto& t = *this;
            t.pListView->setList(ChatListWidget::Type::RequestList);
    });
    connect(pChatsButton , &SelectableButton::clicked , this , [this](int n){
        Q_UNUSED(n);
        auto& t = *this;
        t.pListView->setList(ChatListWidget::Type::ChatList);
    });
}

OptionSectionFrame::OptionSectionFrame(QWidget* parent ,  const ConnectionHandler& processor , SubpageManager& pSubpageManager0 , UserSelectorWidget& userSelectorWidget)
    :QWidget(parent)
{
    pSection = new OptionSection(this , processor , pSubpageManager0 , userSelectorWidget);
    pSection->setGeometry(QRect(0 , 0 , pSection->minimumWidth() , height()));
}

void OptionSectionFrame::resizeEvent(QResizeEvent* event)
{
    auto size = event->size();
    qDebug() <<  pSection->width() << ' ' << size.height();
    pSection->setGeometry(QRect(0 , 0 , pSection->width() , size.height()));
    QWidget::resizeEvent(event);
}

