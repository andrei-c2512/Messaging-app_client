#pragma once
#include <QWidget>
#include "CustomStyledWidgets/CustomButton.h"
#include "CustomWidgets/SearchBar.h"
#include <QScrollArea>
#include "CustomWidgets/ContactView_UserSelector.h"
#include <QMdiSubWindow>
#include "Network/ServerInfoProcessor.h"

class SelectorContactList : public QScrollArea {
    //to be styled in style.qss
    Q_OBJECT
public:
    SelectorContactList(QWidget* parent = nullptr);
    void setContactList(std::vector<ContactInfo*> contactList);
    void resetViews();
    std::vector<int> checkedUsers() const;
private:
    void setupUi();
private:
    QWidget* pWidget;
    std::vector<ContactInfo*> _contactInfoList;
    std::vector<ContactView_UserSelector*> _viewList;
    QVBoxLayout* pLayout;
};
class UserSelectorWidget : public QMdiSubWindow {
    Q_OBJECT
public:
    enum class Role {
        MakeGroupChat, AddToGroupChat
    };
public:
	UserSelectorWidget(QWidget* parent , ServerInfoProcessor& processor);
    void launch(QPoint point , std::vector<ContactInfo*> contactList);
    void launch(QPoint point);
    void launch();
    void setRole(Role role);
    void setChatId(int id);
    //on and off , like a switch
    void flip(QPoint point, std::vector<ContactInfo*> contactList);
    void flip(QPoint point , bool visible);
    void flip(bool visible);
    void flip(QPoint point);
signals:
    void groupCreated(std::vector<const ContactInfo*> list);
private:
    void setupUi();
private:
    void search(const QString& str);
protected:
    void focusOutEvent(QFocusEvent* ev) override;
private:
    ServerInfoProcessor& processor;

    QWidget* pWidget;
	QVBoxLayout* pLayout;
    SelectorContactList* pList;
    SearchBar* pSearchBar;
    CustomButton* pSearchButton;
    CustomButton* pDoneButton;

    Role _role;
    int _chatId;
};