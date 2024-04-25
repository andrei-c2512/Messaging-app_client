#include "SubpageManager.h"

SubpageManager::SubpageManager(QWidget* parent , ServerInfoProcessor& infoProcessor0 , UserSelectorWidget& userSelector)
    :QStackedWidget(parent) , infoProcessor(infoProcessor0)
{
    subPage_list = { new ChatPage(nullptr , infoProcessor , userSelector)};
    subPage_list.emplace_back(new SocialPage(nullptr, infoProcessor , static_cast<ChatPage&>(*subPage_list[0]) , *this));
    for(Page* page : subPage_list)
        QStackedWidget::addWidget(page);

    setCurrentIndex(int(Type::Social));
}
void SubpageManager::setPage(Type type){ 
    if (type == Type::Social)
        static_cast<SocialPage*>(subPage_list[int(type)])->updateLists();

    setCurrentIndex(int(type)); 
}
ChatPage& SubpageManager::chatPage(){
    return *static_cast<ChatPage*>(subPage_list[int(Type::Chat)]);
}

