#ifndef SUBPAGEMANAGER_H
#define SUBPAGEMANAGER_H
#include "ChatPage.h"
#include <QStackedWidget>
#include "SocialPage.h"
#include "CustomWidgets/UserSelectorWidget.h"

class SubpageManager : public QStackedWidget{
public:
    enum class Type{
        Chat,
        Social
    };
public:
    SubpageManager(QWidget* parent , ServerInfoProcessor& pInfoProcessor , UserSelectorWidget& userSelector);
    void setPage(Type type);
    ChatPage& chatPage();
    SocialPage& socialPage();
private:
    std::vector<Page*> subPage_list;
    ServerInfoProcessor& infoProcessor;
};
#endif // SUBPAGEMANAGER_H
