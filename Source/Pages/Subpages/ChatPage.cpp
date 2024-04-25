#include "ChatPage.h"
#include "StyleBase/StyleRepository.h"
#include "Tools.h"


MembersSection::MembersSection(QWidget* parent, ServerInfoProcessor& ServerInfoProcessor) : QScrollArea(parent) , processor(ServerInfoProcessor)
{
    setupUi();
    setObjectName("SelectorContactList");
}
void MembersSection::setContactList(std::vector<int> contactIdList, ServerInfoProcessor& processor, Chat& page,  int adminId)
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
            *b = new ContactView_MembersSection;
            (*b)->setNameFont(StyleRepository::Base::standardFont());
            connect(*b, &ContactView_MembersSection::remove, this, [=](int id) {
                for (int i = 0; i < _viewList.size(); i++)
                {
                    if (_viewList[i]->contactInfo()->id() == id)
                    {
                        _viewList[i]->deleteLater();
                        _viewList.erase(_viewList.begin() + i);
                        break;
                    }
                }
                });
            connect(*b, &ContactView_MembersSection::switchToPrivateChat, this, [=, &processor, &page](int id) {
                setContactList({ id }, processor, page, adminId);
                });
            pLayout->insertWidget(pLayout->count() - 1, *b);
        }

    }
    else if (dif > 0)
    {
        for (short i = oldSize - dif; i < oldSize; i++)
            _viewList[i]->setVisible(false);
    }
    pUserView->setContactInfo(processor);
    GroupMemberOptions::Features init = GroupMemberOptions::Features::Null;
    if (adminId == processor.id())
    {
        init = GroupMemberOptions::Features::YouAreAdmin;
        pUserView->setIsAdmin(true);
    }
    else
        pUserView->setIsAdmin(false);

    pUserView->setVisible(true);
    for (short i = 0; i < contactList.size(); i++)
    {
        GroupMemberOptions::Features flags = init;
        _viewList[i]->setVisible(true);
        _viewList[i]->setContactInfo(*contactList[i]);

        if(adminId == contactList.at(i)->id())
            _viewList[i]->setIsAdmin(true);
        else
            _viewList[i]->setIsAdmin(false);
     
        ContactInfo::ContactStatus status = contactList[i]->flags();

        _viewList[i]->attatchOptions(processor, page, flags);
    }

    _contactInfoList = contactList;
    pMembersLabel->setText("Members - " + QString::number(contactIdList.size() + 1));
}

void MembersSection::removeContact(int id)
{
    for (int i = 0; i < _viewList.size(); i++)
    {
        if (_viewList[i]->contactInfo()->id() == id)
        {
            _viewList[i]->deleteLater();
            _viewList.erase(_viewList.begin() + i);
            _contactInfoList.erase(_contactInfoList.begin() + i);

            pMembersLabel->setText("Members - " + QString::number(_contactInfoList.size() + 1));
            break;
        }
    }
}

void MembersSection::removeUserView() { pUserView->setVisible(false); }
std::vector<ContactInfo*> MembersSection::contactList() const { return _contactInfoList; }

void MembersSection::setupUi()
{
    setWidgetResizable(true);
    pWidget = new QWidget;
    pWidget->setObjectName("ListContainer");

    pMembersLabel = new QLabel;
    const int verticalMargin = 16;
    pMembersLabel->setContentsMargins(0, verticalMargin, 0, verticalMargin);
    pMembersLabel->setFont(StyleRepository::Base::membersHeaderFont());

    pUserView = new ContactView_MembersSection;

    pLayout = new QVBoxLayout(pWidget);
    pLayout->addWidget(pMembersLabel);
    pLayout->addWidget(pUserView);
    pLayout->addStretch(1);

    setWidget(pWidget);
}

void MembersSection::setAdmin(int adminId)
{
    for (ContactView_MembersSection* view : _viewList)
    {
        if (view->contactInfo()->id() == adminId)
            view->setIsAdmin(true);
        else
            view->setIsAdmin(false);
    }
}



ChatPage::ChatPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor , UserSelectorWidget& widget) : Page(parent , ServerInfoProcessor) , userSelector(widget)
{
    setupUi();
    pInfo = nullptr;
}

void ChatPage::setupUi()
{
    pMembersSection = new MembersSection(nullptr, serverInfoProcessor);

    pAddButton = new QAction;
    pAddButton->setIcon(StyleRepository::ToolBar::addPixmap());
    connect(pAddButton, &QAction::triggered, this, [=]() {
        userSelector.setChatId(pInfo->id());
        userSelector.setRole(UserSelectorWidget::Role::AddToGroupChat);
        userSelector.flip(Tools::windowPos(pToolBar) + QPoint(0, pToolBar->height()) ,
        ContactInfo::subtractFromList(serverInfoProcessor.friendList() , pMembersSection->contactList()));
    });

    pLeaveButton = new QAction;
    pLeaveButton->setIcon(StyleRepository::ToolBar::leavePixmap());
    
    connect(pLeaveButton, &QAction::triggered, this, [=]() {
        serverInfoProcessor.leaveFromGroup(pInfo->id());
    });

    QWidget* spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pToolBar = new QToolBar;
    pToolBar->addAction(pAddButton);
    pToolBar->addWidget(spacer);
    pToolBar->addAction(pLeaveButton);

    pVerticalLayout = new QVBoxLayout;
    pVerticalLayout->addWidget(pToolBar);
    pVerticalLayout->addWidget(pMembersSection);

    pMainLayout = new QHBoxLayout(this);
    pChat = new Chat(nullptr , serverInfoProcessor);

    pMainLayout->addWidget(pChat, 4);
    pMainLayout->addLayout(pVerticalLayout, 1);
}
void ChatPage::onBeingRemoved(bool forcefullyremoved)
{
    pChat->onGettingRemoved(forcefullyremoved);
    pMembersSection->removeUserView();
}

void ChatPage::onMemberRemoval(int id)
{
    pMembersSection->removeContact(id);
}
void ChatPage::onMemberAddition(int id)
{
    pMembersSection->setContactList(pInfo->members(), serverInfoProcessor, *pChat, pInfo->adminId());
}

void ChatPage::onNewAdmin(int id)
{
    pMembersSection->setContactList(pInfo->members(), serverInfoProcessor, *pChat, id);
}

void ChatPage::setChat(ChatInfo& info)
{
    if (pInfo)
    {
        disconnect(pInfo, &ChatInfo::memberRemoved, this, &ChatPage::onMemberRemoval);
        disconnect(pInfo, &ChatInfo::removed, this, &ChatPage::onBeingRemoved);
        disconnect(pInfo, &ChatInfo::newMember, this, &ChatPage::onMemberAddition);
        disconnect(pInfo, &ChatInfo::newAdmin, this, &ChatPage::onNewAdmin);
    }

    pInfo = &info;
    connect(pInfo, &ChatInfo::memberRemoved, this, &ChatPage::onMemberRemoval);
    connect(pInfo, &ChatInfo::newMember, this, &ChatPage::onMemberAddition);
    connect(pInfo, &ChatInfo::removed, this, &ChatPage::onBeingRemoved);
    connect(pInfo, &ChatInfo::newAdmin, this, &ChatPage::onNewAdmin);

    if (info.isPrivate())
    {
        pMembersSection->setVisible(false);
        pToolBar->setVisible(false);
    }
    else
    {
        pMembersSection->setVisible(true);
        pToolBar->setVisible(true);
        pMembersSection->setContactList(info.members(), serverInfoProcessor, *pChat, info.adminId());
    }
    pChat->setChat(info);
}


void ChatPage::setChat(int id)
{
    setChat(serverInfoProcessor.getChatById(id));
}
