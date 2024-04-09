#include "ContactView.h"
#include "StyleBase/StyleRepository.h"
#include "StyleBase/ImagePainter.h"
#include "StyleBase/ButtonStyleRepository.h"

RequestOptions::RequestOptions(QWidget* parent) : QWidget(parent)
{
    setupUi();
}
CustomButton& RequestOptions::acceptBtn() { return *pAcceptBtn;}
CustomButton& RequestOptions::rejectBtn() { return *pRejectBtn;}

void RequestOptions::disconnectAll()
{
    pAcceptBtn->disconnect();
    pRejectBtn->disconnect();
}

void RequestOptions::setupUi()
{
    pAcceptBtn = new CustomButton(nullptr , ButtonStyleRepository::acceptButton());
    pRejectBtn = new CustomButton(nullptr , ButtonStyleRepository::rejectButton());

    pLayout = new QHBoxLayout(this);
    pLayout->addStretch(1);
    pLayout->addWidget(pAcceptBtn);
    pLayout->addWidget(pRejectBtn);
    pLayout->addStretch(1);
}


FriendOptions::FriendOptions(QWidget* parent ) : QWidget(parent)
{
    setupUi();
}
void FriendOptions::setupUi(){
    /*
    *     QHBoxLayout* pLayout;
    CustomButton* pSendMessageButton;
    CustomButton* pMoreOptionsBtn;
    QMenu* pMenu;
    QAction* removeAct;
    QAction* blockAct;
    QAction* callAct;
    */
    pSendMessageBtn = new CustomButton(nullptr , ButtonStyleRepository::messageButton()); 
    
    callAct = new QAction;
    callAct->setText("Call");
    callAct->setIcon(StyleRepository::MenuStyle::callPixmap());
    callAct->setFont(StyleRepository::MenuStyle::menuFont());

    blockAct = new QAction;
    blockAct->setIcon(StyleRepository::MenuStyle::blockPixmap());
    blockAct->setText("Block");
    blockAct->setFont(StyleRepository::MenuStyle::menuFont());

    removeAct = new QAction;
    removeAct->setText("Remove");
    removeAct->setIcon(StyleRepository::MenuStyle::removePixmap());
    removeAct->setFont(StyleRepository::MenuStyle::menuFont());

    pMenu = new QMenu;


    pMenu->addAction(callAct);
    pMenu->addAction(blockAct);
    pMenu->addAction(removeAct);

    pMenu->setStyleSheet(StyleRepository::MenuStyle::qMenuStyleSheet());


    pMoreOptionsBtn = new CustomButton(nullptr , ButtonStyleRepository::moreOptionsButton());
    pMoreOptionsBtn->setMenu(pMenu);

    pLayout = new QHBoxLayout(this);
    pLayout->addWidget(pSendMessageBtn);
    pLayout->addWidget(pMoreOptionsBtn);

}

void FriendOptions::disconnectAll()
{
    callAct->disconnect();
    removeAct->disconnect();
    blockAct->disconnect();
    pSendMessageBtn->disconnect();
}

CustomButton& FriendOptions::sendMessageBtn() { return *pSendMessageBtn; }
QAction& FriendOptions::removeAction() { return *removeAct; }
QAction& FriendOptions::blockAction() { return *blockAct; }
QAction& FriendOptions::callAction() { return *callAct; }

StrangerOptions::StrangerOptions(QWidget* parent ) : QWidget(parent)
{
    setupUi();
}
TwoStateButton& StrangerOptions::addFriendBtn() { return *pAddFriendButton;}

void StrangerOptions::setupUi()
{
    pAddFriendButton = new TwoStateButton(nullptr , ButtonStyleRepository::addFriendButton());

    pLayout = new QHBoxLayout(this);
    pLayout->addWidget(pAddFriendButton);
}

void StrangerOptions::disconnectAll()
{
    pAddFriendButton->disconnect();
}

ContactView::ContactView(QWidget* parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_Hover);

    pOptions = nullptr;
    QMargins noMargins = { 0 , 0 , 0 , 0};
    setContentsMargins(0 , 3 , 0 , 3);

    pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(noMargins);
    pProfilePicture = new ProfilePicLabel;
    pProfilePicture->setSizePolicy(QSizePolicy(QSizePolicy::Minimum , QSizePolicy::Minimum));

    pProfilePicture->setPixmap1(StyleRepository::Base::defaultPfpPixmap());

    pName = new QLabel;
    pName->setFont(StyleRepository::Base::standardFont());
    pName->setText("Username");

    pLayout->addWidget(pProfilePicture);
    pLayout->addWidget(pName);
    pLayout->addStretch(1);

    optionList.resize(int(SearchType::Count) , nullptr);

    setObjectName("ContactView");
}


void ContactView::setProfilePicture(QPixmap pixmap){
    pixmap = pixmap.scaled(iconSize);

    ImagePainter painter = ImagePainter(this , std::move(pixmap));
    painter.setColor(StyleRepository::Base::textColor());

    pProfilePicture->setPixmap1(QPixmap::fromImage(painter.image()));

}
void ContactView::setContactInfo(ContactInfo& info)
{
    pInfo = &info;
    pName->setText(pInfo->name());
    if(pInfo->name().isEmpty() == false)
        connect(pInfo, &ContactInfo::removed, this, [=]() {
            emit remove();
        });
}

void ContactView::setName(QString name) { pName->setText(std::move(name));}
void ContactView::setNameVisible(bool visible) { pName->setVisible(visible);}
QString ContactView::name() const { return pName->text();}
QPixmap ContactView::pixmap() const { return pProfilePicture->pixmap();}

void ContactView::attatchOptions(SearchType type , ServerInfoProcessor& processor, ChatPage& page, QStackedWidget& widget)
{
    if (pOptions)
        pOptions->setVisible(false);

    if (optionList[int(type)] == nullptr)
    {
        switch (type)
        {
        case SearchType::AddFriend:
            pOptions = new StrangerOptions;
            break;
        case SearchType::RequestList:
            pOptions = new RequestOptions;
            break;
        case SearchType::BlockedList:
            pOptions = new CustomButton(nullptr, ButtonStyleRepository::removeBlockedUserButton());
            break;
        case SearchType::FriendList:
            pOptions = new FriendOptions;
            break;
        }
        optionList[int(type)] = pOptions;
        pLayout->addWidget(pOptions);
        connectOptions(type, processor, page, widget);
    }
    else
    {
        pOptions = optionList[int(type)];
        connectOptions(type, processor, page, widget);
    }

    pOptions->setVisible(true);
}

void ContactView::connectOptions(SearchType type, ServerInfoProcessor& processor, ChatPage& page, QStackedWidget& widget)
{
    switch (type)
    {
    case SearchType::AddFriend:
        connectStrangerOptions(processor);
        break;
    case SearchType::RequestList:
        connectRequestOptions(processor);
        break;
    case SearchType::BlockedList:
        connectBlockedOptions(processor);
        break;
    case SearchType::FriendList:
        connectFriendOptions(processor, page, widget);
        break;
    }
}

void ContactView::connectFriendOptions(ServerInfoProcessor& processor, ChatPage& page, QStackedWidget& widget)
{
    FriendOptions* w = static_cast<FriendOptions*>(optionList[int(SearchType::FriendList)]);
    w->disconnectAll();
    CustomButton& messageBtn = w->sendMessageBtn();

    connect(&messageBtn, &CustomButton::clicked, this, [=, &processor, &page, &widget]() {
        ChatInfo* chat = processor.privateChatById(pInfo->id());
    if (chat == nullptr)
        processor.createPrivateChatWithFriend(pInfo->id());
    else
    {
        page.setChat(chat->id());
        widget.setCurrentWidget(&page);
    }
        });

    connect(&w->removeAction(), &QAction::triggered, this, [=, &processor]() {
        processor.removeFriend(pInfo->id());
    });
    connect(&w->blockAction(), &QAction::triggered, this, [=, &processor]() {
        processor.blockUser(pInfo->id());
        emit remove();
        });
}
void ContactView::connectStrangerOptions(ServerInfoProcessor& processor)
{
    StrangerOptions* w = static_cast<StrangerOptions*>(optionList[int(SearchType::AddFriend)]);
    w->disconnectAll();
    TwoStateButton& btn = w->addFriendBtn();
    if (pInfo->flags() & (char)ContactInfo::Status::HasRequest)
        btn.setActive(true);
    else
        connect(&btn, &TwoStateButton::clicked, this, [=, &processor, &btn]() {
        if (btn.isActive() == false)
        {
            processor.sendFriendRequest(pInfo->id());
            btn.setActive(true);
        }
        });
}
void ContactView::connectRequestOptions(ServerInfoProcessor& processor)
{
    RequestOptions* w = static_cast<RequestOptions*>(optionList[int(SearchType::RequestList)]);
    w->disconnectAll();
    connect(&w->acceptBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove();
        processor.manageFriendRequest(true, pInfo->id());
        processor.removeRequest(pInfo->id());
        pInfo = nullptr;
        deleteLater();
    });
    connect(&w->rejectBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove();
        processor.manageFriendRequest(false, pInfo->id());
        processor.removeRequest(pInfo->id());
        pInfo = nullptr;
        deleteLater();
    });
}
void ContactView::connectBlockedOptions(ServerInfoProcessor& processor)
{
    CustomButton* btn = static_cast<CustomButton*>(optionList[int(SearchType::BlockedList)]);
    btn->disconnect();

    connect(btn, &CustomButton::clicked, this, [=, &processor] {
        processor.unblockUser(pInfo->id());
        //emit remove();
    });
}


QWidget* ContactView::friendOptions(ServerInfoProcessor& processor, ChatPage& page, QStackedWidget& widget)
{
    FriendOptions* w = new FriendOptions;
    CustomButton& messageBtn = w->sendMessageBtn();

    connect(&messageBtn, &CustomButton::clicked, this, [=, &processor , &page , &widget]() {
        ChatInfo* chat = processor.privateChatById(pInfo->id());
    if (chat == nullptr)
        processor.createPrivateChatWithFriend(pInfo->id());
    else
    {
        page.setChat(chat->id());
        widget.setCurrentWidget(&page);
    }
    });

    connect(&w->removeAction(), &QAction::triggered, this, [=, &processor]() {
        processor.removeFriend(pInfo->id());
        });

    connect(&w->blockAction(), &QAction::triggered, this, [=, &processor]() {
        processor.blockUser(pInfo->id());
        });

    optionList[int(SearchType::FriendList)] = w;
    return w;
}
QWidget* ContactView::strangerOptions(ServerInfoProcessor& processor )
{
    StrangerOptions* w = new StrangerOptions;
    TwoStateButton& btn = w->addFriendBtn();
    if (pInfo->flags() & (char)ContactInfo::Status::HasRequest)
        btn.setActive(true);
    else
        connect(&btn, &TwoStateButton::clicked, this, [=, &processor, &btn]() {
        if (btn.isActive() == false)
        {
            processor.sendFriendRequest(pInfo->id());
            btn.setActive(true);
        }
        });
    optionList[int(SearchType::AddFriend)] = w;
    return w;
}
QWidget* ContactView::requestOptions(ServerInfoProcessor& processor)
{
    RequestOptions* w = new RequestOptions;
    connect(&w->acceptBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove();
        processor.manageFriendRequest(true, pInfo->id());
        processor.removeRequest(pInfo->id());
        pInfo = nullptr;
    deleteLater();
        });
    connect(&w->rejectBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove();
        processor.manageFriendRequest(false, pInfo->id());
        processor.removeRequest(pInfo->id());
        pInfo = nullptr;
        deleteLater();
        });
    optionList[int(SearchType::RequestList)] = w;
    return w;
}
QWidget* ContactView::blockedOptions(ServerInfoProcessor& processor)
{
    QWidget* w = new QWidget;
    optionList[int(SearchType::RequestList)] = w;
    return w;
}


bool ContactView::operator==(const ContactView& rhs)
{
    return pInfo->id() == rhs.pInfo->id();
}

