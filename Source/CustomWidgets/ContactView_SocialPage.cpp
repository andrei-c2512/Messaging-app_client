#include "ContactView_SocialPage.h"
#include "StyleBase/ButtonStyleRepository.h"
#include "StyleBase/StyleRepository.h"

RequestOptions::RequestOptions(QWidget* parent) : QWidget(parent)
{
    setupUi();
}
CustomButton& RequestOptions::acceptBtn() { return *pAcceptBtn; }
CustomButton& RequestOptions::rejectBtn() { return *pRejectBtn; }

void RequestOptions::disconnectAll()
{
    pAcceptBtn->disconnect();
    pRejectBtn->disconnect();
}

void RequestOptions::setupUi()
{
    pAcceptBtn = new CustomButton(nullptr, ButtonStyleRepository::acceptButton());
    pRejectBtn = new CustomButton(nullptr, ButtonStyleRepository::rejectButton());

    pLayout = new QHBoxLayout(this);
    pLayout->addStretch(1);
    pLayout->addWidget(pAcceptBtn);
    pLayout->addWidget(pRejectBtn);
    pLayout->addStretch(1);
}


FriendOptions::FriendOptions(QWidget* parent) : QWidget(parent)
{
    setupUi();
}
void FriendOptions::setupUi() {
    /*
    *     QHBoxLayout* pLayout;
    CustomButton* pSendMessageButton;
    CustomButton* pMoreOptionsBtn;
    QMenu* pMenu;
    QAction* removeAct;
    QAction* blockAct;
    QAction* callAct;
    */
    pSendMessageBtn = new CustomButton(nullptr, ButtonStyleRepository::messageButton());

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


    pMoreOptionsBtn = new CustomButton(nullptr, ButtonStyleRepository::moreOptionsButton());
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

StrangerOptions::StrangerOptions(QWidget* parent) : QWidget(parent)
{
    setupUi();
}
TwoStateButton& StrangerOptions::addFriendBtn() { return *pAddFriendButton; }

void StrangerOptions::setupUi()
{
    pAddFriendButton = new TwoStateButton(nullptr, ButtonStyleRepository::addFriendButton());

    pLayout = new QHBoxLayout(this);
    pLayout->addWidget(pAddFriendButton);
}

void StrangerOptions::disconnectAll()
{
    pAddFriendButton->disconnect();
}

ContactView_SocialPage::ContactView_SocialPage(QWidget* parent) : ContactView(parent) {
    pOptions = nullptr;
    optionList.resize(int(SearchType::Count), nullptr);
    pLayout->addStretch(1);
}
void ContactView_SocialPage::attatchOptions(SearchType type, ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget)
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
    _searchType = type;
    pOptions->setVisible(true);
}

void ContactView_SocialPage::connectOptions(SearchType type, ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget)
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

void ContactView_SocialPage::connectFriendOptions(ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget)
{
    FriendOptions* w = static_cast<FriendOptions*>(optionList[int(SearchType::FriendList)]);
    w->disconnectAll();
    CustomButton& messageBtn = w->sendMessageBtn();

    connect(&messageBtn, &CustomButton::clicked, this, [=, &processor, &page, &widget]() {
        ChatInfo* chat = processor.storage().privateChatById(pInfo->id());
    if (chat == nullptr)
        processor.requestSender().createPrivateChatWithFriend(pInfo->id());
    else
    {
        page.setChat(chat->id());
        widget.setCurrentWidget(&page);
    }
        });

    connect(&w->removeAction(), &QAction::triggered, this, [=, &processor]() {
        processor.requestSender().removeFriend(pInfo->id());
        });
    connect(&w->blockAction(), &QAction::triggered, this, [=, &processor]() {
        processor.requestSender().blockUser(pInfo->id());
        emit remove(pInfo->id());
        });
}
void ContactView_SocialPage::connectStrangerOptions(ConnectionHandler& processor)
{
    StrangerOptions* w = static_cast<StrangerOptions*>(optionList[int(SearchType::AddFriend)]);
    w->disconnectAll();
    TwoStateButton& btn = w->addFriendBtn();
    if (pInfo->flags() & ContactInfo::Status::HasRequest)
        btn.setActive(true);
    else
        connect(&btn, &TwoStateButton::clicked, this, [=, &processor, &btn]() {
        if (btn.isActive() == false)
        {
            processor.requestSender().sendFriendRequest(pInfo->id());
            btn.setActive(true);
        }
            });
}
void ContactView_SocialPage::connectRequestOptions(ConnectionHandler& processor)
{
    RequestOptions* w = static_cast<RequestOptions*>(optionList[int(SearchType::RequestList)]);
    w->disconnectAll();
    connect(&w->acceptBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove(pInfo->id());
    processor.requestSender().manageFriendRequest(true, pInfo->id());
    processor.storage().removeRequest(pInfo->id());
    pInfo = nullptr;
    deleteLater();
        });
    connect(&w->rejectBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove(pInfo->id());
    processor.requestSender().manageFriendRequest(false, pInfo->id());
    processor.storage().removeRequest(pInfo->id());
    pInfo = nullptr;
    deleteLater();
        });
}
void ContactView_SocialPage::connectBlockedOptions(ConnectionHandler& processor)
{
    CustomButton* btn = static_cast<CustomButton*>(optionList[int(SearchType::BlockedList)]);
    btn->disconnect();

    connect(btn, &CustomButton::clicked, this, [=, &processor] {
        processor.requestSender().unblockUser(pInfo->id());
    //emit remove();
        });
}


QWidget* ContactView_SocialPage::friendOptions(ConnectionHandler& processor, ChatPage& page, QStackedWidget& widget)
{
    FriendOptions* w = new FriendOptions;
    CustomButton& messageBtn = w->sendMessageBtn();

    connect(&messageBtn, &CustomButton::clicked, this, [=, &processor, &page, &widget]() {
        ChatInfo* chat = processor.storage().privateChatById(pInfo->id());
        if (chat == nullptr)
            processor.requestSender().createPrivateChatWithFriend(pInfo->id());
        else
        {
            page.setChat(chat->id());
            widget.setCurrentWidget(&page);
        }
        });

    connect(&w->removeAction(), &QAction::triggered, this, [=, &processor]() {
        processor.requestSender().removeFriend(pInfo->id());
        });

    connect(&w->blockAction(), &QAction::triggered, this, [=, &processor]() {
        processor.requestSender().blockUser(pInfo->id());
        });

    optionList[int(SearchType::FriendList)] = w;
    return w;
}
QWidget* ContactView_SocialPage::strangerOptions(ConnectionHandler& processor)
{
    StrangerOptions* w = new StrangerOptions;
    TwoStateButton& btn = w->addFriendBtn();
    if (pInfo->flags() & ContactInfo::Status::HasRequest)
        btn.setActive(true);
    else
        connect(&btn, &TwoStateButton::clicked, this, [=, &processor, &btn]() {
        if (btn.isActive() == false)
        {
            processor.requestSender().sendFriendRequest(pInfo->id());
            btn.setActive(true);
        }
            });
    optionList[int(SearchType::AddFriend)] = w;
    return w;
}
QWidget* ContactView_SocialPage::requestOptions(ConnectionHandler& processor)
{
    RequestOptions* w = new RequestOptions;
    connect(&w->acceptBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove(pInfo->id());
        processor.requestSender().manageFriendRequest(true, pInfo->id());
        processor.storage().removeRequest(pInfo->id());
        pInfo = nullptr;
        deleteLater();
        });
    connect(&w->rejectBtn(), &CustomButton::clicked, this, [=, &processor]() {
        emit remove(pInfo->id());
        processor.requestSender().manageFriendRequest(false, pInfo->id());
        processor.storage().removeRequest(pInfo->id());
        pInfo = nullptr;
        deleteLater();
    });
    optionList[int(SearchType::RequestList)] = w;
    return w;
}
QWidget* ContactView_SocialPage::blockedOptions(ConnectionHandler& processor)
{
    QWidget* w = new QWidget;
    optionList[int(SearchType::RequestList)] = w;
    return w;
}
