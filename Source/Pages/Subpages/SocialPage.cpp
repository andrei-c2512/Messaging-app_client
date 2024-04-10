#include "SocialPage.h"
#include "StyleBase/ButtonStyleRepository.h"
#include "StyleBase/StyleRepository.h"
ContactList::ContactList(QWidget* parent) : QScrollArea(parent)
{
    setupUi();
    _loadLimit = 20;
    setObjectName("ContactList");
}
void ContactList::setContactList(std::vector<ContactInfo*> contactList , ServerInfoProcessor& processor, ChatPage& chatPage ,  
    QStackedWidget& stackedW, SearchType type , bool isSearchBarEmpty)
{
    usedViews = (qint32)contactList.size();
    short dif = short(_viewList.size()) - (std::min(_loadLimit , short(contactList.size())));
    short oldSize = (short)_viewList.size();
    if(dif < 0)
    {
        dif = -dif;
        _viewList.resize(oldSize + dif);
        auto e = _viewList.end();
        for(auto b = _viewList.begin() + oldSize ;  b != e; ++b)
        {
            *b = new ContactView();
            connect(*b , &ContactView::remove , this , [=](){
                for (int i = 0; i < _viewList.size(); i++)
                {
                    if(*_viewList[i] == (**b))
                    {
                        (*b)->deleteLater();
                        _viewList.erase(_viewList.begin() + i);
                        break;
                    }
                }
            });

            pLayout->insertWidget(pLayout->count() - 1 , *b);
        }

    } else if( dif > 0)
    {
        for(short i = oldSize - dif ; i < oldSize ; i++)
            _viewList[i]->setVisible(false);
    }
    for(short i = 0 ; i < usedViews ; i++)
    {
        _viewList[i]->setVisible(true);
        _viewList[i]->setContactInfo(*contactList[i]);
        _viewList[i]->attatchOptions( type , processor , chatPage, stackedW);
    }
    
    _contactInfoList = contactList;
    _searchType = type;
    pEmptyListLabel->setVisible(!bool(_contactInfoList.size()));
    updateLabel(isSearchBarEmpty);
}



void ContactList::setLoadLimit(int limit) { _loadLimit = std::move(limit);}

int ContactList::loadLimit() const { return _loadLimit;}

void ContactList::setupUi()
{
    setWidgetResizable(true);
    pWidget = new QWidget;
    pWidget->setObjectName("ListContainer");

    pSpecialLabel = new MemeWidget;
    QHBoxLayout* pSpecialLayout = new QHBoxLayout;
    pSpecialLayout->addStretch(1);
    pSpecialLayout->addWidget(pSpecialLabel);
    pSpecialLayout->addStretch(1);

    pSpecialLabel->setVisible(false);
    pSpecialLabel->movie().stop();
    pSpecialLabel->player().stop();

    pEmptyListLabel = new QLabel;
    pEmptyListLabel->setFont(StyleRepository::Base::mediumSizeButtonFont());

    pLoadingWidget = new LoadingAnimationWidget;
    pLoadingWidget->setVisible(false);
    pLoadingWidget->stop();

    pLayout = new QVBoxLayout(pWidget);
    pLayout->addWidget(pEmptyListLabel);
    pLayout->addWidget(pLoadingWidget);
    pLayout->addLayout(pSpecialLayout);
    pLayout->addStretch(1);
    setWidget(pWidget);
}

void ContactList::waitForResults()
{
    pLoadingWidget->setVisible(true);
    pLoadingWidget->start();

    for(ContactView* view : _viewList)
        view->setVisible(false);
}
void ContactList::stopWaiting()
{
    pLoadingWidget->setVisible(false);
    pLoadingWidget->stop();

    for(ContactView* view : _viewList)
        view->setVisible(true);
}

void ContactList::updateLabel( bool isSearchBarEmpty)
{
    QString aux;
    switch(_searchType) {
    case SearchType::AddFriend:
        aux = (isSearchBarEmpty) ? "Search for people" : "No results";
        pSpecialLabel->setVisible(false);
        pSpecialLabel->fadeMusic();
        break;
    case SearchType::RequestList:
        aux = (isSearchBarEmpty) ? "You have no requests" : "No results";
        pSpecialLabel->setVisible(false);
        pSpecialLabel->fadeMusic();
        break;
    case SearchType::FriendList:
        aux = (isSearchBarEmpty) ? "You have no friends" : "No results";
        pSpecialLabel->setVisible(false);
        pSpecialLabel->movie().stop();
        pSpecialLabel->fadeMusic();
        break;
    case SearchType::BlockedList:
        aux = (isSearchBarEmpty) ? "" : "No results";

        if(pEmptyListLabel->isVisible())
        {
            pSpecialLabel->startMusic();
            pSpecialLabel->movie().start();
            pSpecialLabel->setVisible( true);
        }
        break;
    default:
        break;
    }
    pEmptyListLabel->setText(std::move(aux));

}


SocialPage::SocialPage(QWidget* parent ,ServerInfoProcessor& pInfoProcessor0 , ChatPage& page, QStackedWidget& stackedWidget0) 
    : Page(parent , pInfoProcessor0) , chatPage(page) , stackedWidget(stackedWidget0)
{
    _searchType = SearchType::FriendList;
    _currentList = &serverInfoProcessor.friendList();
    setupUi();
    connect(&serverInfoProcessor , &ServerInfoProcessor::newFriend , this , [=](){
        if(_searchType == SearchType::FriendList)
        {
            pList->setContactList(serverInfoProcessor.friendList() , serverInfoProcessor , chatPage, stackedWidget, SearchType::FriendList , pSearchBar->document()->isEmpty()) ;
        }
    });
    connect(&serverInfoProcessor , &ServerInfoProcessor::newSearchData , this , [=](){
        pList->stopWaiting();
        pList->setContactList(serverInfoProcessor.searchedForUsers() , serverInfoProcessor , chatPage , stackedWidget, SearchType::AddFriend , pSearchBar->document()->isEmpty());
    });
    connect(&serverInfoProcessor , &ServerInfoProcessor::newFriendRequest , this , [=](){
        if(_searchType == SearchType::RequestList)
        pList->setContactList(serverInfoProcessor.requestList() , serverInfoProcessor , chatPage , stackedWidget, SearchType::RequestList , pSearchBar->document()->isEmpty());
    });
    connect(&serverInfoProcessor, &ServerInfoProcessor::contactInfoLoaded, this, [=]() {

        if(serverInfoProcessor.blockedList().size()) //this will make it so that the meme widget doesn't play when logging in
            pList->setContactList(serverInfoProcessor.blockedList(), serverInfoProcessor, chatPage, stackedWidget, SearchType::BlockedList, pSearchBar->document()->isEmpty());
        pList->setContactList(serverInfoProcessor.requestList(), serverInfoProcessor, chatPage, stackedWidget, SearchType::RequestList, pSearchBar->document()->isEmpty());
        pList->setContactList(serverInfoProcessor.friendList(), serverInfoProcessor, chatPage, stackedWidget, SearchType::FriendList, pSearchBar->document()->isEmpty());
    });
    pList->setContactList(*_currentList , serverInfoProcessor , chatPage, stackedWidget , SearchType::FriendList, pSearchBar->document()->isEmpty());
}
void SocialPage::setSearchType(SearchType type)
{
    _searchType = std::move(type);
    switch(_searchType)
    {
    case SearchType::FriendList:
        _currentList = &serverInfoProcessor.friendList();
        break;
    case SearchType::RequestList:
        _currentList = &serverInfoProcessor.requestList();
        break;
    case SearchType::AddFriend:
        _currentList = &serverInfoProcessor.searchedForUsers();
        break;
    case SearchType::BlockedList:
        _currentList = &serverInfoProcessor.blockedList();
        break;
    }
}

void SocialPage::search(const QString& str)
{
    if(_searchType != SearchType::AddFriend)
    {
        std::vector<std::pair<ContactInfo* , int>> _foundUsers;
        //first one is the the object , second is the pos at which the substring is found
        auto& list = *_currentList;
        for(ContactInfo* c : list)
        {
            int pos = c->name().indexOf(str , 0 , Qt::CaseInsensitive);
            if(pos != -1)
                _foundUsers.emplace_back(std::pair<ContactInfo* , int>(c , pos));
        }
        std::sort(_foundUsers.begin() , _foundUsers.end() , [](std::pair<ContactInfo* , int>& elem , std::pair<ContactInfo* , int>& elem1){
            return elem.second < elem1.second;
        });

        std::vector<ContactInfo*> result(_foundUsers.size());
        for(int i = 0 ; i < result.size() ; i++)
            result[i] = _foundUsers[i].first;

        pList->setContactList(result , serverInfoProcessor , chatPage, stackedWidget, _searchType , pSearchBar->document()->isEmpty());
    }
    else
    {
        if(pSearchBar->toPlainText().length() >= 2)
        {
            serverInfoProcessor.searchForPeople(str);
            pList->waitForResults();
        }
    }
}

void SocialPage::setupUi()
{
    QMargins noMargins = { 0 , 0 , 0 , 0};

    pFriendsBtn = new SelectableButton(nullptr , ButtonStyleRepository::selectableFriendListButton());
    pFriendsBtn->setText("Friends");
    pFriendsBtn->setFont(StyleRepository::Base::mediumSizeButtonFont());

    pRequestsBtn = new SelectableButton(nullptr , ButtonStyleRepository::selectableRequestListButton());
    pRequestsBtn->setText("Requests");
    pRequestsBtn->setFont(StyleRepository::Base::mediumSizeButtonFont());

    pAddFriendBtn = new SelectableButton(nullptr , ButtonStyleRepository::selectableAddFriendButton());
    pAddFriendBtn->setText("Add friend");
    pAddFriendBtn->setFont(StyleRepository::Base::mediumSizeButtonFont());

    pBlockedBtn = new SelectableButton(nullptr , ButtonStyleRepository::selectableBlockedListButton());
    pBlockedBtn->setText("Blocked");
    pBlockedBtn->setFont(StyleRepository::Base::mediumSizeButtonFont());
    pBlockedBtn->setIconSize(QSize(32 , 32));
    pBlockedBtn->setIcon(QPixmap(":/Images/Images/Icons/BlockedIcon.png"));

    pOptionMenu = new OptionMenu;

    //adding the buttons to the optionMenu
    {
        pOptionMenu->addOption(pFriendsBtn);
        pOptionMenu->addOption(pRequestsBtn);
        pOptionMenu->addOption(pAddFriendBtn);
        pOptionMenu->addOption(pBlockedBtn);
        pOptionMenu->useOption(0);
    }
    //connecting the buttons
    {
     connect(pFriendsBtn , &SelectableButton::clicked , this , [=](){
         if(_searchType == SearchType::AddFriend)
             searchedFor_string = pSearchBar->toPlainText();

         if(_searchType != SearchType::FriendList)
         {
            setSearchType(SearchType::FriendList);
            pList->setContactList(*_currentList , serverInfoProcessor , chatPage, stackedWidget, _searchType ,  pSearchBar->document()->isEmpty());
            pSearchBar->setText("");
         }
     });
     connect(pRequestsBtn , &SelectableButton::clicked , this , [=](){
         if(_searchType == SearchType::AddFriend)
             searchedFor_string = pSearchBar->toPlainText();

         if(_searchType != SearchType::RequestList)
         {
             setSearchType(SearchType::RequestList);
             pList->setContactList(*_currentList , serverInfoProcessor , chatPage, stackedWidget, _searchType ,  pSearchBar->document()->isEmpty());
             pSearchBar->setText("");
         }


     });
     connect(pAddFriendBtn , &SelectableButton::clicked , this , [=](){

         if(_searchType != SearchType::AddFriend)
         {
             setSearchType(SearchType::AddFriend);
             pList->setContactList(*_currentList , serverInfoProcessor , chatPage, stackedWidget, _searchType ,  pSearchBar->document()->isEmpty());
             search(searchedFor_string);
             pSearchBar->setText(searchedFor_string);
         }
     });
     connect(pBlockedBtn , &SelectableButton::clicked , this , [=](){
         if(_searchType == SearchType::AddFriend)
             searchedFor_string = pSearchBar->toPlainText();

         if(_searchType != SearchType::BlockedList)
         {
             setSearchType(SearchType::BlockedList);
             pList->setContactList(*_currentList , serverInfoProcessor , chatPage, stackedWidget, _searchType ,  pSearchBar->document()->isEmpty());
         }

     });
    }
    pSearchBar = new SearchBar;
    pSearchBar->setFont(StyleRepository::Base::standardFont());

    pSearchButton = new CustomButton(nullptr , ButtonStyleRepository::searchButton());
    connect(pSearchButton , SIGNAL(clicked()) , pSearchBar , SLOT(makeSearchRequest()));

    connect(pSearchBar , &SearchBar::textChanged , this , [=](){
        search(pSearchBar->toPlainText());
    });
    connect(pSearchBar , SIGNAL(searchRequested(QString)) , this , SLOT(search(QString)));

    pSearchBarLayout = new QHBoxLayout;
    pSearchBarLayout->addWidget(pSearchBar , 1);
    pSearchBarLayout->addWidget(pSearchButton);
    pSearchBarLayout->setContentsMargins(noMargins);
    pSearchBarLayout->addStretch(1);
    pSearchBarLayout->setSpacing(0);

    pList = new ContactList;
    pMainLayout = new QVBoxLayout(this);
    pSearchBarLayout->addStretch(1);
    pMainLayout->addWidget(pOptionMenu);
    pSearchBarLayout->addStretch(1);
    pMainLayout->addLayout(pSearchBarLayout);
    pMainLayout->addWidget(pList, 3);
    pSearchBarLayout->addStretch(1);
}



