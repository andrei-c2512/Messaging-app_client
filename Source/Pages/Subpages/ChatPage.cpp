#include "ChatPage.h"
#include "StyleBase/StyleRepository.h"



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
        // + 1 because we add the app user as well
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

    for (short i = 0; i < contactList.size(); i++)
    {
        GroupMemberOptions::Features flags = init;
        _viewList[i]->setVisible(true);
        _viewList[i]->setContactInfo(*contactList[i]);

        if(adminId == contactList.at(i)->id())
            _viewList[i]->setIsAdmin(true);
     
        ContactInfo::ContactStatus status = contactList[i]->flags();

        _viewList[i]->attatchOptions(processor, page, flags);
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
    }
}



ChatPage::ChatPage(QWidget* parent , ServerInfoProcessor& ServerInfoProcessor) : Page(parent , ServerInfoProcessor)
{
    setupUi();

    connect(&ServerInfoProcessor, &ServerInfoProcessor::unknownListReceived, this, [= , &ServerInfoProcessor]() {
        setChat(ServerInfoProcessor.firstChat());
        });
}

void ChatPage::setupUi()
{
    pMainLayout = new QHBoxLayout(this);
    pChat = new Chat(nullptr , serverInfoProcessor);
    pMembersSection = new MembersSection(nullptr , serverInfoProcessor);

    pMainLayout->addWidget(pChat, 4);
    pMainLayout->addWidget(pMembersSection, 1);
}
void ChatPage::setChat(ChatInfo& info)
{
    if (info.isPrivate())
        pMembersSection->setVisible(false);
    else
    {
        pMembersSection->setVisible(true);
        pMembersSection->setContactList(info.members(), serverInfoProcessor, *pChat, info.adminId());
    }
    pChat->setChat(info);
}


void ChatPage::setChat(int id)
{
    ChatInfo& info = serverInfoProcessor.getChatById(id);
    setChat(info);
}
