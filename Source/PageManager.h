
#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H
#include <QStackedWidget>
#include "StyleBase/ButtonStyles/EditButtonStyleHelper.h"
#include "Pages/LoginPage.h"
#include "Pages/RegisterPage.h"
#include "Pages/LoadingPage.h"
#include "Pages/MainPage.h"

class PageManager : public QStackedWidget{
    Q_OBJECT
public:
    enum class PageType{
        Loading,
        Login,
        Register,
        Main
    };
public:
    PageManager(QWidget* parent , ServerInfoProcessor& pServerInfoProcessor);
signals:
    void pageChanged(PageType type);
private:
    void setupUi();
    void connectButtons(PageManager::PageType type , const Page* page);
    void setPage(PageManager::PageType type);
private:
    std::vector<Page*> pageList;
    ServerInfoProcessor& serverInfoProcessor;
};

#endif // PAGEMANAGER_H
