
#ifndef SOCIALPAGE_H
#define SOCIALPAGE_H
#include <QWidget>
#include <QVBoxLayout>
#include "CustomStyledWidgets/CustomButton.h"
#include "CustomWidgets/ContactView_SocialPage.h"
#include <QScrollArea>
#include "Network/ServerInfoProcessor.h"
#include "CustomWidgets/SearchBar.h"
#include "CustomWidgets/OptionMenu.h"
#include "Pages/Page.h"
#include <QMovie>
#include "CustomWidgets/MemeWidget.h"
#include "CustomStyledWidgets/LoadingAnimationWidget.h"
#include <QStackedWidget>

class ContactList : public QScrollArea{
public:
    ContactList(QWidget* parent = nullptr);
    void setContactList(std::vector<ContactInfo*> contactList ,  ServerInfoProcessor& processor, 
        ChatPage& chatPage0 , QStackedWidget& stackedW , SearchType type , bool isSearchBarEmpty);
    void setLoadLimit(int limit);
    int loadLimit() const;
    void waitForResults();
    void stopWaiting();
private:
    void setupUi();
    void updateLabel(bool isSearchBarEmpty);
private:
    std::vector<ContactInfo*> _contactInfoList;
    std::vector<ContactView_SocialPage*> _viewList;
    qint16 usedViews;
    qint16 _loadLimit;
    QWidget* pWidget;
    QVBoxLayout* pLayout;

    SearchType _searchType;

    QLabel* pEmptyListLabel;
    LoadingAnimationWidget* pLoadingWidget;
    MemeWidget* pSpecialLabel;
};

class SocialPage : public Page{
    Q_OBJECT
public:
    SocialPage(QWidget* parent , ServerInfoProcessor& pInfoProcessor0 , ChatPage& chatPage0 , QStackedWidget& stackedWidget);
    void setSearchType(SearchType type);
    void updateLists();
public slots:
    void search(const QString& str);
private:
    void setupUi();
private:
    QVBoxLayout*    pMainLayout;

    OptionMenu      *   pOptionMenu;
    SelectableButton*   pFriendsBtn;
    SelectableButton*   pRequestsBtn;
    SelectableButton*   pAddFriendBtn;
    SelectableButton*   pBlockedBtn;

    QHBoxLayout*    pSearchBarLayout;
    SearchBar*      pSearchBar;
    CustomButton*   pSearchButton;

    ContactList* pList;

    std::vector<ContactInfo*>* _currentList;

    SearchType _searchType;
    QString searchedFor_string;
    ChatPage& chatPage;
    //the stacked widget it belongs to
    QStackedWidget& stackedWidget;
};

#endif // SOCIALPAGE_H
