#include "UserSelectorWidget.h"
#include "StyleBase/ButtonStyleRepository.h"
#include "StyleBase/StyleRepository.h"


SelectorContactList::SelectorContactList(QWidget* parent) : QScrollArea(parent)
{
    setupUi();
    setObjectName("SelectorContactList");
}
void SelectorContactList::setContactList(std::vector<ContactInfo*> contactList)
{
    short dif = short(_viewList.size()) - short(contactList.size());
    short oldSize = (short)_viewList.size();
    if (dif < 0)
    {
        dif = -dif;
        _viewList.resize(oldSize + dif);
        auto e = _viewList.end();
        for (auto b = _viewList.begin() + oldSize; b != e; ++b)
        {
            *b = new ContactView_UserSelector();
            connect(*b, &ContactView_UserSelector::remove, this, [=](int id) {
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
    for (short i = 0; i < contactList.size(); i++)
    {
        _viewList[i]->setVisible(true);
        _viewList[i]->setContactInfo(*contactList[i]);
        _viewList[i]->attatchCheckbox();
    }

    _contactInfoList = contactList;
}


void SelectorContactList::setupUi()
{
    setWidgetResizable(true);
    pWidget = new QWidget;
    pWidget->setObjectName("ListContainer");


    pLayout = new QVBoxLayout(pWidget);
    pLayout->addStretch(1);
    setWidget(pWidget);
}

void SelectorContactList::resetViews()
{
    for (ContactView_UserSelector* view : _viewList)
        view->setChecked(false);
}

std::vector<int> SelectorContactList::checkedUsers() const 
{
    std::vector<int> userList;
    for (ContactView_UserSelector* view : _viewList)
        if (view->checked())
            userList.emplace_back(view->contactInfo()->id());

    return userList;
}


UserSelectorWidget::UserSelectorWidget(QWidget* parent, ServerInfoProcessor& processor0) :QMdiSubWindow(parent , Qt::FramelessWindowHint | Qt::Popup) , processor(processor0)
{
    setAttribute(Qt::WA_StyledBackground);
    setFocusPolicy(Qt::ClickFocus);
    setupUi();
    setObjectName("UserSelectorWidget");
    _chatId = 0;
}
void UserSelectorWidget::launch(QPoint point, std::vector<ContactInfo*> contactList)
{
    setGeometry(QRect(point, size()));
    pList->setContactList(contactList);
    pList->resetViews();
    setVisible(true);
}

void UserSelectorWidget::launch(QPoint point)
{
    setGeometry(QRect(point, size()));
    launch();
}
void UserSelectorWidget::launch()
{
    pList->setContactList(processor.friendList());
    pList->resetViews();
    setVisible(true);
}

void UserSelectorWidget::setRole(Role role) { _role = role; }
void UserSelectorWidget::setChatId(int id) { _chatId = id; }
void UserSelectorWidget::search(const QString& str)
{
    std::vector<std::pair<ContactInfo*, int>> _foundUsers;
    //first one is the the object , second is the pos at which the substring is found
    auto& list = processor.friendList();
    for (ContactInfo* c : list)
    {
        int pos = c->name().indexOf(str, 0, Qt::CaseInsensitive);
        if (pos != -1)
            _foundUsers.emplace_back(std::pair<ContactInfo*, int>(c, pos));
    }
    std::sort(_foundUsers.begin(), _foundUsers.end(), [](std::pair<ContactInfo*, int>& elem, std::pair<ContactInfo*, int>& elem1) {
        return elem.second < elem1.second;
        });

    std::vector<ContactInfo*> result(_foundUsers.size());
    for (int i = 0; i < result.size(); i++)
        result[i] = _foundUsers[i].first;

    pList->setContactList(result);
}


void UserSelectorWidget::setupUi()
{
    pWidget = new QWidget;

    pList = new SelectorContactList;
    pDoneButton = new CustomButton;
    pDoneButton->setFont(StyleRepository::Base::mediumSizeButtonFont());
    pDoneButton->setText("Done");
    connect(pDoneButton, &CustomButton::clicked, this, [=]() {
        auto list = pList->checkedUsers();
        if (list.size() > 1)
        {
            if (_role == UserSelectorWidget::Role::MakeGroupChat)
                processor.createGroupChat(list);
            else if (_role == UserSelectorWidget::Role::AddToGroupChat)
                processor.addPeopleToGroup(_chatId , list);
        }
        else if(list.size() == 1 && _role == UserSelectorWidget::Role::AddToGroupChat)
            processor.addPeopleToGroup(_chatId, list);

        setVisible(false);
    });

    pSearchButton = new CustomButton(nullptr, ButtonStyleRepository::searchButton());
    pSearchBar = new SearchBar;
    pSearchBar->setFont(StyleRepository::Base::standardFont());

    connect(pSearchBar, &SearchBar::textChanged, this, [=]() {
        search(pSearchBar->toPlainText());
        });
    QHBoxLayout* pSearchBarLayout = new QHBoxLayout;
    pSearchBarLayout->addWidget(pSearchBar);
    pSearchBarLayout->addWidget(pSearchButton);
    pSearchBarLayout->addStretch(1);

    pLayout = new QVBoxLayout(pWidget);
    pLayout->addLayout(pSearchBarLayout);
    pLayout->addWidget(pList, 3);
    pLayout->addStretch(1);
    pLayout->addWidget(pDoneButton);

    connect(&processor, &ServerInfoProcessor::newFriend, this, [=]() {
        if(isVisible() == false)
            pList->setContactList(processor.friendList());
        });



    setWidget(pWidget);
}

void UserSelectorWidget::flip(QPoint point, std::vector<ContactInfo*> contactList)
{
    if (isVisible() == false)
        launch(point , contactList);
    else
        setVisible(false);
}

void UserSelectorWidget::flip(QPoint point , bool visible)
{
    if (visible)
        launch(point);
    else
        setVisible(false);
}


void UserSelectorWidget::flip(bool visible)
{
    if (visible)
        launch();
    else
        setVisible(false);
}

void UserSelectorWidget::flip(QPoint point)
{
    if (isVisible() == false)
        launch(point);
    else
        setVisible(false);
}

void  UserSelectorWidget::focusOutEvent(QFocusEvent* ev)
{
    setVisible(false);
    //QMdiSubWindow::focusOutEvent(ev);
}
